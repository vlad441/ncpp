// C stdlib implementation example
#include <stdint.h>
extern "C" size_t strlen(const char* c);

extern "C" void* memcpy(void* dst, void* src, size_t size){
	char* dst8 = (char*)dst; char* src8 = (char*)src;

	size_t qwords = size >> 3; 
	size_t aligned_size = qwords << 3;
	size_t remaining_size = size - aligned_size;
	while(qwords--) { *(uint64_t*)dst8 = *(uint64_t*)src8; dst8 += 8; src8 += 8; }
	
	while(remaining_size--) { *dst8++ = *src8++; } return dst;
}
