// #include <string.h> // strlen, memcpy, etc.
// #include <stdlib.h> // C lib
#include <stddef.h> // for size_t
#include <limits.h>

#if !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L)
#define noexcept

#ifndef bool
#define bool char //typedef unsigned char bool
#endif

#ifndef true
#define true 1
#define false 0
#endif

#endif //__STDC_VERSION__

// #define Array _c_Array //#undef Array
// #define String _c_String //#undef String
// #define Buffer
// #define HashMap

//enum Mode { HEAP, STACK, STACK_ONLY };
typedef struct { void* ptr; size_t len; size_t msize; unsigned int szT; } Array;

#define _SSO_LEN 7
typedef struct { char* ptr; size_t len; size_t msize;
#if _SSO_LEN > 0
	char _sso[_SSO_LEN]; //bool _isSSO(){ return ptr==_sso; }
#endif
} String;

//typedef struct { char* ptr; size_t len; } CString; //typedef String Buffer; typedef Array HashMap;

#define _allocFN malloc //void* (*)(size_t)
#define _freeFN free //void (*)(void*)

// ================== Array ==================

void Array_init(Array* a, unsigned int szT){ a->ptr=NULL; a->len=0; a->msize=0; a->szT=szT; }
Array Array_new(unsigned int szT){ Array arr; Array_init(&arr, szT); return arr; }

void* Array_get(Array* this, size_t pos){ return (char*)this->ptr+pos*this->szT; }
size_t Array_size(Array* this){ return this->len; }
size_t Array_capacity(Array* this){ return this->msize; }

// size_t Array_stack(Array* a, void* ptr, size_t len, bool ronly=false);

size_t Array_indexOf(Array* a, void* pVal, size_t start);

void Array_reserve(Array* a, size_t sz);
void Array_resize(Array* a, size_t nsize);

//void Array_pushRaw(Array* a, void* ptr, size_t len);
void Array_push(Array* a, void* pVal);

void Array_shrink(Array* a);
void Array_free(Array* a);

void ArrayStr_free(Array* a);

// ================== String ==================

#if _SSO_LEN > 0
#define _String_isSSO(s) ((s)->ptr == (s)->_sso)
#define _String_initSSO(s) do { (s)->ptr = (s)->_sso; (s)->_sso[0]='\0'; s->msize=_SSO_LEN; } while(0)
#else
#define _String_isSSO(s) (0)
#define _String_initSSO(s) do { (s)->ptr = NULL; } while(0)
#endif

void String_copy(String* s, const char* c, size_t len){ String_reserve(s, len);
	if(s->ptr && c){ memcpy(s->ptr, c, len); } s->len = len; if(s->ptr) s->ptr[s->len] = '\0'; }

void String_init(String* s, const char* c){ s->ptr=NULL; s->len=0; s->msize=0; _String_initSSO(s); if(c) String_copy(s, c, strlen(c)); }
String String_new(const char* c){ String str; String_init(&str, c); return str; }

char* String_get(String* this, size_t pos){ return (char*)this->ptr+pos; }
size_t String_size(String* this){ return this->len; }
size_t String_capacity(String* this){ return this->msize; }
void String_set(String* s, const char* c){ String_copy(s, c, strlen(c)); }

// size_t String_stack(String* s, void* ptr, size_t len, bool ronly=false);

size_t String__indexOf(String* s, const char* delim, size_t len, size_t start);
// size_t String__indexOf(String* s, const char* delim, size_t len, size_t start){ if(!delim||len==0||_len < len||start >= _len) return -1;
	// for(size_t i = start; i <= _len - len; ++i){ size_t j = 0; 
		// while(j < len && _ptr[i + j] == delim[j]){ ++j; } if(j==len){ return i; } } return -1; }
// size_t String_indexOf(String* s, const char* delim){ return String__indexOf(s, delim, strlen(delim), 0); }

void String_reserve(String* s, size_t sz){ if(sz <= s->msize||sz<=_SSO_LEN) return;
    sz++; char* nptr = (char*)_allocFN(sz); if(!nptr) return;
    if(s->len>0 && s->ptr) memcpy(nptr, s->ptr, s->len);
    if(!_String_isSSO(s) && s->ptr) _freeFN(s->ptr);
    s->ptr = nptr; s->msize = sz; s->ptr[s->len] = '\0'; }
void String_resize(String* s, size_t len){ String_reserve(s, len+1); s->len=len; s->ptr[s->len]='\0'; return; }

void String__push(String* s, const char* cptr, size_t len);
// void String_push(String* s, const char* c){ String__push(s, c, strlen(c)); }
// void String_pushC(String* s, char c){ String__push(s, &c, 1); }
// void String_pushS(String* s, String* dst){ String__push(s, dst->ptr, dst->len); }

void String__split(String* s, Array* res, const char* delim, size_t len, int maxparts);
// void String_split(String* s, Array* res, const char* delim){ String__split(s, res, delim, strlen(delim), -1); }
void String__splitTokens(String* s, Array* res, int limit, const char* chars);
// void String_splitTokens(String* s, Array* res, int limit){ String__splitTokens(s, res, limit, " \t\r\n\f\v"); }

bool String__startsWith(String* s, const char* prefix, size_t len);
// bool String_startsWith(String* s, const char* prefix){ return String__startsWith(s, prefix, strlen(prefix)); }
// bool String_startsWithS(String* s, String* dst){ return String__startsWith(s, dst->ptr, dst->len); }

bool String__endsWith(String* s, const char* suffix, size_t len);
// bool String_endsWith(String* s, const char* prefix){ return String__endsWith(s, prefix, strlen(prefix)); }
// bool String_endsWithS(String* s, String* dst){ return String__endsWith(s, dst->ptr, dst->len); }

void String_shrink(String* s);
void String_free(String* s){ if(!_String_isSSO(s) && s->ptr) _freeFN(s->ptr); }

#undef _SSO_LEN
#undef _String_isSSO
#undef _String_initSSO

#undef _allocFN
#undef _freeFN