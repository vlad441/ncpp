#include "src/ncpp.cpp"
using namespace ncpp;
Console cons;

#include "tests/tests-header.h"

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version(); //arch_current();

	cons << "1) "; cons.setColor(2) << "Hi, Julien Lete\n"; cons.reset();
	print("2) Is a simple print()\n");
	print("3) Array print: "); print(" "); print(String("Splited,String,Array").split(",")); print("\n");
	print("4) Console print: "); cons.setColor(3) << Array<String>().push("Array").push("for").push("Console") << "\n\n"; cons.reset();
	
	Buffer buff("Hi, ncpp");
	cons << "buff.toString(): " << buff.toString() << "\n";
	cons << "buff.toString(\"hex\"): " << buff.toString("hex") << "\n";
	cons << "buff.cout(): " << buff << "\n";
	
	//cons << "crypto::MD5().digest(): " << crypto::MD5("md5").digest() << std::endl;
}
