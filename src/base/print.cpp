#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32 // === Windows === 
#ifndef STD_OUTPUT_HANDLE //#include <windows.h>
#define STD_OUTPUT_HANDLE -11UL //#undef STD_OUTPUT_HANDLE
#define STD_ERROR_HANDLE -12UL //#undef STD_ERROR_HANDLE
#include <stddef.h> // for size_t
__declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle); struct _OVERLAPPED;
__declspec(dllimport) int __stdcall WriteFile(void* hFile, const void* lpBuffer, unsigned long nNumberOfBytesToWrite, unsigned long* lpNumberOfBytesWritten, struct _OVERLAPPED* lpOverlapped);
#endif
	void print(const char* cptr, size_t len){ WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cptr, (unsigned long)len, NULL, NULL); }
	void _pErr(const char *cptr, size_t len){ WriteFile(GetStdHandle(STD_ERROR_HANDLE), cptr, (unsigned long)len, NULL, NULL); }
#else // === Linux ===
#ifndef STDOUT_FILENO //#include <unistd.h>
#define STDOUT_FILENO 1 //#undef STDOUT_FILENO
#define STDERR_FILENO 2 //#undef STDERR_FILENO
#include <stddef.h> // for size_t
long write(int fd, const void* buf, size_t count);
#endif
	void print(const char* cptr, size_t len){ ::write(STDOUT_FILENO, cptr, len); }
	void _pErr(const char* cptr, size_t len){ ::write(STDERR_FILENO, cptr, len); } //Alt: printErr, cerr?
#endif

#ifdef __cplusplus
size_t strlen(const char* c); }
namespace ncpp{ using ::print; using ::_pErr;
	void print(const char* cstr){ print(cstr, strlen(cstr)); }
	#ifdef NCPP_VER
	const char* version(){ return NCPP_VER; }
	#endif
	void _pErr(const char* cstr){ _pErr(cstr, strlen(cstr)); }
}
#endif
