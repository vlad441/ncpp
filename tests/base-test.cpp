#include "../src/ncpp.cpp"
using namespace ncpp;

#include "tests-header.h"
Console cons;

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version();
	int num=1000; print("sizeof(num): "); print(sizeof(num)); print("\n");
	print("dtos: "); print(18446744073709551615ULL); print("|\n");
	print("dtos: "); print(dtos(18446744073709551615ULL,'\'')); print("|\n");
	print("dtos(double): "); print(dtos(3.234, 2)); print("|\n\n");
	
	cons.setColor(5) << "GetTimestamp('s'):" << GetTimestamp('s') << "\n";
	print("Enter timestamp:"); Date date(stolln(readline())); print("\n");
	cons << "\nDate::toDirectDate(): " << date.toDirectDate() << "\n";
	cons << "\nDate::toReverseDate(): " << date.toReverseDate() << "\n";
	cons.reset() << "\nDate::toISOString(): " << date.toISOString() << "\n";
	cons << "\nDate::toIMFDate(): " << date.toIMFDate() << "\n";
	cons << "\nDate::toTime(): " << date.toTime() << "\n";
	cons << "\nDate::toTime(short): " << date.toTime(true) << "\n";
	date._showDValue(Date::timestampToDate(date.timestamp));

	print("\n -- END PROGRAM --\n");
}