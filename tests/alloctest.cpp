#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static inline uint64_t rdtsc(){
    uint32_t lo, hi;
    __asm__ volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo; }
	
#if _WIN32
#include <windows.h>
long long _FtToUnixTime(FILETIME& ft, char type='s'){ ULARGE_INTEGER t; t.LowPart = ft.dwLowDateTime; t.HighPart = ft.dwHighDateTime;
		if(type=='u'||type=='m'){ return ((t.QuadPart - 116444736000000000ULL)/(type=='u'?10:10000)); }
		else{ return ((t.QuadPart - 116444736000000000ULL)/10000000LL); } }
#else
#include <sys/time.h>
#endif
	
long long GetTimestamp(char type='m'){ long long timestamp = 0; // type: s - secs, m - msecs, u - usecs
#if defined(_WIN32) && _WIN32_WINNT >= 0x0602
	FILETIME ft; GetSystemTimePreciseAsFileTime(&ft); return _FtToUnixTime(ft, type);
#elif _WIN32
	FILETIME ft; GetSystemTimeAsFileTime(&ft); return _FtToUnixTime(ft, type);
#else
	struct timeval tv; gettimeofday(&tv, NULL); if(type=='u'){ timestamp = (long long)(tv.tv_sec) * 1000000 + tv.tv_usec; }
	else if(type=='m'){ timestamp = (long long)(tv.tv_sec)*1000 + tv.tv_usec/1000; }else{ timestamp = (long long)tv.tv_sec; }
#endif
	return timestamp; }

#define ITERATIONS 1000000
#define ARRAY_SIZE 4096
typedef char TEST_T;

int main(){ uint64_t start, end, time1; volatile TEST_T* ptr;

    // --- Stack ---
    time1 = GetTimestamp('u'); start = rdtsc();
    for(int i = 0; i < ITERATIONS; i++){ TEST_T stack_arr[ARRAY_SIZE]; stack_arr[0] = i; ptr = stack_arr; }
    end = rdtsc();
	printf("Stack - %.3f msecs\n", (GetTimestamp('u')-time1)/1000.0);
    printf("Stack - avg cycles: %llu\n", (end - start) / ITERATIONS);

    // --- Heap (malloc/free) ---
    time1 = GetTimestamp('u'); start = rdtsc();
    for (int i = 0; i < ITERATIONS; i++){
        TEST_T* heap_arr = (TEST_T*)malloc(ARRAY_SIZE * sizeof(TEST_T));
        if(!heap_arr) return 1; heap_arr[0] = i; free(heap_arr);
    }
    end = rdtsc();
	printf("Heap - %.3f msecs\n", (GetTimestamp('u')-time1)/1000.0);
    printf("Heap - avg cycles: %llu\n", (end - start) / ITERATIONS);

    return 0;
}