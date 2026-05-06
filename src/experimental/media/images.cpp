#ifdef _WIN32
#include <gdiplus.h>
//#include <shlwapi.h> // -lshlwapi, for SHCreateMemStream //extern "C" IStream* SHCreateMemStream(const BYTE *pInit, UINT cbInit);
#elif defined(NCPP_MEDIA_USE_STB)
// https://github.com/nothings/stb/blob/master/stb_image.h?
// #define STBI_ONLY_JPEG
// #define STBI_ONLY_PNG
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"
#endif

namespace ncpp { //BMP, PNG, JPEG
//Windows Dependencies: -lgdiplus -lole32
//Linux Dependencies: -lpng + -ljpeg  / (stb_image.h + stb_image_write.h)

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
	Buffer toBMP() const;
	Buffer toPNG() const;
	Buffer toJPG(int quality=90) const;
	bool saveBMP(const CString& path) const { return fs::writeFile(path, toBMP()); }
	bool savePNG(const CString& path) const { return fs::writeFile(path, toPNG()); }
	bool saveJPG(const CString& path, int quality=90) const { return fs::writeFile(path, toJPG(quality)); }
	#ifdef _WIN32
	static bool _GDI_inited; static ULONG_PTR gdiplusToken;
	Buffer _serialize(const GUID& fmtGuid, int quality=90) const;
	static bool _initGDI(){ if(_GDI_inited) return true; Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		if(GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL)!=S_OK){ return false; } _GDI_inited=true; return true; }
	static void _freeGDI(){ Gdiplus::GdiplusShutdown(gdiplusToken); }
	#else
	static Image _fromBMP(const Buffer& bmp);
	static Image _fromPNG(const Buffer& png);
	static Image _fromJPG(const Buffer& jpg);
	#endif
	#ifdef STBI_INCLUDE_STB_IMAGE_H
	static Image _loadWithStb(const Buffer& data);
	#elif !defined(_WIN32)
	static bool _loadSOLibs();
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
Image Image::from(const Buffer& fimg){ if(fimg.size()>54&&fimg[0]==0x42&&fimg[1]==0x4D) return _fromBMP(fimg);
	#ifdef STBI_INCLUDE_STB_IMAGE_H
	else if(fimg.size()>=67&&fimg[1]==0x50&&fimg[2]==0x4E&&fimg[3]==0x47) return _fromPNG(fimg);
	else if(fimg.size()>=125&&fimg[0]==0xFF&&fimg[1]==0xD8) return _fromJPG(fimg);
	#endif
	return Image(); }
	
Image Image::_fromBMP(const Buffer& bmp){ if(bmp.size()<54||bmp[0]!=0x42||bmp[1]!=0x4D) return Image();
	int w = bmp.readInt32LE(18); int h = bmp.readInt32LE(22); int bitCnt = bmp.readUInt16LE(28); if(bitCnt < 24) return Image();
    Image img(w, h); int rowSize = (w * 3 + 3) & ~3; int dtOffset = bmp.readUInt32LE(10); Array<PixelT>& px = img.px;
    for(int y = 0; y < h; y++){ int rowStart = dtOffset + (h-1-y)*rowSize;
        for(int x = 0; x < w; x++){ for(int i=0;i<3;i++){ (&px[y*w+x].r)[2-i] = bmp[rowStart+x*3+i]; if(bitCnt<=24) px[y*w+x].a=0xFF; } } } return img; }

#ifdef STBI_INCLUDE_STB_IMAGE_H
Image Image::_loadWithStb(const Buffer& data){ int w, h, channels;
    unsigned char* pixels = stbi_load_from_memory((const unsigned char*)data.data(), (int)data.size(), &w, &h, &channels, 4);
    if(!pixels) return Image(); Image img(w, h); memcpy(img.px.data(), pixels, w*h*4); stbi_image_free(pixels); return img; }

Image Image::_fromPNG(const Buffer& png){ return _loadWithStb(png); }
Image Image::_fromJPG(const Buffer& jpg){ return _loadWithStb(jpg); }
#endif

#ifdef INCLUDE_STB_IMAGE_WRITE_H
Buffer Image::toPNG() const { if(px.empty()||w<=0||h<=0) return ""; int len;
    unsigned char* png_data = stbi_write_png_to_mem((const unsigned char*)px.data(), w*4, w, h, 4, &len);
    if(!png_data) return ""; Buffer res(len); memcpy(res.data(), png_data, len); STBIW_FREE(png_data); return res; }
	
