#ifdef _WIN32
#include <gdiplus.h>
//#include <shlwapi.h> // -lshlwapi, for SHCreateMemStream //extern "C" IStream* SHCreateMemStream(const BYTE *pInit, UINT cbInit);
#elif defined(NCPP_MEDIA_USE_STB)
// https://github.com/nothings/stb/blob/master/stb_image.h?
// https://github.com/guohai/jpeg/blob/master/nanojpeg.c? (NanoJPEG Project)
// #define STBI_ONLY_JPEG
// #define STBI_ONLY_PNG
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"
#endif

namespace ncpp { //BMP, PNG, JPEG
//Windows Dependencies: -lgdiplus -lole32
//Linux Dependencies: -l:libpng16.so.16 + -l:libjpeg.so.62  / (stb_image.h + stb_image_write.h)

#include "colors.h"

struct PixelT { unsigned char r,g,b,a; }; //[R, G, B, A]

struct Image { int w, h; Array<PixelT> px; //char bits=32;
	Image(int w1=0, int h1=0){ resize(w1, h1); }
	Image& resize(int w1, int h1){ w=w1; h=h1; px.resize(w*h); return *this; }
	size_t pixels(){ return px.size(); }
	size_t size(){ return px.size()*sizeof(PixelT); }
	Image& setColor(size_t pos, unsigned int hex=0xFFFFFFFF){ HexToRGBA(&px[pos].r, hex); return *this; }
	unsigned int getColor(size_t pos){ return RGBAToHex(&px[pos].r); }
	Image& fill(unsigned int hex=0xFFFFFFFF){ for(size_t i=0;i<px.size();i++) setColor(i, hex); return *this; }
	
	static Image from(const Buffer& fimg);
	static Image fromFile(const CString& path){ return from(fs::readFile(path)); }
	static Image fromBMP(const Buffer& bmp);
	static Image fromPNG(const Buffer& png);
	static Image fromJPG(const Buffer& jpg);
	Buffer toBMP() const;
	Buffer toPNG() const;
	Buffer toJPG(int quality=90) const;
	bool saveBMP(const CString& path) const { return fs::writeFile(path, toBMP()); }
	bool savePNG(const CString& path) const { return fs::writeFile(path, toPNG()); }
	bool saveJPG(const CString& path, int quality=90) const { return fs::writeFile(path, toJPG(quality)); }
	
	//https://habr.com/ru/articles/243285/
	Image scale(int tW, int tH){ Image img(tW, tH); //"Ближайший сосед". Возможные: Бикубическая интерполяция, Билинейная интерполяция 
		for(int y=0; y < tH; ++y){ int srcY = (y*h) / tH;
			for(int x=0; x < tW; ++x){ int srcX = (x*w) / tW; img.px[y * tW + x] = px[srcY * w + srcX]; } } return img; }
	
	#ifdef _WIN32
	static bool _GDI_inited; static ULONG_PTR gdiplusToken;
	Buffer _serialize(const GUID& fmtGuid, int quality=90) const;
	static bool _initGDI(){ if(_GDI_inited) return true; Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		if(GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL)!=S_OK){ return false; } _GDI_inited=true; return true; }
	static void _freeGDI(){ Gdiplus::GdiplusShutdown(gdiplusToken); }
	static bool init(){ return _initGDI(); }
	#endif //_WIN32
	#ifdef STBI_INCLUDE_STB_IMAGE_H
	static Image _loadWithStb(const Buffer& data);
	static bool init(){}
	#elif !defined(_WIN32) // === libpng + libjpeg ===
	static bool _loadSOLibs();
	static bool init(){ return _loadSOLibs(); }
	static void _libpng_mem_read_cb(void* png_ptr, unsigned char* outBytes, size_t byteCount);
	static void _libpng_mem_write_cb(void* png_ptr, unsigned char* data, size_t length);
    static void _libjpeg_term_destination(void* cinfo);
    static int  _libjpeg_empty_output_buffer(void* cinfo);
    static void _libjpeg_init_destination(void* cinfo);
	#endif
	#ifdef INCLUDE_STB_IMAGE_WRITE_H
	static void _stbi_to_buffer(void *context, void *data, int size);
	#endif
};

