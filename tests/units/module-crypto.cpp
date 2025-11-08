using namespace ncpp::crypto;

void rsa_test(){ long long rsa_time=0; RSA::KeyPair rsakeys;
	NextTest("crypto::RSA::encryptBlock()"); rsetCons(); //warns++; //Not Tested
	//rsa_time = GetTimestamp('u'); rsakeys = RSA::genKeyPair(512); TIME_DIFF("--- == RSA GenKeys Time: ", rsa_time); //Gen Keys
	//rsakeys.first.importPQ(BigInt("15828977"), BigInt("16077463")); rsakeys.second.import(rsakeys.first);
	rsakeys.first.importPQ(BigInt("75797131722695861475172606992031509119172186956780163707139151017251613585203"), BigInt("109992043553178693191440878953539793384844853838531473229602035174807748241603")); rsakeys.second.import(rsakeys.first); //512 bits
	//rsakeys.first.importPQ(BigInt("3161377483674993353580157803776595637801781126949870293893269819186692559042402919457491825746932932076955436456154826228120043500233418509132730979261463"), BigInt("2636561216376348341083701518877045797845347905956092099897315068450551132035414556430188485867254473093194509130087313577553584515715745231063675933326237")); rsakeys.second.import(rsakeys.first); //1024 bits
	
	Buffer plain("hi, c"); //Buffer plain("hello, crypto.");
	cons << " -- RSA Key Size: " << rsakeys.first.bits() << ", " << rsakeys.second.bits() << " bits." << "\n";
	cons << " -- PlainText: " << plain.toString() << " | " << plain << " | " << plain.toIntString() << "\n";
	
	rsa_time = GetTimestamp('u'); Buffer encrypted = RSA::encryptBlock(rsakeys.second, plain);
	TEST_EQ(encrypted.toIntString(), "8235268947183623068183478460196304961213547175072910579233784236435658850672398906533320054125179477276517696070551883850668816422827485613146966261594282");
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

void Crypto_module_test(){
	rsa_test(); //rsa_test_simple();
	testDH();
}