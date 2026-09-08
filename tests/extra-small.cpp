// ====== Minimal CRT ======
#define NULL 0
int main(); typedef unsigned long long size_t; typedef unsigned int DWORD;
#ifdef _WIN32
	#define STD_OUTPUT_HANDLE ((unsigned long)-11)
	extern "C" __declspec(dllimport) void __stdcall ExitProcess(DWORD uExitCode) __attribute__((noreturn));
	extern "C" __declspec(dllimport) int __stdcall WriteFile(void* hFile, const void* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD* lpNumberOfBytesWritten, void* lpOverlapped );
	extern "C" __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle);
	extern "C" void _start(){ ExitProcess(main()); } //WORKS ONLY 64 BIT
	extern "C" void __main(){} //Заглушка для компилятора
#else
	typedef long long ssize_t;
	#define STDOUT_FILENO 1
	extern "C" inline long _syscall3(long n, long a1, long a2, long a3){ long ret; 
		asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory"); return ret; }
	extern "C" inline void _syscall1(long n, long a1){ asm volatile ("syscall" : : "a"(n), "D"(a1) : "rcx", "r11", "memory"); }
	#define SYS_exit 60
	extern "C" void _start(){ _syscall1(SYS_exit, main()); }
	extern "C" ssize_t write(int fd, const void *buf, size_t count){ return (ssize_t)_syscall3(1, (long)fd, (long)buf, (long)count); }
#endif
// ====== User-Space Code ======
extern "C" size_t strlen(const char* c){ size_t len=0; while(*c!='\0'){ len++; c++; } return len; }

#ifdef _WIN32
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
	print("] UTF-8 кирилица にも含み\n");
};

void gcc_version(){ print("[Compiller] ");
#if defined(__clang__)
	print("Clang "); print(__VERSION__); print("\n");
#elif defined(__GNUC__)
	print("GCC "); print(__VERSION__); print("\n");
#elif defined(_MSC_VER)
	print("MSVC? WTF?\n");
#else
	print("(Unknown)\n");
#endif
};

int main(){ //print(NCPP_COMPILE_INFO);
	print("Hello, no-CRT\n"); }
