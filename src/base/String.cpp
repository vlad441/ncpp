namespace ncpp {
struct String; struct CString; struct Buffer;
template <typename T> struct Array;

template <typename T, typename Derived>
struct BaseString { enum Mode { HEAP, STACK, STACK_ONLY };
	BaseString() : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){}
	
	template <typename A, size_t N>
	Derived& stack(A (&arr)[N], bool ronly=false){ _ptr=(T*)arr; _len=N; _msize=N; _mode=ronly?STACK_ONLY:STACK; return (Derived&)*this; }
	template <typename A>
	Derived& stack(A* ptr, size_t len, bool ronly=false){ _ptr=(T*)ptr; _len=len; _msize=len; _mode=ronly?STACK_ONLY:STACK; return (Derived&)*this; }
	char mode() const { return _mode; }
	
	// == STL similar api ===
	T* data() const { return _ptr; }
	T* begin() const { return _ptr; }
	T* end() const { return _ptr+_len; }
	bool empty() const { return _len<=0; }
	size_t size() const { return _len; }
	size_t capacity() const { return _msize; }
	size_t length() const { return _len; }
	
	T& front(){ return *_ptr; }
	const T& front() const { return *_ptr; }
	T& back(){ return *(_ptr+_len-1); }
	const T& back() const { return *(_ptr+_len-1); }
	
	size_t find(const char* s, size_t pos = 0) const { return indexOf(s, pos); }
	size_t find(const char* s, size_t pos, size_t n) const { return indexOf(s, n, pos); }
	size_t find(char c, size_t pos = 0) const { return indexOf(&c, 1, pos); }
	
	size_t find_first_of(const char* chars, size_t pos = 0) const {
        if (!chars || _len == 0 || pos >= _len){ return NPOS; }
        for(size_t i = pos; i < _len; ++i){
            for(size_t j = 0; chars[j] != '\0'; ++j){          
				if (_ptr[i] == chars[j]){ return i; } } } return NPOS; }
    size_t find_last_of(const char* chars, size_t pos = NPOS) const {
        if (!chars || _len == 0){ return NPOS; }
        size_t start_pos = (pos == NPOS || pos >= _len) ? (_len - 1) : pos;
        for(size_t i = start_pos; ; --i){
            for(size_t j = 0; chars[j] != '\0'; ++j){
                if (_ptr[i] == chars[j]){ return i; } } if (i == 0) break; } return NPOS; }	
	size_t find_first_not_of(const char* chars, size_t pos = 0) const {
        if(!chars || _len == 0 || pos >= _len){ return NPOS; }
        for (size_t i = pos; i < _len; ++i){ bool found = false;
            for (size_t j = 0; chars[j] != '\0'; ++j){
                if(_ptr[i] == chars[j]){ found = true; break; } } if(!found){ return i; } } return NPOS; }
	size_t find_last_not_of(const char* chars, size_t pos=NPOS) const {
        if (!chars || _len == 0){ return NPOS; }
        size_t start_pos = (pos == NPOS || pos >= _len) ? (_len-1) : pos;

        for (size_t i = start_pos + 1; i-- > 0; ){ bool found = false;
            for (size_t j = 0; chars[j] != '\0'; ++j){
                if(_ptr[i] == chars[j]){ found = true; break; } } if(!found){ return i; } } return NPOS; }
	// == ==
	
    size_t indexOf(const char* delim, size_t len, size_t start) const {
        if(!delim||len==0||_len < len||start >= _len) return -1;
        for(size_t i = start; i <= _len - len; ++i){ size_t j = 0;
            while(j < len && _ptr[i + j] == delim[j]){ ++j; }
            if(j==len){ return i; } } return -1; }
    size_t indexOf(const char* delim, size_t start=0) const { return indexOf(delim, strlen(delim), start); }
    size_t indexOf(char c, size_t start=0) const { if(start >= _len) return -1;
		for(size_t i = start; i < _len; ++i){ if(_ptr[i] == c) return i; } return -1; }
	template <typename U, typename D> 
	size_t indexOf(const BaseString<U, D>& s, size_t start=0) const { return indexOf((char*)s.data(), s.size(), start); }
	bool includes(char c) const { return indexOf(c)==-1?false:true; }
	bool includes(const char* c) const { return indexOf(c)==NPOS?false:true; }
	
	size_t lastIndexOf(const char* delim, size_t len, size_t start=NPOS) const {
		if(!delim||len==0||_len < len||start >= _len) return -1;
		size_t actual_start = start; if(actual_start > _len - len){ actual_start = _len - len; }
		for (size_t i=actual_start; i-- > 0; ){ size_t j = 0;
			while(j < len && _ptr[i + j] == delim[j]){ ++j; }
			if(j == len){ return i; } } return -1; }
	size_t lastIndexOf(const char* delim) const { return lastIndexOf(delim, strlen(delim)); }
	size_t lastIndexOf(char c) const { for(size_t i=_len; i-- > 0; ){ if(_ptr[i] == c) return i; } return -1; }
		
	template <typename V>
	V _slice(int start, int end=0) const {
		if(start < 0){ start = _len+start; }
		if(end <= 0){ end = _len+end; } if(start < 0){ start = 0;} 
		if((size_t)end > _len){ end = _len; } if (start > end) start = end;
		V result(begin()+start, begin()+end); return result; }
		
	String slice(int start, int end=0) const;
	String substr(int start, int end=0) const;
	
	template <typename V>
	Array<V> _split(const char* delim, size_t len, int maxparts=-1) const { Array<V> result; result.reserve(maxparts>1?maxparts:0);
		size_t start=0; int parts=1; size_t end = indexOf(delim, len, start); if(end==NPOS||maxparts==1){ result.push(V(_ptr, _len)); return result; }
		while(end != NPOS&&(maxparts<=0||parts++<maxparts)){ result.push(_slice<V>(start, end));
			start = end+len; end = indexOf(delim, len, start); } result.push(_slice<V>(start)); return result; }
	template <typename V> Array<V> _split(const char* delim) const { return _split<V>(delim, strlen(delim)); }
	
	Array<String> split(const char* delim, size_t len, int maxparts=-1) const { return _split<String>(delim, len, maxparts); }
	Array<String> split(const char* delim) const { return _split<String>(delim, strlen(delim)); }
	Array<String> split(char delim) const { return _split<String>(&delim, 1); }
	template <typename U, typename D> 
	Array<String> split(const BaseString<U, D>& s) const { return this->_split<String>(s.data(), s.size()); }
	
	template <typename V>
	Array<V> _splitTokens(int limit=-1, const char* chars = " \t\r\n\f\v") const { Array<V> result; size_t currPos=0, tokensCnt=0;
        while (currPos < _len){ size_t startToken = find_first_not_of(chars, currPos); if(startToken == NPOS){ break; }
            size_t endToken = find_first_of(chars, startToken); if(endToken == NPOS){ result.push(_slice<V>(startToken)); break; }
            result.push_back(_slice<V>(startToken, endToken)); tokensCnt++;
            // Если достигнут лимит токенов (и он задан), добавляем оставшуюся часть строки как последний токен
            if(limit!=-1 && tokensCnt >= (size_t)limit){ break; } currPos = endToken; } return result; }
			
	Array<String> splitTokens(int limit=-1, const char* chars = " \t\r\n\f\v") const { return _splitTokens<String>(limit, chars); }
	
	template <typename V>
	Array<V> _splitParts(int partSz) const { Array<V> result; if(partSz <= 0){ result.push(V(_ptr, _len)); return result; }
		size_t numParts = (_len+partSz-1) / partSz; result.reserve(numParts); size_t start = 0; 
		while(start < _len){ size_t end = start+partSz; result.push(_slice<V>((int)start, (int)end)); start = end; } return result; }
		
	Array<String> splitParts(int partSz) const { return _splitParts<String>(partSz); }
	
	bool startsWith(const char* prefix, size_t len) const { if(len==0||len > this->size()) return false;
		for(size_t i = 0; i < len; ++i){ if(_ptr[i] != prefix[i]){ return false; } } return true; }
	bool startsWith(const char* prefix) const { return startsWith(prefix, strlen(prefix)); }
	template <typename U, typename D>
	bool startsWith(const BaseString<U, D>& prefix) const { return startsWith((prefix.size()>0)?(char*)&prefix[0]:NULL, prefix.size()); }
	
	bool endsWith(const char* suffix, size_t len) const { if(len==0||len > _len) return false;
		for(size_t i = 0; i < len; ++i){ if(_ptr[_len-len+i] != suffix[i]){ return false; } } return true; }
	bool endsWith(const char* suffix) const { return endsWith(suffix, strlen(suffix)); }
	template <typename U, typename D>
	bool endsWith(const BaseString<U, D>& suffix) const { return endsWith((suffix.size()>0)?(char*)&suffix[0]:NULL, suffix.size()); }
	//void fill(T value){ std::vector<unsigned char>::assign(size(), value); }
	bool toBool(){ return *this=="true"||*this=="1"||*this=="True"; }
	
	bool operator==(const char* c) const { return _len==strlen(c)&&strncmp((char*)_ptr, c, _len)==0; }
	template <typename U, typename D>
	bool operator==(const BaseString<U, D>& s) const { return _len==s.size()&&strncmp((char*)_ptr, (char*)s.data(), _len)==0; }
	bool operator!=(const char* c) const { return _len!=strlen(c)||strncmp((char*)_ptr, c, _len)!=0; }
	template <typename U, typename D>
	bool operator!=(const BaseString<U, D>& s) const { return _len!=s.size()||strncmp((char*)_ptr, (char*)s.data(), _len)!=0; }
	template <typename U, typename D>
	bool operator<(const BaseString<U, D>& s) const { return strncmp((char*)_ptr, (char*)s.data(), _len)<0; }
	template <typename U, typename D>
	bool operator>(const BaseString<U, D>& s) const { return strncmp((char*)_ptr, (char*)s.data(), _len)>0; }
	
	//T& operator[](size_t pos){ return _ptr[pos]; }
	
	protected: T* _ptr; size_t _len, _msize; char _mode;
};