Buffer Image::toBMP() const { if(px.empty()) return ""; int rowSize = (w*3+3) & ~3; // Выравнивание по 4 байта
    int dataSz = rowSize*h; Buffer res(54); res.fill(0).resize(54+dataSz); res.writeUInt32LE(0, res.size()-4);
	// Заголовок файла (14 байт): 'BM', Размер файла, Резерв, Смещение к данным 
    res.writeUInt16BE(0x424D); res.writeUInt32LE(54+dataSz, 2); res.writeUInt32LE(0, 6); res.writeUInt32LE(54, 10);
    // 2. Информационный заголовок (40 байт)
    res.writeUInt32LE(40, 14); res.writeUInt32LE(w, 18); res.writeUInt32LE(h, 22); // Размер заголовка, Ширина, Высота
    res.writeUInt16LE(1, 26); res.writeUInt16LE(24, 28); res.writeUInt32LE(0, 30); // Плоскости, Бит на пиксель (24-bit), Сжатие (0 = BI_RGB) 
    res.writeUInt32LE(dataSz, 34); // Размер данных
    //res.writeUInt32LE(2835, 38); res.writeUInt32LE(2835, 42); // biXPelsPerMeter, biYPelsPerMeter (72 DPI ≈ 2835 пикселей на метр)

	//for(size_t i=0; i<px.size(); i++){ size_t b=54+i*3; for(int p=0;p<3;p++) res[b+p]=(&px[i].r)[2-p]; } //Цикл для кривого BMP :)
	for(int y = h - 1; y >= 0; y--){ int rowStart = 54+(h-1-y)*rowSize;
        for(int x = 0; x < w; x++){ for(int i=0;i<3;i++) res[rowStart+x*3+i] = (&px[y*w+x].r)[2-i]; } } return res; }

#ifdef _WIN32
bool Image::_GDI_inited=false; ULONG_PTR Image::gdiplusToken=0;
Image Image::from(const Buffer& fimg){ if(fimg.empty()||!_initGDI()) return Image(); 
	//IStream* pStream = ::SHCreateMemStream((const BYTE*)fimg.data(), (UINT)fimg.size()); if(!pStream) return Image();
	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, fimg.size()); if(!hMem) return Image();
    void* pMem = ::GlobalLock(hMem); if(pMem){ memcpy(pMem, fimg.data(), fimg.size()); ::GlobalUnlock(hMem); }
    IStream* pStream = NULL; if(CreateStreamOnHGlobal(hMem, TRUE, &pStream) != S_OK){ GlobalFree(hMem); return Image(); }
	
    Gdiplus::Bitmap* gdiBitmap = Gdiplus::Bitmap::FromStream(pStream);
	if(!gdiBitmap || gdiBitmap->GetLastStatus() != Gdiplus::Ok){ if(gdiBitmap) delete gdiBitmap; pStream->Release(); return Image(); }
	int w = gdiBitmap->GetWidth(); int h = gdiBitmap->GetHeight(); Image img(w, h); //Image img = {w, h};
	Gdiplus::BitmapData data; Gdiplus::Rect rect(0, 0, w, h); gdiBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
    memcpy(img.px.data(), data.Scan0, w*h*4); for(size_t i=0;i<img.px.size();++i){ swap(img.px[i].r, img.px[i].b); } // BGRA -> RGBA
    gdiBitmap->UnlockBits(&data); delete gdiBitmap; pStream->Release(); return img; }
	
Buffer Image::_serialize(const GUID& fmtGuid, int quality) const { if(px.empty()||w<=0||h<=0||!_initGDI()) return "";
	Image img = *this; for(size_t i=0;i<img.px.size();++i){ swap(img.px[i].r, img.px[i].b); } // RGBA -> BGRA
    Buffer result; Gdiplus::Bitmap gdiBitmap(w, h, w*4, PixelFormat32bppARGB, (BYTE*)img.px.data()); 
	
    Gdiplus::EncoderParameters encParams; Gdiplus::EncoderParameters* pEncParams = nullptr; ULONG q; // Настройка качества для JPEG
	const GUID jpeg = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };
    if(IsEqualGUID(fmtGuid, jpeg)){ encParams.Count = 1; encParams.Parameter[0].Guid = Gdiplus::EncoderQuality;
		encParams.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong; encParams.Parameter[0].NumberOfValues = 1;
        q = (ULONG)quality; encParams.Parameter[0].Value = &q; pEncParams = &encParams; }
	
    IStream* pStream = nullptr; if(CreateStreamOnHGlobal(NULL, TRUE, &pStream) != S_OK) return "";
	if(gdiBitmap.Save(pStream, &fmtGuid, pEncParams) != Gdiplus::Ok){ pStream->Release(); return ""; }
	HGLOBAL hg = NULL; GetHGlobalFromStream(pStream, &hg); size_t size = GlobalSize(hg);
	result.resize(size); void* pData = GlobalLock(hg); memcpy(result.data(), pData, size); GlobalUnlock(hg); pStream->Release(); return result; }

