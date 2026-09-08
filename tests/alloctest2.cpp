#include "../src/ncpp.cpp"
#include "../src/experimental/allocators.cpp"
//using namespace ncpp;
#include "tests-header.h"


int main(){ print(NCPP_COMPILE_INFO); os_version();
	ncpp::print("alloctest2\n");
    return 0;
}