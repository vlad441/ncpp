#ifndef NCPP_H_STDINT
#define NCPP_H_STDINT

#if !defined(_64BIT) && (defined(__LP64__) || defined(_WIN64) || __SIZEOF_POINTER__ == 8)
#define _64BIT
#endif

#ifndef _SIZE_T_
#define _SIZE_T_
//typedef __SIZE_TYPE__ size_t;
#ifdef _64BIT
typedef unsigned long long size_t; //typedef long long ssize_t;
#else
typedef unsigned int size_t; //typedef int ssize_t;
#endif
#endif

//typedef __PTRDIFF_TYPE__  ptrdiff_t;
//typedef __UINTPTR_TYPE__  uintptr_t;
//typedef __INTPTR_TYPE__   intptr_t;

typedef char int8_t; typedef unsigned char uint8_t;
#if __SIZEOF_SHORT__ == 2
typedef short int16_t; typedef unsigned short uint16_t;
#elif __SIZEOF_INT__ == 2
typedef int int16_t; typedef unsigned int uint16_t;
#endif

#if __SIZEOF_INT__ == 4
typedef int int32_t; typedef unsigned int uint32_t;
#elif __SIZEOF_LONG__ == 4
typedef long int32_t; typedef unsigned long uint32_t;
#endif

#if __SIZEOF_LONG_LONG__ == 8
typedef long long int64_t; typedef unsigned long long uint64_t;
#elif __SIZEOF_LONG__ == 8
typedef long int64_t; typedef unsigned long uint64_t;
#endif

#if __SIZEOF_LONG_LONG__ == 16
typedef long long int128_t; typedef unsigned long long uint128_t;
#elif __SIZEOF_INT128__
typedef __int128 int128_t; typedef unsigned __int128 uint128_t;
#else
//#include "../experimental/__int128.cpp"
#endif

#if 
    
	

// ======== limits.h ========
#define UCHAR_MAX 255
#define CHAR_MAX 127
#define CHAR_MIN −128
#define USHRT_MAX 65535
#define SHRT_MAX 32767
#define SHRT_MIN −32768

#if __SIZEOF_INT__ == 4 //int
#define UINT_MAX 4294967295
#define INT_MAX 2147483647
#define INT_MIN −2147483648
#elif __SIZEOF_INT__ == 2
#define UINT_MAX USHRT_MAX
#define INT_MAX SHRT_MAX
#define INT_MIN SHRT_MIN
#endif

#if __SIZEOF_LONG__ == 8 //long
#define ULONG_MAX 18446744073709551615ULL
#define LONG_MAX 9223372036854775807LL
#define LONG_MIN −9223372036854775808LL
#elif __SIZEOF_LONG__ == 4
#define ULONG_MAX 4294967295
#define LONG_MAX 2147483647
#define LONG_MIN −2147483648
#endif

#if __SIZEOF_LONG_LONG__ == 8 //long long
#define ULLONG_MAX 18446744073709551615ULL
#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN −9223372036854775808LL
#elif __SIZEOF_LONG_LONG__ >= 16
#define ULLONG_MAX ~((unsigned long long)0)
#define LLONG_MAX (long long)(ULLONG_MAX >> 1)
#define LLONG_MIN (-LLONG_MAX - 1)
#endif
//__SIZEOF_POINTER__ == N - Размер указателя
//__SIZEOF_SHORT__ == N - Размер short
//__SIZEOF_INT__ == N - Размер int
//__SIZEOF_LONG__ == N - Размер long
//__SIZEOF_LONG_LONG__ == N - Размер long long

#endif