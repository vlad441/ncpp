namespace ncpp { namespace crypto {
struct MD5 { static const unsigned char s[64]; static const unsigned int K[64];
    
    MD5(){ reset(); }
    MD5(const Buffer& buff){ reset(); update(buff); }
    static Buffer hash(const Buffer& input){ return MD5().finalize(input); }

    void reset(){ h[0] = 0x67452301; h[1] = 0xefcdab89; h[2] = 0x98badcfe; h[3] = 0x10325476; 
		bufferSize = 0; totalSize = 0; } //std::memset(buffer, 0, 64);
		
	MD5& update(const Buffer& input){ size_t index = 0; totalSize += input.size();
        while(index < input.size()){
            size_t toCopy = std::min(input.size() - index, 64 - bufferSize);
            std::memcpy(buffer + bufferSize, &input[0]+index, toCopy); bufferSize += toCopy; index += toCopy;
            if(bufferSize == 64){ processBlock(buffer); bufferSize = 0; } } return *this; }
    
    Buffer digest(){ unsigned long long bitLen = totalSize * 8; buffer[bufferSize++] = 0x80;
        if(bufferSize > 56){ while(bufferSize < 64) buffer[bufferSize++] = 0;
			processBlock(buffer); bufferSize = 0; }
        
        while(bufferSize < 56) buffer[bufferSize++] = 0;
        for(int i = 0; i < 8; i++){ buffer[bufferSize++] = (bitLen >> (8 * i)) & 0xFF; }
        processBlock(buffer); Buffer resp(16);
        for (int i = 0; i < 4; i++){ 
			for(int j = 0; j < 4; j++){ resp[i * 4 + j] = (h[i] >> (8 * j)) & 0xFF; } } return resp; }
private: unsigned int h[4]; unsigned char buffer[64]; unsigned long long totalSize; size_t bufferSize;
    Buffer finalize(const Buffer& input){
        Buffer data = input; unsigned long long bitLen = input.size() * 8;
        data.push_back(0x80); while((data.size() % 64) != 56) data.push_back(0);
        
        for(int i = 0; i < 8; i++) data.push_back((bitLen >> (8 * i)) & 0xFF);
        for(size_t i = 0; i < data.size(); i += 64) processBlock(&data[i]);
        Buffer resp(16); 
        for(int i = 0; i < 4; i++){ 
			for(int j = 0; j < 4; j++){ resp[i * 4 + j] = (h[i] >> (8 * j)) & 0xFF; } } return resp; }
    void processBlock(const unsigned char* chunk){
        unsigned int w[16], a = h[0], b = h[1], c = h[2], d = h[3], f, g, temp;
        for (int i = 0; i < 16; i++)
            w[i] = (chunk[i * 4]) | (chunk[i * 4 + 1] << 8) | (chunk[i * 4 + 2] << 16) | (chunk[i * 4 + 3] << 24);

        for (int i = 0; i < 64; i++){
            if (i < 16) { f = (b & c) | ((~b) & d); g = i; }
            else if (i < 32) { f = (d & b) | ((~d) & c); g = (5 * i + 1) % 16; }
            else if (i < 48) { f = b ^ c ^ d; g = (3 * i + 5) % 16; }
            else { f = c ^ (b | (~d)); g = (7 * i) % 16; }

            temp = d; d = c; c = b;
            b = b + ((a + f + K[i] + w[g]) << s[i] | (a + f + K[i] + w[g]) >> (32 - s[i]));
            a = temp;
        } h[0] += a; h[1] += b; h[2] += c; h[3] += d; }
};

const unsigned char MD5::s[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

const unsigned int MD5::K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
}; } }
