namespace ncpp {
#ifdef _WIN32
	#include <wchar.h> // for wcslen()
	struct _WString : Array<wchar_t> { _WString() : Array(){}; 
		_WString(const wchar_t* wptr, size_t len) : Array(wptr, wptr+len){};
		_WString(const wchar_t* wptr) : Array(wptr, wptr+wcslen(wptr)){};
		_WString(const void* begin, const void* end) : Array((const wchar_t*)begin, (const wchar_t*)end){};
		_WString(size_t len, wchar_t v) : Array(len, v){}
		_WString(size_t len) : Array(len){}
		const wchar_t* c_str() const { return _ptr; } };

	long long _FtToUnixTime(FILETIME& ft, char type='s'){ ULARGE_INTEGER t; t.LowPart = ft.dwLowDateTime; t.HighPart = ft.dwHighDateTime;
		if(type=='u'||type=='m'){ return ((t.QuadPart - 116444736000000000ULL)/(type=='u'?10:10000)); }
		else{ return ((t.QuadPart - 116444736000000000ULL)/10000000LL); } }
	String _toUTF8(const wchar_t* wptr, size_t len, UINT codep=CP_UTF8){ size_t utf8Len = WideCharToMultiByte(codep, 0, wptr, len, NULL, 0, NULL, NULL);
		if (utf8Len == 0){ print("Error in _toUTF8: "); print(dtos(GetLastError())); print("\n"); return ""; }
		String utf8Str(utf8Len); WideCharToMultiByte(codep, 0, wptr, len, utf8Str.data(), utf8Len, NULL, NULL); return utf8Str; }
	String _toUTF8(const _WString& wstr){ return _toUTF8(wstr.c_str(), wstr.size()); }
	_WString _toWStr(const char* cptr, size_t len, UINT fromp=CP_UTF8){ size_t wideLen = MultiByteToWideChar(fromp, 0, cptr, len, NULL, 0);
		if (wideLen == 0){ print("(!) Error in _toWStr: "); print(dtos(GetLastError())); print("\n"); return L""; }
		_WString wStr(wideLen+1); MultiByteToWideChar(fromp, 0, cptr, len, wStr.data(), wideLen+1); return wStr; }
	_WString _toWStr(const char* cstr){ return _toWStr(cstr, sizeof(cstr)); }
	_WString _toWStr(const CString& utf8str){ return _toWStr(utf8str.c_str(), utf8str.size()); }
	String _OEMtoUTF8(const char* oemptr, size_t len=0){ if(len<=0){ len=strlen(oemptr); } return _toUTF8(_toWStr(oemptr, len, CP_OEMCP)); }
#endif

long long GetTimestamp(char type='m'){ long long timestamp = 0; // type: s - secs, m - msecs, u - usecs
#if defined(_WIN32) && _WIN32_WINNT >= 0x0602
	FILETIME ft; GetSystemTimePreciseAsFileTime(&ft); return _FtToUnixTime(ft, type);
#elif _WIN32
	FILETIME ft; GetSystemTimeAsFileTime(&ft); return _FtToUnixTime(ft, type);
#else
	struct timeval tv; gettimeofday(&tv, NULL); if(type=='u'){ timestamp = static_cast<long long>(tv.tv_sec) * 1000000 + tv.tv_usec; }
	else if(type=='m'){ timestamp = static_cast<long long>(tv.tv_sec)*1000 + tv.tv_usec/1000; }else{ timestamp = (long long)tv.tv_sec; }
#endif
	return timestamp; }

unsigned long long xorshift128plus(){ static unsigned long long s[2]; static unsigned long long init=false;
	if(!init){ s[0] = GetTimestamp('m'); srand(s[0]); s[0]+=rand(); s[1] = s[0]+0x9e3779b97f4a7c15ULL; init=true; for(int i=0;i<5;i++){ xorshift128plus(); } }
	s[0] = s[1]; s[0] ^= s[0] << 23; s[1] ^= s[0] ^ (s[0] >> 17) ^ (s[1] >> 26); return s[1] + s[0]; }
double random(){ return (xorshift128plus() >> 11) * (1.0 / 9007199254740991.0); }
int randInt(int min, int max){ return floor(random() * (max - min + 1)) + min; }
Buffer Buffer::randBytes(int length){ Buffer buff(length); for(size_t i=0;i<buff.size();i++){ buff[i]=randInt(0,255); } return buff; }
String randStr(size_t length, String letters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"){ 
	String res; for(size_t i=0; i<length; ++i){res+=letters[floor(random()*letters.size())]; } return res; }
	
String readline(){ Buffer input; const char BLOCK_SIZE = 64; char buff[BLOCK_SIZE]; unsigned long bytesRead = 0;
#ifdef _WIN32
	while(true){ if(!ReadFile(GetStdHandle(STD_INPUT_HANDLE), buff, BLOCK_SIZE, &bytesRead, NULL)){ return ""; }
		if(bytesRead <= 0){ break; } input.push(buff, bytesRead); if(input[input.size()-1] == '\n'){ input.resize(input.size()-2); break; }
	} return input.toString();
#else
	while(true){ if((bytesRead = read(STDIN_FILENO, buff, BLOCK_SIZE)) <= 0){ return input.toString(); }
		input.push(buff, bytesRead); if(input[input.size()-1] == '\n'){ input.resize(input.size()-1); break; } } return input.toString();
#endif
}
	
struct Err { int code; virtual ~Err() noexcept {}
	Err() : code(0), _err("Unspecified error"){}
	Err(CString msg, int ecode=0) : code(ecode), _err(msg){}
	Err(int ecode) : code(ecode), _err("Code: "){ _err+=dtos(code); }
	virtual const char* what() const throw(){ return _err.c_str(); }
	private: String _err; };
	
#ifdef _WIN32
	//void Sleep(unsigned int msec){ ::Sleep(msec); }
	void usleep(unsigned int usec){ if(usec >= 1000){ Sleep(usec/1000); }
		if((usec=usec%1000)>0){ LARGE_INTEGER freq, start, end; QueryPerformanceFrequency(&freq); 
			QueryPerformanceCounter(&start); double target = (double)usec / 1000000.0; double elapsed = 0.0;
			while (elapsed < target){ QueryPerformanceCounter(&end); elapsed = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart; } } }
#else
    void Sleep(unsigned int msec){ if(msec<1000){ ::usleep(msec*1000); }
		else{ ::sleep(msec/1000); if((msec=msec%1000)>0) ::usleep(msec*1000); } }
#endif

struct Date { long long timestamp; //char utc;
	Date() : timestamp(Date::now()){} ~Date(){} 
	Date(long long msecs, char type='m') : timestamp(msecs){ if(type=='s'){timestamp*=1000;} }
	Date(const String& dateStr) : timestamp(0){ _setDate(dateStr); }
	static long long now(){ return GetTimestamp('m'); }
	
