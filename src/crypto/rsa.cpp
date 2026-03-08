namespace ncpp{ namespace crypto{ namespace RSA{ 
#define PUBLIC_EXPONENT 65537
#define DEFAULT_BITS 1024

//p и q должны быть простыми числами
//PlainText: hi, c | <Buffer 68 69 2c 20 63> | 448441098339
//n=p*q: 318269061*3242605871 = 1032021125756257131
//(p-1)*(q-1): (318269061-1)*(3242605871-1) = 1032021122195382200
//d = modInverse(65537, 1032021122195382200) = 441187966805743673
//Проверка d: (65537*d) mod n=1: 
//C=M^e mod n: 448441098339^65537 mod 1032021125756257131 = 372141831376607994
//M=C^d mod n: 372141831376607994^441187966805743673 mod 1032021125756257131 = 27088326801042741
//modInverse test: https://www.dcode.fr/modular-inverse
	
	BigInt powMod_debug(BigInt base, BigInt exp, const BigInt& mod){ performance::start();
		double time[5]; for(int i=0;i<5;i++){ time[i]=0; } time[0]=performance::now(); long long iterations=0; int tmp;
		
		BigInt result(1);
		while(exp > 0){ iterations++; print("- powMod cycle: "); print(dtos(iterations));
			tmp=performance::now(); if(!exp.IsEven()){ time[1]+=performance::now()-tmp; 
				tmp=performance::now(); result = (result * base) % mod; time[2]+=performance::now()-tmp; }else{ time[1]+=performance::now()-tmp; }
			tmp=performance::now(); base = (base * base) % mod; time[3]+=performance::now()-tmp; tmp=performance::now(); exp /= 2; time[4]+=performance::now()-tmp; }
		
		time[0]=performance::now()-time[0];
		print("-= powMod iterations: "); print(dtos(iterations)); print("\n");
		print("-= powMod exp % 2 == 1 time: "); print(dtos(time[1])); print("\n");
		print("-= powMod result = (result * base) % mod time: "); print(dtos(time[2])); print("\n");
		print("-= powMod base = (base * base) % mod time: "); print(dtos(time[3])); print("\n");
		print("-= powMod exp = exp / 2 time: "); print(dtos(time[4])); print("\n");
		print("===> powMod total time: "); print(dtos(time[0])); print("\n"); return result; }

	BigInt gcd(const BigInt& a, const BigInt& b){ BigInt x = a; BigInt y = b;
		while (y != 0){ BigInt temp = y; y = x % y; x = temp; } return x; }
	
	BigInt modInverse(BigInt a, const BigInt& m0){ BigInt mod=m0, t, q, x0(0), x(1); if(mod == BigInt(1)){ return BigInt(0); }
		//if(a == mod){ print("(!) modInverse: Modular inverse does not exist: 'a' and 'mod' cannot be equal."); return BigInt(); }
		if(gcd(a, mod) != BigInt(1)){ print("(!) modInverse: Modular inverse does not exist: 'a' and 'mod' have a common divisor."); return BigInt(); }
		while (a > 1){ q = a / mod; t = mod; mod = a % mod; a = t; t = x0; x0 = x - q * x0; x = t; } if(x<0){ x += m0; } return x; }
	
	struct PrivateKey; struct PublicKey; struct MontgomeryCtx { BigInt R; BigInt R_inv; BigInt M_prime; };
	PrivateKey genPrivateKey(int bitLength);
	PublicKey genPublicKey(const PrivateKey& privKey);
	Pair<BigInt, BigInt> recoverPQ(const BigInt& n, const BigInt& d, const BigInt& e);
	struct PrivateKey {
		BigInt p;  // prime 1 (int 1)
		BigInt q;  // prime 2 (int 2)
		// основные производные (шифрование)
		BigInt n;  // public modulus (n=p*q)
		BigInt d;  // private exponent
		// вторичные производные (производительность)
		BigInt dP; // d mod (p-1)
		BigInt dQ; // d mod (q-1)
		BigInt qInv; // q^(-1) mod p
		//MontgomeryCtx m_ctx;
		
		PrivateKey(){  }
		PrivateKey(int bitLength){ generate(bitLength); }
		PrivateKey(const BigInt& p1, const BigInt& q1){ importPQ(p1, q1); }
		PrivateKey(const BigInt& n1, const BigInt& d1, const BigInt& e1){ import(n1, d1, e1); }

		void importPQ(const BigInt& p1, const BigInt& q1, const BigInt& e1=PUBLIC_EXPONENT){ n=0; 
			//std::cout << "PrivateKey::import: RSA DEBUG 0: Accepted:" << p1 << ", " << q1 << "(" << p << "," << q << ")" << std::endl;
			p=p1; q=q1; calcDerivedValues(e1); }
		void import(const BigInt& n1, const BigInt& d1, const BigInt& e1=PUBLIC_EXPONENT){ n=n1; d=d1; calcDerivedValues(e1); }
		void calcPQ(const BigInt& n1, const BigInt& d1, const BigInt& e1=PUBLIC_EXPONENT){ Pair<BigInt, BigInt> pq=recoverPQ(n1, d1, e1);
			importPQ(pq.first, pq.second, e1); }
		void generate(int bitLength=DEFAULT_BITS){ *this = genPrivateKey(bitLength); }
		void clear(){}
		void exportKey(){}
		size_t size() const { return d.size(); }
		size_t bits() const { return d.size()*8; }
		private:
			void calcDerivedValues(BigInt e1=PUBLIC_EXPONENT){ if(n==0){ n = p * q; d = modInverse(e1, (p-1)*(q-1));
				dP = d % (p-1); dQ = d % (q-1); qInv = modInverse(q, p); }
				//std::cout << "calcDerivedValues: RSA DEBUG 0: Accepted:" << n << " = " << p << " * " << q << std::endl; 
			}
	};

	struct PublicKey {
		BigInt n;  // modulus
		BigInt e;  // public exponent (default: 65537)
		PublicKey(){ calcDerivedValues(); }
		PublicKey(const PrivateKey& privKey){ import(privKey); }
		PublicKey(const BigInt& n1, const BigInt& e1=PUBLIC_EXPONENT){ n=n1; e=e1; calcDerivedValues(); }
		void import(const PrivateKey& privKey){ if(privKey.n==0){ n=privKey.p*privKey.q; }else{ n=privKey.n; } calcDerivedValues(); }
		void import(const BigInt& n1, const BigInt& e1=PUBLIC_EXPONENT){ n=n1; e=e1; calcDerivedValues(); }
		void clear(){}
		void exportKey(){}
		size_t size() const { return n.size(); }
		size_t bits() const { return n.size()*8; }
		private:
			void calcDerivedValues(){ if(e==0) e = PUBLIC_EXPONENT; }
	};
	
	bool CheckDivisibleBySmallPrimes(const BigInt& n){
		static const int smallArr[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 
			71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 
			179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293};
		for(int i=0;i<62;i++){ if(n % smallArr[i] == 0) return true; } return false; }
	
	bool isPrime(const BigInt& n, int iterations = 5){ //static int attempt=1; std::cout << "isPrime called: " << attempt++ << std::endl;
		if(CheckDivisibleBySmallPrimes(n)) return false;
		if(n < 2){ return false; } if(n != 2 && n % 2 == 0) return false;
		BigInt s = n - 1; while (s % 2 == 0) s /= 2;

		for (int i = 0; i < iterations; i++){
			BigInt a = BigInt(crypto::randomBytes(n.size())) % (n - 1) + 1; // Генерация a в диапазоне [1, n-1]
			BigInt temp = s; BigInt mod = BigInt::powMod(a, temp, n);
			while (temp != n - 1 && mod != 1 && mod != n - 1){
				mod = BigInt::powMod(mod, 2, n); temp *= 2; }
			if(mod != n - 1 && temp % 2 == 0) return false;
		} return true; }
	
	BigInt genPrimeNumb(int bitLength){ BigInt prime; do{ prime = BigInt::safeRandom(bitLength); 
			if(prime % 2 == 0) ++prime; }while(!isPrime(prime)); return prime; }
	
	PrivateKey genPrivateKey(int bitLength=DEFAULT_BITS){ PrivateKey privKey;
		BigInt p = genPrimeNumb(bitLength/2); BigInt q = genPrimeNumb(bitLength/2); privKey.importPQ(p,q);
		//std::cout << "genPrivateKey: RSA DEBUG 0: Accepted: bitLength = " << bitLength << ", p = " << p << ", q = " << q << std::endl;
		return privKey; }
	PublicKey genPublicKey(const PrivateKey& privKey){ PublicKey pubKey(privKey); return pubKey; }

    typedef Pair<PrivateKey, PublicKey> KeyPair; //Buffer a(15),b(15); a[0]=1; b[0]=1; b[14]=1; PrivateKey privKey(a, b);
	KeyPair genKeyPair(int bitLength=DEFAULT_BITS){ PrivateKey privKey(bitLength);
		PublicKey pubKey(privKey); return KeyPair(privKey, pubKey); }
		
	Pair<BigInt, BigInt> recoverPQ(const BigInt& n, const BigInt& d, const BigInt& e=PUBLIC_EXPONENT, int attempts=1000){
		BigInt k = e * d - 1; BigInt t = k; int s = 0; while(t % 2 == 0){ t = t / 2; ++s; }
		for (int i = 0; i < attempts; ++i){
			BigInt a = BigInt::random(n.size());  // Выбор случайного a от 2 до n-2
			BigInt x = BigInt::powMod(a, t, n); if(x == 1 || x == n-1) continue;
			for (int j = 0; j < s - 1; ++j){ x = BigInt::powMod(x, 2, n); if(x == n - BigInt(1)) break;
				if (x == 1){ BigInt p = gcd(x - BigInt(1), n); BigInt q = n / p; return Pair<BigInt, BigInt>(p, q); } }
		} return Pair<BigInt, BigInt>(0, 0); }
	
	BigInt mulMod(const BigInt& a, const BigInt& b, const BigInt& mod, const MontgomeryCtx& ctx);
	
	//PKCS#1 v1.5 или OAEP
	Buffer addPKCS1v15Padding(const Buffer& buffer, int blockSize){ return buffer; //test nothing;
		if ((int)buffer.size() > blockSize-11){ print("(!) Buffer size too large for PKCS#1 v1.5 padding."); return BigInt().toBuff(); }
		Buffer paddedBuffer; paddedBuffer.push_back(0x00); paddedBuffer.push_back(0x02);
		for (size_t i = 0; i < blockSize - buffer.size() - 3; ++i){ paddedBuffer.push_back(randInt(1,255)); }
		paddedBuffer.push_back(0x00); paddedBuffer+=buffer; return paddedBuffer; }
	Buffer removePKCS1v15Padding(const Buffer& buffer){ return buffer; //test nothing;
		if(buffer[0] != 0x00 || buffer[1] != 0x02){ print("(!) Invalid PKCS#1 v1.5 padding."); return BigInt().toBuff(); }
		size_t i = 2; while (i < buffer.size() && buffer[i] != 0x00){ ++i; }
		if(i == buffer.size()){ print("(!) Invalid PKCS#1 v1.5 padding."); return BigInt().toBuff(); } //addPKCS1v15Padding(
		return buffer.slice(i+1); }
		
	//C=M^e mod n (Example: C=65^17 mod 3233 = 2790)
	Buffer encryptBlock(const PublicKey& pubKey, const Buffer& data){ return BigInt::powMod(BigInt(data), pubKey.e, pubKey.n); }
	//M=C^d mod n (Example: M=2790^2753 mod 3233 = 65)
	//Buffer decryptBlock(const PrivateKey& privKey, const Buffer& buff){ return BigInt::powMod(BigInt(buff), privKey.d, privKey.n); } // Simple and Slower variant
	
	Buffer decryptBlock(const PrivateKey& privKey, const Buffer& buff){ //CRT RSA Decrypt
		BigInt C(buff);
		BigInt m1 = BigInt::powMod(C, privKey.dP, privKey.p); // 1. m1 = C^(dp) mod p (Используем p и dP из privKey) 
		BigInt m2 = BigInt::powMod(C, privKey.dQ, privKey.q); // 2. m2 = C^(dq) mod q (Используем q и dQ из privKey)

		BigInt h = privKey.qInv * (m1-m2).abs() % privKey.p; // 3. h = qInv * (m1 - m2) mod p
		BigInt M = m2 + (h * privKey.q); return M.toBuff(); } // 4. M = m2 + h * q
		
	Buffer encrypt(const PublicKey& pubKey, const Buffer& buffer, int blockSize=0){ int keysize=pubKey.size(); //std::cout << "encrypt: INPUT: n = " << pubKey.n << "(" << pubKey.n.cout() << ")" << std::endl;
		Buffer encryptedBuffer; if(blockSize<=0){ blockSize=keysize-11; if(blockSize<=0){ blockSize=1; } }
		for(size_t i = 0; i < buffer.size(); i += blockSize){ //std::cout << "encrypt: RSA DEBUG 1: cycle: " << i << "(Buff: " << buffer.slice(i,i+blockSize) << std::endl;
			encryptedBuffer+=addPKCS1v15Padding(BigInt::powMod(BigInt(buffer.slice(i,i+blockSize)), pubKey.e, pubKey.n), keysize); } //std::cout << "encrypt: RSA DEBUG 2: complete." << std::endl; 
			return encryptedBuffer; }
	
	Buffer decrypt(const PrivateKey& privKey, const Buffer& ebuffer, int blockSize=0){ int keysize=privKey.n.size();
		Buffer dbuffer; if(blockSize<=0){ blockSize=keysize-11; if(blockSize<=0){ blockSize=1; } }
		for (size_t i = 0; i < ebuffer.size(); i += blockSize){ //std::cout << "decrypt: RSA DEBUG 1: cycle: " << i << "(Buff: " << ebuffer.slice(i,i+blockSize) << std::endl;
			//std::cout << " --- powMod(" << BigInt(ebuffer.slice(i,i+blockSize)) << "," << privKey.d << "," << privKey.n << ") = " << BigInt::powMod(BigInt(ebuffer.slice(i,i+blockSize)), privKey.d, privKey.n) << std::endl;
			dbuffer+=removePKCS1v15Padding(BigInt::powMod(BigInt(ebuffer.slice(i,i+blockSize)), privKey.d, privKey.n)); } return dbuffer;
	}
}}}
