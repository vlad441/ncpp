#ifndef NCPP_USESTL
void* operator new(size_t size, void* ptr) noexcept { return ptr; } //Определение для placement new
#endif
namespace ncpp {

template <typename T> void reverse(T* begin, T* end){ T c; while(begin < end){ c = *begin; *begin++ = *--end; *end = c; } }
template <typename N> N min(N num1, N num2){ return num1<=num2?num1:num2; }
template <typename N> N max(N num1, N num2){ return num1>=num2?num1:num2; }

template <typename T> void swap(T& a, T& b){ T c=a; a=b; b=c; }
//template <typename T> void move(T& a, T& b){ T::move(a, b); }
//template <typename T> void sort(T* begin, T* end, char order=1);
//template <typename T> void sort(T* begin, T* end, void(*)(T*, T*));
//template <typename T> size_t _getSzByIter(const T* begin, const T* end){ return (end>begin)?(end-begin):0; }

/*template<typename I>
char _dtos(char str[27], I num, char sep) noexcept { //if(sizeof(I)>8) return 0; //max "18446744073709551615\0" - 21 ch; "18'446'744'073'709'551'615\0" - 27 ch
	int i=0; bool negate=false; if(num < 0){ num = -num; negate=true; }
	do { if(sep!=0&&(i+1)%4==0){ str[i++]=sep; } str[i++] = num % 10 + '0'; } while ((num /= 10) > 0); if(negate) str[i++] = '-';
	reverse(str, str+i); str[i] = '\0'; return i; };
	
template<typename I> void _printNum(I val, char sep=0){ char num[27]; _dtos(num, val, sep); print(num); }
template<typename I> void _printNum128(I val, char sep=0){ char num[52]; _dtos(num, val, sep); print(num); } //max "340'282'366'920'938'463'463'374'607'431'768'211'455\0" - 52 ch */

template <typename K, typename V>
struct Pair { K first; V second; Pair(const K& k=K(), const V& v=V()) : first(k), second(v){}
	K& key(){ return first; } const K& key() const { return first; }
	V& value(){ return second; } const V& value() const { return second; } };

void (*onExcept)(const char* cstr, int errlvl, int type) = NULL;
enum ErrorType { ERR_OTHER, ERR_OOM, ERR_RANGE, ERR_ALLOC };
void Except(const char* cstr, int errlvl=1, int type=0){ enum ErrorLvl { ERR_FATAL, ERR_ERROR, ERR_WARNING, ERR_INFO };
	if(onExcept){ onExcept(cstr, errlvl, type); return; } const char* prefix = "[?]";
	switch(errlvl){
		case ERR_FATAL:   prefix = "(!!) FATAL: "; break;
		case ERR_ERROR:   prefix = "(E!) "; break;
		case ERR_WARNING: prefix = "(!) ";  break;
        case ERR_INFO:    prefix = "(i) ";  break; } _pErr(prefix); _pErr(cstr); if(errlvl==0) exit(1); }
	
struct String; void Except(const String& s, int errlvl=1, int type=0);
};