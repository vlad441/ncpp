namespace ncpp {
#ifdef _WIN32 // Работа с широкими строками (Windows)
	#include <wchar.h> // for wcslen()
	struct _WString : Array<wchar_t> {
		_WString(const wchar_t* wptr, size_t len);
		_WString(const wchar_t* wptr);
		_WString(const void* begin, const void* end);
		_WString(size_t len=0);
		_WString(size_t len, wchar_t v);
		const wchar_t* c_str() const; };

	long long _FtToUnixTime(FILETIME& ft, char type='s');
	String _toUTF8(const wchar_t* wptr, size_t len, UINT codep=CP_UTF8);
	String _toUTF8(const _WString& wstr);
	_WString _toWStr(const char* cptr, size_t len, UINT fromp=CP_UTF8);
	_WString _toWStr(const char* cstr);
	_WString _toWStr(const CString& utf8str);
	String _OEMtoUTF8(const char* oemptr, size_t len=0);
#endif
    // Время и Рандом
    long long GetTimestamp(char type = 's');
    double random();
    int randInt(int min, int max);
    String randStr(size_t length, String letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");

    //Ввод-вывод
    String readline();

    // Ошибки
    struct Err { int code; virtual ~Err() noexcept;
        Err(); Err(CString msg, int ecode = 0); Err(int ecode);
        virtual const char* what() const throw();
    private: String _err; };

    // Системные функции
#ifdef _WIN32
    void usleep(unsigned int usec);
#else
    void Sleep(unsigned int msec);
#endif

    // Бенчмарки
    namespace performance { void start(); double now(); }
    namespace perf = performance;

    // Результат (Result)
    template <typename T, typename E = String>
    struct Result { T data; E reason; bool ok;
        Result(const T& data = T());
        Result(bool ok, const T& d, const E& r = E());

		#if __cplusplus >= 201103L
        explicit operator bool() const;
        explicit operator const T&() const;
		#else
        operator bool() const;
        operator const T&() const;
		#endif

        static Result Ok(const T& data);
        static Result Success(const T& data);
        static Result Err(const E& reason);
		static Result Resolve(const T& data);
        static Result Reject(const E& reason);
    };
}