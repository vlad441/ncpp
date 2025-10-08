#include <stdlib.h>
namespace ncpp { void print(const char *cptr); const char* version(); template<typename I> char dtos(char str[27], I num, char sep); } //Only declaration.
//extern "C" void* _dbg_malloc(size_t size){ ncpp::print("(#) malloc(size_t) called.\n"); return malloc(size); }
//extern "C" void _dbg_free(void* p){ ncpp::print("(#) free(void*) called.\n"); free(p); }

volatile unsigned long long _dbg_allocs=0, _dbg_deallocs=0;
extern "C" void* _dbg_malloc(size_t size){ ++_dbg_allocs; return malloc(size); }
extern "C" void _dbg_free(void* p){ ++_dbg_deallocs; free(p); }
#define malloc _dbg_malloc
#define free _dbg_free

void _dbg_ShowAllocs(){ char num[27]; 
	if(_dbg_allocs>0){ ncpp::print("(#ALLOC DBG) malloc cnt: "); 
		ncpp::dtos(num, _dbg_allocs, '\''); ncpp::print(num); ncpp::print("\n"); _dbg_allocs=0; }
	if(_dbg_deallocs>0){ ncpp::print("(#ALLOC DBG) free cnt: ");
		ncpp::dtos(num, _dbg_deallocs, '\''); ncpp::print(num); ncpp::print("\n"); _dbg_deallocs=0; } }

//void* operator new(size_t size) noexcept { ncpp::print("(#) operator new(size_t) called.\n"); void* ptr = malloc(size); if(ptr!=NULL) return ptr; else { ncpp::print("(!) operator new(size_t) error: Out of memory"); exit(1); } }
//void operator delete(void* ptr) noexcept { ncpp::print("(#) operator delete(void*) called.\n"); free(ptr); }
