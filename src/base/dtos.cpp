#include <ctype.h> // isdigit
namespace ncpp{
	bool IsInt(const char* cstr, size_t len){ size_t i=0; if(len>1&&(cstr[0]=='-'||cstr[0]=='+')) i=1;
		for(; i < len; i++){ if(!isdigit(cstr[i])) return false; } return true; }
	bool IsFloat(const char* cstr, size_t len){ size_t i=0; unsigned char st=0; if(len>1&&(cstr[0]=='-'||cstr[0]=='+')){ ++i; ++st; }
		size_t digits=0; for(; i < len; i++){ if(isdigit(cstr[i])){ ++digits; continue; }
			if(cstr[i]=='.'&&st<2&&len>st+1U){ st=2; continue; }
			if(digits>0&&(cstr[i]=='e'||cstr[i]=='E')&&st<3&&len>st+1U&&len>i+1U){ st=3; continue; }
			if(st==3&&(cstr[i]=='-'||cstr[i]=='+')&&len>i+1U&&(cstr[i-1]=='e'||cstr[i-1]=='E')){ st=4; continue; } return false;
		} return true; } // leetcode passed :)
	bool IsHex(const char* cstr, size_t len){ size_t i=0; if(len>1&&(cstr[0]=='-'||cstr[0]=='+')) i=1;
		for(; i < len; i++){ if(!isxdigit(cstr[i])) return false; } return true; }
	
	template<typename T> struct num_limits { static const long long min = LLONG_MIN; }; template<> struct num_limits<int> { static const int min = INT_MIN; };
	template<> struct num_limits<short> { static const short min = SHRT_MIN; }; template<> struct num_limits<char> { static const char min = CHAR_MIN; };
	template<typename I>
	char dtos(char str[27], I num, char sep) noexcept { //if(sizeof(I)>8) return 0; //max "18446744073709551615\0" - 21 ch; "18'446'744'073'709'551'615\0" - 27 ch
		int i=0; bool negate=false; if(num < 0){ if((long long)num==num_limits<I>::min) ++num; num = -num; negate=true; }
		do { if(sep&&(i+1)%4==0){ str[i++]=sep; } str[i++] = num % 10 + '0'; } while ((num /= 10) > 0); if(negate) str[i++] = '-';
		reverse(str, str+i); str[i] = '\0'; return i; };
	template<typename I>
	String dtos(I num, char sep=0) noexcept { if(sizeof(I)>8) return "(!)(Err: sizeof(I)>8)"; char str[27]; return String(str, dtos(str, num, sep)); };
	
	#define MAX_SAFE_DOUBLE 9007199254740991
	#define D_PRECISION 17 //double - ~15–17; long double - ~21
	#define F_PRECISION 9 //float - ~6-9;
	#define D_NAN 0x7FF8000000000000ULL // EXP (11) = MAX, MANTISSA (52) = 1 (для Quiet NaN). Знак = 0.
	#define D_INF 0x7FF0000000000000ULL // EXP (11) = MAX, MANTISSA (52) = 0. Знак = 0.
	#define D_NINF 0xFFF0000000000000ULL // EXP (11) = MAX, MANTISSA (52) = 0. Знак = 1.
	
	char isNaN(double d){ union { double d; unsigned long long u; } dbits; dbits.d = d;
		unsigned long long exponent = (dbits.u & 0x7FF0000000000000ULL) >> 52; unsigned long long mantissa = (dbits.u & 0x000FFFFFFFFFFFFFULL);
		if(exponent == 0x7FF && mantissa != 0){ return 1; } //NaN 
		if(exponent == 0x7FF && mantissa == 0){ if(dbits.u & 0x8000000000000000ULL){ return 3; }else{ return 2; } } return 0; } //-Inf/Inf
	
	String dtos(double d, char precision=-1) noexcept { char str[27]; int i=0; bool negate=false; if(d<0){ d=-d; negate=true; } 
		switch(isNaN(d)){ case 1: return String("NaN", 3); case 2: return String("Inf", 3); case 3: return String("-Inf", 4); }
		long long num=(long long)d; do{ str[i++] = num % 10 + '0'; }while((num /= 10) > 0); if(negate) str[i++] = '-'; reverse(str, str+i);
		
		if(precision!=0){ str[i++] = '.'; num=(long long)d; d-=num; if(precision==-1) precision=D_PRECISION;
			for(int j = 0; j<precision; j++){ char digit=(char)d; d*=10; digit=(int)d; str[i++] = digit + '0'; d -= digit; } } return String(str, i); };
	String dtos(float num, char precision=-1) noexcept { return dtos((double)num, precision==-1?F_PRECISION:precision); };
	
