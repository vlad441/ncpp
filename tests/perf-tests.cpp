//#include "../src/ncpp-base.cpp"
#include "../src/ncpp.cpp"
using namespace ncpp;

#include "tests-header.h"
Console cons;

void TIME_DIFF(const CString& descr, long long oldtime){ cons.setColor(6) << descr; 
	cons.setColor(3) << dtos((GetTimestamp('u')-oldtime)/1000.0, 3); cons.setColor(5) << " msec\n"; cons.reset(); }

int main(int argc, char* argv[]){ print(NCPP_COMPILE_INFO); os_version();
	int num=1000; print("sizeof(num): "); print(sizeof(num)); print("\n");
	print("dtos: "); print(18446744073709551615ULL); print("|\n");
	print("dtos: "); print(dtos(18446744073709551615ULL,' ')); print("|\n");
	print("dtos(double): "); print(dtos(3.234, 2)); print("|\n\n");
}