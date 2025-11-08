namespace ncpp{
#ifdef _WIN32
#include <windows.h>
	//extern "C" void _print(const char* cptr, size_t len, HANDLE handle){ WriteFile(handle, cptr, (DWORD)len, NULL, NULL); }
	void print(const char* cptr, size_t len){ WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cptr, (DWORD)len, NULL, NULL); }
	//void _pErr(const char *cptr, size_t len){ WriteFile(GetStdHandle(STD_ERROR_HANDLE), cptr, (DWORD)len, NULL, NULL); }
#else
#include <unistd.h>
	void print(const char* cptr, size_t len){ ::write(STDOUT_FILENO, cptr, len); }
	//void _pErr(const char* cptr, size_t len){ write(STDERR_FILENO, cptr, len); } //Alt: printErr, cerr?
#endif
	void print(const char* cstr){ print(cstr, strlen(cstr)); }
	#ifdef NCPP_VER
	const char* version(){ return NCPP_VER; }
	#endif
	//void _pErr(const char *cptr){ _pErr(cstr, strlen(cstr)); }
	//except()?
}
