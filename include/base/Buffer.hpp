namespace ncpp {
    template <typename T> struct is_pointer { static const bool V = false; };
    template <typename T> struct is_pointer<T*> { static const bool V = true; };

    struct Buffer : BaseString<unsigned char, Buffer> {
		typedef unsigned char* Iter; typedef const unsigned char* CIter; typedef CIter ConstIter;
		typedef Iter iterator; typedef CIter const_iterator;
        Buffer(size_t len=0); ~Buffer();
        Buffer(size_t len, unsigned char v);
        Buffer(const void* ptr, size_t len);
        Buffer(const char* cptr);
        Buffer(const Buffer& b);
        Buffer(const void* begin, const void* end);
        template <size_t N> Buffer(const unsigned char (&arr)[N]);
        template <typename T, typename D> Buffer(const BaseString<T, D>& s);

        // Операторы присваивания
        Buffer& operator=(const Buffer& b);
        Buffer& operator=(const char* c);
        template <typename T, typename D> Buffer& operator=(const BaseString<T, D>& s);
        template <size_t N> Buffer& operator=(const unsigned char (&arr)[N]);

        // Методы инициализации (from...)
        Buffer& _from(const CString& str, const CString& type = "str");
        Buffer& _fromHex(const char* hexstr, size_t len);
        Buffer& _fromIntString(const CString& cs);

        static Buffer from(const CString& cs);
        static Buffer from(const CString& str, const CString& type);
        static Buffer fromHex(const char* hex, size_t len);
        static Buffer fromHex(const CString& hex);
        static Buffer fromBase64(const CString& cs);
        static Buffer fromInt(unsigned long long numb);
        static Buffer fromInt(const CString& cs);

        // Управление памятью
        void assign(const void* begin, const void* end);
        Buffer& reserve(size_t sz);
        Buffer& resize(size_t len);
        void clear();
        void shrink_to_fit();
        void shrink();

        // Модификация данных (Insert / Erase)
        void insert(Iter ipos, const void* dptr, size_t len);
        void insert(Iter ipos, const void* first, const void* last);
        void insert(Iter ipos, size_t n, unsigned char v);
        void insert(Iter ipos, unsigned char v);
        void insert(Iter ipos, const Buffer& other);
        void erase(Iter first, Iter last);
        void erase(Iter ipos);

        // Работа со стеком/очередью
        void push_back(unsigned char v);
        void pop_back();
        void push_front(unsigned char v);
        void pop_front();

        // Добавление данных (Push / Pop)
        template <size_t N> Buffer& push(const unsigned char (&arr)[N]);
        Buffer& push(const void* ptr, size_t len);
        Buffer& push(const Buffer& buff);
        Buffer& push(const String& s);
        Buffer& push(const CString& cs);
        Buffer& push(unsigned char v);
        unsigned char pop();

        // Алгоритмы
        Buffer slice(int start, int end = 0) const;
        Array<Buffer> split(const char* delim, size_t len, int maxparts = -1) const;
        Array<Buffer> split(const char* delim) const;
        Array<Buffer> split(char delim) const;
        template <typename U, typename D> Array<Buffer> split(const BaseString<U, D>& s) const;
        Buffer& fill(unsigned char v);
        void reverse();

        // Чтение и запись (Raw / Any)
        void read(void* dst, size_t size, size_t offset = 0);
        void write(const void* src, size_t size, size_t offset);
        void write(const char* cstr, size_t offset = 0);
        void write(const Buffer& buff, size_t offset = 0);
        template <typename T, typename D> void write(const BaseString<T, D>& s, size_t offset = 0);
        
        template <typename T> T readAny(size_t offset = 0);
        template <typename T> void writeAny(const T& data, size_t offset = 0);

        // Битовые операции
        Buffer BitsRead(size_t offset = 0) const;
        void BitsWrite(const Buffer& bits, size_t offset = 0);
        static Buffer BitsRead(const unsigned char* ptr);
        static void BitsWrite(unsigned char* ptr, const Buffer& bits);
        static Buffer BitsRead(const void* ptr);
        static void BitsWrite(void* ptr, const Buffer& bits);
        static int BitsToInt(const unsigned char* bits, size_t len);
        static int BitsToInt(const Buffer& bits);
        static void IntToBits(unsigned char* dst, int numb, size_t bitlen);
        static Buffer IntToBits(int numb, size_t bitlen = sizeof(int) * 8);

        // Endian-specific (BE/LE)
        template <typename T> T readT_BE(size_t offset = 0, int length = sizeof(T)) const;
        template <typename T> T readT_LE(size_t offset = 0, int length = sizeof(T)) const;
        template <typename T> void writeT_BE(T value, size_t offset = 0, int length = sizeof(T));
        template <typename T> void writeT_LE(T value, size_t offset = 0, int length = sizeof(T));

        // Конкретные числовые типы (BE/LE)
        int readInt16BE(size_t offset = 0) const;
        int readInt16LE(size_t offset = 0) const;
        unsigned int readUInt16BE(size_t offset = 0) const;
        unsigned int readUInt16LE(size_t offset = 0) const;
        void writeInt16BE(int value, size_t offset = 0);
        void writeInt16LE(int value, size_t offset = 0);
        void writeUInt16BE(unsigned int value, size_t offset = 0);
        void writeUInt16LE(unsigned int value, size_t offset = 0);
        int32_t readInt32BE(size_t offset = 0) const;
        int32_t readInt32LE(size_t offset = 0) const;
        uint32_t readUInt32BE(size_t offset = 0) const;
        uint32_t readUInt32LE(size_t offset = 0) const;
        void writeInt32BE(int value, size_t offset = 0);
        void writeInt32LE(int value, size_t offset = 0);
        void writeUInt32BE(unsigned int value, size_t offset = 0);
        void writeUInt32LE(unsigned int value, size_t offset = 0);
        long long readInt64BE(size_t offset = 0) const;
        long long readInt64LE(size_t offset = 0) const;
        unsigned long long readUInt64BE(size_t offset = 0) const;
        unsigned long long readUInt64LE(size_t offset = 0) const;
        void writeInt64BE(long long value, size_t offset = 0);
        void writeInt64LE(long long value, size_t offset = 0);
        void writeUInt64BE(unsigned long long value, size_t offset = 0);
        void writeUInt64LE(unsigned long long value, size_t offset = 0);
        double readDoubleBE(size_t offset = 0) const;
        double readDoubleLE(size_t offset = 0) const;
        void writeDoubleBE(double value, size_t offset = 0);
        void writeDoubleLE(double value, size_t offset = 0);

        // Статические методы утилит
        static Buffer concat(const Array<Buffer>& buffs);
        static Buffer concat(const Buffer& buff1, const Buffer& buff2);
        static Buffer randBytes(int length);
        static const CString base64_chars;
        static String base64_encode(unsigned char const* bptr, size_t len);
        static Buffer base64_decode(const CString& encoded_string);
        
        // Математика над Buffer (Арифметика произвольной точности)
        static void increment(Buffer& buff, unsigned int value = 1, bool insert = false);
        static void decrement(Buffer& buff, unsigned int value = 1, bool erase = false);
        static void multiply_single(Buffer& buff, unsigned int value, bool insert = true);
        static unsigned int divide_single(Buffer& buff, unsigned int value, bool erase = true);
        static void removeLeadingZeros(Buffer& buff);
        static void removeLastZeros(Buffer& buff);
        struct Math; // Forward declaration

        // Конвертация
        unsigned int toInt() const noexcept;
        unsigned long long toInt64() const noexcept;
        String toString() const;
        String toString(const CString& type) const;
        String toHex() const;
        String toIntString() const;
        String cout(size_t osize = 50) const;

        // Hex утилиты
        static unsigned char htoc(const char hex[2]);
        static void ctoh(char hex[2], unsigned char value);
        static unsigned long long htoll(const char* hex, char len);
        static unsigned long long htoll(const char* hex);
        static unsigned long long htoll(const String& str);
        static void lltoh(char hex[16], unsigned long long value);
        static String lltoh(unsigned long long value);

        // Доступ по индексу
        unsigned char& operator[](size_t pos);
        const unsigned char& operator[](size_t pos) const;
        unsigned char at(size_t pos) const;

        // Сравнение и сложение
        bool operator==(const Buffer& bf) const;
        Buffer& operator+=(const char* c);
        Buffer& operator+=(const Buffer& b);
        Buffer& operator+=(unsigned char c);
        template <typename T, typename D> Buffer& operator+=(const BaseString<T, D>& s);
        Buffer operator+(const char* c) const;
        Buffer operator+(const Buffer& b) const;
        Buffer operator+(unsigned char c) const;
        template <typename T, typename D> Buffer operator+(const BaseString<T, D>& s) const;
        Buffer& operator<<(const Buffer& b);

        // Swap / Move
        friend void swap(Buffer& a, Buffer& b);
        friend void move(Buffer& a, Buffer& b);

        // Cast
        operator Array<char>() const;

    private:
        #if _BF_SSO_LEN > 0
        unsigned char _sso[_BF_SSO_LEN];
        #endif
        void _init(size_t len);
        void _alloc(size_t len, bool copy = false);
        void _set(const void* ptr, size_t len);
        static bool is_base64(unsigned char c);
    };

    // Внешние функции и специализации шаблонов
    void print(const Buffer& buff);
    void print(const void* ptr);
    void print(const volatile void* ptr);
}