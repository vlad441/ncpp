#if __cplusplus >= 201103L
#ifdef NCPP_USESTL
#include <initializer_list>
#else
namespace std {
    // Мини-заглушка для компилятора
    template<class T> class initializer_list { const T* _ptr; size_t _len;
        initializer_list(const T* p, size_t l) : _ptr(p), _len(l){}
    public: 
        const T* begin() const { return _ptr; }
        const T* end() const { return _ptr+_len; }
		size_t size() const { return _len; }
    };
}
#endif
namespace ncpp { template<class T> using initializer_list = std::initializer_list<T>; }
#endif