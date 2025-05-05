//#include <iostream>
#include "src/ncpp.cpp"
using namespace ncpp;

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

//#include <typeinfo>

///Thread::Signal sg1;
///void timer_test(){ sg1.notify(); }

//void timer_test(Thread::Signal* sgn1){ (*sgn1).notify(); }

int main(int argc, char* argv[]){ cpp_version(); gcc_version();

	/*Buffer buff("Hi, ncpp");
	std::cout << "buff.toString(): " << buff.toString() << std::endl;
	std::cout << "buff.toString(\"hex\"): " << buff.toString("hex") << std::endl;
	std::cout << "buff.cout(): " << buff << std::endl;*/
	
	//Thread::Signal sg1;
	//std::cerr << "first?" << std::endl;
	//setTimeout(timer_test, 1500);
	//std::cerr << "Thread::Signal: start wait" << std::endl;
	//sg1.wait();
	//std::cout << "Thread::Signal: end wait" << std::endl;
	
	
	//std::cout << "sizeof(std::mutex): " << sizeof(std::mutex) << std::endl;
	//std::cout << "sizeof(std::condition_variable): " << sizeof(std::condition_variable) << std::endl; 
	//std::cout << "sizeof(Thread::Signal): " << sizeof(Thread::Signal) << std::endl;
	//std::cout << "crypto::MD5().digest(): " << crypto::MD5("md5").digest() << std::endl;

	print("Hi, Julien Lete\n - Is a print()\n");
}
