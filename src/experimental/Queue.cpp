namespace ncpp {
template <typename T, void* (*_allocFN)(size_t) = malloc, void (*_freeFN)(void*) = free>
struct Queue { Array<T, _allocFN, _freeFN> q;
	typedef T* Iter; typedef const T* CIter; typedef CIter ConstIter; 
	typedef Iter iterator; typedef CIter const_iterator;
	
	// Queue(size_t n=0, const T& val=T()) : Array{ assign(n, val); }
	// Queue(const Array<T>& arr) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ _copy(arr.data(), arr.size()); };
	// Queue(const T* begin, const T* end) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ assign(begin, end); }
	// template <size_t N>
	// Queue(const T (&arr)[N]) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ _copy(arr, N); } //T arr[];
	// #if __cplusplus >= 201103L
	// //Queue(const ncpp::initializer_list<T>& list) : _ptr(NULL), _len(0), _msize(0), _mode(HEAP){ _copy(list.begin(), list.size()); }
	// #endif
	// ~Queue(){ clear(); if(_mode==HEAP&&_ptr!=NULL) _freeFN(_ptr); }
};

template <typename T> void print(const Queue<T>& q){ print(q.cout()); }
}
