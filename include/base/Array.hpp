namespace ncpp { struct String;
    template <typename T> //Array<T> ≈ std::vector<T>
    struct Array {
        enum Mode { HEAP, STACK, STACK_ONLY };
        typedef T* Iter;
        typedef const T* ConstIter;
        typedef Iter iterator;
        typedef ConstIter const_iterator;

        Array(size_t n = 0, const T& val = T());
        Array(const Array<T>& arr);
        Array(const T* begin, const T* end);
        template <size_t N> Array(const T (&arr)[N]);
        ~Array();

        template <typename A, size_t N> Array<T>& stack(A (&arr)[N], bool ronly = false);
        template <typename A> Array<T>& stack(A* ptr, size_t len, bool ronly = false);

        char mode() const;
        T* data() const;
        T* begin() const;
        T* end() const;
        bool empty() const;
        size_t size() const;
        size_t capacity() const;

        void reserve(size_t len);
        void resize(size_t len, const T& val = T());
        void assign(const T* begin, const T* end);
        void assign(size_t n, const T& val);

        void insert(Iter ipos, const T* dptr, size_t len);
        void insert(Iter ipos, const T* first, const void* last);
        void insert(Iter ipos, size_t n, const T& v);
        void insert(Iter ipos, const T& v);
        void insert(Iter ipos, const Array& other);

        Iter erase(Iter first, Iter last);
        Iter erase(Iter ipos);

        void push_back(const T& val);
        void pop_back();

        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        void clear();
        void shrink();
        void shrink_to_fit();

        size_t indexOf(const T& value, size_t start = 0) const;
        Array<T> slice(int start, int end = 0) const;

        void fill(const T& val);
        void splice(size_t pos, size_t count = 1);

        template <size_t N> Array<T>& push(const T (&arr)[N]);
        Array<T>& push(const T* ptr, size_t len);
        Array<T>& push(const T& val);
        T pop();

        void push_front(const T& v);
        void pop_front();

        Array<T>& concat(const Array<T>& arr2);
        static Array<T> concat(const Array<T>& arr1, const Array<T>& arr2);
        void reverse();
		
		template <typename U> T join(const U& delim) const;
        //T join(const T& delim) const;
        //T join(const char* delim) const;
        //T join() const;

        T& operator[](size_t pos);
        const T& operator[](size_t pos) const;
        const T& at(size_t pos) const;

        template <size_t N> Array<T>& operator=(const T (&arr)[N]);
        Array<T>& operator=(const Array& arr);

        String cout() const;

    protected: T* _ptr; size_t _len, _msize; char _mode;

        void _alloc(size_t len);
        void _copy(const T* begin, size_t len, size_t pos = 0);
        void _move_right(size_t pos, size_t roffset);
        void _move_left(size_t pos, size_t loffset);

        template <typename U> T _join(const U& delim) const;

        void _realloc(size_t nsize);
        void _reallocT(size_t nsize);
		void _reallocPOD(size_t nsize);
    };

    template <typename T>
    void print(const Array<T>& arr);
}