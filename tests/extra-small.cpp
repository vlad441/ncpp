// ====== Minimal CRT ======
#define NULL 0
int main(); typedef unsigned long long size_t; typedef unsigned int DWORD;
#ifdef _WIN32
	#define STD_OUTPUT_HANDLE -11UL
	extern "C" __declspec(dllimport) void __stdcall ExitProcess(DWORD uExitCode) __attribute__((noreturn));
	extern "C" __declspec(dllimport) int __stdcall WriteFile(void* hFile, const void* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD* lpNumberOfBytesWritten, void* lpOverlapped );
	extern "C" __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle);
	extern "C" void _start(){ ExitProcess(main()); } //for 64 bit (В x32 линкер превращает _start() -> start(), т.е. нужно __start() -> _start())
	//extern "C" void start(){ ExitProcess(main()); } //for 32 bit (x32 линкер превращает _start() -> start())
	extern "C" void __main(){} //Заглушка для GCC
#else //Linux: x86_64 only
	typedef long ssize_t; //typedef unsigned long size_t;
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

int main(){ //print(NCPP_COMPILE_INFO);
	print("Hello, no-CRT\n"); }
