using namespace ncpp::crypto;

void hash_test(){
	NextTest("crypto::MD5.update()"); 
	TEST_EQ(MD5().update("").digest(),    Buffer::fromHex("d41d8cd98f00b204e9800998ecf8427e"));
	TEST_EQ(MD5().update("md5").digest(), Buffer::fromHex("1bc29b36f623ba82aaf6724fd3b16718"));
	
	NextTest("crypto::SHA1.update()"); 
	TEST_EQ(SHA1().update("").digest(), 	Buffer::fromHex("da39a3ee5e6b4b0d3255bfef95601890afd80709"));
	TEST_EQ(SHA1().update("sha1").digest(), Buffer::fromHex("415ab40ae9b7cc4e66d6769cb2c08106e8293b48"));
	
	NextTest("crypto::SHA256.update()"); 
	TEST_EQ(SHA256().update("").digest(), 		Buffer::fromHex("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
	TEST_EQ(SHA256().update("sha256").digest(), Buffer::fromHex("5d5b09f6dcb2d53a5fffc60c4ac0d55fabdf556069d6631545f42aa6e3500f2e"));
	
	NextTest("crypto::SHA512.update()"); 
	TEST_EQ(SHA512().update("").digest(), 		Buffer::fromHex("cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e"));
	TEST_EQ(SHA512().update("sha512").digest(), Buffer::fromHex("1f9720f871674c18e5fecff61d92c1355cd4bfac25699fb7ddfe7717c9669b4d085193982402156122dfaa706885fd64741704649795c65b2a5bdec40347e28a"));
	
	NextTest("crypto::SHA512(384).update()");
	TEST_EQ(SHA512(384).update("").digest(),  	   Buffer::fromHex("38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b"));
	TEST_EQ(SHA512(384).update("sha384").digest(), Buffer::fromHex("7ce49e4dcba0ff9b73658f077b077d852e30405d1e2890f9b80e7fac4970bb405f6dfcf1ebc1642019f05a818e59d218"));
	
	NextTest("crypto::SHA3_256.update()"); 
	TEST_EQ(SHA3_256().update("").digest(),   	Buffer::fromHex("a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a"));
	TEST_EQ(SHA3_256().update("sha3").digest(), Buffer::fromHex("6f8c90edbfe5c62f414208f03f62d3c4347774108ba5d6204733bc1fd5700015"));
}

void aes_test(){ Buffer key128(16, 4), key256(32, 4), iv(16, 0), ivGCM(12, 0), msg("Hello, ncpp. AES test."), encrypted, decrypted;
	NextTest("crypto::AES::ECB_encrypt()"); 
	encrypted=AES::ECB_encrypt(key128, msg); TEST_EQ(encrypted, Buffer::fromHex("665008970b9e66586afc8daf8675595e068cf68833bad7faa1eee0164936eab9"));
	encrypted=AES::ECB_encrypt(key256, msg); TEST_EQ(encrypted, Buffer::fromHex("f179e114a4c0e16827ff5c0e02ad3cb24355fbcad5304055112e091761a06237"));
    NextTest("crypto::AES::ECB_decrypt()"); decrypted=AES::ECB_decrypt(key256, encrypted); TEST_EQ(msg, decrypted);
	
	NextTest("crypto::AES::CBC_encrypt()"); 
	encrypted=AES::CBC_encrypt(key128, msg, iv); TEST_EQ(encrypted, Buffer::fromHex("665008970b9e66586afc8daf8675595e2b02172318137b3ff888c63a6ea9f5a0"));
	encrypted=AES::CBC_encrypt(key256, msg, iv); TEST_EQ(encrypted, Buffer::fromHex("f179e114a4c0e16827ff5c0e02ad3cb24d24fa9ffb39111202a0e70182d5ffa4"));
	NextTest("crypto::AES::CBC_decrypt()"); decrypted=AES::CBC_decrypt(key256, encrypted, iv); TEST_EQ(msg, decrypted);
	
	// NextTest("crypto::AES::GCM_encrypt()");
	// encrypted=AES::GCM_encrypt(key128, msg, ivGCM); TEST_EQ(encrypted, Buffer::fromHex("c698c2f2ec0f2b18a24d42fd8f98294b091642d77910"));
	// encrypted=AES::GCM_encrypt(key256, msg, ivGCM); TEST_EQ(encrypted, Buffer::fromHex("3879401be7ca600db9b826c083dcb4bb7984f6647c40"));
	// NextTest("crypto::AES::GCM_decrypt()"); decrypted=AES::GCM_decrypt(key256, encrypted, ivGCM); TEST_EQ(msg, decrypted); //Expected tag: <Buffer 5c 56 9a 78 74 a3 ce 9d b5 ac 8e 47 66 33 ac 3d>
}

void rsa_test(){ long long rsa_time=0; RSA::KeyPair rsakeys;
	NextTest("crypto::RSA::encryptBlock()"); rsetCons(); //warns++; //Not Tested
	//rsa_time = GetTimestamp('u'); rsakeys = RSA::genKeyPair(1024); TIME_DIFF("--- == RSA GenKeys Time: ", rsa_time); //Gen Keys
	//rsakeys.first.importPQ(BigInt("15828977"), BigInt("16077463")); rsakeys.second.import(rsakeys.first);
	//rsakeys.first.importPQ(BigInt("75797131722695861475172606992031509119172186956780163707139151017251613585203"), BigInt("109992043553178693191440878953539793384844853838531473229602035174807748241603")); rsakeys.second.import(rsakeys.first); //512 bits
	rsakeys.first.importPQ(BigInt("3161377483674993353580157803776595637801781126949870293893269819186692559042402919457491825746932932076955436456154826228120043500233418509132730979261463"), BigInt("2636561216376348341083701518877045797845347905956092099897315068450551132035414556430188485867254473093194509130087313577553584515715745231063675933326237")); 
	rsakeys.second.import(rsakeys.first); //1024 bits
	
	Buffer plain("hi, c"); //Buffer plain("hello, crypto.");
	cons << " -- RSA Key Size: " << rsakeys.first.bits() << ", " << rsakeys.second.bits() << " bits." << "\n";
	cons << " -- PlainText: " << plain.toString() << " | " << plain << " | " << plain.toIntString() << "\n";
	
	rsa_time = GetTimestamp('u'); Buffer encrypted = RSA::encryptBlock(rsakeys.second, plain);
	//TEST_EQ(encrypted.toIntString(), "8235268947183623068183478460196304961213547175072910579233784236435658850672398906533320054125179477276517696070551883850668816422827485613146966261594282"); //512 bits
	TEST_EQ(encrypted.toIntString(), "5623542485754736347978575501181970951979708163687114623030162026083814801639803726866927761209678137047458371683527034360734680727342429814600742809980164311129388909936072219668450882176946644864185058631449908263641080982843111238818230995303266505558788441378295881179836308028235595724066748921091479409"); //1024 bits
	cons << " -- RSA Encrypted: " << encrypted << " | " << encrypted.toIntString() << "\n";
	TIME_DIFF("--- == Time: ", rsa_time);
	
	NextTest("crypto::RSA::decryptBlock()"); rsetCons();
	rsa_time = GetTimestamp('u'); Buffer decrypted = RSA::decryptBlock(rsakeys.first, encrypted); TEST_EQ(plain, decrypted);
	cons << " -- RSA Decrypted: " << decrypted.toString() << " | " << decrypted << " | " << decrypted.toIntString() << "\n";
	TIME_DIFF("--- == Time: ", rsa_time);
	cons << "   == RSA Debug: PrivKey.n = " << rsakeys.first.n.toString() << "\n";
	cons << "   == RSA Debug: PrivKey.d = " << rsakeys.first.d.toString() << "\n";
}

void rsa_test_simple(){ RSA::KeyPair rsakeys; rsakeys.first.importPQ(61, 53, 17); rsakeys.second.import(rsakeys.first); rsakeys.second.e=17;
	//Buffer plain("hello, crypto."); 
	Buffer plain=BigInt(65).toBuff();
	
	cons << "RSA Key Size: " << rsakeys.first.bits() << ", " << rsakeys.second.bits() << " bits." << "\n";
	cons << "PlainText: " << plain.toString() << " | " << plain << "\n";
	
	Buffer encrypted = RSA::encryptBlock(rsakeys.second, plain); cons << "RSA Encrypted: " << encrypted << "\n";
	Buffer decrypted = RSA::decryptBlock(rsakeys.first, encrypted); cons << "RSA Decrypted: " << decrypted.toString() << " | " << decrypted << "\n"; }
	
void testDH(){ double dh_time=GetTimestamp('u');
	NextTest("crypto::DH::computeSecret()"); rsetCons(); 
	cons << "[DH] Generating keys...\n";
    DH::KeyPair alice = DH::genKeyPair(16); DH::KeyPair bob = DH::genKeyPair(16);
	
    cons << "[DH] Exchanging public keys and computing shared secrets...\n";
    BigInt secretA = DH::computeSecret(bob.second, alice.first);
    BigInt secretB = DH::computeSecret(alice.second, bob.first);
	
    cons << "[DH] Shared Secret (Alice): " << secretA.toHex() << "\n";
    cons << "[DH] Shared Secret   (Bob): " << secretB.toHex() << "\n";
	TIME_DIFF("[DH] --- Time: ", dh_time); TEST_EQ(secretA, secretB); TEST_EQ(secretA.toHex(), secretB.toHex());
}

void Module_Crypto_test(){ 
	hash_test(); aes_test();
	#ifdef ARM64_OR_RISC_V //TODO: RSA -> BigInt: ARM64 and RISC-V problemo.
	NextTest("[ARM64 & RISC-V] crypto::RSA tests skiped."); ARCH_SKIP();
	#else
	rsa_test(); //rsa_test_simple();
	testDH();
	#endif
}
