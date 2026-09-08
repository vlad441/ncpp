#define _SSO_LEN 8
struct string {
	string(){ _init(0); }
	string(const char* ptr, size_t len){ _init(len); _set(ptr, len); }
	string(const char* cptr){ _init(strlen(cptr)); _set(cptr, _len); }
	string(size_t len, char v){ _init(len); memset(_ptr, v, _len); }
	string(const string& s){ _init(s.size()); _set(s.c_str(), s.size()); }
	//string(const string& s, size_t pos, size_t len = npos);
	//template <class InputIterator>  string(InputIterator first, InputIterator last);
	~string(){ if(!_isSso()) free(_ptr); }
	
	const char* c_str() const { return _ptr; }
	const char* begin() const { return _ptr; }
	char* begin(){ return _ptr; }
	const char* end() const { return _ptr+_len; }
	char* end(){ return _ptr+_len; }
	bool empty() const { return _len<=0||(_len==1&&*_ptr=='\0'); }
	size_t size() const { return _len; }
	size_t length() const { return _len; }
	size_t capacity() const { return _msize; } //C++11
	void reserve(size_t new_cap=0); //C++20
	void shrink_to_fit(); //C++20
	
	const char* test() const { return "stdcpp_string_test"; }
	
	bool operator==(const char* c) const { return _len==strlen(c)&&strncmp(_ptr, c, _len)==0; }
	bool operator==(const string& s) const { return _len==s.size()&&strncmp(_ptr, s.c_str(), _len)==0; }
	bool operator<(const string& s) const { return strncmp(_ptr, s.c_str(), _len)<0; }
	bool operator>(const string& s) const { return strncmp(_ptr, s.c_str(), _len)>0; }
	string& operator=(const char* c){ _set(c, strlen(c)); return *this; }
	string& operator=(const string& s){ _set(s.c_str(), s.size()); return *this; }
	
	string& operator+=(const char* c){ size_t len=strlen(c), len0=_len; _resize(_len+len); memcpy(_ptr+len0, c, len); _ptr[_len]='\0'; return *this; }
	string& operator+=(const string& s){ size_t len0=_len; _resize(_len+s.size()); memcpy(_ptr+len0, s.c_str(), s.size()); _ptr[_len]='\0'; return *this; }
	string operator+(const char* c) const { string nstr(*this); return nstr+=c; }
	string operator+(const string& s) const { string nstr(*this); return nstr+=s; }
	char& operator[](size_t pos){ return _ptr[pos]; }
	const char& operator[](size_t pos) const { return _ptr[pos]; }
		
	bool _isSso(){ return _ptr==_sso||_ptr==NULL; }
	private: char* _ptr; size_t _len, _msize; char _sso[_SSO_LEN]; //enum Mode { HEAP, STACK, STACK_ONLY }; char mode;
	
		void _init(size_t len){ _len=len++; if(_len<_SSO_LEN){ _ptr=&_sso[0]; _sso[_len]='\0'; _msize=0; }else{ _alloc(len); } }
		
		void _alloc(size_t sz, bool copy=false){ _msize=sz<_SSO_LEN*2?_SSO_LEN*2:sz; _ptr=(char*)malloc(_msize);
			if(_ptr==NULL){ print("std::string malloc error: Out of memory"); exit(1); } if(copy){ memcpy(_ptr, _sso, _SSO_LEN); } }
		
		void _reserve(size_t sz){ if(sz<=_msize||sz<=_SSO_LEN) return; if(_isSso()){ _alloc(sz, true); return; }
			_msize=(sz<_msize*2)?_msize*2:sz; _ptr=(char*)realloc(_ptr, _msize);
			if(_ptr==NULL){ print("std::string realloc error: Out of memory"); exit(1); } }
		void _resize(size_t len){ _len=len; _reserve(len+1); }
			
		void _set(const char* ptr, size_t len){ _resize(len); memcpy(_ptr, ptr, len); _ptr[len]='\0'; }
};
std::ostream& operator<<(std::ostream& os, const string& str){ os << str.c_str(); return os; }