struct CString : BaseString<const char, CString>{ //CString ≈ std::string_view
	CString(){ _ptr=NULL; _len=0; }
	CString(const char* c, size_t len){ _ptr=c; _len=len; }
	CString(const char* c){ _ptr=c; _len=strlen(c); }
	CString(const String& s);
	template <typename T, typename D>
	CString(const BaseString<T, D>& s){ _ptr=(const char*)s.data(); _len=s.size(); }
	//CString(const Buffer& b){ _ptr=(const char*)b.data(); _len=s.size(); }
	CString(const char* first, const char* end){ _len = (end > first)?(end-first) : 0; if(_len>0){ _ptr=first; }else{ _ptr=NULL; } }
	template <size_t N>
	CString(const char (&arr)[N]){ _ptr=arr; _len=N; } //const char arr[];
	const char* c_str() const { return _ptr; }
	
	const char& operator[](size_t pos) const { return _ptr[pos]; }
	char at(size_t pos) const { return (_ptr&&pos<_len)?_ptr[pos]:0; }
	
	CString& operator=(const char* c){ _ptr=c; _len=strlen(c); return *this; }
	template <typename T, typename D>
	CString& operator=(const BaseString<T, D>& s){ _ptr=(const char*)s.c_str(); _len=s.size(); return *this; }
	String operator+(const char* c) const;
	template <typename T, typename D>
	String operator+(const BaseString<T, D>& s) const;
	
