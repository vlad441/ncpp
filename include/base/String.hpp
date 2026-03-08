namespace ncpp { struct String; struct CString; struct Buffer;
    template <typename T> struct Array;

    template <typename T, typename Derived>
    struct BaseString {
        enum Mode { HEAP, STACK, STACK_ONLY };

        BaseString();

        template <typename A, size_t N> Derived& stack(A (&arr)[N], bool ronly = false);
        template <typename A> Derived& stack(A* ptr, size_t len, bool ronly = false);
        
        char mode() const;
        T* data() const;
        T* begin() const;
        T* end() const;
        bool empty() const;
        size_t size() const;
        size_t capacity() const;
        size_t length() const;

        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        size_t find(const char* s, size_t pos = 0) const;
        size_t find(const char* s, size_t pos, size_t n) const;
        size_t find(char c, size_t pos = 0) const;

        size_t find_first_of(const char* chars, size_t pos = 0) const;
        size_t find_last_of(const char* chars, size_t pos = NPOS) const;
        size_t find_first_not_of(const char* chars, size_t pos = 0) const;
        size_t find_last_not_of(const char* chars, size_t pos = NPOS) const;

        size_t indexOf(const char* delim, size_t len, size_t start) const;
        size_t indexOf(const char* delim, size_t start = 0) const;
        size_t indexOf(char c, size_t start = 0) const;
        template <typename U, typename D> size_t indexOf(const BaseString<U, D>& s, size_t start = 0) const;

        bool includes(char c) const;
        bool includes(const char* c) const;

        size_t lastIndexOf(const char* delim, size_t len, size_t start = NPOS) const;
        size_t lastIndexOf(const char* delim) const;
        size_t lastIndexOf(char c) const;

        String slice(int start, int end = 0) const;
        String substr(int start, int end = 0) const;

        template <typename V> Array<V> _split(const char* delim, size_t len, int maxparts = -1) const;
        template <typename V> Array<V> _split(const char* delim) const;

        Array<String> split(const char* delim, size_t len, int maxparts = -1) const;
        Array<String> split(const char* delim) const;
        Array<String> split(char delim) const;
        template <typename U, typename D> Array<String> split(const BaseString<U, D>& s) const;

        template <typename V> Array<V> _splitTokens(int limit = -1, const char* chars = " \t\r\n\f\v") const;
        Array<String> splitTokens(int limit = -1, const char* chars = " \t\r\n\f\v") const;

        bool startsWith(const char* prefix, size_t len) const;
        bool startsWith(const char* prefix) const;
        template <typename U, typename D> bool startsWith(const BaseString<U, D>& prefix) const;

        bool endsWith(const char* suffix, size_t len) const;
        bool endsWith(const char* suffix) const;
        template <typename U, typename D> bool endsWith(const BaseString<U, D>& suffix) const;

        bool toBool();

        bool operator==(const char* c) const;
        template <typename U, typename D> bool operator==(const BaseString<U, D>& s) const;
        bool operator!=(const char* c) const;
        template <typename U, typename D> bool operator!=(const BaseString<U, D>& s) const;
        template <typename U, typename D> bool operator<(const BaseString<U, D>& s) const;
        template <typename U, typename D> bool operator>(const BaseString<U, D>& s) const;

    protected: T* _ptr; size_t _len, _msize; char _mode;
        template <typename V> V _slice(int start, int end = 0) const;
    };

    struct CString : BaseString<const char, CString> { //CString ≈ std::string_view
        CString();
        CString(const char* c, size_t len);
        CString(const char* c);
        CString(const String& s);
        template <typename T, typename D> CString(const BaseString<T, D>& s);
        CString(const char* first, const char* end);
        template <size_t N> CString(const char (&arr)[N]);

        const char* c_str() const;
        const char& operator[](size_t pos) const;
        char at(size_t pos) const;

        CString& operator=(const char* c);
        template <typename T, typename D> CString& operator=(const BaseString<T, D>& s);
        
        String operator+(const char* c) const;
        template <typename T, typename D> String operator+(const BaseString<T, D>& s) const;
    };

    #define _SSO_LEN 7
    struct String : BaseString<char, String> { //String ≈ std::string
		typedef char* Iter; typedef const char* CIter; typedef CIter ConstIter;
		typedef Iter iterator; typedef CIter const_iterator;
		String(size_t len=0);
        String(size_t len, char v);
        String(const char* ptr, size_t len);
        String(const char* cptr);
        String(const String& s);
		String& operator=(const String& s);
        template <typename T, typename D> String(const BaseString<T, D>& s);
        String(const char* begin, const char* end);
        template <size_t N> String(const char (&arr)[N]);
        ~String();

        const char* c_str() const;
        String& reserve(size_t sz);
        String& resize(size_t len);

        void push_back(char v);
        void pop_back();
        void clear();
        void shrink_to_fit();
        void shrink();

        template <size_t N> String& push(const char (&arr)[N]);
        String& push(const char* ptr, size_t len);
        String& push(const String& s);
        String& push(const CString& cs);
        String& push(char v);
        char pop();
        String& fill(char v);

        String replace(const CString& from, const CString& to, bool all = false) const;
        String replaceAll(const CString& from, const CString& to) const;
        String toLowerCase() const;
        String toUpperCase() const;
        String trim() const;

        char& operator[](size_t pos);
        const char& operator[](size_t pos) const;
        char at(size_t pos) const;

        String& operator=(const char* c);
        template <typename T, typename D> String& operator=(const BaseString<T, D>& s);
        template <size_t N> String& operator=(const char (&arr)[N]);

        String& operator+=(const char* c);
        String& operator+=(const String& s);
        String& operator+=(char c);
        template <typename T, typename D> String& operator+=(const BaseString<T, D>& s);

        String operator+(const char* c) const;
        String operator+(const String& s) const;
        String operator+(char c) const;
        template <typename T, typename D> String operator+(const BaseString<T, D>& s) const;

        String& operator<<(const char* c);
        String& operator<<(const String& s);
        String& operator<<(const Buffer& buff);
        template <typename T, typename D> String& operator<<(const BaseString<T, D>& s);
        String& operator<<(long long num);
        template <typename T> String& operator<<(const Array<T>& arr);

        friend void swap(String& a, String& b);
        friend void move(String& a, String& b);
        operator Array<char>() const;

        bool _isSSO();

    private:
        #if _SSO_LEN > 0
        char _sso[_SSO_LEN];
        #endif
        void _init(size_t len);
        void _alloc(size_t sz, bool copy = false);
        void _set(const char* ptr, size_t len);
        void _append(const char* ptr, size_t len);
        void _append(char c);
    };

    template <typename T, typename D> String operator+(const char* c, const BaseString<T, D>& s);
    template <typename T, typename D> void print(const BaseString<T, D>& s);
}