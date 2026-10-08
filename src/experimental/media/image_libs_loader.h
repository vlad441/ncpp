// For Linux: libpng + libjpeg loader
// --- Объявления libpng ---
#ifndef PNG_COLOR_TYPE_PALETTE
#define PNG_COLOR_TYPE_PALETTE 3
#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_RGB 2
#define PNG_INFO_tRNS 0x10
#define PNG_FILLER_AFTER 1
#endif

// --- Объявления libjpeg --- //https://refspecs.linuxfoundation.org/LSB_3.1.1/LSB-Desktop-generic/LSB-Desktop-generic/libjpeg-ddefs.html
//#define JPEG_LIB_VERSION 62 // Version 6b
struct jpeg_error_mgr { void (*error_exit) (void* cinfo); void (*emit_message) (void* cinfo, int msg_level); void (*output_message) (void* cinfo);
    void (*format_message) (void* cinfo, char* buffer); int reset_error_count; int msg_code; char pad[128]; // заглушка для гарантий мин. размера
};
struct jpeg_source_mgr { const unsigned char* next_input_byte; size_t bytes_in_buffer; void (*init_source)(void* cinfo);
    int (*fill_input_buffer)(void* cinfo); void (*skip_input_data)(void* cinfo, long int num_bytes);
    int (*resync_to_restart)(void* cinfo, int desired); void (*term_source)(void* cinfo);
};
struct jpeg_destination_mgr { unsigned char* next_output_byte; size_t free_in_buffer; void (*init_destination)(void* cinfo);
    int (*empty_output_buffer)(void* cinfo); void(*term_destination)(void* cinfo); 
};
struct jpeg_memory_mgr; struct jpeg_progress_mgr;
#define jpeg_common_fields \
  struct jpeg_error_mgr* err; struct jpeg_memory_mgr* mem; struct jpeg_progress_mgr* progress; \
  void* client_data; bool is_decompressor; int global_state
 
#define PAD_SZ 632 //size_t decompress_test_sizes[] = { 632 };
struct jpeg_decompress_struct { jpeg_common_fields; struct jpeg_source_mgr* src; int image_width; int image_height;
	int num_components; char pad[PAD_SZ]; // заглушка для полей структуры версий 6b-9x
};
//size_t compress_test_sizes[] = { 520 };
struct jpeg_compress_struct { jpeg_common_fields; struct jpeg_destination_mgr* dest; int image_width; int image_height; 
	int input_components; int in_color_space; double input_gamma; int data_precision; int num_components; int jpeg_color_space;
	// ... мусорные поня, но необходимы для точного смещения next_scanline ...
    void* comp_info; void* quant_tbl_ptrs[4]; void* dc_huff_tbl_ptrs[4]; void* ac_huff_tbl_ptrs[4];
    unsigned char arith_dc_L[16]; unsigned char arith_dc_U[16]; unsigned char arith_ac_K[16]; int num_scans; char pad_align2[4];
    void* scan_info; int raw_data_in; int arith_code; int optimize_coding; int CCIR601_sampling; int smoothing_factor; int dct_method;
    unsigned int restart_interval; int restart_in_rows; int write_JFIF_header; unsigned char JFIF_major_version; unsigned char JFIF_minor_version;
	unsigned char density_unit; char pad_align3[1]; unsigned short X_density; unsigned short Y_density; int write_Adobe_marker;
    unsigned int next_scanline; // 4 байта (смещение 304)
	char pad[PAD_SZ];
};
#undef PAD_SZ
#undef jpeg_common_fields


