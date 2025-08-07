#include <stdlib.h>
namespace ncpp { void print(const char *cptr); const char* version(); } //Only declaration.
extern "C" void* _dbg_malloc(size_t size){ ncpp::print("(#) malloc(size_t) called.\n"); return malloc(size); }
extern "C" void _dbg_free(void* p){ ncpp::print("(#) free(void*) called.\n"); free(p); }

#define malloc _dbg_malloc
#define free _dbg_free

//void* operator new(size_t size) noexcept { ncpp::print("(#) operator new(size_t) called.\n"); void* ptr = malloc(size); if(ptr!=NULL) return ptr; else { ncpp::print("(!) operator new(size_t) error: Out of memory"); exit(1); } }
//void operator delete(void* ptr) noexcept { ncpp::print("(#) operator delete(void*) called.\n"); free(ptr); }
