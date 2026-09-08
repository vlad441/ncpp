namespace ncpp {
	size_t UTF8Len(const char* cptr, size_t len){ size_t ulen = 0; //байт НЕ должен иметь вид 10xxxxxx
		for(size_t i=0; i<len; ++i){ if (((unsigned char)(cptr[i]) & 0xC0) != 0x80){ ulen++; } } return ulen; }
	size_t UTF8Len(const CString& cs){ return UTF8Len(cs.c_str(), cs.size()); }
	size_t UTF8Len(const String& s){ return UTF8Len(s.c_str(), s.size()); }
	
	int UTF8Next(const char* cptr, size_t len, size_t& pos){ if(pos>=len) return -1; unsigned char fbyte = (unsigned char)cptr[pos];
		if(fbyte < 0x80){ pos++; return fbyte; } int value = 0, bCnt = 0;  // ASCII: 0x00 – 0x7F
		if((fbyte & 0xE0) == 0xC0){ value = fbyte & 0x1F; bCnt = 2; }
		else if((fbyte & 0xF0) == 0xE0){ value = fbyte & 0x0F; bCnt = 3; }
		else if((fbyte & 0xF8) == 0xF0){ value = fbyte & 0x07; bCnt = 4; }
		else{ pos++; return 0xFFFD; } if(pos+bCnt > len){ pos = len; return -1; }
        for(int i = 1; i < bCnt; ++i){ unsigned char nbyte = (unsigned char)cptr[pos+i]; if((nbyte & 0xC0) != 0x80) break; // Ошибка формата
            value = (value << 6) | (nbyte & 0x3F); } pos += bCnt; return value; }
	int UTF8Next(const String& s, size_t& pos){ return UTF8Next(s.c_str(), s.size(), pos); }
	int UTF8Next(const CString& cs, size_t& pos){ return UTF8Next(cs.c_str(), cs.size(), pos); }
	
	char UTF8Encode(char symb[4], int point){ if(point < 0) return 0;
		if(point <= 0x7F){ symb[0] = (char)point; return 1; }
		else if(point <= 0x7FF){ // 2 байта: 110xxxxx 10xxxxxx
			symb[0] = (char)(0xC0 | (point >> 6)); symb[1] = (char)(0x80 | (point & 0x3F)); return 2; }
		else if(point <= 0xFFFF){ // 3 байта: 1110xxxx 10xxxxxx 10xxxxxx			
			symb[0] = (char)(0xE0 | (point >> 12)); symb[1] = (char)(0x80 | ((point >> 6) & 0x3F));
			symb[2] = (char)(0x80 | (point & 0x3F)); return 3; }
		else if(point <= 0x10FFFF){ // 4 байта: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			symb[0] = (char)(0xF0 | (point >> 18)); symb[1] = (char)(0x80 | ((point >> 12) & 0x3F));
			symb[2] = (char)(0x80 | ((point >> 6) & 0x3F)); symb[3] = (char)(0x80 | (point & 0x3F)); return 4; }
		return 0; }
		
	void UTF8Push(String& s, int point){ char symb[4]; s.push(symb, UTF8Encode(symb, point)); }
}