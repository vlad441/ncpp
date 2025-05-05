//SHA2 algo hash family, includes SHA256, SHA512, SHA384.
namespace ncpp { namespace crypto {
struct SHA256 { static const uint32_t H0[8]; static const uint32_t K[64];
	
	SHA256(){ reset(); }
	SHA256(const Buffer& buff) { reset(); update(buff); }
	static Buffer hash(const Buffer& input){ return SHA256(input).digest(); }

	void reset(){ length=0; bufferSize=0; memcpy(h, H0, 64); memset(buffer, 0, 64); }

	SHA256& update(const Buffer& input){ length += input.size() * 8; size_t index = 0;
		while (index < input.size()){
			size_t toCopy = std::min(input.size() - index, static_cast<size_t>(64 - bufferSize));
			std::memcpy(buffer + bufferSize, &input[index], toCopy); bufferSize += toCopy; index += toCopy;
			if(bufferSize == 64){ processBlock(buffer); bufferSize = 0; } } return *this; }

	Buffer digest(){ size_t padSize = 0; unsigned char block[64]; std::memcpy(block, buffer, bufferSize); 
		block[bufferSize] = 0x80; padSize = 1; size_t totalLen = bufferSize + 1 + 8; // текущие данные + 0x80 + длина
		size_t zeroPad = (totalLen % 64 == 0) ? 0 : 64 - (totalLen % 64);

		if(bufferSize + padSize + 8 <= 64){ // Один блок
			std::memset(block + bufferSize + padSize, 0, zeroPad);
			for(int i = 0; i < 8; ++i){ block[64 - 8 + i] = (unsigned char)((length >> (56 - 8 * i)) & 0xFF); }
			processBlock(block); }
		else{ // Два блока
			std::memset(block + bufferSize + padSize, 0, 64 - (bufferSize + padSize));
			processBlock(block); std::memset(block, 0, 64); // Второй блок: нули + длина
			for(int i = 0; i < 8; ++i){ block[64 - 8 + i] = (unsigned char)((length >> (56 - 8 * i)) & 0xFF); }
			processBlock(block); } Buffer result(32);
		for(int i = 0; i < 8; ++i){
			result[i * 4]     = (h[i] >> 24) & 0xFF;
			result[i * 4 + 1] = (h[i] >> 16) & 0xFF;
			result[i * 4 + 2] = (h[i] >> 8)  & 0xFF;
			result[i * 4 + 3] = h[i] & 0xFF; } return result; }

private: uint32_t h[8]; uint64_t length; unsigned char buffer[64]; size_t bufferSize; // Регистры хеша, Длина сообщения в битах, Буфер для накопления данных
	void processBlock(const unsigned char* block){ uint32_t w[64];
		for(int i = 0; i < 16; ++i){
			w[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) | (block[i * 4 + 2] << 8) | block[i * 4 + 3]; }
		for(int i = 16; i < 64; ++i){
			uint32_t s0 = (w[i-15] >> 7 | w[i-15] << 25) ^ (w[i-15] >> 18 | w[i-15] << 14) ^ (w[i-15] >> 3);
			uint32_t s1 = (w[i-2] >> 17 | w[i-2] << 15) ^ (w[i-2] >> 19 | w[i-2] << 13) ^ (w[i-2] >> 10);
			w[i] = w[i-16] + s0 + w[i-7] + s1; }
			
		uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
		for(int i = 0; i < 64; ++i) {
			uint32_t S1 = (e >> 6 | e << 26) ^ (e >> 11 | e << 21) ^ (e >> 25 | e << 7);
			uint32_t ch = (e & f) ^ (~e & g);
			uint32_t temp1 = hh + S1 + ch + K[i] + w[i];
			uint32_t S0 = (a >> 2 | a << 30) ^ (a >> 13 | a << 19) ^ (a >> 22 | a << 10);
			uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint32_t temp2 = S0 + maj;

			hh = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
		} h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e; h[5] += f; h[6] += g; h[7] += hh; }
};

const uint32_t SHA256::H0[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };

const uint32_t SHA256::K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

struct SHA512 { static const uint64_t H0_512[8]; static const uint64_t H0_384[8]; static const uint64_t K[80];

