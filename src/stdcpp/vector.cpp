template <typename T>
struct vector {
	vector(size_t n=0, const T& val=T()) : _ptr(NULL), _len(0), _msize(0){ _fill(n, val); }
	vector(const vector<T>& v) : _ptr(NULL), _len(0), _msize(0){ _copy(v); };
	//template <class InputIterator> vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type());
	~vector(){ clear(); if(_ptr!=NULL) free(_ptr); }
	
	const T* begin() const { return _ptr; }
	T* begin(){ return _ptr; }
	const T* end() const { return _ptr+_len; }
	T* end(){ return _ptr+_len; }
	bool empty() const { return _len<=0; }
	size_t size() const { return _len; }
	size_t capacity() const { return _msize; }
	const T& back() const { return *(_ptr+_len-1); }
	T& back(){ return *(_ptr+_len-1); }
	
	void reserve(size_t len){ if(len<=_msize) return; if(_ptr==NULL){ _alloc(len); return; }
		_msize=(len<_msize*2)?_msize*2:len; _ptr=(T*)realloc(_ptr, _msize*sizeof(T));
		if(_ptr==NULL){ print("std::vector realloc error: Out of memory"); exit(1); } }
	void resize(size_t len, const T& val=T()){ if(_len==len) return; size_t len0=_len; _len=len;
		if(_len<len0){ for(size_t i=_len-1;i>=len0;i--){ _ptr[i].~T(); } }
		else{ reserve(len); for(size_t i=len0;i<_len;i++){ new (&_ptr[i]) T(val); } } }
		
	void push_back(const T& val){ reserve(++_len); new (&_ptr[_len-1]) T(val); }
	void pop_back(){ resize(--_len); }
	
	void clear(){ for(size_t i=0;i<_len;i++){ _ptr[i].~T(); } _len=0; }
	void shrink_to_fit(); //C++11
	
	//bool operator==(const char* c) const { return _len==strlen(c)&&strncmp(_ptr, c, _len)==0; }
	//bool operator==(const std::string& s) const { return _len==s.size()&&strncmp(_ptr, s.c_str(), _len)==0; }
	vector<T>& operator=(const vector& v){ _copy(v); return *this; }
	
	T& operator[](size_t pos){ return _ptr[pos]; }
	const T& operator[](size_t pos) const { return _ptr[pos]; }
		
	private: T* _ptr; size_t _len, _msize;
	
		void _alloc(size_t len){ if(len<=0) return; _msize=len; _ptr=(T*)malloc(_msize*sizeof(T));
			if(_ptr==NULL){ print("std::vector malloc error: Out of memory"); exit(1); } }
			
		void _fill(size_t len, const T& val){ _len=len; reserve(len); for(size_t i=0;i<_len;i++){ _ptr[i]=val; } }
		
		void _copy(const vector<T>& v){ if(this == &v) return; clear(); if(v.size()<=0) return; _len=v.size();
			reserve(_len); for(size_t i=0;i<_len;i++){ new (&_ptr[i]) T(v[i]); } }
};
