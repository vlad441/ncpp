#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#ifndef _WIN32
#include "linux_syscalls.h"
#endif

#ifdef NCPP_NOCRT
int main();
#ifdef _WIN32
__declspec(dllimport) void __stdcall ExitProcess(unsigned int uExitCode) __attribute__((noreturn));
//__declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle); struct _OVERLAPPED;
//__declspec(dllimport) int __stdcall WriteFile(void* hFile, const void* lpBuffer, unsigned long nNumberOfBytesToWrite, unsigned long* lpNumberOfBytesWritten, struct _OVERLAPPED* lpOverlapped);
void _start(){ ExitProcess(main()); } //for 64 bit (В x32 линкер превращает _start() -> start(), т.е. нужно __start() -> _start())
void __main(){} //Заглушка для компилятора
#else
void _start(){ _syscall1(SYS_exit, main()); }
#endif
#endif // NCPP_NOCRT

#ifdef _WIN32 //Windows malloc()
__attribute__((noreturn, cold, unused)) void exit(int status){ ExitProcess(status); }

HANDLE __defheap = GetProcessHeap();
void* malloc(size_t size){ return HeapAlloc(__defheap, 0, size); }
void free(void* ptr){ HeapFree(__defheap, 0, ptr); }
void* realloc(void* ptr, size_t size){ return HeapReAlloc(__defheap, 0, ptr, size);  }
#else //Linux malloc()
void* _sysMalloc(size_t size, bool exec){ size_t sz = size+sizeof(size_t); 
	int prot=PROT_READ | PROT_WRITE; if(exec) prot |= PROT_EXEC;
	void* ptr = mmap(NULL, sz, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); if(ptr == MAP_FAILED){ return NULL; }
    *(size_t*)ptr = sz; return (char*)ptr+sizeof(size_t); }
void _sysFree(void* ptr){ if(!ptr) return; ptr = (char*)ptr-sizeof(size_t); munmap(ptr, *(size_t*)ptr); }
	
void* malloc(size_t size){ return _sysMalloc(size, 0); }
void free(void* ptr){ _sysFree(ptr); }

void* memcpy(void* dst, const void* src, size_t size);
void* realloc(void* ptr, size_t size){ if (!ptr) return malloc(size); if(size==0){ free(ptr); return NULL; }
    size_t* real_ptr = (size_t*)((char*)ptr-sizeof(size_t)); size_t old_sz = (*real_ptr)-sizeof(size_t);
    void* nptr = malloc(size); if(!nptr) return NULL;
	size_t copy_sz = (old_sz < size) ? old_size : size; memcpy(nptr, ptr, copy_sz); free(ptr); return nptr; }
#endif
// ========= string.h =========
size_t strlen(const char* c){ const char* с0=c; while(*с){ c++; } return (size_t)(c-с0); }
size_t strnlen(const char* c, size_t maxlen){ const char* с0=c; while(maxlen&&*c){ c++; maxlen--; } return (size_t)(c-с0); }

void* memcpy(void* dst, const void* src, size_t size){ char* dst8 = (char*)dst; char* src8 = (char*)src;
	#if _64BIT
	size_t qwords = size >> 3; size_t aligned_sz = qwords << 3;
	while(qwords--){ *(uint64_t*)dst8 = *(uint64_t*)src8; dst8+=8; src8+=8; }
	#else
	size_t dwords = size >> 2; size_t aligned_sz = dwords << 2; // Сколько 4-байтовых слов; Общий размер скопированного;
    while(dwords--){ *(uint32_t*)dst8 = *(uint32_t*)src8; dst8+=4; src8+=4; }
	#endif
	size_t remaining_size = size - aligned_sz; // Оставшийся хвост (0, 1, 2, или 3 байта)
	while(remaining_size--){ *dst8++ = *src8++; } return dst; }
	
void memset(void* ptr, unsigned char value, size_t n){ unsigned char* c=(unsigned char*)ptr; while(n){ *c=value; c++; n--; } }
char memcmp(const void* p1, const void* p2, size_t n);
char strcmp(const char* c1, const char* c2);
char strncmp(const char* c1, const char* c2, size_t n);
	
#ifdef __cplusplus
}
#endif

