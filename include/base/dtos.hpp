namespace ncpp {
    // Проверки типов строк
    bool IsInt(const char* cstr, size_t len);
    bool IsFloat(const char* cstr, size_t len);
    bool IsHex(const char* cstr, size_t len);

    // Вспомогательные лимиты для корректной обработки отрицательных чисел
    template<typename T> struct num_limits { static const long long min = LLONG_MIN; };
    template<> struct num_limits<int> { static const int min = INT_MIN; };
    template<> struct num_limits<short> { static const short min = SHRT_MIN; };
    template<> struct num_limits<char> { static const char min = CHAR_MIN; };

    // Конвертация Number -> String (dtos)
    template<typename I> char dtos(char str[27], I num, char sep) noexcept;
    template<typename I> String dtos(I num, char sep = 0) noexcept;
    
    char isNaN(double d);
    String dtos(double d, char precision = -1) noexcept;
    String dtos(float num, char precision = -1) noexcept;

    // Конвертация String -> Number (stoin, stolln, stoulln, stodn, stofn)
    int stoin(const char* cstr, unsigned char len) noexcept;
    int stoin(const char* cstr) noexcept;
    int stoin(const String& s) noexcept;

    long long stolln(const char* cstr, unsigned char len) noexcept;
    long long stolln(const char* cstr) noexcept;
    long long stolln(const String& s) noexcept;

    unsigned long long stoulln(const char* cstr, unsigned char len) noexcept;
    unsigned long long stoulln(const char* cstr) noexcept;
    unsigned long long stoulln(const String& s) noexcept;

    double stodn(const char* cstr, size_t len) noexcept;
    double stodn(const char* cstr) noexcept;
    double stodn(const String& s) noexcept;

    float stofn(const char* cstr, size_t len) noexcept;
    float stofn(const char* cstr) noexcept;
    float stofn(const String& s) noexcept;

    void print(long long v);
}