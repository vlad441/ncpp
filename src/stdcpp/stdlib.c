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
	//#define STD_OUTPUT_HANDLE ((unsigned long)-11)
	__declspec(dllimport) void __stdcall ExitProcess(unsigned int uExitCode) __attribute__((noreturn));
	//__declspec(dllimport) int __stdcall WriteFile(void* hFile, const void* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD* lpNumberOfBytesWritten, void* lpOverlapped );
	//__declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle);
	void _start(){ ExitProcess(main()); } //WORKS ONLY 64 BIT
	void __main(){} //Заглушка для компилятора
#else
	void _start(){ _syscall1(SYS_exit, main()); }
#endif
#endif

#ifdef _WIN32 //Windows malloc()
HANDLE __defheap = GetProcessHeap();
void* malloc(size_t size){ return HeapAlloc(__defheap, 0, size); }
void* realloc(void* ptr, size_t size){ return HeapReAlloc(__defheap, 0, ptr, size);  }
void free(void* ptr){ HeapFree(__defheap, 0, ptr); }
#else //Linux malloc()
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

/* void* malloc(size_t size){ // Минимальная реализация malloc через mmap для Linux
    size_t total_size = size + sizeof(size_t);  // Добавляем место под хранение размера блока
	void* ptr = mmap(0, total_size, 3, 0x22, -1, 0); // mmap: addr=0, length, prot=READ|WRITE, flags=PRIVATE|ANONYMOUS, fd=-1, offset=0
    if(ptr == (void*)-1) return 0;

    *(size_t*)ptr = total_size; // Сохраняем размер для free
    return (void*)((char*)ptr + sizeof(size_t)); }
void free(void* ptr){ if(!ptr) return; void* start = (char*)ptr-sizeof(size_t); size_t size = *(size_t*)start; munmap(start, size); } */
#endif

size_t strlen(const char* c){ if(!c) return 0; size_t len=0; while(*c!='\0'){ len++; c++; } return len; }
size_t strnlen(const char* c, size_t maxlen){ size_t i=0; for(;i<maxlen&&c[i]!='\0'; i++); return i; }

void* memcpy(void* dst, const void* src, size_t size){ char* dst8 = (char*)dst; char* src8 = (char*)src;
	#if _64BIT
	size_t qwords = size >> 3; size_t aligned_sz = qwords << 3;
	while(qwords--){ *(uint64_t*)dst8 = *(uint64_t*)src8; dst8 += 8; src8 += 8; }
	#else
	size_t dwords = size >> 2; size_t aligned_sz = dwords << 2; // Сколько 4-байтовых слов; Общий размер скопированного;
    while(dwords--){ *(uint32_t*)dst8 = *(uint32_t*)src8; dst8 += 4; src8 += 4; }
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