	//friend String operator+(const char* c, const CString& cs);
};

#define _SSO_LEN 7
struct String : BaseString<char, String>{ //String ≈ std::string
	typedef char* Iter; typedef const char* CIter; typedef CIter ConstIter;
	typedef Iter iterator; typedef CIter const_iterator;
	String(size_t len=0){ _init(len); }
	String(size_t len, char v){ _init(len); memset(_ptr, v, _len); }
	String(const char* ptr, size_t len){ _init(len); _set(ptr, len); }
	String(const char* cptr){ _init(strlen(cptr)); _set(cptr, _len); }
	String(const String& s){ _init(s._len); _set(s._ptr, s._len); }
	String& operator=(const String& s){ _set(s.c_str(), s.size()); return *this; }
	template <typename T, typename D>
	String(const BaseString<T, D>& s){ //print("String::BaseString init constructor called.\n"); 
		_init(s.size()); _set(s.data(), s.size()); }
	String(const char* begin, const char* end){ size_t len = (end > begin)?(end-begin) : 0; _init(len); _set(begin, len); }
	template <size_t N>
	String(const char (&arr)[N]){ _init(strnlen(arr, N)); _set(arr, _len); } //char arr[];
	~String(){ if(_mode==HEAP&&_ptr!=NULL) free(_ptr); }
	
	
	// == STL similar api ===
	const char* c_str() const { return _ptr; }
	
