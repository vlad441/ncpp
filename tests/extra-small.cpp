#include <cstdlib> // for size_t, atoi и atof, etc...
#include <cstring>// for strlen, etc...
//--#include <cstddef> // only typoe def? size_t, NULL, etc...
#define NCPP_VER "v0.0.0-0"
#ifdef _WIN32
#include <windows.h>
	void print(const char *cptr, size_t len){ WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cptr, (DWORD)len, NULL, NULL); }
#else
	void print(const char *cptr, size_t len){ write(STDOUT_FILENO, cptr, len); }
#endif
	void print(const char *cstr){ print(cstr, strlen(cstr)); }

void cpp_version(){ print("[");
#if __cplusplus >= 202302L
	print(">=C++23");
#elif __cplusplus >= 202002L
	print("C++20");
#elif __cplusplus >= 201703L
	print("C++17");
#elif __cplusplus >= 201402L
	print("C++14");
#elif __cplusplus >= 201103L
	print("C++11");
#else
	print("c++98");
#endif
	//print("] UTF-8 кирилица にも含み ("); print(__cplusplus); print(")\n");
	print("] UTF-8 кирилица にも含み\n"); print(" - \\n NewStr Test\n"); print(" - __VERSION__ = "); print(__VERSION__); print("\n");
};

/*void gcc_version(){ print("[Compiller] ");
#if defined(__clang__)
	print("Clang "); print(__clang_major__); print("."); print(__clang_minor__); print("."); print(__clang_patchlevel__); print("\n");
#elif defined(__GNUC__)
	print("GCC "); print(__GNUC__); print("."); print(__GNUC_MINOR__); print("."); print(__GNUC_PATCHLEVEL__); print("\n");
#elif defined(_MSC_VER)
	print("MSVC?\n");
#endif
};*/

//void __attribute__((naked)) _start(){
void _start(){
    const char msg[] = "Hello!\n";
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteFile(hStdOut, msg, sizeof(msg)-1, NULL, NULL);
    ExitProcess(0);
}