//Image::toJPG(int quality) const: stbi_write_jpg_to_mem not implemented in STB
void Image::_stbi_to_buffer(void* ctx, void* data, int size){ Buffer* bf = (Buffer*)ctx; 
	size_t len0 = bf->size(); bf->resize(len0+size); memcpy(bf->data()+len0, data, size); }

Buffer Image::toJPG(int quality) const { if (px.empty()) return ""; Buffer res;
    if(!stbi_write_jpg_to_func(_stbi_to_buffer, &res, w, h, 4, px.data(), quality)){ return ""; } return res; }
#endif

#endif
}

#if !defined(_WIN32) && !defined(STBI_INCLUDE_STB_IMAGE_H) // === use libpng + libjpeg ===
// === Сигнатуры libjpeg ===
typedef void (*PFN_jpeg_create_decompress)(void* cinfo, int version, size_t structsize);
typedef int  (*PFN_jpeg_read_header)(void* cinfo, int require_image);
typedef int  (*PFN_jpeg_start_decompress)(void* cinfo);
typedef int  (*PFN_jpeg_read_scanlines)(void* cinfo, unsigned char** scanlines, unsigned int max_lines);
typedef int  (*PFN_jpeg_finish_decompress)(void* cinfo);
typedef void (*PFN_jpeg_destroy_decompress)(void* cinfo);
typedef void (*PFN_jpeg_mem_src)(void* cinfo, const unsigned char* inbuffer, unsigned long insize);
typedef void* (*PFN_jpeg_std_error)(void* err);

// === Сигнатуры libpng ===
typedef void* (*PFN_png_create_read_struct)(const char* ver, void* error_ptr, void* error_fn, void* warn_fn);
typedef void* (*PFN_png_create_info_struct)(void* png_ptr);
typedef void  (*PFN_png_set_read_fn)(void* png_ptr, void* io_ptr, void* read_data_fn);
typedef void  (*PFN_png_read_info)(void* png_ptr, void* info_ptr);
typedef void  (*PFN_png_get_IHDR)(void* png_ptr, void* info_ptr, uint32_t* w, uint32_t* h, int* bit, int* color, int* interl, int* comp, int* filt);
typedef void  (*PFN_png_read_image)(void* png_ptr, unsigned char** row_pointers);
typedef void  (*PFN_png_destroy_read_struct)(void** png_ptr, void** info_ptr, void** end_ptr);

bool Image::_loadSOLibs(){ //if(_SO_loaded) return true;
	jpgLib = DLIB_LOAD("libjpeg.so.62"); if(!jpgLib) jpgLib = DLIB_LOAD("libjpeg.so.8"); if(!jpgLib) jpgLib = DLIB_LOAD("libjpeg.so");
	if(jpgLib){
		jpeg_create_decompress = (PFN_jpeg_create_decompress)DGET_ADDR(jpgLib, "jpeg_CreateDecompress"); if(!jpeg_create_decompress) return false;
		jpeg_mem_src = (PFN_jpeg_mem_src)DGET_ADDR(jpgLib, "jpeg_mem_src"); if(!jpeg_mem_src) return false;
		jpeg_read_header = (PFN_jpeg_read_header)DGET_ADDR(jpgLib, "jpeg_read_header"); if(!jpeg_read_header) return false;
		jpeg_start_decompress = (PFN_jpeg_start_decompress)DGET_ADDR(jpgLib, "jpeg_start_decompress"); if(!jpeg_start_decompress) return false;
		jpeg_read_scanlines = (PFN_jpeg_read_scanlines)DGET_ADDR(jpgLib, "jpeg_read_scanlines"); if(!jpeg_read_scanlines) return false;
		jpeg_std_error = (PFN_jpeg_std_error)DGET_ADDR(jpgLib, "jpeg_std_error"); if(!jpeg_std_error) return false;
	} return true;
}

//Image Image::_fromPNG(const Buffer& png);
//Image Image::_fromJPG(const Buffer& jpg);

//Buffer Image::toPNG() const;
//Buffer Image::toJPG(int quality) const;
#endif