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

#define UNIX_EPOCH 1970
struct Date { long long timestamp; static char utc;
	Date() : timestamp(Date::now()){} ~Date(){} 
	Date(long long msecs, char type='s') : timestamp(msecs){ if(type=='m') timestamp/=1000; }
	Date(const CString& dateStr) : timestamp(0){ _setDate(dateStr); }
	static long long now(){ return GetTimestamp('s'); }
	
	struct DValue { long long year; char month, day, hour, minute; float seconds; };
	long long getYear() const { return _getYear(timestamp); } //365.2425
	int getMonth() const { return timestampToDate(timestamp).month; }
	int getDate() const { return timestampToDate(timestamp).day; }
	int getDay() const { return timestampToDate(timestamp).day; }
	#define EPOCH_DAYWEEK_OFFSET 3
	int getDayWeek() const { return (timestamp/86400+EPOCH_DAYWEEK_OFFSET)%7; }
	
	
	String toDirectDate(char sp1='.', bool toshort=false) const { DValue dv = timestampToDate(timestamp+utc*3600); char sp[2]; sp[0]=sp1; sp[1]='\0';
		String r; r<<(dv.day<9?"0":"")<<dv.day+1<<sp<<(dv.month<9?"0":"")<<dv.month+1<<sp<<dv.year<<" "; _toTimeDv(dv, r, toshort); return r; }
	String toReverseDate(char sp1='.', bool toshort=false) const { DValue dv = timestampToDate(timestamp+utc*3600); char sp[2]; sp[0]=sp1; sp[1]='\0';
		String r; r<<dv.year<<sp<<(dv.month<9?"0":"")<<dv.month+1<<sp<<(dv.day<9?"0":"")<<dv.day+1<<" "; _toTimeDv(dv, r, toshort); return r; }
	String toString(char sp='.') const { return toReverseDate(sp); }
	String toString(const CString& mode, char sp='.') const { if(mode=="reverse"){ return toReverseDate(); }
		else if(mode=="direct"){ return toDirectDate(); }else if(mode=="iso"||mode=="isostring"){ return toISOString(); }
		else if(mode=="imf"||mode=="imf-fixdate"){ return toIMFDate(); }else{ return toReverseDate(); } }
	String toISOString() const { DValue dv = timestampToDate(timestamp);
		String r; r<<dv.year<<"-"<<(dv.month<9?"0":"")<<dv.month+1<<"-"<<(dv.day<9?"0":"")<<dv.day+1<<"T";
		r<<(dv.hour<10?"0":"")<<dv.hour<<":"<<(dv.minute<10?"0":"")<<dv.minute<<":"<<(dv.seconds<10?"0":"")<<dv.seconds<<"Z"; return r; }
	String toTime(bool toshort=false) const { String ss; int ttsecs = timestamp % 86400; int tmp = ttsecs/3600; 
		ss<<(tmp<10?"0":"")<<tmp<<":"; ttsecs-=tmp*3600; tmp=ttsecs/60; ss<<(tmp<10?"0":"")<<tmp; 
		if(!toshort){ ttsecs-=tmp*60; ss<<":"<<(ttsecs<10?"0":"")<<ttsecs; } return ss; }
	void _toTimeDv(const DValue& dv, String& ss, bool toshort=false) const { ss<<(dv.hour<10?"0":"")<<dv.hour<<":"<<(dv.minute<10?"0":"")<<dv.minute; 
		if(!toshort){ ss<<":"<<(dv.seconds<10?"0":"")<<dv.seconds; } }
	