#ifdef NCPP_IMAGE_LNKLIB // Using linker -lpng + -ljpeg
extern "C" {
    // --- Объявления libpng ---
    void* png_create_read_struct(const char* ver, void* error_ptr, void* error_fn, void* warn_fn);
    void* png_create_write_struct(const char* ver, void* error_ptr, void* error_fn, void* warn_fn);
    void* png_create_info_struct(void* png_ptr);
    void  png_set_read_fn(void* png_ptr, void* io_ptr, void* read_data_fn);
    void  png_set_write_fn(void* png_ptr, void* io_ptr, void* write_data_fn, void* output_flush_fn);
    void  png_read_info(void* png_ptr, void* info_ptr);
    void  png_write_info(void* png_ptr, void* info_ptr);
    void  png_get_IHDR(void* png_ptr, void* info_ptr, uint32_t* w, uint32_t* h, int* bit, int* color, int* interl, int* comp, int* filt);
    void  png_set_IHDR(void* png_ptr, void* info_ptr, uint32_t w, uint32_t h, int bit, int color, int interl, int comp, int filt);
    void  png_read_image(void* png_ptr, unsigned char** row_pointers);
    void  png_write_image(void* png_ptr, unsigned char** row_pointers);
    void  png_write_end(void* png_ptr, void* info_ptr);
    void  png_destroy_read_struct(void** png_ptr, void** info_ptr, void** end_ptr);
    void  png_destroy_write_struct(void** png_ptr, void** info_ptr);
	
    void* png_get_io_ptr(void* png_ptr);
    void  png_set_palette_to_rgb(void* png_ptr);
    void  png_set_expand_gray_1_2_4_to_8(void* png_ptr);
    int   png_get_valid(void* png_ptr, void* info_ptr, uint32_t flag);
    void  png_set_tRNS_to_alpha(void* png_ptr);
    void  png_set_strip_16(void* png_ptr);
    void  png_set_filler(void* png_ptr, uint32_t filler, int flags);
    void  png_read_update_info(void* png_ptr, void* info_ptr);

    // --- Объявления libjpeg (декомпрессия) ---
    void  jpeg_CreateDecompress(void* cinfo, int version, size_t structsize);
    int   jpeg_read_header(void* cinfo, int require_image);
    int   jpeg_start_decompress(void* cinfo);
    unsigned int jpeg_read_scanlines(void* cinfo, unsigned char** scanlines, unsigned int max_lines);
    int   jpeg_finish_decompress(void* cinfo);
    void  jpeg_destroy_decompress(void* cinfo);
    void  jpeg_mem_src(void* cinfo, const unsigned char* inbuffer, unsigned long insize);
    void* jpeg_std_error(void* err);

    // --- Объявления libjpeg (компрессия) ---
    void  jpeg_CreateCompress(void* cinfo, int version, size_t structsize);
    void  jpeg_set_defaults(void* cinfo);
    void  jpeg_set_quality(void* cinfo, int quality, int baseline);
    void  jpeg_start_compress(void* cinfo, int write_all_tables);
    unsigned int jpeg_write_scanlines(void* cinfo, unsigned char** scanlines, unsigned int num_lines);
    void  jpeg_finish_compress(void* cinfo);
    void  jpeg_destroy_compress(void* cinfo);
    void  jpeg_mem_dest(void* cinfo, unsigned char** outbuffer, unsigned long* outsize);
}
bool ncpp::Image::_loadSOLibs(){ return 1; }
#else // === Using dynamic load in runtime: dlsym ===
// --- Сигнатуры libpng ---
typedef void* (*PFN_png_create_read_struct)(const char* ver, void* error_ptr, void* error_fn, void* warn_fn);
typedef void* (*PFN_png_create_write_struct)(const char* ver, void* error_ptr, void* error_fn, void* warn_fn);
typedef void* (*PFN_png_create_info_struct)(void* png_ptr);
typedef void  (*PFN_png_set_read_fn)(void* png_ptr, void* io_ptr, void* read_data_fn);
typedef void  (*PFN_png_set_write_fn)(void* png_ptr, void* io_ptr, void* write_data_fn, void* output_flush_fn);
typedef void  (*PFN_png_read_info)(void* png_ptr, void* info_ptr);
typedef void  (*PFN_png_write_info)(void* png_ptr, void* info_ptr);
typedef void  (*PFN_png_get_IHDR)(void* png_ptr, void* info_ptr, uint32_t* w, uint32_t* h, int* bit, int* color, int* interl, int* comp, int* filt);
typedef void  (*PFN_png_set_IHDR)(void* png_ptr, void* info_ptr, uint32_t w, uint32_t h, int bit, int color, int interl, int comp, int filt);
typedef void  (*PFN_png_read_image)(void* png_ptr, unsigned char** row_pointers);
typedef void  (*PFN_png_write_image)(void* png_ptr, unsigned char** row_pointers);
typedef void  (*PFN_png_write_end)(void* png_ptr, void* info_ptr);
typedef void  (*PFN_png_destroy_read_struct)(void** png_ptr, void** info_ptr, void** end_ptr);
typedef void  (*PFN_png_destroy_write_struct)(void** png_ptr, void** info_ptr);

typedef void* (*PFN_png_get_io_ptr)(void* png_ptr);
typedef void  (*PFN_png_set_palette_to_rgb)(void* png_ptr);
typedef void  (*PFN_png_set_expand_gray_1_2_4_to_8)(void* png_ptr);
typedef int   (*PFN_png_get_valid)(void* png_ptr, void* info_ptr, uint32_t flag);
typedef void  (*PFN_png_set_tRNS_to_alpha)(void* png_ptr);
typedef void  (*PFN_png_set_strip_16)(void* png_ptr);
typedef void  (*PFN_png_set_filler)(void* png_ptr, uint32_t filler, int flags);
typedef void  (*PFN_png_read_update_info)(void* png_ptr, void* info_ptr);

