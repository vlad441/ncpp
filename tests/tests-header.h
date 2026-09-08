#ifndef NCPP_TESTS_H
#define NCPP_TESTS_H
//Predefined macros: https://ru.cppreference.com/cpp/preprocessor/replace

//cpp_version
#if __cplusplus >= 202603L 
	#define NCPP_CPPVER ">=C++26"
#elif __cplusplus >= 202302L
	#define NCPP_CPPVER "C++23"
#elif __cplusplus >= 202002L
	#define NCPP_CPPVER "C++20"
#elif __cplusplus >= 201703L
	#define NCPP_CPPVER "C++17"
#elif __cplusplus >= 201402L
	#define NCPP_CPPVER "C++14"
#elif __cplusplus >= 201103L
	#define NCPP_CPPVER "C++11"
#else
	#define NCPP_CPPVER "C++98"
#endif

//gcc_version
#if defined(__clang__)
	#define NCPP_GCCVER __VERSION__ //cons << "Clang " << __clang_major__ << "." << __clang_minor__ << "." <<  __clang_patchlevel__ << "\n";
#elif defined(__GNUC__)
	#define NCPP_GCCVER "GCC " __VERSION__ //cons << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." <<  __GNUC_PATCHLEVEL__ << "\n";
#elif defined(_MSC_VER)
	#define NCPP_GCCVER "MSVC? WTF?"
#else
	#define NCPP_GCCVER "(Unknown)"
#endif

//arch_current
#ifdef __x86_64__
	#define NCPP_ARCHSTR "[x86_64]"
#elif defined(__i686__) //x86 32 bit
	#define NCPP_ARCHSTR "[i686]"
#elif defined(__i386__) //x86 32 bit
	#define NCPP_ARCHSTR "[i386]"
#elif defined(__aarch64__) || defined(__arm64__)
	#define NCPP_ARCHSTR "[ARM64]"
#elif defined(__arm__)
	#define NCPP_ARCHSTR "[ARM]"
#elif defined(__riscv) || __riscv_xlen == 64
	#define NCPP_ARCHSTR "[RISC-V]"
#else
	#define NCPP_ARCHSTR "[?]"
#endif
#if defined(__LP64__) || defined(_WIN64) || __SIZEOF_POINTER__==8
	#ifdef __LP64__
		#define NCPP_ARCHSTR_EX NCPP_ARCHSTR " 64 bit (LP64)"
	#elif _WIN64
		#define NCPP_ARCHSTR_EX NCPP_ARCHSTR " 64 bit (WIN64)"
	#else
		#define NCPP_ARCHSTR_EX NCPP_ARCHSTR " 64 bit"
	#endif
#else
	#define NCPP_ARCHSTR_EX NCPP_ARCHSTR " 32 bit"
#endif

//ncpp_version
#ifdef NCPP_LIB_USE
	#undef NCPP_LIB_USE
	#define NCPP_LIB_USE " (Compiled via LIB)"
#else
	#define NCPP_LIB_USE " (Directly compiled)"
#endif

void os_version(){ using namespace ncpp; print("[OS] "); StringMap info = ::ncpp::system::os_info(); print(info["OS"]);
#ifdef _WIN32
	print(" | NT: "); print(info["NT"]);
#else
	print(" | Kernel: "); print(info["Kernel"]);
#endif
	print("\n"); //print(info.cout()); print("\nCPU: "); print(system::CPU::info().cout()); print("\nRAM: "); print(system::RAM::strUsage()); print("\n");
}

#define NCPP_COMPILE_INFO "[" NCPP_CPPVER "] UTF-8 кирилица にも含み " "[Compiller] " NCPP_GCCVER "\n" \
	"[NCPP_VER] " NCPP_VER NCPP_LIB_USE " | [Arch]" NCPP_ARCHSTR_EX "\n"

#endif //NCPP_TESTS_H