	SHA512(int sha_size=512) : sha_size(sha_size){ reset(); }
	SHA512(const Buffer& buff) { reset(); update(buff); }
	static Buffer hash(const Buffer& input){ return SHA512(input).digest(); }

	void reset(){ length = 0; bufferSize = 0; memset(buffer, 0, 128);
		if(sha_size==384){ memcpy(h, H0_384, 64); }else{ memcpy(h, H0_512, 64); } }

	SHA512& update(const Buffer& input){ length += input.size() * 8; size_t index = 0;
		while (index < input.size()){
			size_t toCopy = std::min(input.size() - index, static_cast<size_t>(128 - bufferSize));
			std::memcpy(buffer + bufferSize, &input[index], toCopy); bufferSize += toCopy; index += toCopy;
			if (bufferSize == 128){ processBlock(buffer); bufferSize = 0; }
		} return *this; }

	Buffer digest(){ buffer[bufferSize++] = 0x80;
		if(bufferSize > 112){
			while(bufferSize < 128) buffer[bufferSize++] = 0; // Дополняем нулями
			processBlock(buffer); bufferSize = 0; }
		// Дополняем оставшуюся часть буфера нулями
		while(bufferSize < 112) buffer[bufferSize++] = 0;
		// Добавляем длину сообщения в битах (в big-endian)
		for(int i = 0; i < 8; ++i){ buffer[120 + i] = (length >> (56 - i * 8)) & 0xFF; }
		processBlock(buffer); Buffer result((sha_size == 384) ? 48 : 64);
        for (int i = 0; i < ((sha_size == 384) ? 6 : 8); ++i) {
            for (int j = 0; j < 8; ++j){ result[i * 8 + j] = (h[i] >> (56 - j * 8)) & 0xFF; } }
        return result; }

private: uint64_t h[8]; uint64_t length; unsigned char buffer[128]; size_t bufferSize; int sha_size;

	void processBlock(const unsigned char* block){
		uint64_t w[80];
		for (int i = 0; i < 16; ++i) {
			w[i] = ((uint64_t)block[i * 8] << 56) | ((uint64_t)block[i * 8 + 1] << 48) |
					((uint64_t)block[i * 8 + 2] << 40) | ((uint64_t)block[i * 8 + 3] << 32) |
					((uint64_t)block[i * 8 + 4] << 24) | ((uint64_t)block[i * 8 + 5] << 16) |
					((uint64_t)block[i * 8 + 6] << 8) | (uint64_t)block[i * 8 + 7];
		}
		for (int i = 16; i < 80; ++i) {
			uint64_t s0 = (w[i-15] >> 1 | w[i-15] << 63) ^ (w[i-15] >> 8 | w[i-15] << 56) ^ (w[i-15] >> 7);
			uint64_t s1 = (w[i-2] >> 19 | w[i-2] << 45) ^ (w[i-2] >> 61 | w[i-2] << 3) ^ (w[i-2] >> 6);
			w[i] = w[i-16] + s0 + w[i-7] + s1;
		}
		uint64_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], k = h[7];
		for (int i = 0; i < 80; ++i) {
			uint64_t S1 = (e >> 14 | e << 50) ^ (e >> 18 | e << 46) ^ (e >> 41 | e << 23);
			uint64_t ch = (e & f) ^ (~e & g);
			uint64_t temp1 = k + S1 + ch + K[i] + w[i];
			uint64_t S0 = (a >> 28 | a << 36) ^ (a >> 34 | a << 30) ^ (a >> 39 | a << 25);
			uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint64_t temp2 = S0 + maj;
			k = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
		}
		h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e; h[5] += f; h[6] += g; h[7] += k;
	}
};

const uint64_t SHA512::H0_512[8] = { 0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
    0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL };

const uint64_t SHA512::H0_384[8] = { 0xcbbb9d5dc1059ed8ULL, 0x629a292a367cd507ULL, 0x9159015a3070dd17ULL, 0x152fecd8f70e5939ULL,
    0x67332667ffc00b31ULL, 0x8eb44a8768581511ULL, 0xdb0c2e0d64f98fa7ULL, 0x47b5481dbefa4fa4ULL };

const uint64_t SHA512::K[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};


} }
