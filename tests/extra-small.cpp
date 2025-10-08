// ====== Minimal CRT ======
#define NULL 0
int main(); typedef unsigned int size_t; typedef size_t DWORD;
extern "C" size_t strlen(const char* c){ size_t len=0; while(*c!='\0'){ len++; c++; } return len; }
#ifdef _WIN32
	//#include <windows.h>
	#define STD_OUTPUT_HANDLE ((unsigned long)-11)
	extern "C" __declspec(dllimport) void __stdcall ExitProcess(DWORD uExitCode) __attribute__((noreturn));
	extern "C" __declspec(dllimport) int __stdcall WriteFile(void* hFile, const void* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD* lpNumberOfBytesWritten, void* lpOverlapped );
	extern "C" __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle);
	//extern "C" __attribute__((naked)) void _start();
	extern "C" void _start(){ ExitProcess(main()); } //WORKS ONLY 64 BIT
	extern "C" void __main(){}
#else
#endif
// ====== User-Space Code ======

#ifdef _WIN32
	void print(const char *cptr, size_t len){ WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cptr, (DWORD)len, NULL, NULL); }
#else
#include <unistd.h>
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
	print("] UTF-8 кирилица にも含み\n");
};

void gcc_version(){ print("[Compiller] ");
#if defined(__clang__)
	print("Clang "); print(__VERSION__); print("\n"); //std::cout << "Clang " << __clang_major__ << "." << __clang_minor__ << "." <<  __clang_patchlevel__ << std::endl;
#elif defined(__GNUC__)
	print("GCC "); print(__VERSION__); print("\n"); //std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." <<  __GNUC_PATCHLEVEL__ << std::endl;
#elif defined(_MSC_VER)
	print("MSVC? WTF?\n");
#else
	print("(Unknown)\n");
#endif
};

int main(){ //cpp_version(); gcc_version();
	print("Hello, no-CRT\n"); }
