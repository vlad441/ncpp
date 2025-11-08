#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

#ifdef _WIN32
HANDLE __defheap = GetProcessHeap();
void* malloc(size_t size){ return HeapAlloc(__defheap, 0, size); }
void* realloc(void* ptr, size_t size){ return HeapReAlloc(__defheap, 0, ptr, size);  }
void free(void* ptr){ HeapFree(__defheap, 0, ptr); }
#endif

size_t strlen(const char* c){ if(!c) return 0; size_t len=0; while(*c!='\0'){ len++; c++; } return len; }

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

