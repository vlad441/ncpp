#ifdef _WIN32 // == Windows Headers ==
#include <winsock2.h> // (includes <windows.h>)
#include <ws2tcpip.h>
#include <mswsock.h>  // for AcceptEx
#include <wchar.h> // for wcslen()
#if defined(__GNUC__) && __GNUC__ < 4
#include "sys/gcc3_winxpdef.hpp"
#endif

//namespace ncpp { typedef HMODULE LibHandle; } //Windows Dynamic Lib Loader 
#define DLIB_EXT ".dll" 
#define DLIB_LOAD(name) LoadLibraryA(name)
#define DLIB_FREE(handle) FreeLibrary(handle)
#define DGET_ADDR(handle, name) GetProcAddress(handle, name)
#else // == Linux Headers ==
#include <dlfcn.h>
//namespace ncpp { typedef void* LibHandle; } //Linux Dynamic Lib Loader
#define DLIB_EXT ".so"
#define DLIB_LOAD(name) dlopen(name, RTLD_NOW) //RTLD_NOW, RTLD_LAZY
#define DLIB_FREE(handle) dlclose(handle)
#define DGET_ADDR(handle, name) dlsym(handle, name)
#endif

namespace ncpp {
#ifdef _WIN32
	struct _WString : Array<wchar_t> {
		_WString(const wchar_t* wptr, size_t len) : Array(wptr, wptr+len){};
		_WString(const wchar_t* wptr) : Array(wptr, wptr+wcslen(wptr)){};
		_WString(const void* begin, const void* end) : Array((const wchar_t*)begin, (const wchar_t*)end){};
		_WString(size_t len=0) : Array(len){}
		_WString(size_t len, wchar_t v) : Array(len, v){}
		const wchar_t* c_str() const { return _ptr; } };

	long long _FtToUnixTime(FILETIME& ft, char type='s'){ ULARGE_INTEGER t; t.LowPart = ft.dwLowDateTime; t.HighPart = ft.dwHighDateTime;
		if(type=='u'||type=='m'){ return ((t.QuadPart - 116444736000000000ULL)/(type=='u'?10:10000)); }
		else{ return ((t.QuadPart - 116444736000000000ULL)/10000000LL); } }
	String _toUTF8(const wchar_t* wptr, size_t len, UINT codep=CP_UTF8){ size_t utf8Len = WideCharToMultiByte(codep, 0, wptr, len, NULL, 0, NULL, NULL);
		if(utf8Len == 0){ Except("Error in _toUTF8: "+dtos(GetLastError())+"\n"); return ""; }
		String utf8Str(utf8Len); WideCharToMultiByte(codep, 0, wptr, len, utf8Str.data(), utf8Len, NULL, NULL); return utf8Str; }
	String _toUTF8(const _WString& wstr){ return _toUTF8(wstr.c_str(), wstr.size()); }
	_WString _toWStr(const char* cptr, size_t len, UINT fromp=CP_UTF8){ size_t wideLen = MultiByteToWideChar(fromp, 0, cptr, len, NULL, 0);
		if(wideLen == 0){ Except("Error in _toWStr: "+dtos(GetLastError())+"\n"); return L""; }
		_WString wStr(wideLen+1); MultiByteToWideChar(fromp, 0, cptr, len, wStr.data(), wideLen+1); return wStr; }
	_WString _toWStr(const char* cstr){ return _toWStr(cstr, sizeof(cstr)); }
	_WString _toWStr(const CString& utf8str){ return _toWStr(utf8str.c_str(), utf8str.size()); }
	String _OEMtoUTF8(const char* oemptr, size_t len=0){ if(len<=0){ len=strlen(oemptr); } return _toUTF8(_toWStr(oemptr, len, CP_OEMCP)); }
#else
#endif

long long GetTimestamp(char type='s'){ // type: s - secs, m - msecs, u - usecs
#if defined(_WIN32) && _WIN32_WINNT >= 0x0602
	FILETIME ft; GetSystemTimePreciseAsFileTime(&ft); return _FtToUnixTime(ft, type);
#elif _WIN32
	FILETIME ft; GetSystemTimeAsFileTime(&ft); return _FtToUnixTime(ft, type);
#else
	long long timestamp=0; struct timeval tv; gettimeofday(&tv, NULL); if(type=='u'){ timestamp = (long long)(tv.tv_sec) * 1000000 + tv.tv_usec; }
	else if(type=='m'){ timestamp = (long long)(tv.tv_sec)*1000 + tv.tv_usec/1000; }else{ timestamp = (long long)tv.tv_sec; } return timestamp;
#endif
}

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

namespace performance { static long long _start_usec=0;
	void start(){ _start_usec=GetTimestamp('u'); }
	double now(){ return (GetTimestamp('u')-_start_usec)/1000.0; } }
namespace perf = performance;

template <typename T, typename E = String>
struct Result { T data; E reason; bool ok; //int code; unsigned char _raw[sizeof(T)>sizeof(E) ? sizeof(T):sizeof(E)];
	Result(const T& data=T()) : ok(true), data(data){}
	Result(bool ok, const T& d, const E& r=E()) : ok(ok), data(d), reason(r){}
	//Result(bool ok, const E& reason) : ok(ok){ if(!ok){ this->reason=reason; }else{ data=reason; } }
	
	#if __cplusplus >= 201103L
	explicit operator bool() const { return ok; }
	explicit operator const T&() const { return data; }
	#else
	operator bool() const { return ok; }
	operator const T&() const { return data; }
	#endif
	
	//const T& data(); //getData()
	//const E& reason(); //getErr()
	
	static Result Ok(const T& data){ return Result::Resolve(data); }
	static Result Success(const T& data){ return Result::Resolve(data); }
	static Result Err(const E& reason){ return Result::Reject(reason); }
	
	static Result Resolve(const T& data){ Result r(true); r.data=data; return r; } //return Result(true, data, E());
	static Result Reject(const E& reason){ Result r(false); r.reason=reason; return r; } //return Result(false, T(), reason);
	
}; }