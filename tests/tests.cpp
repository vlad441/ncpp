#include <iostream>
#ifdef NCPP_LIB_TEST
#include "../include/ncpp.h"
#else
#include "../src/ncpp.cpp"
#endif

using namespace ncpp;
int tested=0, passed=0, warnings=0, errors=0;
int warns=0, errs=0; std::string currtest;
Array<String> errsarr;
Array<String> warnarr;

//http::SaveStream("http://static.wikia.nocookie.net/test1666/images/c/c9/T8vlSlTOp3o.jpg/revision/latest?cb=20171017123525&path-prefix=ru", "./saved_stream");

#include "tests-header.h"

void clog(std::string str, bool endl=false){ std::cout << str << std::endl; }
void subm(){ std::string resp="OK"; if(errs>0){ resp="--ERR--"; errsarr.push(currtest); }
	else if(warns>0){ resp="(+/-)"; warnarr.push(currtest); }else{ passed++; } 
	warnings+=warns; warns=0; errors+=errs; errs=0; std::cout << resp << std::endl; }
void begin_test(std::string str){ currtest=str; tested++; std::cout << str << " ... "; }
void ntest(std::string str){ subm(); currtest=str; tested++; std::cout << str << " ... "; } //next_test

void base_test(){
	//=== Buffer ===
	begin_test("Buffer()"); Buffer buff; if(buff.size()!=0){ errs++; }
	
	ntest("Buffer(size_t size, unsigned char value=0)"); buff = Buffer(5, 12);
	if(buff.size()!=5||buff[0]!=12){ errs++; }
	
	ntest("Buffer(const char* cstr)"); buff = Buffer("Hi, ncpp");
	if(strncmp((char*)&buff[0], "Hi, ncpp", 8)!=0||buff!=Buffer("Hi, ncpp")){ errs++; }
	
	ntest("Buffer(const void* ptr, size_t size)"); buff = Buffer("Hi,", 3);
	if(buff.size()!=3||strncmp((char*)&buff[0], "Hi,", 3)!=0||buff!=Buffer("Hi,")){ errs++; }
	
	//ntest("Buffer(const std::string& str, const char* type=\"str\")"); buff = Buffer("Hi, ncpp", "base64");
	
	//Buffer(const std::string& str, const char* type="str"){ _init(str, type); } // Конструктор для константной строки.
	//Buffer(std::vector<unsigned char> vec) : std::vector<unsigned char>(vec){}
	// === END TEST ===
	subm();
}

/*void testDH() {
    std::cout << "[DH] Generating keys..." << std::endl;
    KeyPair alice = generateKeyPair();
    KeyPair bob = generateKeyPair();

    std::cout << "[DH] Exchanging public keys and computing shared secrets..." << std::endl;
    BigInt secretA = computeSecret(bob.second, alice.first);
    BigInt secretB = computeSecret(alice.second, bob.first);

    std::cout << "[DH] Shared Secret (Alice): " << secretA.toHexString() << std::endl;
    std::cout << "[DH] Shared Secret (Bob):   " << secretB.toHexString() << std::endl;
    std::cout << "[DH] Match: " << (secretA == secretB ? "true" : "false") << std::endl;
}
/*[DH] Generating keys...
[DH] Exchanging public keys and computing shared secrets...
[DH] Shared Secret (Alice): b18f8f93379e9e5bb668515b79dc2af49911cc7f7ef07344c54506f856fca8dc1ca30ddd3cf3d0fdbd9b9ae2477c499ae6aae39e28f267ca59abc7a3495057f37fd295e5e7883b285bcaf19f24a0fad6ce65f883409b44bd8f9fed50799e25be336d05fcb95182065536ab2022c23dc8f277026a8885d8061eb4c5a69640a3ec
[DH] Shared Secret (Bob):   b18f8f93379e9e5bb668515b79dc2af49911cc7f7ef07344c54506f856fca8dc1ca30ddd3cf3d0fdbd9b9ae2477c499ae6aae39e28f267ca59abc7a3495057f37fd295e5e7883b285bcaf19f24a0fad6ce65f883409b44bd8f9fed50799e25be336d05fcb95182065536ab2022c23dc8f277026a8885d8061eb4c5a69640a3ec
[DH] Match: true*/

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version();

	base_test();
	
	std::cout << std::endl << "== Tests Complete ==" << std::endl;
	std::cout << " Tested|Passed: " << tested << " | " << passed << "; Warnings|Errors: " << warnings << " | " << errors << std::endl;
	if(errors>0){ std::cout << " Errors pull: " << errsarr << std::endl; }
	else if(warnings>0){ std::cout << " Warnings pull: " << warnarr << std::endl; }
}