//Buffer Image::toBMP() const { static const GUID bmp =  { 0x557cf400, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } }; return _serialize(bmp); }
Buffer Image::toPNG() const { static const GUID png =  { 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } }; return _serialize(png); }
Buffer Image::toJPG(int quality) const { static const GUID jpeg = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } }; return _serialize(jpeg, quality); }
#else // === Linux ===
Image Image::from(const Buffer& fimg){ if(fimg.size()>=54&&fimg[0]==0x42&&fimg[1]==0x4D) return fromBMP(fimg);
	else if(fimg.size()>=67&&fimg[1]==0x50&&fimg[2]==0x4E&&fimg[3]==0x47) return fromPNG(fimg);
	else if(fimg.size()>=125&&fimg[0]==0xFF&&fimg[1]==0xD8) return fromJPG(fimg); return Image(); }
	
Image Image::fromBMP(const Buffer& bmp){ if(bmp.size()<54||bmp[0]!=0x42||bmp[1]!=0x4D) return Image();
	int w = bmp.readInt32LE(18); int h = bmp.readInt32LE(22); int bitCnt = bmp.readUInt16LE(28); if(bitCnt < 24) return Image();
    Image img(w, h); int rowSize = (w * 3 + 3) & ~3; int dtOffset = bmp.readUInt32LE(10); Array<PixelT>& px = img.px;
    for(int y = 0; y < h; y++){ int rowStart = dtOffset + (h-1-y)*rowSize;
        for(int x = 0; x < w; x++){ for(int i=0;i<3;i++){ (&px[y*w+x].r)[2-i] = bmp[rowStart+x*3+i]; if(bitCnt<=24) px[y*w+x].a=0xFF; } } } return img; }

#ifdef STBI_INCLUDE_STB_IMAGE_H
Image Image::_loadWithStb(const Buffer& data){ int w, h, channels;
    unsigned char* pixels = stbi_load_from_memory((const unsigned char*)data.data(), (int)data.size(), &w, &h, &channels, 4);
    if(!pixels) return Image(); Image img(w, h); memcpy(img.px.data(), pixels, w*h*4); stbi_image_free(pixels); return img; }

Image Image::fromPNG(const Buffer& png){ return _loadWithStb(png); }
Image Image::fromJPG(const Buffer& jpg){ return _loadWithStb(jpg); }
#endif

#ifdef INCLUDE_STB_IMAGE_WRITE_H
Buffer Image::toPNG() const { if(px.empty()||w<=0||h<=0) return ""; int len;
    unsigned char* png_data = stbi_write_png_to_mem((const unsigned char*)px.data(), w*4, w, h, 4, &len);
    if(!png_data) return ""; Buffer res(len); memcpy(res.data(), png_data, len); STBIW_FREE(png_data); return res; }
	
//Image::toJPG(int quality) const: stbi_write_jpg_to_mem not implemented in STB
void Image::_stbi_to_buffer(void* ctx, void* data, int size){ Buffer* bf = (Buffer*)ctx; bf->push(data, size); }

Buffer Image::toJPG(int quality) const { if (px.empty()) return ""; Buffer res;
    if(!stbi_write_jpg_to_func(_stbi_to_buffer, &res, w, h, 4, px.data(), quality)){ return ""; } return res; }
#endif

#endif //_WIN32
}

