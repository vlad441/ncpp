namespace ncpp { struct String;
template <typename T> struct is_pointer { static const bool V = false; }; //is_pointer type traits
template <typename T> struct is_pointer<T*> { static const bool V = true; };

#define _MIN_ALLOC 16
#define _BF_SSO_LEN 0 
struct Buffer : BaseString<unsigned char, Buffer>{ typedef unsigned char* Iter; typedef const unsigned char* ConstIter;
	typedef Iter iterator; typedef ConstIter const_iterator;
	Buffer(){ _init(0); }
	Buffer(const void* ptr, size_t len){ _init(len); _set(ptr, len); }
	Buffer(const char* cptr){ _init(strlen(cptr)); _set(cptr, _len); }
	Buffer(size_t len){ _init(len); }
	Buffer(size_t len, unsigned char v){ _init(len); memset(_ptr, v, _len); }
	Buffer(const Buffer& b){ _init(b.size()); _set(b.data(), b.size()); }
	Buffer& operator=(const Buffer& b){ _set(b.data(), b.size()); return *this; }
	Buffer(const void* begin, const void* end){ _init(0); assign(begin, end); }
	template <size_t N>
	Buffer(const unsigned char (&arr)[N]){ _init(N); _set(arr, N); } //unsigned char arr[];
	template <typename T, typename D>
	Buffer(const BaseString<T, D>& s){ _init(s.size()); _set(s.data(), s.size()); }
	~Buffer(){ if(_mode==HEAP&&_ptr!=NULL) free(_ptr); }
	// == init
	Buffer& _from(const CString& str, const CString& type="str"){
		if(type=="hex"){ _fromHex(str.c_str(), str.size()); }else if(type=="base64"){ *this=base64_decode(str); }
		else if(type=="int"||type=="number"){ _fromIntString(str); }else{ assign(str.begin(), str.end()); } return *this; }
	Buffer& _fromHex(const char* hexstr, size_t len){ len/=2; if(this->size()!=len){ resize(len); }
        for(size_t i=0;i<len;++i){ (*this)[i] = htoc(hexstr+2*i); } return *this; }
	Buffer& _fromIntString(const CString& cs){ this->clear(); size_t i=0; if(cs.size()>0&&(cs[0]=='-'||cs[0]=='+')) i=1;
		Buffer temp(cs.size()); for(; i < cs.size(); ++i){ temp[i] = cs[i] - '0'; } size_t st = 0;
		while(st < temp.size() && !(st == temp.size() - 1 && temp[st] == 0)){ unsigned int remainder = 0;
			for(size_t i = st; i < temp.size(); ++i){ unsigned int current = remainder * 10 + temp[i]; temp[i] = current / 256; remainder = current % 256; }
			push((unsigned char)remainder); while(st < temp.size() && temp[st] == 0) st++; } reverse(); return *this; }
	static Buffer from(const CString& cs){ return Buffer(cs.data(), cs.size()); }
	static Buffer from(const CString& str, const CString& type){ return Buffer()._from(str, type); }
	static Buffer fromHex(const char* hex, size_t len){ return Buffer()._fromHex(hex, len); }
	static Buffer fromHex(const CString& hex){ Buffer buff; buff._fromHex(hex.c_str(), hex.size()); return buff; }
	static Buffer fromBase64(const CString& cs){ return Buffer(base64_decode(cs)); }
	static Buffer fromInt(unsigned long long numb){ Buffer buff(8); buff.writeInt64BE(numb); removeLeadingZeros(buff); return buff; }
	static Buffer fromInt(const CString& cs){ return Buffer()._fromIntString(cs); }
	
	// == STL similar methods ===
	//const char* c_str() const { throw; return NULL; }
	
	void assign(const void* begin, const void* end){ const unsigned char *b=(const unsigned char*)begin, *e=(const unsigned char*)end;
		size_t len = (e > b)?(e-b) : 0; _set(b, len); }
	
	Buffer& reserve(size_t sz){ if(sz<=_msize) return *this; if(_ptr==NULL||_mode!=HEAP){ _alloc(sz, true); return *this; }
		_msize=(sz<_msize*2)?_msize*2:sz; _ptr=(unsigned char*)realloc(_ptr, _msize);
		if(_ptr==NULL){ print("ncpp::Buffer realloc error: Out of memory"); exit(1); } return *this; }
	Buffer& resize(size_t len){ reserve(len); _len=len; return *this; }
	
	void insert(Iter ipos, const void* dptr, size_t len){ if(len==0) return; size_t pos=_len;
		if(ipos!=NULL && ipos >= _ptr && ipos <= _ptr+_len){ pos = ipos-_ptr; } size_t len0=_len; size_t nlen = _len+len; reserve(nlen);
		if(pos < len0){ memmove(ipos+len, ipos, len0-pos); }
        memcpy(_ptr+pos, dptr, len); _len = nlen; return; }
	void insert(Iter ipos, const void* first, const void* last){ const unsigned char *b=(const unsigned char*)first, *e=(const unsigned char*)last;
		size_t len = (e > b)?(e-b) : 0; insert(ipos, b, len); }
	void insert(Iter ipos, size_t n, unsigned char v){ if(n == 0) return; size_t pos=_len;
		if(ipos!=NULL && ipos >= _ptr && ipos <= _ptr+_len){ pos = ipos-_ptr; } size_t len0=_len; size_t nlen = _len+n; reserve(nlen);
		if(pos < len0){ memmove(_ptr+pos+n, _ptr+pos, len0-pos); }
        for(size_t i = 0; i<n; ++i){ _ptr[pos + i] = v; } _len = nlen; return; }
	void insert(Iter ipos, unsigned char v){ insert(ipos, &v, 1); }
	void insert(Iter ipos, const Buffer& other){ insert(ipos, other.data(), other.size()); }
	
	void erase(Iter first, Iter last){ if(first >= last || first < _ptr || last > _ptr+_len) return;
        size_t end_pos = last-_ptr; memmove(first, last, _len-end_pos); _len -= last-first; }
	void erase(Iter ipos){ erase(ipos, ipos+1); }
	
	void push_back(unsigned char v){ push(v); }
	void pop_back(){ resize(--_len); }
	
	void clear(){ _len=0; }
	void shrink_to_fit(){ if(_len>=_msize||_ptr==NULL||_mode!=HEAP) return; _ptr=(unsigned char*)realloc(_ptr, _msize=_len);
		if(_ptr==NULL){ print("ncpp::Buffer shrink mem error"); exit(1); } return; }
	void shrink(){ shrink_to_fit(); }
	// == переобразования
	template <size_t N>
	Buffer& push(const unsigned char (&arr)[N]){ size_t len0=_len; resize(_len+N); memcpy(_ptr+len0, arr, N); return *this; }
	Buffer& push(const void* ptr, size_t len){ size_t len0=_len; resize(_len+len); memcpy(_ptr+len0, ptr, len); return *this; }
	Buffer& push(const Buffer& buff){ push(buff.data(), buff.size()); return *this; }
	Buffer& push(unsigned char v){ reserve(++_len); _ptr[_len-1]=v; return *this; }
	unsigned char pop(){ resize(--_len); return _ptr[_len]; }
	
	void push_front(unsigned char v){ insert(begin(), v); }
	void pop_front(){ erase(begin()); }
	
	Buffer slice(int start, int end=0) const { return _slice<Buffer>(start, end); }
	
	Array<Buffer> split(const char* delim, size_t len, int maxparts=-1) const { return _split<Buffer>(delim, len, maxparts); }
	Array<Buffer> split(const char* delim) const { return _split<Buffer>(delim, strlen(delim)); }
	Array<Buffer> split(char delim) const { return _split<Buffer>(&delim, 1); }
	template <typename U, typename D> 
	Array<Buffer> split(const BaseString<U, D>& s) const { return this->_split<Buffer>(s.data(), s.size()); }
	
	Buffer& fill(unsigned char v){ memset(_ptr, v, _len); return *this; }
	//Buffer& concat(const Buffer& buff2){ *this+=buff2; return *this; }
	void reverse(){ ncpp::reverse(this->begin(), this->end()); }
	
	void read(void* dst, size_t size, size_t offset=0){ if(_len < offset+size){ size = _len-offset; } memcpy(dst, _ptr+offset, size); }
	void write(const void* src, size_t size, size_t offset){ if(_len < offset+size){ resize(offset+size); } memcpy(_ptr+offset, src, size); }
	void write(const char* cstr, size_t offset=0){ return write(cstr, strlen(cstr), offset); }
	void write(const Buffer& buff, size_t offset=0){ if(buff.empty()) return; return write(buff.data(), buff.size(), offset); }
	template <typename T, typename D>
	void write(const BaseString<T, D>& s, size_t offset=0){ if(s.empty()) return; return write(s.data(), s.size(), offset); }
	template <typename T>
	T readAny(size_t offset=0){ if(ncpp::is_pointer<T>::V){ print("(!) readAny(T&): not accept pointers type"); return T(); }
		T data; read(&data, sizeof(data), offset); return data; }
	template <typename T>
	void writeAny(const T& data, size_t offset=0){ if(ncpp::is_pointer<T>::V){ print("(!) writeAny(T&): not accept pointers type"); return; }
		return write(&data, sizeof(data), offset); }
	
	Buffer BitsRead(size_t offset=0) const { return BitsRead(_ptr+offset); }
	void BitsWrite(const Buffer& bits, size_t offset=0){ BitsWrite(_ptr+offset, bits); }
	
	// == представления
	template <typename T>
	T readT_BE(size_t offset=0, int length=sizeof(T)) const { T value=readT_LE<T>(offset); ncpp::reverse((char*)&value, (char*)&value+length); return value; }
	template <typename T>
	T readT_LE(size_t offset=0, int length=sizeof(T)) const { if(offset+length > _len){ print("(!) readT_LE: out of range"); return T(); }
		T value=T(); memcpy(&value, _ptr+offset, length); return value; }
	
	template <typename T>
	void writeT_BE(T value, size_t offset=0, int length=sizeof(T)){ writeT_LE<T>(value, offset); ncpp::reverse(_ptr+offset, _ptr+offset+length); }
	template <typename T>
	void writeT_LE(T value, size_t offset=0, int length=sizeof(T)){ if(_len < offset+length){ resize(offset+length); } memcpy(_ptr+offset, &value, length); }
	
	int readInt16BE(size_t offset=0) const { return readT_BE<int16_t>(offset); }
	int readInt16LE(size_t offset=0) const { return readT_LE<int16_t>(offset); }
	unsigned int readUInt16BE(size_t offset=0) const { return readT_BE<uint16_t>(offset); }
	unsigned int readUInt16LE(size_t offset=0) const { return readT_LE<uint16_t>(offset); }
	void writeInt16BE(int value, size_t offset=0){ writeT_BE<int16_t>(value, offset); }
	void writeInt16LE(int value, size_t offset=0){ writeT_LE<int16_t>(value, offset); }
	void writeUInt16BE(unsigned int value, size_t offset=0){ writeT_BE<uint16_t>(value, offset); }
	void writeUInt16LE(unsigned int value, size_t offset=0){ writeT_LE<uint16_t>(value, offset); }
	
	int32_t readInt32BE(size_t offset=0) const { return readT_BE<int32_t>(offset); }
	int32_t readInt32LE(size_t offset=0) const { return readT_LE<int32_t>(offset); }
	uint32_t readUInt32BE(size_t offset=0) const { return readT_BE<uint32_t>(offset); }
	uint32_t readUInt32LE(size_t offset=0) const { return readT_LE<uint32_t>(offset); }
	void writeInt32BE(int value, size_t offset=0){ writeT_BE<int32_t>(value, offset); }
	void writeInt32LE(int value, size_t offset=0){ writeT_LE<int32_t>(value, offset); }
	void writeUInt32BE(unsigned int value, size_t offset=0){ writeT_BE<uint32_t>(value, offset); }
	void writeUInt32LE(unsigned int value, size_t offset=0){ writeT_LE<uint32_t>(value, offset); }
	
	long long readInt64BE(size_t offset=0) const { return readT_BE<int64_t>(offset); }
	long long readInt64LE(size_t offset=0) const { return readT_LE<int64_t>(offset); }
	unsigned long long readUInt64BE(size_t offset=0) const { return readT_BE<uint64_t>(offset); }
	unsigned long long readUInt64LE(size_t offset=0) const { return readT_LE<uint64_t>(offset); }
	void writeInt64BE(long long value, size_t offset=0){ writeT_BE<int64_t>(value, offset); }
	void writeInt64LE(long long value, size_t offset=0){ writeT_LE<int64_t>(value, offset); }
	void writeUInt64BE(unsigned long long value, size_t offset=0){ writeT_BE<uint64_t>(value, offset); }
	void writeUInt64LE(unsigned long long value, size_t offset=0){ writeT_LE<uint64_t>(value, offset); }
		
	double readDoubleBE(size_t offset=0) const { return readT_BE<double>(offset); }
	double readDoubleLE(size_t offset=0) const { return readT_LE<double>(offset); }
	void writeDoubleBE(double value, size_t offset=0){ writeT_BE<double>(value, offset); }
	void writeDoubleLE(double value, size_t offset=0){ writeT_LE<double>(value, offset); }
	// === static funcs ===
	static Buffer BitsRead(const unsigned char* ptr){ unsigned char bits[8];
		for(unsigned char indx = 0; indx < 8; ++indx){ bits[indx] = (*ptr >> (7 - indx)) & 1; } return Buffer(bits); }
	static void BitsWrite(unsigned char* ptr, const Buffer& bits){ *ptr=0;
		for(size_t indx = 0; indx < bits.size(); ++indx){ *ptr = (*ptr << 1) | bits[indx]; } }
	static Buffer BitsRead(const void* ptr){ return BitsRead((const unsigned char*)ptr); }
	static void BitsWrite(void* ptr, const Buffer& bits){ BitsWrite((unsigned char*)ptr, bits); }
	
	static int BitsToInt(const unsigned char* bits, size_t len){ int numb = 0; for(size_t i = 0; i < len; ++i){ numb = (numb << 1) | bits[i]; } return numb; }
	static int BitsToInt(const Buffer& bits){ return BitsToInt(bits.data(), bits.size()); }
	static void IntToBits(unsigned char* dst, int numb, size_t bitlen){ for(int i = bitlen - 1; i >= 0; --i){ dst[i] = (numb >> i) & 1; } }
	static Buffer IntToBits(int numb, size_t bitlen=sizeof(int)*8){ Buffer bits(bitlen); IntToBits(bits.data(), numb, bits.size()); return bits; }
		
	static Buffer concat(const Array<Buffer>& buffs){ Buffer result; for(size_t i=0;i<buffs.size();i++){ result+=buffs[i]; } return result; }
	static Buffer concat(const Buffer& buff1, const Buffer& buff2){ return buff1+buff2; }
	static Buffer randBytes(int length); 
	
	static const CString base64_chars;
	static String base64_encode(unsigned char const* bptr, size_t len){ if(len<1){ return ""; }
		String ret; int i = 0; int j = 0; unsigned char char_array_3[3]; unsigned char char_array_4[4];

		while (len--){ char_array_3[i++] = *(bptr++);
			if(i == 3){
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;
				for (i = 0; (i < 4); i++){ ret += base64_chars[char_array_4[i]]; } i = 0; } }

		if(i){
			for (j = i; j < 3; j++){ char_array_3[j] = '\0'; }
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			for (j = 0; (j < i + 1); j++){ ret += base64_chars[char_array_4[j]]; }
			while ((i++ < 3)){ ret += '='; } } return ret; }
	static Buffer base64_decode(const CString& encoded_string){
		int in_len = encoded_string.size(); int i = 0; int j = 0; int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3]; Buffer ret;

		while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
			char_array_4[i++] = encoded_string[in_]; in_++;
			if (i == 4){
				for (i = 0; i < 4; i++){ char_array_4[i] = base64_chars.find(char_array_4[i]); }
				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
				for (i = 0; (i < 3); i++){ ret.push_back(char_array_3[i]); } i = 0; } }

		if (i){ for(j = i; j < 4; j++){ char_array_4[j] = 0; }
			for (j = 0; j < 4; j++){ char_array_4[j] = base64_chars.find(char_array_4[j]); }
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			for(j = 0; (j < i - 1); j++){ ret.push_back(char_array_3[j]); } } return ret; }
	
	// === BE int increment/decrement ===
	static void increment(Buffer& buff, unsigned int value=1, bool insert = false){
		if(value==1&&!insert){ for(int j = buff.size() - 1; j >= 0; --j){ if(++buff[j] != 0) return; } return; } unsigned int carry = value;
		for(int i = buff.size() - 1; i >= 0; --i){ unsigned int sum = buff[i] + carry; buff[i] = sum & 0xFF; carry = sum >> 8; }
		if(insert && carry != 0){ while(carry != 0){ buff.insert(buff.begin(), carry & 0xFF); carry >>= 8; } } }

	static void decrement(Buffer& buff, unsigned int value=1, bool erase = false){ unsigned int borrow = value;
		if(value==1&&!erase){ for(int j = buff.size() - 1; j >= 0; --j){ if(--buff[j] != 0xFF) return; } return; }
		for(int i = buff.size() - 1; i >= 0; --i){ int diff = buff[i] - (borrow & 0xFF); buff[i] = (diff + 256) & 0xFF;
			borrow = (diff < 0) ? 1 : 0; value >>= 8; borrow += (value & 0xFF); }
		if(erase){ if(borrow > 0){ buff.fill(0); } removeLeadingZeros(buff); } }
		
	static void multiply_single(Buffer& buff, unsigned int value, bool insert = true){ unsigned int carry = 0;
		for(int i = buff.size() - 1; i >= 0; --i){ unsigned int product = buff[i] * value + carry; buff[i] = product & 0xFF; carry = product >> 8; }
		if (insert && carry != 0){ while(carry != 0){ buff.insert(buff.begin(), carry & 0xFF); carry >>= 8; } } }

	static unsigned int divide_single(Buffer& buff, unsigned int value, bool erase = true){
		if(value == 0){ print("(!) Matan rule 0: Never divide by zero :)"); return 0; } unsigned int remainder = 0;
		for(size_t i = 0; i < buff.size(); ++i){ unsigned int current = (remainder << 8) | buff[i]; buff[i] = current / value; remainder = current % value; }
		if(erase){ removeLeadingZeros(buff); } return remainder; }
	
	static void removeLeadingZeros(Buffer& buff){ if(buff.size() <= 1) return; size_t firstNonZero = 0;
			while(firstNonZero < buff.size()-1 && buff[firstNonZero] == 0) ++firstNonZero;
			if(firstNonZero > 0){ buff.erase(buff.begin(), buff.begin()+firstNonZero); } }
	static void removeLastZeros(Buffer& buff){ while(buff.size() > 1 && buff.back() == 0) buff.pop(); }
	struct Math; // == арифметика (implemented in ncpp-bigint.cpp)
	// === ===
	unsigned int toInt() const noexcept { unsigned int value = 0; size_t sz=sizeof(int); if(this->size()<sz){ sz=this->size(); }
			for(size_t i = 0; i<sz; ++i){ value = (value << 8) | (*this)[i]; } return value; }
	unsigned long long toInt64() const noexcept { unsigned long long value = 0; size_t sz=sizeof(long long); if(this->size()<sz){ sz=this->size(); }
		for(size_t i = 0; i<sz; ++i){ value = (value << 8) | (*this)[i]; } return value; }
		
	unsigned char& operator[](size_t pos){ return _ptr[pos]; }
	const unsigned char& operator[](size_t pos) const { return _ptr[pos]; }
	unsigned char at(size_t pos) const { return (_ptr&&pos<_len)?_ptr[pos]:0; }
	
	bool operator==(const Buffer& bf) const { return _len==bf.size()&&memcmp(_ptr, bf.data(), _len)==0; }
	
	Buffer& operator=(const char* c){ _set((const unsigned char*)c, strlen(c)); return *this; }
	template <typename T, typename D>
	Buffer& operator=(const BaseString<T, D>& s){ _set((const unsigned char*)s.data(), s.size()); return *this; }
	template <size_t N>
	Buffer& operator=(const unsigned char (&arr)[N]){ _set(arr, N); return *this; }
	Buffer& operator+=(const char* c){ push(c, strlen(c)); return *this; }
	Buffer& operator+=(const Buffer& b){ push(b.data(), b.size()); return *this; }
	Buffer& operator+=(unsigned char c){ push(c); return *this; }
	template <typename T, typename D>
	Buffer& operator+=(const BaseString<T, D>& s){ push(s.data(), s.size()); return *this; }
	Buffer operator+(const char* c) const { size_t len=strlen(c); Buffer nbuff(_len+len); nbuff.write(*this); nbuff.write(c, len, _len); return nbuff; }
	Buffer operator+(const Buffer& b) const { Buffer nbuff(_len+b.size()); nbuff.write(*this); nbuff.write(b, _len); return nbuff; }
	Buffer operator+(unsigned char c) const { Buffer nbuff(_len+1); nbuff.write(*this); nbuff[_len]=c;  return nbuff; }
	template <typename T, typename D>
	Buffer operator+(const BaseString<T, D>& s) const { Buffer nbuff(*this); return nbuff+=s; }
	Buffer& operator<<(const Buffer& b){ push(b.data(), b.size()); return *this; }
	
	String toString() const { return (empty())?String():String((const char*)_ptr, _len); }
	String toString(const CString& type) const {
		if(type=="hex"){ return toHex(); } else if(type=="base64"){ return base64_encode(_ptr, _len); }
		else if(type=="raw"||type=="buff"){ return cout(); }
		else if(type=="int"||type=="number"||type=="dec"){ return toIntString(); }
		else{ return toString(); } }

	String toHex() const { String out; out.reserve(_len*2); char hex[3]; hex[2]=0;
		for(size_t i = 0;i<_len;i++){ ctoh(hex, (*this)[i]); out+=hex; } return out; }
	String toIntString() const { if(this->empty()){ return "0"; } String result; Buffer temp(*this); 
		 if(temp.size() == 1 && temp[0] == 0){ return "0"; } Buffer newTemp; newTemp.reserve(temp.size()); 
		 while(!temp.empty()){ unsigned int remainder = 0; bool leading_zeros_in_newTemp = true; 

			for (size_t i = 0; i < temp.size(); ++i){
				unsigned int current = remainder * 256 + temp[i];
				unsigned char quotient = current / 10; remainder = current % 10;
				if (quotient > 0 || !leading_zeros_in_newTemp){ newTemp.push_back(quotient); leading_zeros_in_newTemp = false; }
			} result.push_back('0'+remainder); swap(temp, newTemp); newTemp.clear(); }
		ncpp::reverse(result.begin(), result.end()); if(result.empty()){ return "0"; } return result; }
		
	String cout(size_t osize=50) const { String out("<Buffer"); osize=_len>osize?osize:_len; char hex[3]; hex[2]=0;
		for(size_t i = 0;i<osize;i++){ ctoh(hex, (*this)[i]); out+=" "; out+=hex; }
		if(_len>osize){ out+=" ... "; out+=dtos(_len-osize); out+=" bytes more>"; }else{ out+=">"; } return out; }
	
	static unsigned char htoc(const char hex[2]){ char res[2]; memset(res, 0, 2); //high,low;
		for(unsigned char i=0;i<2;i++){ char c = hex[i];
			if(c >= '0' && c <= '9'){ res[i] = c - '0'; } 
			else if(c >= 'A' && c <= 'F'){ res[i] = c - 'A' + 10; }
			else if(c >= 'a' && c <= 'f'){ res[i] = c - 'a' + 10; } } return ((res[0] << 4) | res[1]); }
	static void ctoh(char hex[2], unsigned char value){ char high = (value >> 4) & 0x0F; char low = value & 0x0F;
		hex[0] = (high < 10) ? ('0' + high) : ('a' + (high - 10)); hex[1] = (low < 10) ? ('0' + low) : ('a' + (low - 10)); }
	static unsigned long long htoll(const char* hex, char len){ unsigned long long num = 0; len=len/2; if(len>8) len=8;
		for(int i = 0; i < len; ++i){ num = (num << 8) | htoc(&hex[i*2]); } return num; }
	static unsigned long long htoll(const char* hex){ return htoll(hex, strlen(hex)); }
	static unsigned long long htoll(const String& str){ return htoll(str.data(), str.size()); }
	static void lltoh(char hex[16], unsigned long long value){ for(int i = 7; i >= 0; --i){ ctoh(&hex[i*2], value & 0xFF); value >>= 8; } }
	static String lltoh(unsigned long long value){ char hex[16]; if(!value){ return String(2, '0'); }
		for(int i = 7; i >= 0; --i){ ctoh(&hex[i*2], value & 0xFF); value >>= 8; }
		for(int i=0; i<16; i+=2){ if(hex[i]=='0'&&hex[i+1]=='0') continue; return String(hex+i, hex+16); } return String(hex); }
	
	friend void swap(Buffer& a, Buffer& b){ unsigned char* tmpc = a._ptr; a._ptr = b._ptr; b._ptr = tmpc;
		size_t tmp = a._len; a._len = b._len; b._len = tmp;
		tmp = a._msize; a._msize = b._msize; b._msize = tmp;
		tmp = a._mode; a._mode = b._mode; b._mode = tmp; }
	friend void move(Buffer& a, Buffer& b){ b._ptr = a._ptr; a._ptr = NULL;
		b._len = a._len; a._len = 0; b._msize = a._msize; a._msize = 0; a._mode = b._mode; }
		
	operator Array<char>() const { return Array<char>((char*)_ptr, (char*)_ptr+_len); }
	
	private:
		#if _BF_SSO_LEN > 0
		unsigned char _sso[_BF_SSO_LEN];
		void _init(size_t len){ _len=len++; if(_len<_BF_SSO_LEN){ _mode=STACK; _ptr=_sso; _msize=_BF_SSO_LEN; }else{ _mode=HEAP; _alloc(len); } }
		#else
		void _init(size_t len){ _len=len; _mode=HEAP; _ptr=NULL; _msize=0; _alloc(len); }
		#endif
		
		// == allocator
		void _alloc(size_t len, bool copy=false){ if(len<=0) return; if(_mode==STACK_ONLY){ print("ncpp::Buffer malloc error: mode=STACK_ONLY"); exit(1); }
			_msize=len<_MIN_ALLOC?_MIN_ALLOC:len; unsigned char* ptr0=_ptr; _ptr=(unsigned char*)malloc(_msize);
			if(_ptr==NULL){ print("ncpp::Buffer malloc error: Out of memory"); exit(1); } if(copy&&ptr0!=NULL){ memcpy(_ptr, ptr0, _len); } _mode=HEAP; }
			
		void _set(const void* ptr, size_t len){ if(len<=0){ _len=0; return; } resize(len); memcpy(_ptr, ptr, len); }
		//== ==
		static bool is_base64(unsigned char c){ return (isalnum(c) || (c == '+') || (c == '/')); }
}; 
const CString Buffer::base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

template <> String Array<Buffer>::cout() const { String ss("["); if(_len>0){ ss+=_ptr[0]; }
	for(size_t i=1;i<this->size();i++){ ss << ", " << _ptr[i].cout(); } ss+="]"; return ss; }

void print(const Buffer& buff){ print(buff.cout()); }
void print(const void* ptr){ String str="0x"; str+=Buffer::lltoh((size_t)ptr); print(str); }
void print(const volatile void* ptr){ print((const void*)ptr); }
String& String::operator<<(const Buffer& buff){ (*this)+=buff.cout(); return *this; } }
