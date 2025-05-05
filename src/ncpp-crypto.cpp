namespace ncpp{ namespace crypto{
#ifdef _WIN32
#include <wincrypt.h>
#endif
	Buffer randomBytes(size_t length){ Buffer bytes(length); 
	#ifdef _WIN32
		HCRYPTPROV hProvider = 0;
		if(!CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT)){ throw std::runtime_error("CryptAcquireContext failed"); }
		if(!CryptGenRandom(hProvider, length, &bytes[0])){ CryptReleaseContext(hProvider, 0); 
			throw std::runtime_error("CryptGenRandom failed: "+dtos(GetLastError())); } CryptReleaseContext(hProvider, 0);
	#else
		std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
		if(!urandom){ throw std::runtime_error("Failed to open /dev/urandom"); }
		urandom.read((char*)&bytes[0], length); if(!urandom){ throw std::runtime_error("Failed to read from /dev/urandom"); }
	#endif
	return bytes; }

	struct Vigenere { int pos; Vigenere() : pos(0){}
		static Buffer encode(const Buffer& key, Buffer value){ for(size_t i=0;i<value.size();i++){ value[i] += key[i % key.size()]; } return value; }
		static Buffer decode(const Buffer& key, Buffer value){ for(size_t i=0;i<value.size();i++){ value[i] -= key[i % key.size()]; } return value; }
		Buffer encrypt(const Buffer& key, Buffer value){ 
			for(size_t i=0;i<value.size();i++){ value[i] += key[pos % key.size()]; pos = (pos + 1) % key.size(); } return value; }
		Buffer decrypt(const Buffer& key, Buffer value){
			for(size_t i=0;i<value.size();i++){ value[i] -= key[pos % key.size()]; pos = (pos + 1) % key.size(); } return value; }
	};
}}

#include "crypto/aes.cpp"
#include "crypto/rsa.cpp"
#include "crypto/dh.cpp"

#include "crypto/md5.cpp"
#include "crypto/sha1.cpp"
#include "crypto/sha2.cpp"
#include "crypto/sha3.cpp"


namespace ncpp{ namespace crypto{
	template <typename T>
	Buffer checksum(T& algo, const String& path){ algo.reset(); fs::FStream f = fs::createReadStream(path);
		if(f.destroyed){ throw Err("checksum: open file err: "+path); return Buffer(); } 
		Buffer buff(65536); while(f.read(&buff)>0){ algo.update(buff); } f.close(); return algo.digest(); }
			
	template <typename T>
	struct Hash { T algo; 
		Hash(){ algo.reset(); }
		Hash(const Buffer& input){ algo.reset(); algo.update(input); }
		static Buffer hash(const Buffer& input){ return T::hash(input); }

		void reset() { algo.reset(); }
		Hash& update(const Buffer& input){ algo.update(input); return *this; }
		Buffer digest(){ return algo.digest(); }
		Buffer checksum(const String& path){ return checksum(algo, path); }
	};
}}
