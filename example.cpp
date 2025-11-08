#include "src/ncpp.cpp"
using namespace ncpp;
Console cons;

#include "tests/tests-header.h"
//#include <typeinfo>

///Thread::Signal sg1;
///void timer_test(){ sg1.notify(); }

//void timer_test(Thread::Signal* sgn1){ (*sgn1).notify(); }

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version();

	cons.setColor(2) << "colored output\n"; cons.reset();
	print("Hi, Julien Lete\n - Is a print()\n\n");
	
	Buffer buff("Hi, ncpp");
	cons << "buff.toString(): " << buff.toString() << "\n";
	cons << "buff.toString(\"hex\"): " << buff.toString("hex") << "\n";
	cons << "buff.cout(): " << buff << "\n";
	
	//std::cout << "sizeof(std::mutex): " << sizeof(std::mutex) << std::endl;
	//std::cout << "sizeof(std::condition_variable): " << sizeof(std::condition_variable) << std::endl; 
	//std::cout << "sizeof(Thread::Signal): " << sizeof(Thread::Signal) << std::endl;
	//std::cout << "crypto::MD5().digest(): " << crypto::MD5("md5").digest() << std::endl;
}