#if !defined(_WIN32) && !defined(STBI_INCLUDE_STB_IMAGE_H) // === For Linux: use libpng + libjpeg ===
#include "image_libs_loader.h"
namespace ncpp {
// ------ libpng: read ------
struct _BuffOffset { const unsigned char* data; size_t size; size_t offset; }; // Контекст для чтения прямо из Buffer (минимально возможная структура)

void Image::_libpng_mem_read_cb(void* ioctx, unsigned char* outBytes, size_t byteCount){
    _BuffOffset* ctx = (_BuffOffset*)png_get_io_ptr(ioctx); if(!ctx || ctx->offset >= ctx->size) return;
    if(ctx->offset + byteCount > ctx->size){ byteCount = ctx->size - ctx->offset; }
	memcpy(outBytes, ctx->data + ctx->offset, byteCount); ctx->offset += byteCount; }

Image Image::fromPNG(const Buffer& png){ if(!_loadSOLibs() || !png_create_read_struct || png.empty()) return Image();
    void* png_ptr = png_create_read_struct("1.6.0", NULL, NULL, NULL); if(!png_ptr) return Image();
    void* info_ptr = png_create_info_struct(png_ptr); if(!info_ptr){ png_destroy_read_struct(&png_ptr, NULL, NULL); return Image(); }
    _BuffOffset ctx; ctx.data = png.data(); ctx.size = png.size(); ctx.offset=0;
	png_set_read_fn(png_ptr, &ctx, (void*)_libpng_mem_read_cb); png_read_info(png_ptr, info_ptr);
	
    unsigned int w=0, h=0; int bit_depth=0, color_type=0; png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
    // Приведение форматов
    if(color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
    if(bit_depth == 16) png_set_strip_16(png_ptr);
    if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY){ png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER); } // Добиваем до 4 байт (RGBA)
	png_read_update_info(png_ptr, info_ptr); 
	
	Image img(w, h); Array<unsigned char*> row_ptrs(h); for(unsigned int y=0; y<h; ++y){ row_ptrs[y] = (unsigned char*)&img.px[y*w]; }
    png_read_image(png_ptr, row_ptrs.data()); png_destroy_read_struct(&png_ptr, &info_ptr, NULL); return img; }
	
// ------ libjpeg: read ------

Image Image::fromJPG(const Buffer& jpg){ if(!_loadSOLibs() || !jpeg_CreateDecompress || !jpeg_std_error) return Image();
    struct jpeg_decompress_struct cinfo; struct jpeg_error_mgr jerr;
    cinfo.err = (struct jpeg_error_mgr*)jpeg_std_error(&jerr); // В libjpeg функция jpeg_std_error ожидает указатель на jpeg_error_mgr; // и заполняет его стандартными обработчиками
    
    // Вызываем jpeg_CreateDecompress передавая размер структуры для контроля версий ABI: Захардкодил 632 байт для версии 6b
	jpeg_CreateDecompress(&cinfo, 62, 632); //(size_t)sizeof(struct jpeg_decompress_struct)
    jpeg_mem_src(&cinfo, jpg.data(), (unsigned long)jpg.size()); // В версии 6b есть jpeg_mem_src?
    if(jpeg_read_header(&cinfo, 1) != 1){ jpeg_destroy_decompress(&cinfo); return Image(); } // 3. Читаем заголовок JPEG

    jpeg_start_decompress(&cinfo); int w = cinfo.image_width; int h = cinfo.image_height; int channels = cinfo.num_components; Image img(w, h);

    int row_stride = w*channels; Buffer row_buffer(row_stride); // 6. Построчное чтение сканирующих линий (scanlines) // libjpeg требует буфер под каждую строку
    for(int y=0; y<h; ++y){ unsigned char* row_pointer[1]; row_pointer[0] = row_buffer.data();
		jpeg_read_scanlines(&cinfo, row_pointer, 1); PixelT* dst_row = &img.px[y*w];
        if(channels == 3){
            for(int x=0; x<w; ++x){
				dst_row[x].r = row_buffer[x*3+0]; dst_row[x].g = row_buffer[x*3+1]; dst_row[x].b = row_buffer[x*3+2]; dst_row[x].a = 0xFF; }
        }else if(channels == 1){
            for(int x=0; x<w; ++x){ unsigned char val = row_buffer[x]; dst_row[x].r = val; dst_row[x].g = val; dst_row[x].b = val; dst_row[x].a = 0xFF; }
        }else if(channels == 4){
            for(int x=0; x<w; ++x){
                dst_row[x].r = row_buffer[x*4+0]; dst_row[x].g = row_buffer[x*4+1];
                dst_row[x].b = row_buffer[x*4+2]; dst_row[x].a = row_buffer[x*4+3]; } }
    }
    jpeg_finish_decompress(&cinfo); jpeg_destroy_decompress(&cinfo); return img; }
	
// ------ libpng: write ------

void Image::_libpng_mem_write_cb(void* ioctx, unsigned char* data, size_t size){ Buffer* buf = (Buffer*)png_get_io_ptr(ioctx); buf->push(data, size); }

