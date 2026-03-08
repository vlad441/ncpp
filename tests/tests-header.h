#ifndef NCPP_TESTS_H
#define NCPP_TESTS_H
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
	print("C++98");
#endif
	print("] UTF-8 кирилица にも含み\n"); }

void gcc_version(){ print("[Compiller] ");
#if defined(__clang__)
	print(__VERSION__); print("\n"); //std::cout << "Clang " << __clang_major__ << "." << __clang_minor__ << "." <<  __clang_patchlevel__ << std::endl;
#elif defined(__GNUC__)
	print("GCC "); print(__VERSION__); print("\n"); //std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." <<  __GNUC_PATCHLEVEL__ << std::endl;
#elif defined(_MSC_VER)
	print("MSVC? WTF?\n");
#else
	print("(Unknown)\n");
#endif
} 

void ncpp_version(){ print("[NCPP_VER] "); print(NCPP_VER); 
#ifdef NCPP_LIB_USE
	print(" (Compiled via LIB)");
#else
	print(" (Directly compiled)");
#endif
	_ncpp_check_ver(); }

void arch_current(){ print(" | [Arch]");
#ifdef __x86_64__
	print("[x86_64]");
#elif defined(__i686__) //x86 32 bit
	print("[i686]");
#elif defined(__i386__) //x86 32 bit
	print("[i386]");
#elif defined(__aarch64__) || defined(__arm64__)
	print("[ARM64]");
#elif defined(__arm__)
	print("[ARM]");
#elif defined(__riscv) || defined(__riscv_64) || __riscv_xlen == 64
	print("[RISC-V]");
#else
	print("[?]");
#endif
#if defined(__LP64__) || defined(_WIN64) || __SIZEOF_POINTER__==8
	print(" 64 bit.");
	#ifdef __LP64__
	print(" (LP64)");
	#endif
	#ifdef _WIN64
	print(" (WIN64)");
	#endif
#else
    print(" 32 bit.");
#endif
	print("\n");
}
#endif

