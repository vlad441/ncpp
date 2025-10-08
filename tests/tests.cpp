#ifdef NCPP_LIB_TEST
#include "../include/ncpp.h"
#else
#include "../src/ncpp.cpp"
#endif

using namespace ncpp;
int tested=0, passed=0, warnings=0, errors=0;
int warns=0, errs=0; String currtest, tresp;
Array<String> errsarr; Array<String> warnarr;
Console cons; //performance::start(); long long time1=performance::now();
//void AssertErr(){}, void AssertWarn(){}

//http::SaveStream("http://static.wikia.nocookie.net/test1666/images/c/c9/T8vlSlTOp3o.jpg/revision/latest?cb=20171017123525&path-prefix=ru", "./saved_stream");

#include "tests-header.h"

void clog(const CString& str, bool endl=false){ cons << str << "\n"; }
void subm(){ if(currtest.empty()) return; tresp="OK"; if(errs>0){ tresp="--ERR--"; errsarr.push(currtest); }
	else if(warns>0){ tresp="(+/-)"; warnarr.push(currtest); }else{ passed++; }
	warnings+=warns; warns=0; errors+=errs; errs=0; cons << tresp << "\n"; }
void NextTest(const CString& str){ subm(); currtest=str; tested++; cons << " " << str << " ... "; }

void base_Array_test(){}
void base_String_test(){}

void base_Buffer_test(){
	//=== Buffer === 
	NextTest("Buffer()"); Buffer buff; if(buff.size()!=0){ errs++; }
	
	const char* ctest="Test str"; Buffer* _bptr=NULL;
	NextTest("Buffer(const void* ptr, size_t len)"); buff = Buffer(ctest, 4);
	if(buff.size()!=4||strncmp((char*)&buff[0], ctest, 4)!=0){ errs++; }
	
	NextTest("Buffer(const char* cptr)"); buff = Buffer("Hi, ncpp");
	if(strncmp((char*)&buff[0], "Hi, ncpp", 8)!=0||buff!=Buffer("Hi, ncpp")){ errs++; }
	
	NextTest("Buffer(size_t size)"); buff = Buffer(6); if(buff.size()!=6){ errs++; } //Этот конструктор не стирает выделенную память
	NextTest("Buffer(size_t len, unsigned char v)"); buff = Buffer(5, 12); if(buff.size()!=5||buff[0]!=12||buff[4]!=12){ errs++; }
	
	NextTest("Buffer(const Buffer& b)"); buff = Buffer(ctest); _bptr = new Buffer(buff); Buffer& copy = *_bptr;
	if(copy!=buff||strncmp((char*)&copy[0], (char*)&buff[0], 8)!=0||copy.size()!=8||buff.size()!=8){ errs++; } delete _bptr;
	buff = ""; if(buff.size()!=0){ errs++; } // Тест на баг конструктора копирования.
	
	//Buffer(const CString& str, const CString& type="str");
	//Buffer(const BaseString<T, D>& s){ _init(s.size()); _set(s.data(), s.size()); }
	
	NextTest("Buffer(const void* begin, const void* end)"); buff = Buffer(ctest+2,ctest+7);
	if(buff.size()!=5||buff!="st st"||strncmp((char*)&buff[0], "st st", 5)!=0){ errs++; }
	
	NextTest("Buffer(const unsigned char (&arr)[N])"); unsigned char uarr[]={5,8,11}; buff = Buffer(uarr);
	if(buff.size()!=3||buff[0]!=5||buff[1]!=8||buff[2]!=11){ errs++; }
	
	//Buffer(std::vector<unsigned char> vec) : std::vector<unsigned char>(vec){}
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
	// === BEGIN TEST ===
	base_Array_test();
	base_String_test();
	base_Buffer_test();
	// === END TEST ===
	subm();
	
	cons.setColor(3) << "\n== Tests Complete ==\n";
	cons.reset() << " Tested|Passed: "; cons.setColor(3) << tested; cons.reset() << " | "; cons.setColor(2) << passed;
	cons.reset() << "; Warnings|Errors: "; cons.setColor(3) << warnings; cons.reset() << " | "; cons.setColor(1) << errors; cons.reset() << "\n";
	if(errors>0){ cons << " Errors pull: "; cons.setColor(1) << errsarr; cons.reset() << "\n"; return 1; }
	else if(warnings>0){ cons << " Warnings pull: "; cons.setColor(3) << warnarr; cons.reset() << "\n"; return 2; } return 0; }