	static String getDayWeek_Name(int day) { const char* const W_NAMES[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" }; 
		 return (day>=0&&day<7)?W_NAMES[day]:"UNKNOWN"; }
	static String getMonth_Name(int month) { const char* const M_NAMES[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" }; 
		 return (month>=0&&month<12)?M_NAMES[month]:"UNKNOWN"; }
		
	String toIMFDate() const { //IMF-fixdate: Day, DD Mon YYYY HH:MM:SS GMT+UTC
		DValue dv = timestampToDate(timestamp); String r; r << getDayWeek_Name(getDayWeek()) << ", "; 
		r<<(dv.day<9?"0":"")<<dv.day+1<<" " << getMonth_Name(dv.month) << " " << dv.year << " "; _toTimeDv(dv, r, false); r <<" GMT"; return r; }
	
	Date& operator+=(const Date& other){ this->timestamp+=other.timestamp; return *this; }
	Date operator+(const Date& other) const { return this->timestamp+other.timestamp; }
	Date& operator+=(long long value){ this->timestamp+=value; return *this; }
	Date operator+(long long value) const { return this->timestamp+value; }
	
	Date& operator-=(const Date& other){ this->timestamp-=other.timestamp; return *this; }
	Date operator-(const Date& other) const { return this->timestamp-other.timestamp; }
	Date& operator-=(long long value){ this->timestamp-=value; return *this; }
	Date operator-(long long value) const { return this->timestamp-value; }
	
	static bool isLeapYear(int year){ return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }
	String intervalStr() const { String ss; ss << timestamp/86400 << " days, "; return ss << toTime(); }
	static DValue timestampToDate(long long timestamp){ DValue dv; int days=0; dv.year = _getYear(timestamp, &days); dv.month=0;
		while(days >= _MonthDays(dv.month, dv.year)&&dv.month<12){ dv.month++; days-=_MonthDays(dv.month, dv.year); }
		dv.day = days; int ttsecs = timestamp % 86400; dv.hour = ttsecs/3600; ttsecs-=dv.hour*3600;
		dv.minute = ttsecs/60; ttsecs-=dv.minute*60; dv.seconds = ttsecs; return dv; }
	long long DateTotimestamp(const DValue& dv);
	static DValue parseDate(const String& dateStr){ DValue dv; if(dateStr.size()>16&&dateStr[3]==','&&dateStr[4]==' '){ return _parseIMFDate(dateStr); }
		Array<String> darr = dateStr.replaceAll(".", "-").split("-"); darr.resize(3);
		bool isdirect=(darr[0].size()<=2&&darr[2].size()>2)?true:false; dv.month=stoin(darr[1]); 
		if(isdirect){ dv.day=stoin(darr[0]); }else{ dv.year=stoin(darr[0]); } darr = darr[2].replace("T"," ").split(" ");
		if(isdirect){ dv.year=stoin(darr[0]); }else{ dv.day=stoin(darr[0]); } darr = darr[1].split(":"); 
		dv.hour=stoin(darr[0]); dv.minute=stoin(darr[1]); dv.seconds=stoin(darr[2].split("Z")[0]); return dv; }
	static void _showDValue(const DValue& dv){ print("DValue: "); print(dv.year); print(" "); print(dv.month); print(" "); print(dv.day); 
		print(" | "); print(dv.hour); print(":"); print(dv.minute); print(":"); print(dv.seconds); print("\n"); }
	private:
	static DValue _parseIMFDate(const String& dateStr){ DValue dv; Array<String> darr = dateStr.split(" "); darr.resize(5);
		dv.day = stoin(darr[1]); dv.month = stoin(darr[2]); dv.year = stoin(darr[3]); darr = darr[4].split(":"); darr.resize(3); 
		dv.hour = stoin(darr[0]); dv.minute = stoin(darr[1]); dv.seconds = stoin(darr[2]); return dv; }
	static long long _getYear(long long timestamp, int* daysLeft=NULL){ long long days = timestamp/86400; long long year=UNIX_EPOCH;
		while(true){ int yearDays = isLeapYear(year)?366:365; if(days < yearDays){ break; } days -= yearDays; year++; }
		//print("(#DEBUG) _getYear: days = "); print(days); print("\n");
		if(daysLeft) *daysLeft=days; return year; } //365.2425
	static int _GrigoryVisokosAbs(int year){ return (year/4)-(year/100)+(year/400); }
	static int _GrigoryVisokosEpoch(int year){ return _GrigoryVisokosAbs(year)-_GrigoryVisokosAbs(UNIX_EPOCH); }
	static int _GrigoryVisokos(int startYear, int endYear){ return _GrigoryVisokosAbs(endYear)-_GrigoryVisokosAbs(startYear); }
	static int _MonthDays(int month, int year){ static const int monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
		int days=(month>=0&&month<12)?monthDays[month]:0; if(month == 1 && isLeapYear(year)) days++; return days; }
	int _MonthDaysAbs(int month, int year) const { const int daysOfMonth[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}; 
		int days=(month>0&&month<12)?daysOfMonth[month]:0; if(isLeapYear(year) && month > 1){ days+=0; } return days; }
	void _setDate(const String& dateStr){ // Предполагаются форматы: "YYYY-MM-DD HH:MM:SS", "DD-MM-YYYY HH:MM:SS"
		//timestamp = DateTotimestamp(parseDate(dateStr)); return;
		DValue dv = parseDate(dateStr); timestamp = (long long)(dv.year-UNIX_EPOCH) * 365 * 24 * 60 * 60; // годы
		timestamp += (long long)_MonthDaysAbs(dv.month, dv.year) * 24 * 60 * 60; // месяцы
		timestamp += (long long)(dv.day-1+_GrigoryVisokosEpoch(dv.year)) * 24 * 60 * 60; // дни
		timestamp += dv.hour * 3600; timestamp += dv.minute * 60; timestamp += dv.seconds; }
}; char Date::utc=0;

namespace performance { static long long _start_usec=0;
	void start(){ _start_usec=GetTimestamp('u'); }
	double now(){ return (GetTimestamp('u')-_start_usec)/1000.0; } }
namespace perf = performance;
}