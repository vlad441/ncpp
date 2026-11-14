namespace ncpp {
//head — куда писать следующий байт.
//tail — откуда читать следующий байт.
//Заполнен, когда (head + 1) % N == tail (классика — жертвуем одним слотом).
//Пуст, когда head == tail.
//Размер = (head - tail + N) % N.
	
struct RingBuffer { private: Buffer bf; size_t _tail, _head; public:
	typedef unsigned char* Iter; typedef const unsigned char* CIter; typedef CIter ConstIter;
	typedef Iter iterator; typedef CIter const_iterator;
	
	size_t capacity() const { return bf.size(); } 
    size_t size()     const { return (_head - _tail + capacity()) % capacity(); }
    bool   empty()    const { return _head == _tail; }
    //bool   full()     const { return (_head + 1) % capacity() == _tail; }
    //size_t free()     const { return capacity()-1-size(); }
	
	const unsigned char& operator[](size_t i) const { return bf[(_tail + i) % capacity()]; }
};

}
