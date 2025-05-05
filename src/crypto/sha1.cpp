namespace ncpp { namespace crypto {
struct SHA1 { unsigned int h0, h1, h2, h3, h4; unsigned char block[64]; size_t blockSize, totalLen;

	SHA1(){ reset(); }
	SHA1(const Buffer& buff){ reset(); update(buff); }
	static Buffer hash(const Buffer& input){ return SHA1(input).digest(); }

	void reset(){ h0 = 0x67452301; h1 = 0xEFCDAB89; h2 = 0x98BADCFE;
		h3 = 0x10325476; h4 = 0xC3D2E1F0; blockSize = 0; totalLen = 0; }

	SHA1& update(const Buffer& input){
		for(size_t i = 0; i < input.size(); ++i){ block[blockSize++] = input[i]; totalLen++;
			if(blockSize == 64){ processBlock(block); blockSize = 0; } } return *this; }

	Buffer digest(){ unsigned char final[64]; size_t i = 0;
		for(; i < blockSize; ++i){ final[i] = block[i]; } final[i++] = 0x80;
		if (i > 56){ while(i < 64){ final[i++] = 0; } processBlock(final); i = 0; }
		while(i < 56) final[i++] = 0;
		uint64_t bitLen = totalLen * 8;
		for(int j = 7; j >= 0; --j){ final[i++] = (bitLen >> (j * 8)) & 0xFF; }
		processBlock(final); Buffer out(20);
		unsigned int hs[5] = { h0, h1, h2, h3, h4 };
		for(int i = 0; i < 5; ++i){
			for(int j = 0; j < 4; ++j){ out[i * 4 + j] = (hs[i] >> (24 - j * 8)) & 0xFF; } } return out; }

	private:
		void processBlock(const unsigned char* block){ unsigned int w[80];
			for (int i = 0; i < 16; ++i){ w[i] = (block[i*4] << 24) | (block[i*4+1] << 16) | (block[i*4+2] << 8) | block[i*4+3]; }
			for (int i = 16; i < 80; ++i){ unsigned int val = w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]; w[i] = (val << 1) | (val >> 31); }

			unsigned int a = h0, b = h1, c = h2, d = h3, e = h4, f, k, temp;
			for (int i = 0; i < 80; ++i){
				if(i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
				else if(i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
				else if(i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
				else{ f = b ^ c ^ d; k = 0xCA62C1D6; }

				unsigned int ra = (a << 5) | (a >> 27);
				unsigned int rb = (b >> 2) | (b << 30);
				temp = ra + f + e + k + w[i];
				e = d; d = c; c = rb; b = a; a = temp;
			} h0 += a; h1 += b; h2 += c; h3 += d; h4 += e; }
};
} }