	int getYear() const { long long days = timestamp/86400/1000; long long years1 = days/365;
		long long leapDays = (1970+years1)/4-(1970+years1)/100+(1970+years1)/400; return 1970+((days+leapDays)/365); } //365.2425
	int getMonth() const;
	int getDate() const;
	int getDay() const;
	int getDayWeek() const;
	
	String toDirectDate(char sp='.') const { int year, month, day, hour, minute; float seconds; _timestampToDate(year, month, day, hour, minute, seconds);
		String r; r<<(day<10?"0":"")<<day<<sp<<(month<10?"0":"")<<month<<sp<<year<<" ";
		r<<(hour<10?"0":"")<<hour<<":"<<(minute<10?"0":"")<<minute<<":"<<(seconds<10?"0":"")<<seconds; return r; }
	String toReverseDate(char sp='.') const { int year, month, day, hour, minute; float seconds; _timestampToDate(year, month, day, hour, minute, seconds);
		String r; r<<year<<sp<<(month<10?"0":"")<<month+1<<sp<<(day<10?"0":"")<<day+1<<" ";
		r<<(hour<10?"0":"")<<hour<<":"<<(minute<10?"0":"")<<minute<<":"<<(seconds<10?"0":"")<<seconds; return r; }
	String toString(char sp='.') const { return toReverseDate(sp); }
	String toString(const String& mode, char sp='.') const { if(mode=="reverse"){ return toReverseDate(); }
		else if(mode=="direct"){ return toDirectDate(); }else if(mode=="iso"||mode=="isostring"){ return toISOString(); }else{ return toReverseDate(); } }
	String toISOString() const { int year, month, day, hour, minute; float seconds; _timestampToDate(year, month, day, hour, minute, seconds);
		String r; r<<year<<"-"<<(month<10?"0":"")<<month<<"-"<<(day<10?"0":"")<<day<<"T";
		r<<(hour<10?"0":"")<<hour<<":"<<(minute<10?"0":"")<<minute<<":"<<(seconds<10?"0":"")<<seconds<<"Z"; return r; }
	
	Date& operator+=(const Date& other){ this->timestamp+=other.timestamp; return *this; }
	long long operator+(const Date& other) const { return this->timestamp+other.timestamp; }
	Date& operator+=(long long value){ this->timestamp+=value; return *this; }
	long long operator+(long long value) const { return this->timestamp+value; }
	
