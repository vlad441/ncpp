namespace ncpp {
struct String; struct CString; struct Buffer;
template <typename T>
struct Array; //struct _CBaseString? //struct PODArray<T> -> String<char> -> Buffer<unsigned char>?

#define _MIN_ALLOC 16
template <typename T>
struct PODArray { enum Mode { HEAP, STACK, STACK_ONLY }; typedef T* Iter; typedef const T* ConstIter;
	typedef Iter iterator; typedef ConstIter const_iterator;
	PODArray() : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){}
	PODArray(const T* ptr, size_t len){ _init(len); _copy((T*)ptr, len); }
	PODArray(size_t len){ _init(len); }
	PODArray(size_t len, T v){ _init(len); memset(_ptr, v, _len); }
	PODArray(const PODArray& arr){ _init(arr.size()); _copy(arr.data(), arr.size()); }
	PODArray(const T* begin, const T* end){ _init(0); assign(begin, end); }
	template <size_t N>
	PODArray(const T (&arr)[N]){ _init(N); _copy(arr, N); }
	~PODArray(){ if(_mode==HEAP&&_ptr!=NULL) free(_ptr); }
	
	template <typename A, size_t N>
	PODArray& stack(A (&arr)[N], bool ronly=false){ _ptr=(T*)arr; _len=N; _msize=N; _mode=ronly?STACK_ONLY:STACK; return *this; }
	template <typename A>
	PODArray& stack(A* ptr, size_t len, bool ronly=false){ _ptr=(T*)ptr; _len=len; _msize=len; _mode=ronly?STACK_ONLY:STACK; return *this; }
	char mode() const { return _mode; }
	
	// == STL similar api ===
	T* data() const { return _ptr; }
	T* begin() const { return _ptr; }
	T* end() const { return _ptr+_len; }
	bool empty() const { return _len<=0; }
	size_t size() const { return _len; }
	size_t capacity() const { return _msize; }
	size_t length() const { return _len; }
	
	void assign(const T* begin, const T* end){ size_t len = (end > begin)?(end-begin) : 0; _copy(begin, len); }
	
	PODArray& reserve(size_t sz){ if(sz<=_msize) return *this; if(_ptr==NULL||_mode!=HEAP){ _alloc(sz, true); return *this; }
		_msize=(sz<_msize*2)?_msize*2:sz; _ptr=(T*)realloc(_ptr, _msize*sizeof(T));
		if(_ptr==NULL){ print("ncpp::PODArray realloc error: Out of memory"); exit(1); } return *this; }
	PODArray& resize(size_t len){ reserve(len); _len=len; return *this; }
	
	void insert(Iter ipos, const void* dptr, size_t len){ if(len==0) return; size_t pos=_len;
		if(ipos!=NULL && ipos >= _ptr && ipos <= _ptr+_len){ pos = ipos-_ptr; } size_t len0=_len; size_t nlen = _len+len; reserve(nlen);
		if(pos < len0){ memmove(ipos+len, ipos, (len0-pos)*sizeof(T)); }
        memcpy(_ptr+pos, dptr, len*sizeof(T)); _len = nlen; return; }
	void insert(Iter ipos, const void* first, const void* last){ const unsigned char *b=(const unsigned char*)first, *e=(const unsigned char*)last;
		size_t len = (e > b)?(e-b) : 0; insert(ipos, b, len); }
	void insert(Iter ipos, size_t n, T v){ if(n == 0) return; size_t pos=_len;
		if(ipos!=NULL && ipos >= _ptr && ipos <= _ptr+_len){ pos = ipos-_ptr; } size_t len0=_len; size_t nlen = _len+n; reserve(nlen);
		if(pos < len0){ memmove(_ptr+pos+n, _ptr+pos, (len0-pos)*sizeof(T)); }
        for(size_t i = 0; i<n; ++i){ _ptr[pos + i] = v; } _len = nlen; return; }
	void insert(Iter ipos, T v){ insert(ipos, &v, 1); }
	void insert(Iter ipos, const PODArray& other){ insert(ipos, other.data(), other.size()); }
	
	void erase(Iter first, Iter last){ if(first >= last || first < _ptr || last > _ptr+_len) return;
        size_t end_pos = last-_ptr; memmove(first, last, (_len-end_pos)*sizeof(T)); _len -= last-first; }
	void erase(Iter ipos){ erase(ipos, ipos+1); }
	
	void push_back(T v){ push(v); }
	void pop_back(){ resize(--_len); }
	
	void swap(PODArray& other){ unsigned char* temp_ptr = _ptr; _ptr = other._ptr; other._ptr = temp_ptr;
		size_t tmp = _len; _len = other._len; other._len = tmp;
		tmp = _msize; _msize = other._msize; other._msize = tmp;
		tmp = _mode; _mode = other._mode; other._mode = tmp; }
	
	void clear(){ _len=0; }
	void shrink_to_fit(){ if(_len>=_msize||_ptr==NULL||_mode!=HEAP) return; _ptr=(T*)realloc(_ptr, (_msize=_len)*sizeof(T));
		if(_ptr==NULL){ print("ncpp::PODArray shrink mem error"); exit(1); } return; }
	void shrink(){ shrink_to_fit(); }
	// == ==
	template <size_t N>
	PODArray& push(const T (&arr)[N]){ size_t len0=_len; resize(_len+N); memcpy(_ptr+len0, arr, N*sizeof(T)); return *this; }
	PODArray& push(const void* ptr, size_t len){ size_t len0=_len; resize(_len+len); memcpy(_ptr+len0, ptr, len*sizeof(T)); return *this; }
	PODArray& push(T v){ reserve(++_len); _ptr[_len-1]=v; return *this; }
	unsigned char pop(){ resize(--_len); return _ptr[_len]; }
	
	void push_front(unsigned char v){ insert(begin(), v); }
	void pop_front(){ erase(begin()); }
	
	PODArray& concat(const PODArray& arr2){ push(arr2.data(), arr2.size()); return *this; }
	
	bool operator==(const PODArray& arr) const { return _len==arr.size()&&strncmp(_ptr, arr.data(), _len*sizeof(T))==0; }
	bool operator!=(const PODArray& arr) const { return _len!=arr.size()||strncmp(_ptr, arr.data(), _len*sizeof(T))!=0; }
	template <size_t N>
	PODArray& operator=(const T (&arr)[N]){ _copy(arr, N); return *this; }
	PODArray& operator=(const PODArray& arr){ _copy(arr.data(), arr.size()); return *this; }
	const T& operator[](size_t pos) const { return _ptr[pos]; }
	T& operator[](size_t pos){ return _ptr[pos]; }
	
	protected: T* _ptr; size_t _len, _msize; char _mode;
		// == allocator
		void _init(size_t len){ print("PODArray::_init() called.\n"); _mode=HEAP; _ptr=NULL; _len=len; _msize=0; _alloc(len); }
		
		void _alloc(size_t len, bool copy=false){ if(len<=0) return; if(_mode==STACK_ONLY){ print("ncpp::PODArray malloc error: mode=STACK_ONLY"); exit(1); }
			_msize=len<_MIN_ALLOC?_MIN_ALLOC:len; T* ptr0=_ptr; _ptr=(T*)malloc(_msize*sizeof(T));
			if(_ptr==NULL){ print("ncpp::PODArray malloc error: Out of memory"); exit(1); } if(copy&&ptr0!=NULL){ memcpy(_ptr, ptr0, _len*sizeof(T)); } _mode=HEAP; }
			
		void _copy(const void* ptr, size_t len, size_t pos=0){ resize(len+pos); memcpy(_ptr+pos, ptr, len*sizeof(T)); }
}; }