// --- Сигнатуры libjpeg (декомпрессия) ---
typedef void  (*PFN_jpeg_CreateDecompress)(void* cinfo, int version, size_t structsize);
typedef int   (*PFN_jpeg_read_header)(void* cinfo, int require_image);
typedef int   (*PFN_jpeg_start_decompress)(void* cinfo);
typedef unsigned int (*PFN_jpeg_read_scanlines)(void* cinfo, unsigned char** scanlines, unsigned int max_lines);
typedef int   (*PFN_jpeg_finish_decompress)(void* cinfo);
typedef void  (*PFN_jpeg_destroy_decompress)(void* cinfo);
typedef void  (*PFN_jpeg_mem_src)(void* cinfo, const unsigned char* inbuffer, unsigned long insize);
typedef void* (*PFN_jpeg_std_error)(void* err);

// --- Сигнатуры libjpeg (компрессия) ---
typedef void  (*PFN_jpeg_CreateCompress)(void* cinfo, int version, size_t structsize);
typedef void  (*PFN_jpeg_set_defaults)(void* cinfo);
typedef void  (*PFN_jpeg_set_quality)(void* cinfo, int quality, int baseline);
typedef void  (*PFN_jpeg_start_compress)(void* cinfo, int write_all_tables);
typedef unsigned int (*PFN_jpeg_write_scanlines)(void* cinfo, unsigned char** scanlines, unsigned int num_lines);
typedef void  (*PFN_jpeg_finish_compress)(void* cinfo);
typedef void  (*PFN_jpeg_destroy_compress)(void* cinfo);
typedef void  (*PFN_jpeg_mem_dest)(void* cinfo, unsigned char** outbuffer, unsigned long* outsize);

// --- Указатели libpng ---
PFN_png_create_read_struct png_create_read_struct = nullptr;
PFN_png_create_write_struct png_create_write_struct = nullptr;
PFN_png_create_info_struct png_create_info_struct = nullptr;
PFN_png_set_read_fn png_set_read_fn = nullptr;
PFN_png_set_write_fn png_set_write_fn = nullptr;
PFN_png_read_info png_read_info = nullptr;
PFN_png_write_info png_write_info = nullptr;
PFN_png_get_IHDR png_get_IHDR = nullptr;
PFN_png_set_IHDR png_set_IHDR = nullptr;
PFN_png_read_image png_read_image = nullptr;
PFN_png_write_image png_write_image = nullptr;
PFN_png_write_end png_write_end = nullptr;
PFN_png_destroy_read_struct png_destroy_read_struct = nullptr;
PFN_png_destroy_write_struct png_destroy_write_struct = nullptr;

PFN_png_get_io_ptr png_get_io_ptr = nullptr;
PFN_png_set_palette_to_rgb png_set_palette_to_rgb = nullptr;
PFN_png_set_expand_gray_1_2_4_to_8 png_set_expand_gray_1_2_4_to_8 = nullptr;
PFN_png_get_valid png_get_valid = nullptr;
PFN_png_set_tRNS_to_alpha png_set_tRNS_to_alpha = nullptr;
PFN_png_set_strip_16 png_set_strip_16 = nullptr;
PFN_png_set_filler png_set_filler = nullptr;
PFN_png_read_update_info png_read_update_info = nullptr;

// --- Указатели libjpeg ---
PFN_jpeg_CreateDecompress jpeg_CreateDecompress = nullptr;
PFN_jpeg_mem_src jpeg_mem_src = nullptr;
PFN_jpeg_read_header jpeg_read_header = nullptr;
PFN_jpeg_start_decompress jpeg_start_decompress = nullptr;
PFN_jpeg_read_scanlines jpeg_read_scanlines = nullptr;
PFN_jpeg_finish_decompress jpeg_finish_decompress = nullptr;
PFN_jpeg_destroy_decompress jpeg_destroy_decompress = nullptr;
PFN_jpeg_std_error jpeg_std_error = nullptr;

PFN_jpeg_CreateCompress jpeg_CreateCompress = nullptr;
PFN_jpeg_mem_dest jpeg_mem_dest = nullptr;
PFN_jpeg_set_defaults jpeg_set_defaults = nullptr;
PFN_jpeg_set_quality jpeg_set_quality = nullptr;
PFN_jpeg_start_compress jpeg_start_compress = nullptr;
PFN_jpeg_write_scanlines jpeg_write_scanlines = nullptr;
PFN_jpeg_finish_compress jpeg_finish_compress = nullptr;
PFN_jpeg_destroy_compress jpeg_destroy_compress = nullptr;

