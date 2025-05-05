#include <iostream>
#include "src/ncpp.cpp"

using namespace ncpp;

//http::SaveStream("http://static.wikia.nocookie.net/test1666/images/c/c9/T8vlSlTOp3o.jpg/revision/latest?cb=20171017123525&path-prefix=ru", "./saved_stream");

/*void matan_test(){ // 26959946667150639794667015087019625481340285887712943950607281029120 / 65537 = 411369862324345633682759587515748744699029340490302332279586814
    std::cout << std::endl;
	BigInt r1("26959946667150639794667015087019625481340285887712943950607281029120"); 
	//Buffer::Math::divide_knuth(r1, 65537, true).first; // === Crush Test
	Buffer::Math::divide_single(r1, 65537, true);
	std::cout << "Buffer::Math::divide_single (etalon): " << r1 << " (R: 2)" << std::endl; // эталонное значение
	std::cout << "-------Buffer::Math::divide: " << Buffer::Math::divide_knuth(BigInt("26959946667150639794667015087019625481340285887712943950607281029120"), BigInt("65537")).first.toIntString() << std::endl;
	std::cout << "---Buffer::Math::divide: 180/7 = " << Buffer::Math::divide_knuth(BigInt("180"), BigInt("7")).first.toIntString() << " (Correct: ~25.71 - R: 5)" << std::endl;
	std::cout << "---Buffer::Math::divide: 500/80 = " << Buffer::Math::divide_knuth(BigInt("500"), BigInt("80")).first.toIntString() << " (Correct: 6,25 - R: 20)" << std::endl;
	std::cout << "---Buffer::Math::divide: 23242/123 = " << Buffer::Math::divide_knuth(BigInt("23242"), BigInt("123")).first.toIntString() << " (Correct: ~188,95 - R: 118)" << std::endl;
	std::cout << "---Buffer::Math::divide: 1892642/1623 = " << Buffer::Math::divide_knuth(BigInt("1892642"), BigInt("1623")).first.toIntString() << " (Correct: ~1166,13 - R: 224)" << std::endl;
	std::cout << "---Buffer::Math::divide: 162305008000/12305008000 = " << Buffer::Math::divide_knuth(BigInt("162305008000"), BigInt("12305008000")).first.toIntString() << " (Correct: ~13.19 - R: 2339904000)" << std::endl;
}*/

void cbor_test(){ Buffer cbor; Object obj; obj["test"]="txt"; obj["double"]=12.334; obj["int"]=13; obj["buff"]=Buffer(3,4); obj["obj"]=Object(); obj["obj"]["buff"]=Buffer(3,1);
	obj["obj"]["deep"]=Object(); obj["obj"]["deep"]["state"]=true;
	std::cout << "obj: " << obj << std::endl; 
	cbor=CBOR::serialize(obj); std::cout << "cbor buff: " << cbor << std::endl;  
	obj=CBOR::parse(cbor); std::cout << "cbor parsed: " << obj << std::endl; 
	std::cout << "cbor parsed(obj): " << obj["obj"].asObj() << std::endl;
	
	cbor=Buffer::from("0dff0904", "hex"); obj=CBOR::parse(cbor);
	std::cout << "== cbor buff 2: " << cbor << std::endl; 
	std::cout << "== cbor parsed 2: " << obj << std::endl;
	
}

void matan_test(){ // 26959946667150639794667015087019625481340285887712943950607281029120 / 65537 = 411369862324345633682759587515748744699029340490302332279586814
    std::cout << std::endl;
	BigInt r1("26959946667150639794667015087019625481340285887712943950607281029120"); Buffer::Math::divide_single(r1, 65537, true);
	std::cout << "Buffer::Math::divide_single: " << r1 << std::endl;
	std::cout << "-------Buffer::Math::divide: " << Buffer::Math::divide(BigInt("26959946667150639794667015087019625481340285887712943950607281029120"), BigInt("65537")).toIntString() << std::endl;
	std::cout << "---Buffer::Math::divide: 180/7 = " << Buffer::Math::divide(BigInt("180"), BigInt("7")).toIntString() << " (Correct: ~25.71 - R: 5)" << std::endl;
	std::cout << "---Buffer::Math::divide: 500/80 = " << Buffer::Math::divide(BigInt("500"), BigInt("80")).toIntString() << " (Correct: 6,25 - R: 20)" << std::endl;
	std::cout << "---Buffer::Math::divide: 23242/123 = " << Buffer::Math::divide(BigInt("23242"), BigInt("123")).toIntString() << " (Correct: ~188,95 - R: 118)" << std::endl;
	std::cout << "---Buffer::Math::divide: 1892642/1623 = " << Buffer::Math::divide(BigInt("1892642"), BigInt("1623")).toIntString() << " (Correct: ~1166,13 - R: 224)" << std::endl;
	std::cout << "---Buffer::Math::divide: 162305008000/12305008000 = " << Buffer::Math::divide(BigInt("162305008000"), BigInt("12305008000")).toIntString() << " (Correct: ~13.19 - R: 2339904000)" << " | R: " << Buffer::Math::mod(BigInt("162305008000"), BigInt("12305008000")).toIntString() << std::endl;
}