	String& reserve(size_t sz){ if(sz<=_msize||sz<=_SSO_LEN) return *this; if(_mode!=HEAP){ _alloc(sz, true); return *this; }
		_msize=(sz<_msize*2)?_msize*2:sz; _ptr=(char*)realloc(_ptr, _msize);
		if(_ptr==NULL){ Except("ncpp::String realloc error: Out of memory\n", 0, ERR_OOM); } return *this; }
	String& resize(size_t len){ reserve(len+1); _len=len; _ptr[len]='\0'; return *this; }
	
	String& erase(size_t pos=0, size_t len = NPOS){ if(pos >= _len) return *this; if(len > _len-pos){ len = _len-pos; } erase(_ptr+pos, _ptr+pos+len); return *this; }
	void erase(Iter first, Iter last){ if(first >= last || first < _ptr || last > _ptr+_len) return;
        size_t end_pos = last-_ptr; memmove(first, last, _len-end_pos); _len -= last-first; }
	void erase(Iter ipos){ erase(ipos, ipos+1); }
	
	void push_back(char v){ push(v); }
	void pop_back(){ resize(--_len); }
	
	void clear(){ _len=0; }
	void shrink(){ if(_len>=_msize||_ptr==NULL||_mode!=HEAP||_isSSO()) return; 
		if(_len==0){ free(_ptr); _ptr=NULL;
		#if _SSO_LEN > 0
			stack(_sso); _len=0;
		#endif
			return; }
		_ptr=(char*)realloc(_ptr, _msize=_len); if(_ptr==NULL){ Except("ncpp::String shrink mem error\n", 0); } }
	void shrink_to_fit(){ shrink(); }
	// == ==
	template <size_t N>
	String& push(const char (&arr)[N]){ _append(arr, N); return *this; }
	String& push(const char* ptr, size_t len){ _append(ptr, len); return *this; }
	String& push(const String& s){ _append(s.data(), s.size()); return *this; }
	String& push(const CString& cs){ _append(cs.data(), cs.size()); return *this; }
	String& push(char v){ _append(v); return *this; }
	char pop(){ if(_len>0){ resize(--_len); } return *(_ptr+_len); }
	String& fill(char v){ memset(_ptr, v, _len); return *this; }
	
	String replace(const CString& from, const CString& to, bool all = false) const {
		String result(*this); if(from.empty()){ return result; } size_t cpos=0; size_t fpos;
		while((fpos = result.indexOf(from, cpos))!=NPOS){ String tmp; 
			tmp.push(result.c_str(), fpos); tmp.push(to.c_str(), to.size());
			tmp.push(result.c_str()+fpos+from.size(), result.size() - (fpos + from.size()));
			result = tmp; cpos = fpos+to.size(); if(!all){ break; } } return result; }
	String replaceAll(const CString& from, const CString& to) const { return replace(from, to, true); }
	