	#define INTMAX_HALF10 INT_MAX/10
	#define INTMAX_MOD10 INT_MAX%10
	int stoin(const char* cstr, unsigned char len) noexcept { unsigned char i=0;
		while(i<len&&cstr[i]==' ') i++; if(!IsInt(cstr+i, len-i)) return 0; bool nsign=false;
		long long value = 0; if(cstr[i]=='-'){ i+=1; nsign=true; }else if(cstr[i]=='+') i+=1;
		for(; i < len; ++i){ unsigned char digit = cstr[i]-'0';
			if(value > INTMAX_HALF10||(value==INTMAX_HALF10&&digit > INTMAX_MOD10)){ return nsign?INT_MIN:INT_MAX; }
			value=value*10+digit; } if(nsign) return -value; return value; }
	int stoin(const char* cstr) noexcept { return stoin(cstr, strlen(cstr)); }
	int stoin(const String& s) noexcept { return stoin(s.c_str(), s.size()); }
	
	#define LLMAX_HALF10 LLONG_MAX/10
	#define LLMAX_MOD10 LLONG_MAX%10
	long long stolln(const char* cstr, unsigned char len) noexcept { unsigned char i=0;
		while(i<len&&cstr[i]==' ') i++; if(!IsInt(cstr+i, len-i)) return 0; bool nsign=false;
		long long value = 0; if(cstr[i]=='-'){ i+=1; nsign=true; }else if(cstr[i]=='+') i+=1;
		for(; i < len; ++i){ unsigned char digit = cstr[i]-'0';
			if(value > LLMAX_HALF10||(value==LLMAX_HALF10&&digit > LLMAX_MOD10)){ return nsign?LLONG_MIN:LLONG_MAX; }
			value=value*10+digit; } if(nsign) return -value; return value; }
	long long stolln(const char* cstr) noexcept { return stolln(cstr, strlen(cstr)); }
	long long stolln(const String& s) noexcept { return stolln(s.c_str(), s.size()); }
	
	#define ULLMAX_HALF10 ULLONG_MAX/10
	#define ULLMAX_MOD10 ULLONG_MAX%10
	unsigned long long stoulln(const char* cstr, unsigned char len) noexcept { unsigned char i=0; 
		while(i<len&&cstr[i]==' ') i++; if(!IsInt(cstr+i, len-i)) return 0; unsigned long long value = 0; i+=cstr[i]=='-'?1:cstr[i]=='+'?1:0; 
		for(; i < len; ++i){ unsigned char digit = cstr[i]-'0';
			if(value > ULLMAX_HALF10||(value==ULLMAX_HALF10&&digit > ULLMAX_MOD10)){ return ULLONG_MAX; }
			value=value*10+digit; } return value; }
	unsigned long long stoulln(const char* cstr) noexcept { return stoulln(cstr, strlen(cstr)); }
	unsigned long long stoulln(const String& s) noexcept { return stoulln(s.c_str(), s.size()); }
		
	double stodn(const char* cstr, size_t len) noexcept { if(!IsFloat(cstr, len)) return 0.0; return ::atof(cstr); }
	double stodn(const char* cstr) noexcept { return stodn(cstr, strlen(cstr)); }
	double stodn(const String& s) noexcept { return stodn(s.c_str(), s.size()); }
	
	float stofn(const char* cstr, size_t len) noexcept { return stodn(cstr, len); }
	float stofn(const char* cstr) noexcept { return stodn(cstr, strlen(cstr)); }
	float stofn(const String& s) noexcept { return stodn(s.c_str(), s.size()); }
	
	void print(long long v){ print(dtos(v)); }
	//void print(unsigned long long v){ print(dtos(v)); }
	String& String::operator<<(long long num){ (*this)+=dtos(num); return *this; }
}
