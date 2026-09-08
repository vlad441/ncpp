#ifdef NCPP_LIB_USE
//#include "../include/ncpp.h"
#include "../src/ncpp.cpp.h"
#else
#include "../src/ncpp.cpp"
#endif
using namespace ncpp;

#include "tests-header.h"
//#include "../src/experimantal/xml-parser.cpp"
#ifndef NCPP_LIB_USE
Console cons;
#endif

struct RGB { float r; float g; float b; };
RGB HexColorToFloat(unsigned int hex){ RGB rgb; rgb.r = ((hex >> 16) & 0xFF) / 255.0f;
	rgb.g = ((hex >> 8) & 0xFF) / 255.0f; rgb.b = (hex & 0xFF) / 255.0f; return rgb; }
	
#ifndef _WIN32
//#include <sys/utsname.h> //for ::uname(&sysinfo);
#endif
int main(int argc, char* argv[]){ print(NCPP_COMPILE_INFO); os_version();
#ifdef _WIN32
	//setlocale(0,""); SetConsoleCP(65001); SetConsoleOutputCP(65001);
#endif
	int num=1000; print("sizeof(num): "); print(sizeof(num)); print("\n");
	print("dtos: "); print(18446744073709551615ULL); print("|\n");
	#ifndef NCPP_LIB_USE
	print("dtos: "); print(dtos(18446744073709551615ULL,'\'')); print("|\n");
	print("dtos(double): "); print(dtos(3.234, 2)); print("|\n\n");
	#endif
	
	/*cons.setColor(5) << "GetTimestamp('s'):" << GetTimestamp('s') << "\n";
	print("Enter timestamp:"); Date date(stolln(readline())); print("\n");
	cons << "\nDate::toDirectDate(): " << date.toDirectDate() << "\n";
	cons << "\nDate::toReverseDate(): " << date.toReverseDate() << "\n";
	cons.reset() << "\nDate::toISOString(): " << date.toISOString() << "\n";
	cons << "\nDate::toIMFDate(): " << date.toIMFDate() << "\n";
	cons << "\nDate::toTime(): " << date.toTime() << "\n";
	cons << "\nDate::toTime(short): " << date.toTime(true) << "\n";
	date._showDValue(Date::timestampToDate(date.timestamp));*/
	
	//Mouse::SetPos(100,100); Sleep(200);
	//Mouse::SetPos(110,150); Sleep(400);
	//Mouse::SetPos(200,250); Keyboard::PressKey('k');
	
	//while(true){ String input = readline(); print(IsFloat(input.data(), input.size())); print("\n"); }
	//while(true){ String input = readline(); print(input.split(" ", 1)); print("\n"); }
	
	//while(true){ BigInt n(readline()); print(n.parts); print(";\n"); }

#if __SIZEOF_INT128__
    print("__SIZEOF_INT128__ - SUPPORTED\n");
	print("sizeof(__int128): "); print(sizeof(__int128)); print("\n");
#else
    print("__SIZEOF_INT128__ - NOT SUPPORTED\n");
#endif
	#ifndef NCPP_LIB_USE
	print("BigInt::ELEM_BASE = "); print(BigInt::ELEM_BASE); print("\n");
	print("BigInt::ELEM_SZ = "); print(BigInt::ELEM_SZ); print("\n");
	print("sizeof(BigInt::CARRY_T) = "); print(sizeof(BigInt::CARRY_T)); print("\n");
	//#include <sys/utsname.h>
	#ifndef _WIN32
	struct utsname sysinfo;
	if(uname(&sysinfo) == 0){
		print("Name (sysname): "); print(sysinfo.sysname); print("\n");
		print("Kernel (release): "); print(sysinfo.release); print("\n");
		print("Arch (machine): "); print(sysinfo.machine); print("\n");
    }else{ print("(!)uname failed\n"); }
	#endif
	
	//uint128_t myval=13; print("uint128_t myval = "); myval._print128('\''); print("\n");
	//uint128_t myval=(~(uint128_t)0); print("uint128_t myval = "); char cstr[52]; dtos(cstr, myval, '\''); print(cstr); print("\n");
	//print("uint128_t myval = "); print(myval._toBuff()); print("\n");
	//myval = uint128_t::mul_64x64_128(0xFFFFFFFFFFFFFFFAULL, 0xFFFFFFFFFFFFFFFAULL);
	//print("uint128_t::mul_64x64_128 = "); myval._print(); print("\n");
	//print("uint128_t::mul_64x64_128 = "); print(myval._toBuff()); print("\n");
	//340282366920938463242013678547253592100
	
	//while(true){ BigInt n(readline()); print(n.parts); print("\n"); print(n.toString()); print(" == metka :)\n"); }
	
	Array<int> arrI; arrI.push(13).push(10); int i=arrI.join(10);
	Array<String> arr; arr.push("st").push("13"); String str=arr.join("|");
	print("Array<String>: "); print(arr); print("\n");
	print("Array<int>: "); print(arrI); print("\n");
	print("str: "); print(str); print("\n");
	print("i: "); print(i); print("\n");
	
	while(true){ long long timestamp = stolln(readline()); print("Timestamp: "); print(timestamp); print("\n"); 
		Date::DValue dv = Date::parseTimestamp(timestamp); Date date(timestamp);
		Date::_showDValue(dv);
		print("Date.toTime: "); print(date.toTime()); print("\n");
		print("Date.toTime(true): "); print(date.toTime(true)); print("\n");
		print("date.toIMFDate(): "); print(date.toIMFDate()); print("\n");
		print("Date: "); print(date.toReverseDate()); print("\n"); 
		print("-- Date(String dateStr).timestamp: "); print(Date(date.toReverseDate()).timestamp); print("\n\n"); }
		
	#endif
	print("\n -- END PROGRAM --\n");
}