	struct RegExp; // not implemented
	String toLowerCase() const { String result(*this); //Only for ASCII ( c &= ~0x20; )?
		for(size_t i = 0; i < result.size(); ++i){ char c = result[i]; if(c >= 'A' && c <= 'Z'){ result[i] = c+('a'-'A'); } } return result; }
	String toUpperCase() const { String result(*this); //Only for ASCII
		for(size_t i = 0; i < result.size(); ++i){ char c = result[i]; if(c >= 'a' && c <= 'z'){ result[i] = c-('a'-'A'); } } return result; }
	String trim() const { size_t first = find_first_not_of(" \t\r\n\f\v"); size_t last = find_last_not_of(" \t\r\n\f\v");
		if(first == NPOS || last == NPOS){ return String(""); } return slice(first, last+1); }
	
	char& operator[](size_t pos){ return _ptr[pos]; }
	const char& operator[](size_t pos) const { return _ptr[pos]; }
	char at(size_t pos) const { return (_ptr&&pos<_len)?_ptr[pos]:0; }
	
	String& operator=(const char* c){ _set(c, strlen(c)); return *this; }
	template <typename T, typename D>
	String& operator=(const BaseString<T, D>& s){ _set((const char*)s.data(), s.size()); return *this; }
	template <size_t N>
	String& operator=(const char (&arr)[N]){ _set(arr, strnlen(arr, N)); return *this; }
	String& operator+=(const char* c){ _append(c, strlen(c)); return *this; }
	String& operator+=(const String& s){ _append(s.c_str(), s.size()); return *this; }
	String& operator+=(char c){ _append(c); return *this; }
	template <typename T, typename D>
	String& operator+=(const BaseString<T, D>& s){ _append((const char*)s.data(), s.size()); return *this; }
	String operator+(const char* c) const { String nstr(*this); return nstr+=c; }
	String operator+(const String& s) const { String nstr(*this); return nstr+=s; }
	String operator+(char c) const { String nstr(*this); return nstr+=c; }
	template <typename T, typename D>
	String operator+(const BaseString<T, D>& s) const { String nstr(*this); return nstr+=s; }

	String& operator<<(const char* c){ _append(c, strlen(c)); return *this; }
	//explicit String& operator<<(char c){ _append(c); return *this; }
	String& operator<<(const String& s){ _append(s.data(), s.size()); return *this; }
	String& operator<<(const Buffer& buff);
	template <typename T, typename D> String& operator<<(const BaseString<T, D>& s){ _append(s.data(), s.size()); return *this; }
	String& operator<<(long long num);
	//String& operator<<(double num){}
	template <typename T> String& operator<<(const Array<T>& arr){ *this+=arr.cout(); return *this; }
		
	operator Array<char>() const { return Array<char>((char*)_ptr, (char*)_ptr+_len); }
	
	void swap(String& other){ ncpp::swap(*this, other); }
	friend void swap(String& a, String& b){ char* tmpc = a._ptr; a._ptr = b._ptr; b._ptr = tmpc;
		size_t tmp = a._len; a._len = b._len; b._len = tmp;
		tmp = a._msize; a._msize = b._msize; b._msize = tmp;
		tmp = a._mode; a._mode = b._mode; b._mode = tmp; }
		
	#if __cplusplus >= 201103L //move for C++11
	String(String&& tmp) noexcept { move(*this, tmp); }
	String& operator=(String&& tmp) noexcept { if(this!=&tmp) move(*this, tmp); return *this; }
	String& steal(String& tmp){ move(*this, tmp); return *this; }
	String& steal(String&& tmp){ move(*this, tmp); return *this; }
	friend void move(String& dst, String&& tmp){ move(dst, (String&)tmp); }
	#else //move for C++98
	String& steal(const String& victim){ move(*this, (String&)victim); return *this; }
	friend void move(String& dst, const String& victim){ move(dst, (String&)victim); }
	#endif
	friend void move(String& dst, String& tmp){ if(dst._mode==HEAP&&dst._ptr!=NULL) free(dst._ptr);
		#if _SSO_LEN > 0
		if(tmp._isSSO()){ dst=tmp; tmp.clear(); return; } dst._ptr = tmp._ptr; tmp._ptr = tmp._sso; dst._msize = tmp._msize; tmp._msize=_SSO_LEN;
		#else
		dst._ptr = tmp._ptr; tmp._ptr = NULL; dst._msize = tmp._msize; tmp._msize = 0;
		#endif
		dst._len = tmp._len; tmp._len = 0;
		dst._mode = tmp._mode; tmp._mode = STACK; }
		
