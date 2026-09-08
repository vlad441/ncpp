#include <stdlib.h> // for size_t, atoi и atof, etc...
#include <stddef.h> // <cstddef> only typoe def? size_t, NULL, etc...
#include <string.h> // <cstring> for strlen, etc...

#ifdef _WIN32
#include <windows.h>
	void print(const char *cptr, size_t len){ WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cptr, (DWORD)len, NULL, NULL); }
#else
#include <unistd.h>
	void print(const char *cptr, size_t len){ write(STDOUT_FILENO, cptr, len); }
#endif
	void print(const char *cstr){ print(cstr, strlen(cstr)); }

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../src/stdcpp/stdcpp.cpp.h"

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

int main(){ print(NCPP_COMPILE_INFO);
	//std::cout << "sizeof(std::string): " << sizeof(std::string) << std::endl;
	//std::cout << "sizeof(test::string): " << sizeof(test::string) << std::endl;
	
	//const char* c="h"; //std::string str(c);
	//test::string tstr; tstr=c; print(tstr.c_str());
	//test::string tstr; print(tstr.c_str());
	//std::cout << "str: " << str << " | size: " << str.size() << "\n == capacity: " << str.capacity() << std::endl;
	
	//for(int i=0;i<10;i++){ tstr+="a"; std::cout << "tstr: " << tstr.c_str() << " | size: " << tstr.size() << "\n == capacity: " << tstr.capacity() << " | sso: " << tstr._isSso() << std::endl; }
	
	//test::vector<char> tvec; //for(int i=0;i<10;i++){ tvec.push_back('c'); std::cout << "tvec size: " << tvec.size() << " | capacity: " << tvec.capacity() << std::endl; }
	
	typedef test::unordered_map<test::string, test::string> MyMap;
	
	std::map<std::string, std::string> m1; MyMap t1;
	std::cout << "sizeof(std::map): " << sizeof(m1) << std::endl;
	std::cout << "sizeof(test::MyMap): " << sizeof(t1) << std::endl;
	std::cout << "std::map.size(): " << m1.size() << std::endl;
	std::cout << "test::map.size(): " << t1.size() << std::endl;
	
	m1["honkai"]="impact"; std::cout << "std::map.size(): " << m1.size() << std::endl;
	t1["honkai"]="impact"; std::cout << "test::map.size(): " << t1.size() << std::endl;
	t1["jopa"]="hui"; std::cout << "test::map.size(): " << t1.size() << std::endl;
	t1["3n"]="3n"; std::cout << "test::map.size(): " << t1.size() << std::endl;
	t1["4n"]="4n"; std::cout << "test::map.size(): " << t1.size() << std::endl;
		
	//std::cout << "test::map tree: \n" << t1._tree() << std::endl;
	
	t1.erase("honkai"); t1.erase("popa"); t1.erase("jopa"); t1.erase("4n"); t1["7n"]="7n"; t1["10n"]="10n";
	
	for(MyMap::const_iterator it = t1.begin(); it != t1.end(); ++it){
		std::cout << "test::map Key: " << it->first << ", Value: " << it->second << std::endl; }
	//std::cout << "test::map tree: \n" << t1._tree() << std::endl;
	
	std::cout << "test::map find: 3n - " << (t1.find("3n")!=t1.end()) << std::endl;
	std::cout << "test::map find: 7n - " << (t1.find("7n")!=t1.end()) << std::endl;
	std::cout << "test::map find: 10n - " << (t1.find("10n")!=t1.end()) << std::endl;
	std::cout << "test::map find: none - " << (t1.find("none")!=t1.end()) << std::endl;
	
	test::set<test::string> s1;
	
}