Buffer Image::toPNG() const { if(!_loadSOLibs() || !png_create_write_struct || px.empty() || w <= 0 || h <= 0) return Buffer();
    void* png_ptr = png_create_write_struct("1.6.0", NULL, NULL, NULL); if(!png_ptr) return Buffer();
    void* info_ptr = png_create_info_struct(png_ptr); if(!info_ptr){ png_destroy_write_struct(&png_ptr, NULL); return Buffer(); }
    Buffer png; png_set_write_fn(png_ptr, &png, (void*)_libpng_mem_write_cb, NULL);
    
    // w, h, bit_depth = 8, color_type = 6 (PNG_COLOR_TYPE_RGBA), interlace = 0, compression = 0, filter = 0
    png_set_IHDR(png_ptr, info_ptr, (uint32_t)w, (uint32_t)h, 8, 6, 0, 0, 0); png_write_info(png_ptr, info_ptr);
    
    Array<unsigned char*> row_ptrs(h); for(int y=0; y<h; ++y){ row_ptrs[y] = (unsigned char*)&px[y*w]; }
    png_write_image(png_ptr, row_ptrs.data()); png_write_end(png_ptr, NULL); png_destroy_write_struct(&png_ptr, &info_ptr); return png; }

// ------ libjpeg: write ------
struct _JpegMemDestCtx { struct jpeg_destination_mgr pub; Buffer* buff; unsigned char* int_buff; size_t bufsize; }; // Структура-контекст для записи JPEG в Buffer

void Image::_libjpeg_init_destination(void* cinfo){}

int Image::_libjpeg_empty_output_buffer(void* cinfo){ struct jpeg_compress_struct* comp = (struct jpeg_compress_struct*)cinfo;
    _JpegMemDestCtx* ctx = (_JpegMemDestCtx*)comp->dest; ctx->buff->push(ctx->int_buff, ctx->bufsize);
    comp->dest->next_output_byte = ctx->int_buff; comp->dest->free_in_buffer = ctx->bufsize; return 1; }
	
void Image::_libjpeg_term_destination(void* cinfo){ struct jpeg_compress_struct* comp = (struct jpeg_compress_struct*)cinfo;
	struct jpeg_destination_mgr* dest = (struct jpeg_destination_mgr*)comp->dest;
	_JpegMemDestCtx* ctx = (_JpegMemDestCtx*)dest; size_t written = ctx->bufsize - dest->free_in_buffer;
    if(written>0) ctx->buff->push(ctx->int_buff, written); }

Buffer Image::toJPG(int quality) const { if(!_loadSOLibs() || !jpeg_CreateCompress || px.empty() || w <= 0 || h <= 0) return Buffer();
    struct jpeg_compress_struct cinfo; struct jpeg_error_mgr jerr; cinfo.err = (struct jpeg_error_mgr*)jpeg_std_error(&jerr);
	//Захардкодил 520 байт для версии 6b
    jpeg_CreateCompress(&cinfo, 62, 520); //(size_t)sizeof(struct jpeg_compress_struct)

    Buffer res; const size_t buffSz = 4096; unsigned char internal_buf[buffSz];
    _JpegMemDestCtx my_dest; my_dest.buff = &res; my_dest.int_buff = internal_buf; my_dest.bufsize = buffSz;

    my_dest.pub.init_destination = _libjpeg_init_destination;
    my_dest.pub.empty_output_buffer = _libjpeg_empty_output_buffer;
    my_dest.pub.term_destination = _libjpeg_term_destination;
    my_dest.pub.next_output_byte = internal_buf;
    my_dest.pub.free_in_buffer = buffSz;
    cinfo.dest = (struct jpeg_destination_mgr*)&my_dest;

    cinfo.image_width = w; cinfo.image_height = h; cinfo.input_components = 3; cinfo.in_color_space = 2;
    jpeg_set_defaults(&cinfo); jpeg_set_quality(&cinfo, quality, 1); jpeg_start_compress(&cinfo, 1); Buffer row_rgb(w*3);
	
	while(cinfo.next_scanline < (unsigned int)cinfo.image_height){ int y = cinfo.next_scanline; const PixelT* src_row = &px[y*w]; unsigned char* dst_row = row_rgb.data();
        for(int x = 0; x < w; ++x){ dst_row[x*3+0] = src_row[x].r; dst_row[x*3+1] = src_row[x].g; dst_row[x*3+2] = src_row[x].b; }
        unsigned char* row_ptr[1]; row_ptr[0] = dst_row; jpeg_write_scanlines(&cinfo, row_ptr, 1); }
    jpeg_finish_compress(&cinfo); jpeg_destroy_compress(&cinfo); return res; }
}
#endif // === For Linux: use libpng + libjpeg ===
