#ifdef NCPP_LIB_TEST
#include "../include/ncpp.h"
#else
#include "../src/ncpp.cpp"
#endif

using namespace ncpp;
int tested=0, passed=0, warnings=0, errors=0;
int warns=0, errs=0; String currtest;
Array<String> errsarr;
Array<String> warnarr;
Console cons;

//http::SaveStream("http://static.wikia.nocookie.net/test1666/images/c/c9/T8vlSlTOp3o.jpg/revision/latest?cb=20171017123525&path-prefix=ru", "./saved_stream");

#include "tests-header.h"

void clog(const CString& str, bool endl=false){ cons << str << "\n"; }
void subm(){ String resp="OK"; if(errs>0){ resp="--ERR--"; errsarr.push(currtest); }
	else if(warns>0){ resp="(+/-)"; warnarr.push(currtest); }else{ passed++; } 
	warnings+=warns; warns=0; errors+=errs; errs=0; cons << resp << "\n"; }
void begin_test(const CString& str){ currtest=str; tested++; cons << str << " ... "; }
void ntest(const CString& str){ subm(); currtest=str; tested++; cons << str << " ... "; } //next_test

void base_test(){
	//=== Buffer ===
	begin_test("Buffer()"); Buffer buff; if(buff.size()!=0){ errs++; }
	
	ntest("Buffer(size_t size, unsigned char value=0)"); buff = Buffer(5, 12);
	if(buff.size()!=5||buff[0]!=12){ errs++; }
	
	ntest("Buffer(const char* cstr)"); buff = Buffer("Hi, ncpp");
	if(strncmp((char*)&buff[0], "Hi, ncpp", 8)!=0||buff!=Buffer("Hi, ncpp")){ errs++; }
	
	ntest("Buffer(const void* ptr, size_t size)"); buff = Buffer("Hi,", 3);
	if(buff.size()!=3||strncmp((char*)&buff[0], "Hi,", 3)!=0||buff!=Buffer("Hi,")){ errs++; }
	
	//ntest("Buffer(const CString& str, const char* type=\"str\")"); buff = Buffer("Hi, ncpp", "base64");
	
	//Buffer(const CString& str, const char* type="str"){ _init(str, type); } // Конструктор для константной строки.
	//Buffer(std::vector<unsigned char> vec) : std::vector<unsigned char>(vec){}
	// === END TEST ===
	subm();
}

/*void testDH() {
    cons << "[DH] Generating keys...\n";
    KeyPair alice = generateKeyPair();
    KeyPair bob = generateKeyPair();

    cons << "[DH] Exchanging public keys and computing shared secrets...\n";
    BigInt secretA = computeSecret(bob.second, alice.first);
    BigInt secretB = computeSecret(alice.second, bob.first);

    cons << "[DH] Shared Secret (Alice): " << secretA.toHexString() << "\n";
    cons << "[DH] Shared Secret (Bob):   " << secretB.toHexString() << "\n";
    cons << "[DH] Match: " << (secretA == secretB ? "true" : "false") << "\n";
}
/*[DH] Generating keys...
[DH] Exchanging public keys and computing shared secrets...
[DH] Shared Secret (Alice): b18f8f93379e9e5bb668515b79dc2af49911cc7f7ef07344c54506f856fca8dc1ca30ddd3cf3d0fdbd9b9ae2477c499ae6aae39e28f267ca59abc7a3495057f37fd295e5e7883b285bcaf19f24a0fad6ce65f883409b44bd8f9fed50799e25be336d05fcb95182065536ab2022c23dc8f277026a8885d8061eb4c5a69640a3ec
[DH] Shared Secret (Bob):   b18f8f93379e9e5bb668515b79dc2af49911cc7f7ef07344c54506f856fca8dc1ca30ddd3cf3d0fdbd9b9ae2477c499ae6aae39e28f267ca59abc7a3495057f37fd295e5e7883b285bcaf19f24a0fad6ce65f883409b44bd8f9fed50799e25be336d05fcb95182065536ab2022c23dc8f277026a8885d8061eb4c5a69640a3ec
[DH] Match: true*/

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version();

	base_test();
	
	cons.setColor(3) << "\n== Tests Complete ==\n";
	cons.reset() << " Tested|Passed: "; cons.setColor(3) << tested; cons.reset() << " | "; cons.setColor(2) << passed;
	cons.reset() << "; Warnings|Errors: "; cons.setColor(3) << warnings; cons.reset() << " | "; cons.setColor(1) << errors; cons.reset() << "\n";
	if(errors>0){ cons << " Errors pull: " << errsarr << "\n"; }
	else if(warnings>0){ cons << " Warnings pull: " << warnarr << "\n"; }
}
