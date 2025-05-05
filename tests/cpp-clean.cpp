//#include <cstdlib> // for size_t, atoi и atof, etc...
#include <stddef.h> // <cstddef> only typoe def? size_t, NULL, etc...
#include <string.h> // <cstring> for strlen, etc...
//#include <cstdio> // for printf, std::sprintf
//#include <iostream>
#define NCPP_VER "v0.0.0-0"
#ifdef _WIN32
#include <windows.h>
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

class TestC { int v1; };

int main(){ cpp_version(); gcc_version(); 
	print(NCPP_VER); print("\n"); //printf(NCPP_VER);
	//std::cout << "std::cout" << std::endl;
	
	TestC t1; print("TestC t1\n");
}

//GCC 4.9.2 - Windows 32bit .exe file size:
//only print() - 11 776 bytes
//+printf() - 34 816 bytes
//+std::vector() - 133 632 bytes
//+std::string() - 451 072 bytes
//+std::stringstream() - 562 688 bytes
//+std::map() - 135 680 bytes
//+std::unordered_map() - 453 632 bytes
//+<iostream> - 591 360 bytes

//GCC 4.9.2 glibc - Linux 64bit file size:
//only print() -  735 816 bytes
//+printf() - 735784 байт
//+<iostream> - 1 465 768 байт

//GCC 4.9.2 musl - Linux 64bit file size:
//musl-gcc print();