	private: 
		#if _SSO_LEN > 0
		char _sso[_SSO_LEN]; bool _isSSO(){ return _ptr==_sso; }
		void _init(size_t len){ _len=len++; if(_len<_SSO_LEN){ _mode=STACK; _ptr=_sso; _sso[_len]='\0'; _msize=_SSO_LEN; }else{ _mode=HEAP; _alloc(len); } }
		#else
		void _init(size_t len){ _len=len; _mode=HEAP; _ptr=NULL; _msize=0; if(_len>0){ _alloc(len); } }
		#endif
		
		// == allocator
		void _alloc(size_t sz, bool copy=false){ if(_mode==STACK_ONLY){ Except("ncpp::String malloc error: mode=STACK_ONLY\n", 0); return; }
			_msize=sz<_SSO_LEN*2?_SSO_LEN*2:sz; char* ptr0=_ptr; _ptr=(char*)malloc(_msize);
			if(_ptr==NULL){ Except("ncpp::String malloc error: Out of memory\n", 0, ERR_OOM); }
			if(copy){ memcpy(_ptr, ptr0, _len); } _mode=HEAP; }
			
		void _set(const char* ptr, size_t len){ if(len<=0){ _len=0; return; } resize(len); memcpy(_ptr, ptr, len); }
		
		void _append(const char* ptr, size_t len){ size_t len0=_len; resize(_len+len); memcpy(_ptr+len0, ptr, len); _ptr[_len]='\0'; }
		void _append(char c){ resize(_len+1); _ptr[_len-1]=c; }
};

CString::CString(const String& s){ _ptr=(const char*)s.data(); _len=s.size(); }
String CString::operator+(const char* c) const { String nstr(*this); return nstr+=c; }
template <typename T, typename D> String CString::operator+(const BaseString<T, D>& s) const { String nstr(*this); return nstr+=s; }
template <typename T, typename D> String operator+(const char* c, const BaseString<T, D>& s){ String nstr(c); return nstr+=s; }
//String operator+(const char* c, const CString& cs){ String nstr(c); return nstr+=cs; }
//String operator+(const char* c, const String& s){ String nstr(c); return nstr+=s; }

template <typename T, typename D>
String BaseString<T, D>::slice(int start, int end) const { return _slice<String>(start, end); }
template <typename T, typename D>
String BaseString<T, D>::substr(int start, int end) const { return _slice<String>(start, end); }

template <typename T> String Array<T>::cout() const { String ss("["); if(_len>0){ ss<<_ptr[0]; }
	for(size_t i=1;i<this->size();i++){ ss << ", " << _ptr[i]; } ss+="]"; return ss; }
template <> String Array<String>::cout() const { String ss("["); if(_len>0){ ss+="\""; ss<<_ptr[0]; }
	for(size_t i=1;i<this->size();i++){ ss << "\", \"" << _ptr[i]; } if(_len>0) ss+="\""; ss+="]"; return ss; }
	
void Except(const String& s, int errlvl, int type){ Except(s.c_str(), errlvl, type); }

template <typename T, typename D>
void print(const BaseString<T, D>& s){ print(s.data(), s.size()); } 

#ifdef NCPP_LIB_BUILD
//template struct BaseString<char, String>; //Принудительно сгенерировать код BaseString<char, String>
//extern template struct Array<String>; //Не генерировать код Array<String> тут
#endif
}