#define IMG_DLOAD_FUNC(handle, name) \
    name = (PFN_##name)DGET_ADDR(handle, #name); if(!name){ Except("Image Libs Loader: Failed to load function: " #name "\n"); return 0; }

bool ncpp::Image::_loadSOLibs(){ static bool loaded = false; if(loaded) return true;
	// --- Загрузка libpng ---
    void* pngLib = DLIB_LOAD("libpng16.so.16"); if(!pngLib) pngLib = DLIB_LOAD("libpng12.so.0"); if(!pngLib) pngLib = DLIB_LOAD("libpng.so");
	if(!pngLib){ Except("Image Libs Loader: Failed load lib: libpng.so\n"); }
    if(pngLib){
        IMG_DLOAD_FUNC(pngLib, png_create_read_struct);
        IMG_DLOAD_FUNC(pngLib, png_create_write_struct);
        IMG_DLOAD_FUNC(pngLib, png_create_info_struct);
        IMG_DLOAD_FUNC(pngLib, png_set_read_fn);
        IMG_DLOAD_FUNC(pngLib, png_set_write_fn);
        IMG_DLOAD_FUNC(pngLib, png_read_info);
        IMG_DLOAD_FUNC(pngLib, png_write_info);
        IMG_DLOAD_FUNC(pngLib, png_get_IHDR);
        IMG_DLOAD_FUNC(pngLib, png_set_IHDR);
        IMG_DLOAD_FUNC(pngLib, png_read_image);
        IMG_DLOAD_FUNC(pngLib, png_write_image);
        IMG_DLOAD_FUNC(pngLib, png_write_end);
        IMG_DLOAD_FUNC(pngLib, png_destroy_read_struct);
        IMG_DLOAD_FUNC(pngLib, png_destroy_write_struct);
		
		IMG_DLOAD_FUNC(pngLib, png_get_io_ptr);
		IMG_DLOAD_FUNC(pngLib, png_set_palette_to_rgb);
		IMG_DLOAD_FUNC(pngLib, png_set_expand_gray_1_2_4_to_8);
		IMG_DLOAD_FUNC(pngLib, png_get_valid);
		IMG_DLOAD_FUNC(pngLib, png_set_tRNS_to_alpha);
		IMG_DLOAD_FUNC(pngLib, png_set_strip_16);
		IMG_DLOAD_FUNC(pngLib, png_set_filler);
		IMG_DLOAD_FUNC(pngLib, png_read_update_info);
    }
    // --- Загрузка libjpeg ---
    void* jpgLib = DLIB_LOAD("libjpeg.so.62"); if(!jpgLib) jpgLib = DLIB_LOAD("libjpeg.so.8"); if(!jpgLib) jpgLib = DLIB_LOAD("libjpeg.so");
	if(!jpgLib){ Except("Image Libs Loader: Failed load lib: libjpeg.so\n"); }
    if(jpgLib){
        IMG_DLOAD_FUNC(jpgLib, jpeg_CreateDecompress);
        IMG_DLOAD_FUNC(jpgLib, jpeg_mem_src);
        IMG_DLOAD_FUNC(jpgLib, jpeg_read_header);
        IMG_DLOAD_FUNC(jpgLib, jpeg_start_decompress);
        IMG_DLOAD_FUNC(jpgLib, jpeg_read_scanlines);
        IMG_DLOAD_FUNC(jpgLib, jpeg_finish_decompress);
        IMG_DLOAD_FUNC(jpgLib, jpeg_destroy_decompress);
        IMG_DLOAD_FUNC(jpgLib, jpeg_std_error);

        IMG_DLOAD_FUNC(jpgLib, jpeg_CreateCompress);
        IMG_DLOAD_FUNC(jpgLib, jpeg_mem_dest);
        IMG_DLOAD_FUNC(jpgLib, jpeg_set_defaults);
        IMG_DLOAD_FUNC(jpgLib, jpeg_set_quality);
        IMG_DLOAD_FUNC(jpgLib, jpeg_start_compress);
        IMG_DLOAD_FUNC(jpgLib, jpeg_write_scanlines);
        IMG_DLOAD_FUNC(jpgLib, jpeg_finish_compress);
        IMG_DLOAD_FUNC(jpgLib, jpeg_destroy_compress);
    }
	loaded = (jpgLib!=nullptr&&pngLib!=nullptr); return loaded; }
#undef IMG_DLOAD_FUNC
#endif //NCPP_IMAGE_LNKLIB