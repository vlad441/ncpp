namespace ncpp { namespace crypto {

struct Keccak { uint64_t state[25]; unsigned char buffer[200];	
	size_t rate, capacity, outLen, bufferSize; char delim;

	Keccak(size_t rateBits=1088, size_t capacity=512, size_t bitlen=256)
		: rate(rateBits/8), capacity(capacity/8), outLen(bitlen/8), bufferSize(0), delim(0x06){ reset(); }
		
	void reset(){ std::memset(state, 0, sizeof(uint64_t)*25); bufferSize=0; }

	void update(const unsigned char* input, size_t len){
		for(size_t i = 0; i < len; ++i){ buffer[bufferSize++] = input[i];
			if(bufferSize == rate){ absorbBlock(); bufferSize = 0; } } }

	Buffer digest(){ buffer[bufferSize++] = delim;
		if(bufferSize > rate - 1){
			while(bufferSize < rate) buffer[bufferSize++] = 0;
			absorbBlock(); bufferSize = 0; }
		while(bufferSize < rate - 1) buffer[bufferSize++] = 0;
		buffer[bufferSize++] = 0x80; absorbBlock(); 
		Buffer out(outLen); size_t outpos = 0;
		while(outpos < outLen){
			for(size_t i = 0; i < rate && outpos < outLen; ++i)
				out[outpos++] = ((unsigned char*)state)[i];
			if(outpos < outLen) keccakF(); } return out; }

private:
	void absorbBlock(){
		for(size_t i = 0; i < rate / 8; ++i){ uint64_t r = 0;
			for(int j = 0; j < 8; ++j){ r |= (uint64_t)buffer[i * 8 + j] << (8 * j); }
			state[i] ^= r; } keccakF(); }

	void keccakF(){
		const uint64_t RC[24] = {
			0x0000000000000001ULL,0x0000000000008082ULL,0x800000000000808aULL,0x8000000080008000ULL,
			0x000000000000808bULL,0x0000000080000001ULL,0x8000000080008081ULL,0x8000000000008009ULL,
			0x000000000000008aULL,0x0000000000000088ULL,0x0000000080008009ULL,0x000000008000000aULL,
			0x000000008000808bULL,0x800000000000008bULL,0x8000000000008089ULL,0x8000000000008003ULL,
			0x8000000000008002ULL,0x8000000000000080ULL,0x000000000000800aULL,0x800000008000000aULL,
			0x8000000080008081ULL,0x8000000000008080ULL,0x0000000080000001ULL,0x8000000080008008ULL };
		const int r[25] = { 0,  1, 62, 28, 27, 36, 44,  6, 55, 20, 3, 10, 43, 25, 39, 41, 45, 15, 21,  8, 18,  2, 61, 56, 14 };
		for(int round = 0; round < 24; ++round){
			uint64_t C[5], D[5];
			for(int x = 0; x < 5; ++x) C[x] = state[x] ^ state[x+5] ^ state[x+10] ^ state[x+15] ^ state[x+20];
			for(int x = 0; x < 5; ++x) D[x] = C[(x+4)%5] ^ ((C[(x+1)%5] << 1) | (C[(x+1)%5] >> (64 - 1)));
			for(int i = 0; i < 25; ++i) state[i] ^= D[i % 5];

			uint64_t B[25];
			for(int x = 0; x < 5; ++x)
			for(int y = 0; y < 5; ++y){
				int idx = x + 5*y;
				int newX = y;
				int newY = (2*x + 3*y) % 5;
				B[newX + 5*newY] = (state[idx] << r[idx]) | (state[idx] >> (64 - r[idx]));
			}
			for(int i = 0; i < 25; ++i) state[i] = B[i];

			for(int y = 0; y < 5; ++y){
				uint64_t tmp[5];
				for(int x = 0; x < 5; ++x) tmp[x] = state[x + 5*y];
				for(int x = 0; x < 5; ++x)
					state[x + 5*y] = tmp[x] ^ ((~tmp[(x+1)%5]) & tmp[(x+2)%5]);
			} state[0] ^= RC[round]; }
	}
};

struct SHA3 : Keccak {
    SHA3(int bitlen = 256) : Keccak(1600-2*bitlen, bitlen*2, bitlen){}
    SHA3(const Buffer& input, int bitlen = 256) : Keccak(1600-2*bitlen, bitlen*2, bitlen){ Keccak::update(&input[0], input.size()); }
    static Buffer hash(const Buffer& input, int bitlen = 256) { return SHA3(input, bitlen).digest(); }
    SHA3& update(const Buffer& input){ Keccak::update(&input[0], input.size()); return *this; }
};

struct SHA3_256 : Keccak {
	SHA3_256() : Keccak(1088, 512, 256){}
    SHA3_256(const Buffer& input) : Keccak(1088, 512, 256){ Keccak::update(&input[0], input.size()); }
    static Buffer hash(const Buffer& input) { return SHA3_256(input).digest(); }
    SHA3_256& update(const Buffer& input){ Keccak::update(&input[0], input.size()); return *this; }
};

struct SHA3_512 : SHA3 { SHA3_512() : SHA3(512){}
    SHA3_512(const Buffer& input) : SHA3(input, 512){} };

} }