void rsa_test(){ crypto::RSA::KeyPair rsakeys;// = crypto::RSA::genKeyPair(512);
	//rsakeys.first.importPQ(BigInt("15828977"), BigInt("16077463")); rsakeys.second.import(rsakeys.first);
	rsakeys.first.importPQ(BigInt("75797131722695861475172606992031509119172186956780163707139151017251613585203"), BigInt("109992043553178693191440878953539793384844853838531473229602035174807748241603")); rsakeys.second.import(rsakeys.first); //512 bits
	//rsakeys.first.importPQ(BigInt("3161377483674993353580157803776595637801781126949870293893269819186692559042402919457491825746932932076955436456154826228120043500233418509132730979261463"), BigInt("2636561216376348341083701518877045797845347905956092099897315068450551132035414556430188485867254473093194509130087313577553584515715745231063675933326237")); rsakeys.second.import(rsakeys.first); //1024 bits
	Buffer plain("hi, c"); //Buffer plain("hello, crypto.");
	Buffer encrypted = crypto::RSA::encryptBlock(rsakeys.second, plain);
	Buffer decrypted = crypto::RSA::decryptBlock(rsakeys.first, encrypted);
	std::cout << "RSA Key Size: " << rsakeys.first.length << ", " << rsakeys.second.length << " bits." << std::endl;
	std::cout << "PlainText: " << plain.toString() << " | " << plain << " | " << plain.toIntString() << std::endl;
	std::cout << "RSA Encrypted: " << encrypted << " | " << encrypted.toIntString() << std::endl;
	std::cout << "RSA Decrypted: " << decrypted.toString() << " | " << decrypted << " | " << decrypted.toIntString() << std::endl;
	std::cout << "==RSA Debug: PrivKey.n = " << rsakeys.first.n << std::endl;
	std::cout << "==RSA Debug: PrivKey.d = " << rsakeys.first.d << std::endl;
}

void rsa_test_simple(){ crypto::RSA::KeyPair rsakeys;
	rsakeys.first.importPQ(61, 53, 17); rsakeys.second.import(rsakeys.first); rsakeys.second.e=17;
	//Buffer plain("hello, crypto."); 
	Buffer plain=BigInt(65);
	Buffer encrypted = crypto::RSA::encryptBlock(rsakeys.second, plain);
	Buffer decrypted = crypto::RSA::decryptBlock(rsakeys.first, encrypted);
	std::cout << "RSA Key Size: " << rsakeys.first.length << ", " << rsakeys.second.length << " bits." << std::endl;
	std::cout << "PlainText: " << plain.toString() << " | " << plain << std::endl;
	std::cout << "RSA Encrypted: " << encrypted << std::endl;
	std::cout << "RSA Decrypted: " << decrypted.toString() << " | " << decrypted << std::endl; }
// modInverse test: https://www.dcode.fr/modular-inverse

//p и q должны быть простыми числами
//PlainText: hi, c | <Buffer 68 69 2c 20 63> | 448441098339
//n=p*q: 318269061*3242605871 = 1032021125756257131
//(p-1)*(q-1): (318269061-1)*(3242605871-1) = 1032021122195382200
//d = modInverse(65537, 1032021122195382200) = 441187966805743673
//Проверка d: (65537*d) mod n=1: 
//C=M^e mod n: 448441098339^65537 mod 1032021125756257131 = 372141831376607994
//M=C^d mod n: 372141831376607994^441187966805743673 mod 1032021125756257131 = 27088326801042741

