namespace ncpp {
struct String;
template <typename T> //Array<T> ≈ std::vector<T>
struct Array { enum Mode { HEAP, STACK, STACK_ONLY };
	typedef T* Iter; typedef const T* CIter; typedef CIter ConstIter; 
	typedef Iter iterator; typedef CIter const_iterator;
	
	Array(size_t n=0, const T& val=T()) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ assign(n, val); }
	Array(const Array<T>& arr) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ _copy(arr.data(), arr.size()); };
	Array(const T* begin, const T* end) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ assign(begin, end); }
	template <size_t N>
	Array(const T (&arr)[N]) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ _copy(arr, N); } //T arr[];
	#if __cplusplus >= 201103L
	//Array(const initializer_list<T>& list) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ _copy(list.begin(), list.size()); }
	#endif
	~Array(){ clear(); if(_mode==HEAP&&_ptr!=NULL) free(_ptr); }
	
	template <typename A, size_t N>
	Array<T>& stack(A (&arr)[N], bool ronly=false){ clear(); if(_mode==HEAP&&_ptr!=NULL) free(_ptr);
		_ptr=(T*)arr; _len=N; _msize=N; _mode=ronly?STACK_ONLY:STACK; return *this; }
	template <typename A>
	Array<T>& stack(A* ptr, size_t len, bool ronly=false){ clear(); if(_mode==HEAP&&_ptr!=NULL) free(_ptr);
		_ptr=(T*)ptr; _len=len; _msize=len; _mode=ronly?STACK_ONLY:STACK; return *this; }
	char mode() const { return _mode; }
	// == STL similar api ===
	
	T* data() const { return _ptr; }
	T* begin() const { return _ptr; }
	T* end() const { return _ptr+_len; }
	bool empty() const { return _len<=0; }
	size_t size() const { return _len; }
	size_t capacity() const { return _msize; }
	
	void reserve(size_t len){ if(len<=_msize) return; if(_ptr==NULL||_mode!=HEAP){ _alloc(len); return; } _realloc((len<_msize*2)?_msize*2:len); }
	void resize(size_t len, const T& val=T()){ if(_len==len) return;
		if(len>_len){ reserve(len); for(size_t i=_len;i<len;i++){ new (_ptr+i) T(val); } }
		else{ for(size_t i=_len; i-- > len; ){ _ptr[i].~T(); } } _len=len; }
		
	void assign(const T* begin, const T* end){ _copy(begin, ((end > begin)?(end-begin) : 0)); }
	void assign(size_t n, const T& val){ if(n<=0) return; clear(); reserve(n); 
		for(size_t i=0;i<n;i++){ new (&_ptr[i]) T(val); } _len=n; }
		
	void insert(Iter ipos, const T* dptr, size_t len){ if(len==0) return; size_t pos=_len;
		if(ipos!=NULL && ipos >= _ptr && ipos <= _ptr+_len){ pos = ipos-_ptr; } size_t len0=_len; size_t nlen = _len+len; reserve(nlen);
		if(pos < len0){ _move_right(pos, len); } _copy(dptr, len, pos); _len = nlen; return; }
	void insert(Iter ipos, const T* first, const T* last){ const T *b=first, *e=last; size_t len = (e > b)?(e-b) : 0; insert(ipos, b, len); }
	void insert(Iter ipos, size_t n, const T& v);
	void insert(Iter ipos, const T& v){ insert(ipos, &v, 1); }
	void insert(Iter ipos, const Array& other){ insert(ipos, other.data(), other.size()); }
	
	Iter erase(Iter first, Iter last){ if(first >= last || first < _ptr || last > _ptr+_len) return end();
		_move_left((size_t)(first-_ptr), (size_t)(last-first)); return first; }
	Iter erase(Iter ipos){ return erase(ipos, ipos+1); }
		
	void push_back(const T& val){ push(val); }
	void pop_back(){ resize(_len-1); }
	
	T& front(){ return *_ptr; }
	const T& front() const { return *_ptr; }
	T& back(){ return *(_ptr+_len-1); }
	const T& back() const { return *(_ptr+_len-1); }
	
	void clear(){ for(size_t i=0;i<_len;i++){ _ptr[i].~T(); } _len=0; }
	void shrink(){ if(_len>=_msize||_ptr==NULL||_mode!=HEAP) return; if(_len==0){ free(_ptr); _ptr=NULL; return; } _reallocT(_len); }
	void shrink_to_fit(){ shrink(); }
	// == ==
	size_t indexOf(const T& value, size_t start=0) const { for(size_t i=start; i<_len; ++i){ if(_ptr[i] == value){ return i; } } return -1; }
	Array<T> slice(int start, int end=0) const {
		if(start < 0){ start = _len+start; }
		if(end <= 0){ end = _len+end; } if(start < 0){ start = 0; }
		if((size_t)end > _len){ end = _len; } if (start >= end) start = end;
		Array<T> result(_ptr+start, _ptr+end); return result; }
	
	void fill(const T& val){ for(size_t i=0;i<_len;i++){ _ptr[i]=val; } }
	void splice(size_t pos, size_t count=1){ if(this->empty()) return; _move_left(pos, count); }
	
	template <size_t N>
	Array<T>& push(const T (&arr)[N]){ size_t len0=_len; resize(_len+N); _copy(arr, N, len0); return *this; }
	Array<T>& push(const T* ptr, size_t len){ size_t len0=_len; resize(_len+len); _copy(ptr, len, len0); return *this; }
	Array<T>& push(const T& val){ reserve(_len+1); ++_len; new (&_ptr[_len-1]) T(val); return *this; }
	//void push(const T& val){ reserve(_len+1); ++_len; if(is_pod<T>::V){ _ptr[_len-1]=val; }else{ new (&_ptr[_len-1]) T(val); } } //type traits example
	T pop(){ T val(_ptr[_len-1]); resize(_len-1); return val; }
	
	void push_front(const T& v){ insert(begin(), v); }
	void pop_front(){ erase(begin()); }
	
	Array<T>& concat(const Array<T>& arr2){ push(arr2.data(), arr2.size()); return *this; }
	static Array<T> concat(const Array<T>& arr1, const Array<T>& arr2){ Array<T> ret = arr1; ret.concat(arr2); return ret; }
	void reverse(){ ncpp::reverse(this->begin(),this->end()); }
	
	template <typename U> T join(const U& delim) const { if(this->empty()) return T(); T result = _ptr[0];
		for(size_t i = 1; i < this->size(); ++i){ result += delim; result += _ptr[i]; } return result; }
	//T join(const T& delim) const { return _join(delim); }
	//T join(const char* delim) const { return _join(delim); }
	//T join() const { return _join(""); }
	
	T& operator[](size_t pos){ return _ptr[pos]; }
	const T& operator[](size_t pos) const { return _ptr[pos]; }
	const T& at(size_t pos) const { static const T cval=T(); return (_ptr&&pos<_len)?_ptr[pos]:cval; }
	//const T& at(size_t pos) const { if(_len==0||!_ptr||pos>=_len){ Except("Array::at() error: Out of range.\n", 1, ERR_RANGE); return; } return _ptr[pos]; }
	
	//bool operator==(const Array& arr) const;
	template <size_t N>
	Array<T>& operator=(const T (&arr)[N]){ _copy(arr, N); return *this; }
	Array<T>& operator=(const Array& arr){ _copy(arr.data(), arr.size()); return *this; }
	//Array<T>& operator+=(const T& v){ return push(v); }
	//Array<T> operator+(const T& v) const { Array arr(*this); return arr+=v; }
	
	String cout() const;
	
	void swap(Array& other){ ncpp::swap(*this, other); }
	friend void swap(Array& a, Array& b){ char* tmpc = a._ptr; a._ptr = b._ptr; b._ptr = tmpc;
		size_t tmp = a._len; a._len = b._len; b._len = tmp;
		tmp = a._msize; a._msize = b._msize; b._msize = tmp;
		tmp = a._mode; a._mode = b._mode; b._mode = tmp; }
	
	#if __cplusplus >= 201103L //move for C++11
	Array(Array&& tmp) noexcept { move(*this, tmp); }
	Array& operator=(Array&& tmp) noexcept { if(this!=&tmp) move(*this, tmp); return *this; }
	Array& steal(Array& tmp){ move(*this, tmp); return *this; }
	Array& steal(Array&& tmp){ move(*this, tmp); return *this; }
	friend void move(Array& dst, Array&& tmp){ move(dst, (Array&)tmp); }
	#else //move for C++98
	Array& steal(const Array& victim){ move(*this, (Array&)victim); return *this; }
	friend void move(Array& dst, const Array& victim){ move(dst, (Array&)victim); }
	#endif
	friend void move(Array& dst, Array& tmp){ dst.clear(); if(dst._mode==HEAP&&dst._ptr!=NULL) free(dst._ptr);
		dst._ptr = tmp._ptr; tmp._ptr = NULL; 
		dst._len = tmp._len; tmp._len = 0;
		dst._msize = tmp._msize; tmp._msize = 0;
		dst._mode = tmp._mode; tmp._mode = HEAP; }
	
	protected: T* _ptr; size_t _len, _msize; char _mode;
		void _alloc(size_t len){ if(_mode==STACK_ONLY){ Except("ncpp::Array malloc error: mode=STACK_ONLY\n", 0); }
			if(len<=0) return; _msize=len; _ptr=(T*)malloc(_msize*sizeof(T));
			if(_ptr==NULL){ Except("ncpp::Array malloc error: Out of memory\n", 0, ERR_OOM); } }
		
		void _copy(const T* begin, size_t len, size_t pos=0){ if(len<=0){ clear(); return; } if(begin == this->begin()||begin==NULL) return; reserve(len+pos);
			if(pos==0){ clear(); }else{ for(size_t i=pos;i<min(_len, len+pos);i++){ _ptr[i].~T(); } } // clear space pos <-> pos+len;
			for(size_t i=0;i<len;i++){ new (&_ptr[pos+i]) T(*(begin+i)); } _len=len+pos; } // fill new values
			
		void _move_right(size_t pos, size_t roffset){ if(roffset <= 0 || pos >= _len) return; resize(_len+roffset);
            for(long i = _len-1; i >= (long)pos; --i){ _ptr[i+roffset]=_ptr[i]; } }
        void _move_left(size_t pos, size_t loffset){ if(loffset <= 0 || pos >= _len) return; if(loffset>_len-pos){ loffset=_len-pos; }
			for(size_t i = pos; i < _len - loffset; ++i){ _ptr[i] = _ptr[i+loffset]; } resize(_len-loffset); }
			
		//void _realloc(size_t nsize){ _reallocPOD(nsize); }
		void _realloc(size_t nsize){ _reallocT(nsize); }
			
		void _reallocT(size_t nsize){ //print("(#DEBUG) ncpp::Array _reallocT called: nsize="); _printNum(nsize); print("\n");
			T* nptr = (T*)malloc(nsize*sizeof(T)); if(nptr == NULL){ Except("ncpp::Array _reallocT error: Out of memory\n", 0, ERR_OOM); }
			size_t copy_cnt = ncpp::min(_len, nsize); 
			#if __cplusplus >= 201103L
			for(size_t i=0; i<copy_cnt; i++){ new (nptr+i) T((T&&)_ptr[i]); }
			#else
			for(size_t i=0; i<copy_cnt; i++){ new (nptr+i) T(_ptr[i]); }
			#endif
			for(size_t i=0; i<_len; ++i){ _ptr[i].~T(); }
			if(_ptr != NULL){ free(_ptr); } _ptr = nptr; _msize = nsize; }
			
		void _reallocPOD(size_t nsize){ _msize=nsize; _ptr=(T*)realloc(_ptr, _msize*sizeof(T));
			if(_ptr==NULL){ Except("ncpp::Array realloc error: Out of memory\n", 0, ERR_OOM); } }
};

//template <typename T> void Array<T*>::resize(size_t len){ reserve(len); _len=len; }
//template <typename T> void Array<T*>::_realloc(size_t nsize){ _reallocPOD(nsize); }
//template <typename T> void Array<T*>::_realloc(size_t nsize){ _reallocPOD(nsize); }

//template<typename T, size_t N>
//struct StackArray : Array<T> { StackArray(){ this->_ptr=arr; this->_len=N; this->_msize=N; this->_mode=this->STACK_ONLY; } private: T arr[N]; };

/*template<typename T, size_t N> //SArray<T> ≈ std::array<T>
struct SArray { T _ptr[N]; size_t size() const { return N; } }; //ConstArray? FixedArray? */

#ifdef NCPP_LIB_BUILD
//template struct Array<int>; //Принудительно сгенерировать код Array<int>
//template struct Array<char>;
#endif

template <typename T> void print(const Array<T>& arr){ print(arr.cout()); }
}
