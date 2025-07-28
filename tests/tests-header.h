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
	print("] UTF-8 кирилица にも含み\n"); }

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
} void ncpp_version(){ print("[NCPP_VER] "); print(NCPP_VER); print("\n"); _ncpp_check_ver(); }