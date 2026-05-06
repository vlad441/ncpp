#ifndef LLONG_MAX
#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX - 1LL)
#define ULLONG_MAX 18446744073709551615ULL
#endif

extern "C" inline size_t strnlen(const char* s, size_t maxlen){ size_t i; for(i=0; i<maxlen&&s[i]!='\0'; i++); return i; }