	Date& operator-=(const Date& other){ this->timestamp-=other.timestamp; return *this; }
	long long operator-(const Date& other) const { return this->timestamp-other.timestamp; }
	Date& operator-=(long long value){ this->timestamp-=value; return *this; }
	long long operator-(long long value) const { return this->timestamp-value; }
	private:
	bool _isGrigoryVisokos(int year) const { return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }
	int _GrigoryVisokosAbs(int year) const { return (year/4) - (year/100) + (year/400); }
	int _GrigoryVisokos(int startYear, int endYear) const { return _GrigoryVisokosAbs(endYear)-_GrigoryVisokosAbs(startYear); }
	int _MonthDays(int month, int year) const { const int daysOfMonth[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}; 
		int days=(month>0&&month<12)?daysOfMonth[month]:0; if(_isGrigoryVisokos(year) && month > 1){ days+=0; } return days; }
	static void _parseDate(const String& dateStr, int& year, int& month, int& day, int& hour, int& minute, float& seconds){
		Array<String> darr = dateStr.replaceAll(".", "-").split("-"); darr.resize(3); bool isdirect=(darr[0].size()<=2&&darr[2].size()>2)?true:false;
		month=stoin(darr[1]); if(isdirect){ day=stoin(darr[0]); }else{ year=stoin(darr[0]); } darr = darr[2].replace("T"," ").split(" "); 
		if(isdirect){ year=stoin(darr[0]); }else{ day=stoin(darr[0]); } darr = darr[1].split(":"); 
		hour=stoin(darr[0]); minute=stoin(darr[1]); seconds=stoin(darr[2].split("Z")[0]);
		//std::cout << "_parseDate(darr): " << darr << std::endl;
		
		/*int argscnt = std::sscanf(dateStr.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &year, &month, &day, &hour, &minute, &seconds);
		if(argscnt<3){ std::cout << "(!) Date: sscanf parse failed: less 3 args parsed" << std::endl; return; }
		std::cout << "Date: sscanf parsed args: " << argscnt << std::endl;*/
		
		//std::cout << year << "; " << month << "; " << day << " " << hour << ":" << minute << ":" << seconds << " | Etalon: 1725065286957" << std::endl;
	}
	void _setDate(const String& dateStr){ // Предполагаются форматы: "YYYY-MM-DD HH:MM:SS", "DD-MM-YYYY HH:MM:SS"
		int year=0, month=0, day=0, hour=0, minute=0; float seconds=0; _parseDate(dateStr, year, month, day, hour, minute, seconds);
		timestamp = (long long)(year-1970) * 365 * 24 * 60 * 60 * 1000; // годы
		timestamp += (long long)_MonthDays(month, year) * 24 * 60 * 60 * 1000; // месяцы
		timestamp += (long long)(day-1+_GrigoryVisokos(1970, year)) * 24 * 60 * 60 * 1000; // дни
		timestamp += hour * 3600 * 1000; timestamp += minute * 60 * 1000; timestamp += seconds * 1000; }
	void _timestampToDate(int& year, int& month, int& day, int& hour, int& minute, float& seconds) const { month=0; year = getYear(); 
		int totalsecs = (timestamp/1000)-((year-1970)*365LL+_GrigoryVisokos(1970, year))*86400;
		//std::cout << "-- formula: (("<<year<<"-1970)*365+"<<(_GrigoryVisokos(1970, year))<<")*86400 = " << ((year-1970)*365LL+_GrigoryVisokos(1970, year))*86400 << std::endl;
		//std::cout << "totalsecs = " << totalsecs << std::endl;
		//std::cout << "_MonthDays("<<month<<", "<<year<<") = " << _MonthDays(month, year) << "(days="<< (float)totalsecs/86400 << "; totalsecs = " << totalsecs << std::endl;
		int totalDays=totalsecs/86400; while(totalDays > _MonthDays(month, year)&&month<11){ month++; } totalsecs-=_MonthDays(month, year)*86400;
		//std::cout << "2) _MonthDays("<<month<<", "<<year<<") = " << _MonthDays(month, year) << "(days="<< (float)totalsecs/86400 << "; totalsecs = " << totalsecs << std::endl;
		day = floor(totalsecs/86400); totalsecs-=day*86400; hour = floor(totalsecs/3600); totalsecs-=hour*3600; 
		minute = floor(totalsecs/60); totalsecs-=minute*60; seconds = totalsecs; }
};

namespace performance { static long long _start_usec=0;
	void start(){ _start_usec=GetTimestamp('u'); }
	double now(){ return (GetTimestamp('u')-_start_usec)/1000.0; } }
namespace perf = performance;
}