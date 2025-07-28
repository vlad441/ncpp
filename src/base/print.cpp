namespace ncpp{
#ifdef _WIN32
#include <windows.h>
	void print(const char *cptr, size_t len){ WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cptr, (DWORD)len, NULL, NULL); }
	//void pErr(const char *cptr, size_t len){ WriteFile(GetStdHandle(STD_ERROR_HANDLE), cptr, (DWORD)len, NULL, NULL); }
#else
#include <unistd.h>
	void print(const char *cptr, size_t len){ write(STDOUT_FILENO, cptr, len); }
	//void pErr(const char *cptr, size_t len){ write(STDERR_FILENO, cptr, len); } //Alt: printErr, cerr?
#endif
	void print(const char *cstr){ print(cstr, strlen(cstr)); }
	//void pErr(const char *cptr){ pErr(cstr, strlen(cstr)); } 
	//void printLn(const char *cptr, size_t len){ print(cstr, len); print("\n"); }
	//void printLn(const char *cstr){ print(cstr, strlen(cstr)); print("\n"); }
	//void printLn(){ print("\n"); }
	//except() ?
}
