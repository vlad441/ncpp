#include "../src/ncpp.cpp"
using namespace ncpp;

#include "tests-header.h"

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version();
	int num=1000; print("sizeof(num): "); print(sizeof(num)); print("\n");
	print("dtos: "); print(18446744073709551615ULL); print("|\n");
	print("dtos: "); print(dtos(18446744073709551615ULL,'\'')); print("|\n");
	print("dtos(double): "); print(dtos(3.234, 2)); print("|\n\n");

	print("\n -- END PROGRAM --\n");
}