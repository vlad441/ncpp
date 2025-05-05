namespace ncpp{ namespace crypto{ namespace AES{
	const unsigned char sbox[256] = {
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
	const unsigned char rsbox[256] = {
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
	}; const unsigned char Rcon[15] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6C, 0xD8, 0xAB, 0x4D, 0x9A };
	
	// Функции для умножения в поле GF(2^8) для AES
	unsigned char mul02(unsigned char x){ return (x << 1) ^ ((x & 0x80) ? 0x1b : 0x00); }
	unsigned char mul03(unsigned char x){ return mul02(x) ^ x; }
	unsigned char mul09(unsigned char x){ return mul02(mul02(mul02(x))) ^ x; }
	unsigned char mul0b(unsigned char x){ return mul02(mul02(mul02(x))) ^ mul02(x) ^ x; }
	unsigned char mul0d(unsigned char x){ return mul02(mul02(mul02(x))) ^ mul02(mul02(x)) ^ x; }
	unsigned char mul0e(unsigned char x){ return mul02(mul02(mul02(x))) ^ mul02(mul02(x)) ^ mul02(x); }
	
    void SubBytes(Buffer& state){ for(size_t i = 0; i < state.size(); ++i){ state[i] = sbox[state[i]]; } }
    void InvSubBytes(Buffer& state){ for (size_t i = 0; i < state.size(); ++i){ state[i] = rsbox[state[i]]; } }

    void ShiftRows(Buffer& state){ unsigned char temp;
		temp = state[1]; state[1] = state[5]; state[5] = state[9]; state[9] = state[13]; state[13] = temp;
		temp = state[2]; state[2] = state[10]; state[10] = temp; temp = state[6]; state[6] = state[14]; state[14] = temp;
		temp = state[15]; state[15] = state[11]; state[11] = state[7]; state[7] = state[3]; state[3] = temp; }

	void InvShiftRows(Buffer& state){ unsigned char temp;
		temp = state[13]; state[13] = state[9]; state[9] = state[5]; state[5] = state[1]; state[1] = temp;
		temp = state[2]; state[2] = state[10]; state[10] = temp; temp = state[6]; state[6] = state[14]; state[14] = temp;
		temp = state[3]; state[3] = state[7]; state[7] = state[11]; state[11] = state[15]; state[15] = temp; }

	void MixColumns(Buffer& state){ unsigned char tmp[16];
		for (int i = 0; i < 4; ++i){
			unsigned char s0 = state[4*i + 0]; unsigned char s1 = state[4*i + 1];
			unsigned char s2 = state[4*i + 2]; unsigned char s3 = state[4*i + 3];

			tmp[4*i + 0] = (unsigned char)(mul02(s0) ^ mul03(s1) ^ s2 ^ s3);
			tmp[4*i + 1] = (unsigned char)(s0 ^ mul02(s1) ^ mul03(s2) ^ s3);
			tmp[4*i + 2] = (unsigned char)(s0 ^ s1 ^ mul02(s2) ^ mul03(s3));
			tmp[4*i + 3] = (unsigned char)(mul03(s0) ^ s1 ^ s2 ^ mul02(s3));
		} state.assign(tmp, tmp + 16); }

	void InvMixColumns(Buffer& state){ unsigned char tmp[16];
		for (int i = 0; i < 4; ++i) {
			unsigned char s0 = state[4*i + 0]; unsigned char s1 = state[4*i + 1];
			unsigned char s2 = state[4*i + 2]; unsigned char s3 = state[4*i + 3];

			tmp[4*i + 0] = (unsigned char)(mul0e(s0) ^ mul0b(s1) ^ mul0d(s2) ^ mul09(s3));
			tmp[4*i + 1] = (unsigned char)(mul09(s0) ^ mul0e(s1) ^ mul0b(s2) ^ mul0d(s3));
			tmp[4*i + 2] = (unsigned char)(mul0d(s0) ^ mul09(s1) ^ mul0e(s2) ^ mul0b(s3));
			tmp[4*i + 3] = (unsigned char)(mul0b(s0) ^ mul0d(s1) ^ mul09(s2) ^ mul0e(s3));
		} state.assign(tmp, tmp + 16); }

    void AddRoundKey(Buffer& state, const Buffer& roundKey){ for(size_t i = 0; i < state.size(); ++i){ state[i] ^= roundKey[i]; } }
	void KeyExpansion(const Buffer& key, std::vector<Buffer>& roundKeys, const int& rounds){
        int keySize = key.size(); int numWords = keySize / 4; int numRoundKeys = 4 * (rounds + 1); 
		roundKeys.resize(numRoundKeys/4); unsigned char temp[4];

        for(size_t i = 0; i < roundKeys.size(); ++i){ roundKeys[i].resize(16); }
        for(int i = 0; i < numWords; ++i){ for(int j = 0; j < 4; ++j){ roundKeys[i / 4][4 * (i % 4) + j] = key[4 * i + j]; } } // Первые 4 roundKeys
        for(int i = numWords; i < numRoundKeys; ++i){ // остальные roundKeys
            for (int j = 0; j < 4; ++j){ temp[j] = roundKeys[(i - 1) / 4][4 * ((i - 1) % 4) + j]; }

            if (i % numWords == 0){ unsigned char tempByte = temp[0]; // RotWord
                temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = tempByte;
                for(int j = 0; j < 4; ++j){ temp[j] = sbox[temp[j]]; } // SubWord
                temp[0] ^= Rcon[i / numWords - 1]; // Rcon
            } else if (numWords > 6 && i % numWords == 4){ for(int j = 0; j < 4; ++j){ temp[j] = sbox[temp[j]]; } } // SubWord
            for(int j = 0; j < 4; ++j){ roundKeys[i / 4][4 * (i % 4) + j] = roundKeys[(i - numWords) / 4][4 * ((i - numWords) % 4) + j] ^ temp[j]; } // XOR
        }
	}
	int calc_rounds(const Buffer& key){ int rounds=10; if(key.size()==16){ rounds=10; }else if(key.size() == 24){ rounds = 12; }
		else if(key.size()==32){ rounds = 14; }else{ throw std::runtime_error("AES: Invalid key size. Length: "+dtos((int)key.size()*8)+" bits"); } 
		return rounds; }
	
	void addPKCS7Padding(Buffer& data, const char& blockSize=16){
		int dataSize = data.size(); int paddingLength = blockSize - (dataSize % blockSize);
		if(paddingLength == 0){ paddingLength = blockSize; }
		char paddingValue = static_cast<char>(paddingLength);
		for (int i = 0; i < paddingLength; ++i){ data.push_back(paddingValue); } }
	void removePKCS7Padding(Buffer& data, const char& blockSize=16){ if (data.empty()){ return; }
		char lastByte = data.back();
		int paddingLength = static_cast<int>(lastByte);
		if(paddingLength > blockSize || paddingLength == 0){ throw std::runtime_error("AES: Invalid PKCS7 padding length. paddingLength: "+dtos(paddingLength)+"; blockSize: "+dtos(blockSize)); }
		for(int i = 0; i < paddingLength; ++i){ data.pop_back(); } }
	
	Buffer encryptBlock(const std::vector<Buffer>& roundKeys, Buffer block, const int& rounds=10){
        AddRoundKey(block, roundKeys[0]);
        for (int round = 1; round < rounds; ++round){
            SubBytes(block); ShiftRows(block);
            MixColumns(block); AddRoundKey(block, roundKeys[round]); }
        SubBytes(block); ShiftRows(block); AddRoundKey(block, roundKeys[rounds]); return block; }

    Buffer decryptBlock(const std::vector<Buffer>& roundKeys, Buffer block, const int& rounds=10){
        AddRoundKey(block, roundKeys[rounds]);
        for (int round = rounds - 1; round > 0; --round){
            InvShiftRows(block); InvSubBytes(block);
            AddRoundKey(block, roundKeys[round]); InvMixColumns(block); }
        InvShiftRows(block); InvSubBytes(block); AddRoundKey(block, roundKeys[0]); return block; }
	
	Buffer ECB_encrypt(const Buffer& key, Buffer value){ static const char blockSize=16; int rounds=calc_rounds(key); 
		std::vector<Buffer> roundKeys; KeyExpansion(key, roundKeys, rounds); addPKCS7Padding(value, blockSize); 
		Buffer encryptedData; size_t dataSize = value.size();
		for (size_t i = 0; i < dataSize; i += blockSize){
			Buffer block = value.slice(i, i+blockSize);
			Buffer encryptedBlock = encryptBlock(roundKeys, block, rounds);
			encryptedData.insert(encryptedData.end(), encryptedBlock.begin(), encryptedBlock.end());
		} return encryptedData; }
	
	Buffer ECB_decrypt(const Buffer& key, Buffer value){ static const char blockSize=16; int rounds=calc_rounds(key); 
		std::vector<Buffer> roundKeys; KeyExpansion(key, roundKeys, rounds);
		Buffer decryptedData; size_t dataSize = value.size();
		for (size_t i = 0; i < dataSize; i += blockSize){
			Buffer block = value.slice(i, i+blockSize);
			Buffer decryptedBlock = decryptBlock(roundKeys, block, rounds);
			decryptedData.insert(decryptedData.end(), decryptedBlock.begin(), decryptedBlock.end());
		} removePKCS7Padding(decryptedData, blockSize); return decryptedData; }
		
	void XOR(Buffer& buff1, const Buffer& buff2){ size_t minSize = std::min(buff1.size(), buff2.size()); 
		for(size_t i = 0; i<minSize; ++i){ buff1[i] ^= buff2[i]; } }

    Buffer CBC_encrypt(const Buffer& key, Buffer value, Buffer iv=Buffer(16)){ static const char blockSize = 16; 
		int rounds = calc_rounds(key); std::vector<Buffer> roundKeys; 
		KeyExpansion(key, roundKeys, rounds); addPKCS7Padding(value, blockSize);

        Buffer encryptedData; size_t dataSize = value.size(); Buffer prevBlock = iv;
        for (size_t i = 0; i < dataSize; i += blockSize){
            Buffer block = value.slice(i, i + blockSize); XOR(block, prevBlock);
            Buffer encryptedBlock = encryptBlock(roundKeys, block, rounds);
            encryptedData.insert(encryptedData.end(), encryptedBlock.begin(), encryptedBlock.end());
            prevBlock = encryptedBlock; } return encryptedData; }

    Buffer CBC_decrypt(const Buffer& key, Buffer value, Buffer iv=Buffer(16)){ static const char blockSize = 16; 
		int rounds = calc_rounds(key); std::vector<Buffer> roundKeys; KeyExpansion(key, roundKeys, rounds);

        Buffer decryptedData; size_t dataSize = value.size(); Buffer prevBlock = iv;
        for (size_t i = 0; i < dataSize; i += blockSize){
            Buffer block = value.slice(i, i + blockSize);
            Buffer decryptedBlock = decryptBlock(roundKeys, block, rounds); XOR(decryptedBlock, prevBlock);
            decryptedData.insert(decryptedData.end(), decryptedBlock.begin(), decryptedBlock.end());
            prevBlock = block; } removePKCS7Padding(decryptedData, blockSize); return decryptedData; }
			
	Buffer GCM_multiply(Buffer x, Buffer y){ Buffer z(16); Buffer v = y;
		for (int i = 0; i < 128; ++i){
			if (x[i / 8] & (1 << (7 - (i % 8)))){ XOR(z, v); } bool lsb = v[15] & 1;
			for(int j = 15; j > 0; --j){ v[j] = (v[j] >> 1) | (v[j - 1] << 7); }
			v[0] >>= 1; if (lsb){ v[0] ^= 0xe1; } } return z; }

	Buffer GCM_ghash(Buffer h, Buffer aad, Buffer cipherText){ 
		Buffer y(16); size_t aadLen = aad.size(); size_t cipherTextLen = cipherText.size();
		for (size_t i = 0; i < aadLen; i += 16){ // Обработка AAD
			Buffer block = aad.slice(i, std::min(aadLen, i + 16));
			XOR(y, block); y = GCM_multiply(y, h); }
		for (size_t i = 0; i < cipherTextLen; i += 16){ // Обработка зашифрованного текста
			Buffer block = cipherText.slice(i, std::min(cipherTextLen, i + 16));
			XOR(y, block); y = GCM_multiply(y, h); }
		// Обработка длины AAD и зашифрованного текста
		Buffer lengthBlock(16); unsigned long long aadBits = aadLen * 8; 
		unsigned long long cipherTextBits = cipherTextLen * 8;
		for (int i = 0; i < 8; ++i){
			lengthBlock[15 - i] = static_cast<unsigned char>(cipherTextBits >> (8 * i));
			lengthBlock[7 - i] = static_cast<unsigned char>(aadBits >> (8 * i)); }
		XOR(y, lengthBlock); y = GCM_multiply(y, h); return y; }
	
    Buffer NormalizeIV(Buffer& iv){
		if (iv.size() == 12){ iv.resize(16); iv[12] = 0; iv[13] = 0; iv[14] = 0; iv[15] = 1; } 
		else { Buffer h(16); iv = GCM_ghash(h, iv, Buffer()); } return iv; }
    
	Buffer GCM_encrypt(const Buffer& key, Buffer value, Buffer iv=Buffer(12), Buffer aad=Buffer()){
		static const char blockSize = 16; int rounds = calc_rounds(key); std::vector<Buffer> roundKeys; 
		KeyExpansion(key, roundKeys, rounds); Buffer encryptedData; size_t dataSize = value.size(); iv = NormalizeIV(iv);
		Buffer y0 = encryptBlock(roundKeys, iv, rounds);
		for (size_t i = 0; i < dataSize; i += blockSize){ Buffer::Math::increment(iv, 1);
			Buffer block = value.slice(i, i + blockSize);
			XOR(block, encryptBlock(roundKeys, iv, rounds));
			encryptedData.insert(encryptedData.end(), block.begin(), block.end()); }
		
		Buffer h = encryptBlock(roundKeys, Buffer(blockSize), rounds);
		Buffer tag = GCM_ghash(h, aad, encryptedData); XOR(tag, y0);
		encryptedData.insert(encryptedData.end(), tag.begin(), tag.end());
		return encryptedData; }

	Buffer GCM_decrypt(const Buffer& key, Buffer value, Buffer iv=Buffer(12), Buffer aad=Buffer()){
		static const char blockSize = 16; int rounds = calc_rounds(key); 
        std::vector<Buffer> roundKeys; KeyExpansion(key, roundKeys, rounds);
		Buffer tag; tag.assign(value.end() - blockSize, value.end()); value.resize(value.size() - blockSize);
		Buffer decryptedData; size_t dataSize = value.size(); iv = NormalizeIV(iv);
		Buffer y0 = encryptBlock(roundKeys, iv, rounds);

		for (size_t i = 0; i < dataSize; i += blockSize){ Buffer::Math::increment(iv, 1);
			Buffer block = value.slice(i, i + blockSize);
			XOR(block, encryptBlock(roundKeys, iv, rounds));
			decryptedData.insert(decryptedData.end(), block.begin(), block.end()); }
			
		Buffer h = encryptBlock(roundKeys, Buffer(blockSize), rounds);
		Buffer expectedTag = GCM_ghash(h, aad, value); XOR(expectedTag, y0);
		if(tag != expectedTag){ throw std::runtime_error("GCM_ghash: Authentication tag mismatch. Tag: "+tag.cout()+"; Expected: "+expectedTag.cout()); }
		return decryptedData; }
}}}
