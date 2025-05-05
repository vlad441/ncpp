namespace ncpp{
	long long stolln(const std::string& str) noexcept; //atoll()?
	void print(const char *cptr, size_t len);
	void print(const char *cstr);
	void print(const std::string& str);
	std::string dtos(double num, int precision=-1) noexcept;
    template<typename T>
	std::string dtos(T num) noexcept;
	int stoin(const std::string& str) noexcept;
	double stodn(const std::string& str) noexcept;
	float stofn(const std::string& str) noexcept;
	long long GetTimestamp(char type='m');
    void Sleep(unsigned int msec);
	bool IsInt(const std::string& str);
	struct Err : std::exception { int code; virtual ~Err() noexcept;
		Err() : code(0), _err("Unspecified error");
		Err(const std::string& msg, int errcode=0) : code(errcode), _err(msg);
		Err(int errcode) : code(errcode), _err("Code: "+dtos(code));
		virtual const char* what() const throw(); };
	struct Buffer; struct String;
    template <typename T>
    struct Array : std::vector<T> {
		typename std::vector<T>::value_type value_type;
		Array() : std::vector<T>();
		~Array(); // Деструктор
		Array(size_t size) : std::vector<T>(size);
		Array(const T* ptr, size_t size) : std::vector<T>(ptr, ptr+size);
		Array(const std::vector<T>& vec) : std::vector<T>(vec);
		void push(T value);
		T pop();
		void unshift(T value);
		T shift();
		long indexOf(const T& value, int start=0) const;
		Array<T> slice(int start, int end=0) const;
		void splice(int index, int count=1);
		Array<T>& concat(const Array<T>& arr2);
		static Array<T> concat(const Array<T>& arr1, const Array<T>& arr2);
		void reverse();
		void minsize(size_t msize);
		T join(T& delim) const;
		T join(const std::string& delim) const;
		T join() const;
		template <typename U>
		U _join(const std::string& delim) const;
		std::string cout() const;
		std::string cout_str() const;
        Array<T>& operator+=(const Array<T>& arr2);
        Array<T> operator+(const Array<T>& arr2);
    };
    template <typename T>
    struct Array2D : Array<Array<T> >;
	std::ostream& operator<<(std::ostream& os, const Array<std::string>& arr);
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Array<T>& arr);
	struct String : std::string {
		String() : std::string();
        String(const std::string& s) : std::string(s);
        String(const char* cstr) : std::string(cstr);
        String(const char* ptr, size_t len) : std::string(ptr, len);
		int indexOf(const char* cptr, size_t len, size_t start) const;
        int indexOf(const char* cstr, size_t start = 0) const;
        int indexOf(const std::string& substr, size_t start = 0) const;
        int indexOf(char c, size_t start = 0) const;
		bool includes(const std::string& substr) const;
        char back() const;
        template <typename T>
		Array<T> _split(const char* delim, size_t len=0) const;
		template <typename T>
		Array<T> _split(const std::string& delim) const noexcept;
		Array<String> split(const char* delim, size_t len=0) const noexcept;
        Array<String> split(const std::string& delim) const noexcept;
		String slice(int start, int end=0) const;
		bool startsWith(const char* prefix, size_t len=0) const;
		bool startsWith(const String& prefix) const;
		bool endsWith(const char* suffix, size_t len=0) const;
		bool endsWith(const String& suffix) const;
		String replace(const std::string& from, const std::string& to) const;
		String replaceAll(const std::string& from, const std::string& to) const;
		struct RegExp; // not implemented
		String toLowerCase() const;
		String toUpperCase() const;
        static std::string toLowerCase(std::string str);
        static std::string toUpperCase(std::string str);
        String trim() const;
    };
	std::ostream& operator<<(std::ostream& os, const Array<String>& arr);
	struct Buffer : std::vector<unsigned char>{ //typedef std::vector<unsigned char> BfType;
        Buffer() : std::vector<unsigned char>(); ~Buffer();
		Buffer(size_t size, unsigned char value=0) : std::vector<unsigned char>(size, value); // Конструктор с параметром размера
		Buffer(const char* cstr); // Конструктор для c-строки
        Buffer(const void* ptr, size_t size); // Конструктор для массива байт с длинной.
		Buffer(const std::string& str, const char* type="str"); // Конструктор для константной строки.
		Buffer(std::vector<unsigned char> vec) : std::vector<unsigned char>(vec);
		Buffer(std::vector<unsigned char>::const_iterator first, std::vector<unsigned char>::const_iterator last) : std::vector<unsigned char>(first, last); // Итераторы
		void _init(const std::string& str, const char* type="str");
        void _fromHex(const char* hexstr, size_t len=0);
        void _fromIntString(const std::string& str);
		static Buffer from(const std::string& str, const char* type="str");
		static Buffer from(unsigned long long numb);
		static Buffer fromHex(const char* hexstr, size_t len=0);
		static Buffer fromHex(const std::string& hexstr);
        long indexOf(const char* delim, size_t len, size_t start) const;
		long indexOf(const char* delim, size_t start=0) const;
		long indexOf(const Buffer& delim, size_t start=0) const;
		void push(unsigned char value);
		Array<Buffer> split(const char* delim, size_t len=0) const;
		Array<Buffer> split(const Buffer& delim) const;
		template <typename T>
		Array<T> _split(const char* delim, size_t len=0) const;
		template <typename T>
		Array<T> _split(const std::string& delim) const;
		Buffer slice(int start, int end=0) const;
		bool startsWith(const char* prefix, size_t len=0) const;
		bool startsWith(const Buffer& prefix) const;
		bool endsWith(const char* suffix, size_t len=0) const;
		bool endsWith(const Buffer& suffix) const;
		void fill(unsigned char value);
		Buffer& concat(const Buffer& buff2);
		void reverse();
		void read(void* dst, size_t size, size_t offset=0);
		void write(const void* src, size_t size, size_t offset);
		void write(const char* cstr, size_t offset=0);
		void write(const Buffer& buff, size_t offset=0);
		void write(const std::string& str, size_t offset=0);
		template <typename T>
		T readAny(size_t offset=0);
		template <typename T>
		void writeAny(const T& data, size_t offset=0);
		Array<char> BitsRead(size_t offset=0) const;
		void BitsWrite(const std::vector<char>& bits, size_t offset=0);
		template <typename T>
		T readT_BE(size_t offset=0, int length=sizeof(T)) const;
		template <typename T>
		T readT_LE(size_t offset=0, int length=sizeof(T)) const { if(offset+length > this->size()){ throw std::out_of_range("readT_LE: out of range"); } 
			T value=T(); memcpy(&value, &(*this)[0]+offset, length); return value; }
		template <typename T>
		void writeT_BE(T value, size_t offset=0, int length=sizeof(T)){ writeT_LE<T>(value, offset); 
			std::reverse(&(*this)[0]+offset, &(*this)[0]+offset+length); }
		template <typename T>
		void writeT_LE(T value, size_t offset=0, int length=sizeof(T)){ if(this->size() < offset+length){ resize(offset+length); }
			memcpy(&(*this)[0]+offset, &value, length); }
		int readInt16BE(size_t offset=0) const;
		int readInt16LE(size_t offset=0) const;
		unsigned int readUInt16BE(size_t offset=0) const;
		unsigned int readUInt16LE(size_t offset=0) const;
		void writeInt16BE(int value, size_t offset=0);
		void writeInt16LE(int value, size_t offset=0);
		void writeUInt16BE(unsigned int value, size_t offset=0);
		void writeUInt16LE(unsigned int value, size_t offset=0);
		int readInt32BE(size_t offset=0) const;
		int readInt32LE(size_t offset=0) const;
		unsigned int readUInt32BE(size_t offset=0) const;
		unsigned int readUInt32LE(size_t offset=0) const;
		void writeInt32BE(int value, size_t offset=0);
		void writeInt32LE(int value, size_t offset=0);
		void writeUInt32BE(unsigned int value, size_t offset=0);
		void writeUInt32LE(unsigned int value, size_t offset=0);
		long long readInt64BE(size_t offset=0) const;
		long long readInt64LE(size_t offset=0) const;
		unsigned long long readUInt64BE(size_t offset=0) const;
		unsigned long long readUInt64LE(size_t offset=0) const;
		void writeInt64BE(long long value, size_t offset=0);
		void writeInt64LE(long long value, size_t offset=0);
		void writeUInt64BE(unsigned long long value, size_t offset=0);
		void writeUInt64LE(unsigned long long value, size_t offset=0);
		double readDoubleBE(size_t offset=0) const;
		double readDoubleLE(size_t offset=0) const;
		void writeDoubleBE(double value, size_t offset=0);
		void writeDoubleLE(double value, size_t offset=0);
		static Array<char> BitsRead(const unsigned char* ptr);
		static void BitsWrite(unsigned char* ptr, const std::vector<char>& bits);
		static Array<char> BitsRead(const void* ptr);
		static void BitsWrite(void* ptr, const std::vector<char>& bits);
		static int BitsToInt(const std::vector<char>& bits);
		static Array<char> IntToBits(int numb, size_t len=sizeof(int)*8){ Array<char> bits(len);	
			for(int i = len - 1; i >= 0; --i); return bits; }
		static Buffer concat(const Array<Buffer>& buffs);
		static Buffer concat(const Buffer& buff1, const Buffer& buff2);
		static Buffer randBytes(int length); 
		static const std::string base64_chars;
		static String base64_encode(const Buffer& buffer);
		static Buffer base64_decode(const std::string& encoded_string);
		static void removeLeadingZeros(Buffer& buff);
		struct Math; // == арифметика (implemented in ncpp-bigint.cpp)
		unsigned int toInt() const noexcept;
		unsigned long long toInt64() const noexcept;
        String toString() const;
		String toString(std::string type) const;
        String toHexString() const;
		String toIntString() const;
		std::string cout() const;
		Buffer& operator+=(const Buffer& buff2);
		Buffer operator+(const Buffer& buff2) const;
		static unsigned char htoc(const char hex[2]);
		static void ctoh(unsigned char value, char hex[2]);
		static unsigned int htoi(const std::string& hexstr);
		static unsigned long long htoll(const std::string& hexstr);
		static std::string itoh(unsigned int value);
		static std::string itoh(unsigned long long value);
	};
	const std::string Buffer::base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::ostream& operator<<(std::ostream& os, const Buffer& buffer);
	template <>
	Buffer Array<Buffer>::join(Buffer& dbuff) const;
	template <> 
	Buffer Array<Buffer>::join(const std::string& delimiter) const;
	template <>
	Buffer Array<Buffer>::join() const;
	unsigned long long xorshift128plus();
	double random();
	int randInt(int min, int max);
	Buffer Buffer::randBytes(int length);
	String randStr(size_t length, String letters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
	template <typename K, typename V>
	struct HashMap : std::unordered_map<K, V> { typedef std::unordered_map<K, V> MapType;
		typedef typename MapType::iterator iterator; typedef typename MapType::const_iterator const_iterator;
		Array<K> keys() const;
		bool has(const K& key) const;
		static HashMap<K, V>& assign(HashMap<K, V>& target, const HashMap<K, V>& map2);
		static HashMap<K, V>& assign(HashMap<K, V>& target, const Array<HashMap<K, V> >& maps);
		std::string cout() const;
		std::string cout2() const;
		String toJSON() const;
		String toString() const; 
		std::string type() const; 
	};
	template <typename K, typename V>
	std::ostream& operator<<(std::ostream& os, const HashMap<K, V>& map);
	template <typename K>
	struct HashSet : std::unordered_set<K> { typedef std::unordered_set<K> MapType;
		typedef typename MapType::iterator iterator; typedef typename MapType::const_iterator const_iterator;
		Array<K> keys() const;
		bool has(const K& key) const;
		inline void erase_ptr(K key);
		void clear_ptr();
		std::string cout() const;
		String toString() const;
		std::string type() const; };
	template <typename K>
	std::ostream& operator<<(std::ostream& os, const HashSet<K>& set);
	struct StringMap : HashMap<std::string, String> {
		std::string cout() const;
		String toJSON() const;
		String toString() const;
	};
	typedef HashMap<std::string, double> DoubleMap;
	struct Date { long long timestamp; //char utc;
		Date() : timestamp(Date::now()); ~Date(); 
		Date(long long msecs, char type='m') : timestamp(msecs);
		Date(const String& dateStr) : timestamp(0);
		static long long now();
		int getYear() const;
		int getMonth() const;
		int getDate() const;
		int getDay() const;
		int getDayWeek() const;
		std::string toDirectDate(char sp='.') const;
		std::string toReverseDate(char sp='.') const;
		std::string toString(char sp='.') const;
		std::string toString(const String& mode, char sp='.') const;
		std::string toISOString() const;
		Date& operator+=(const Date& other);
		long long operator+(const Date& other) const;
		Date& operator+=(long long value);
		long long operator+(long long value) const;
		Date& operator-=(const Date& other);
		long long operator-(const Date& other) const;
		Date& operator-=(long long value);
		long long operator-(long long value) const;
	};
}
namespace std { // Специализация struct hash<String> для std C++11
	template <>
	struct hash<ncpp::String>; }