/*void BigInt_Test(){  
	std::cout << "long long multiply: 318269061 * 3242605871 = " << 318269061*3242605871 << std::endl;
	std::cout << "BigInt multiply:    318269061 * 3242605871 = " << BigInt("318269061")*BigInt("3242605871") << std::endl; 
	std::cout << "Encrypt powMod(512 bits) = " << crypto::RSA::powMod(BigInt("448441098339"), 65537, BigInt("8337081413648785545319633427184794663442231257812474053748924357873034396348443147463459176658193871428912054316060197154657584308858711521584149869800409")) << std::endl;
	std::cout << "Decrypt powMod(512 bits) = " << crypto::RSA::powMod(BigInt("8235268947183623068183478460196304961213547175072910579233784236435658850672398906533320054125179477276517696070551883850668816422827485613146966261594282"), BigInt("1255199083090049391575275386820153027208820907591676175112388980867803384176392806418198950849985557076510219996257973803858569120555626307266334391903437"), BigInt("8337081413648785545319633427184794663442231257812474053748924357873034396348443147463459176658193871428912054316060197154657584308858711521584149869800409")) << std::endl;
}

void unixsock_srv(){ UnixSocket usock("/tmp/hrenota.sock"); 
	std::cout << "UnixSocket: bind: " << usock.bind() << std::endl; usock.listen();
	std::cout << "UnixSocket: sockfd: " << usock.sockfd << std::endl;
	
	std::cout << "UnixSocket: Waiting connections..." << std::endl;
	UnixSocket clsock = usock._accept();
	std::cout << "UnixSocket: new connection: " << clsock.sockfd << std::endl;
	std::cout << "UnixSocket: data: " << clsock.recv() << std::endl;
}
void unixsock_cl(){ UnixSocket usock("/tmp/hrenota.sock"); 
	std::cout << "UnixSocket: isconnected: " << usock.connect() << std::endl;
	std::cout << "UnixSocket: data send: " << usock.send("hi, unix") << std::endl;
}*/

void cpp_version(){ print("[");
#if __cplusplus >= 202302L
	print(">=C++23");
#elif __cplusplus >= 202002L
	print("C++20");
#elif __cplusplus >= 201703L
	print("C++17");
#elif __cplusplus >= 201402L
	print("C++14");
#elif __cplusplus >= 201103L
	print("C++11");
#else
	print("c++98");
#endif
	print("] UTF-8 кирилица にも含み\n");
};

void gcc_version(){ print("[Compiller] ");
#if defined(__clang__)
	print("Clang "); print(__VERSION__); print("\n"); //std::cout << "Clang " << __clang_major__ << "." << __clang_minor__ << "." <<  __clang_patchlevel__ << std::endl;
#elif defined(__GNUC__)
	print("GCC "); print(__VERSION__); print("\n"); //std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." <<  __GNUC_PATCHLEVEL__ << std::endl;
#elif defined(_MSC_VER)
	print("MSVC? WTF?\n");
#else
	print("(Unknown)\n");
#endif
};

//#include <typeinfo>

//struct Promise { Thread::Signal signal; }; Promise promise;

//void timer_test(void* hui){ promise.signal.notify(); }

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); 
	//matan_test();
	//cbor_test(); 
	//rsa_test(); rsa_test_simple();
	//BigInt_Test();
	
	//crypto::DH::testDH();
	
	//unixsock_srv();
	//unixsock_cl();
	
	/*std::cout << "typeid(int): " << typeid(int).name() << std::endl; 
	std::cout << "typeid(char): " << typeid(char).name() << std::endl; 
	std::cout << "typeid(std::string): " << typeid(std::string).name() << std::endl; 
	std::cout << "typeid(std::string): " << typeid(typeid(std::string)).name() << std::endl;
	std::cout << std::endl;
	std::cout << "Disk::info(): " << system::Disk::info() << std::endl;
	std::cout << "Disk::models(): " << system::Disk::models() << std::endl;
	
	Array<system::Disk::DiskInfo> d1 = system::Disk::drives();
	Array<system::Disk::DiskInfo> d2 = system::Disk::logical();
	
	std::cout << "Disk::drives(): "; for(size_t i=0;i<d1.size();i++){ std::cout << "{ model: " << d1[i].model << ", size: " << d1[i].size << ", label: " << d1[i].label << "}, " << std::endl; }
	std::cout << "Disk::logical(): "; for(size_t i=0;i<d2.size();i++){ std::cout << "{ model: " << d2[i].model << ", space: [" << d2[i].used << "/" << d2[i].size << "], label: " << d2[i].label << "}, " << std::endl; }
	std::cout << "Disk::strUsage(): " << system::Disk::strUsage() << std::endl;
	std::cout << "Disk::strTotal(): " << system::Disk::strTotal() << std::endl;*/
	
	/*std::cerr << "first?" << std::endl;
	setTimeout(timer_test, 1500, NULL);
	std::cerr << "Thread::Signal: start wait" << std::endl;
	promise.signal.wait();
	std::cout << "Thread::Signal: end wait" << std::endl;
	
	
	std::cout << "sizeof(std::mutex): " << sizeof(std::mutex) << std::endl;
	std::cout << "sizeof(std::condition_variable): " << sizeof(std::condition_variable) << std::endl; 
	std::cout << "sizeof(Thread::Signal): " << sizeof(Thread::Signal) << std::endl;*/
	
	//print("Hi, Julien Lete\n - Is a print()\n");
}
