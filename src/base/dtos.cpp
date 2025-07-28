#include <ctype.h> // isdigit
namespace ncpp{
	bool IsInt(const char* cstr, size_t len){ size_t i=0; if(len>1&&cstr[0]=='-') i=1;
		for(; i < len; i++){ if(!isdigit(cstr[i])) return false; } return true; }
	bool IsFloat(const char* cstr, size_t len){ size_t i=0; bool fch=false; if(len>1&&cstr[0]=='-') i=1;
		for(; i < len; i++){ if(!isdigit(cstr[i])){ if(cstr[i]=='.'||cstr[i]=='e'){ if(fch){ fch=true; }else{ return false; } } } } return true; }
	
	#define MAX_SAFE_DOUBLE 9007199254740991
	#define D_PRECISION 17 //double - ~15–17; long double - ~21
	#define F_PRECISION 7 //float - ~6-9;
	template<typename I>
	char dtos(char str[27], I num, char sep) noexcept { if(sizeof(I)>8) return 0; //max "18446744073709551615\0" - 21 ch; "18'446'744'073'709'551'615\0" - 27 ch
		int i=0; bool negate=false; if(num < 0){ num = -num; negate=true; }
		do { if(sep!=0&&(i+1)%4==0){ str[i++]=sep; } str[i++] = num % 10 + '0'; } while ((num /= 10) > 0); if(negate) str[i++] = '-';
		reverse(str, str+i); str[i] = '\0'; return i; };
	template<typename I>
	String dtos(I num, char sep=0) noexcept { char str[27]; return String(str, dtos(str, num, sep)); };
	String dtos(double d, char precision=-1) noexcept { char str[27]; int i=0; bool negate=false; long long num=(long long)d;
		if(num < 0){ num = -num; negate=true; } do { str[i++] = num % 10 + '0'; } while ((num /= 10) > 0); if(negate) str[i++] = '-'; reverse(str, str+i);
		
		if(precision!=0){ str[i++] = '.'; num=(int)d; d-=num; if(precision==-1) precision=D_PRECISION;
			for(int j = 0; j<precision; j++){ char digit=(char)d;
				d *= 10; digit = (int)d; str[i++] = digit + '0'; d -= digit; } }
		return String(str, i); };
	String dtos(float num, char precision=-1) noexcept { return dtos((double)num, precision==-1?F_PRECISION:precision); };
	
	int stoin(const char* cstr, size_t len) noexcept { if(!IsInt(cstr, len)) return 0; return ::atoi(cstr); }
	int stoin(const char* cstr) noexcept { return stoin(cstr, strlen(cstr)); }
	int stoin(const String& s) noexcept { return stoin(s.c_str(), s.size()); }
	
	long long stolln(const char* cstr, size_t len) noexcept { if(!IsInt(cstr, len)) return 0; return ::atoll(cstr); }
	long long stolln(const char* cstr) noexcept { return stolln(cstr, strlen(cstr)); }
	long long stolln(const String& s) noexcept { return stolln(s.c_str(), s.size()); }
		
	double stodn(const char* cstr, size_t len) noexcept { if(!IsFloat(cstr, len)) return 0.0; return ::atof(cstr); }
	double stodn(const char* cstr) noexcept { return stodn(cstr, strlen(cstr)); }
	double stodn(const String& s) noexcept { return stodn(s.c_str(), s.size()); }
	
	float stofn(const char* cstr, size_t len) noexcept { return stodn(cstr, len); }
	float stofn(const char* cstr) noexcept { return stodn(cstr, strlen(cstr)); }
	float stofn(const String& s) noexcept { return stodn(s.c_str(), s.size()); }
	
	void print(long long v){ print(dtos(v)); }
	String& String::operator<<(long long num){ (*this)+=dtos(num); return *this; }
}
