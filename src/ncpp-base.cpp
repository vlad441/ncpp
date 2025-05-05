#if __cplusplus < 201103L // старее чем C++11 (C++98, C++03)
namespace std{ int stoi(const std::string& str){ return ::atoi(str.c_str()); }
	double stod(const std::string& str){ return ::atof(str.c_str()); }
	float stof(const std::string& str){ return ::atof(str.c_str()); }
	long long stoll(const std::string& str){ char* end; long long result = strtoll(str.c_str(), &end, 10);
    if(end == str.c_str()){ throw std::invalid_argument("stoll: parameter is not a number"); } return result; }
	
	template <typename T>
	struct is_pointer { static const bool value = false; };
	template <typename T> // Частичная специализация для указателей
	struct is_pointer<T*> { static const bool value = true; }; }
#endif
namespace ncpp{
	#define MAX_SAFE_DOUBLE 9007199254740991
	#define D_PRECISION 17 //double - 17; long double - 21
	long long stolln(const std::string& str) noexcept { try{ return std::stoll(str); }catch(const std::exception& err){ return 0; } } //atoll()?
#ifdef _WIN32
	void print(const char *cstr); void print(const std::string& str);
	template<typename T>
	std::string dtos(T num) noexcept;
	long long _FtToUnixTime(FILETIME& ft, char type='s'){ ULARGE_INTEGER t; t.LowPart = ft.dwLowDateTime; t.HighPart = ft.dwHighDateTime;
		if(type=='u'||type=='m'){ return ((t.QuadPart - 116444736000000000ULL)/(type=='u'?10:10000)); }
		else{ return ((t.QuadPart - 116444736000000000ULL)/10000000LL); } }
	std::string _toUTF8(const wchar_t* wptr, int len, UINT codep=CP_UTF8){ int utf8Len = WideCharToMultiByte(codep, 0, wptr, len, NULL, 0, NULL, NULL)-1;
		if (utf8Len == 0){ print("Error in _toUTF8: "); print(dtos(GetLastError())); print("\n"); return ""; }
		std::string utf8Str(utf8Len, '\0'); WideCharToMultiByte(codep, 0, wptr, len, &utf8Str[0], utf8Len, NULL, NULL); return utf8Str; }
	std::string _toUTF8(const std::wstring& wstr){ return _toUTF8(wstr.c_str(), wstr.size()); }
	std::wstring _toWStr(const char* cptr, int len, UINT fromp=CP_UTF8){ int wideLen = MultiByteToWideChar(fromp, 0, cptr, len, NULL, 0);
		if (wideLen == 0){ print("Error in _toWStr: "); print(dtos(GetLastError())); print("\n"); return L""; }
		std::wstring wStr(wideLen, L'\0'); MultiByteToWideChar(fromp, 0, cptr, len, &wStr[0], wideLen); return wStr; }
	std::wstring _toWStr(const std::string& utf8str){ return _toWStr(utf8str.c_str(), utf8str.size()); }
	std::string _OEMtoUTF8(const char* oemptr, int len=0){ if(len<=0){ len=strlen(oemptr); } return _toUTF8(_toWStr(oemptr, len, CP_OEMCP)); }
	void print(const char *cptr, size_t len){ WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cptr, (DWORD)len, NULL, NULL); }
#else
	void print(const char *cptr, size_t len){ write(STDOUT_FILENO, cptr, len); }
#endif
	void print(const char *cstr){ print(cstr, strlen(cstr)); }
	void print(const std::string& str){ print(str.c_str(), str.size()); }
#if __cplusplus >= 201103L // C++11 или новее
	std::string dtos(double num, int precision=-1) noexcept { if(precision==0){ return std::to_string((long long)num); }
		else if(precision<=-2){ return std::to_string(num); }else{ std::stringstream ss; if(precision<0){ ss << std::setprecision(D_PRECISION); }
			else{ ss << std::fixed << std::setprecision(precision); } ss << num; return ss.str(); } }
    template<typename T>
	std::string dtos(T num) noexcept { return std::to_string(num); }
	int stoin(const std::string& str) noexcept { try{ return std::stoi(str); }catch(...){ return 0; } }
	double stodn(const std::string& str) noexcept { try{ return std::stod(str); }catch(...){ return 0; } }
	float stofn(const std::string& str) noexcept { try{ return std::stof(str); }catch(...){ return 0; } }
	
	long long GetTimestamp(char type='m'){ auto now = std::chrono::system_clock::now(); auto duration = now.time_since_epoch();
		if(type=='u'){ return std::chrono::duration_cast<std::chrono::microseconds>(duration).count(); }
		else if(type=='m'){ return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(); }
		else{ return std::chrono::duration_cast<std::chrono::seconds>(duration).count(); } }
#else
	std::string dtos(double num, int precision=-1) noexcept { std::stringstream ss; if(precision==0){ ss << (long long)num; return ss.str(); }
		if(precision<0){ ss << std::setprecision(D_PRECISION); }else{ ss << std::fixed << std::setprecision(precision); } ss << num; return ss.str(); }
	template<typename T>
	std::string dtos(T num) noexcept { std::stringstream ss; ss << num; return ss.str(); }
	int stoin(const std::string& str) noexcept { return ::atoi(str.c_str()); }
	double stodn(const std::string& str) noexcept { return ::atof(str.c_str()); }
	float stofn(const std::string& str) noexcept { return ::atof(str.c_str()); }
		
	long long GetTimestamp(char type='m'){ long long timestamp = 0; // type: s - secs, m - msecs, u - usecs
	#ifdef _WIN32
		FILETIME ft; GetSystemTimeAsFileTime(&ft); return _FtToUnixTime(ft, type);
	#else
		struct timeval tv; gettimeofday(&tv, NULL); if(type=='u'){ timestamp = static_cast<long long>(tv.tv_sec) * 1000000 + tv.tv_usec; }
		else if(type=='m'){ timestamp = static_cast<long long>(tv.tv_sec)*1000 + tv.tv_usec/1000; }else{ timestamp = (long long)tv.tv_sec; }
	#endif
		return timestamp; }
#endif
#ifdef _WIN32
	//void Sleep(unsigned int msec){ ::Sleep(msec); }
	void usleep(unsigned int usec){ if(usec >= 1000){ Sleep(usec/1000); }
		if((usec=usec%1000)>0){ LARGE_INTEGER freq, start, end; QueryPerformanceFrequency(&freq); 
			QueryPerformanceCounter(&start); double target = (double)usec / 1000000.0; double elapsed = 0.0;
			while (elapsed < target){ QueryPerformanceCounter(&end); elapsed = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart; } } }
#else
    void Sleep(unsigned int msec){ if(msec<1000){ ::usleep(msec*1000); }
		else{ ::sleep(msec/1000); if((msec=msec%1000)>0) ::usleep(msec*1000); } }
#endif
	bool IsInt(const std::string& str){ int symbols = 0; int digits = 0;
		for(size_t i = 0; i < str.size(); i++){ symbols++; if(::isdigit(str[i])){ digits++; } }
		if(str.size()>1&&str[0]=='-'){ digits++; } return symbols == digits; }
		
	struct Err : std::exception { int code; virtual ~Err() noexcept {}
		Err() : code(0), _err("Unspecified error"){}
		Err(const std::string& msg, int errcode=0) : code(errcode), _err(msg){}
		Err(int errcode) : code(errcode), _err("Code: "+dtos(code)){}
		virtual const char* what() const throw(){ return _err.c_str(); }
		private: std::string _err; };
	
	struct Buffer; struct String;
    template <typename T>
    struct Array : std::vector<T> {
		typename std::vector<T>::value_type value_type;
		Array() : std::vector<T>(){}
		~Array(){} // Деструктор
		Array(size_t size) : std::vector<T>(size){}
		Array(const T* ptr, size_t size) : std::vector<T>(ptr, ptr+size){}
		Array(const std::vector<T>& vec) : std::vector<T>(vec){}
		
		void push(T value){ this->push_back(value); }
		T pop(){ if(!this->empty()){ T val=this->back(); this->erase(this->end()-1); return val; }else{ return T(); } }
		void unshift(T value){ this->push_front(value); }
		T shift(){ if(!this->empty()){ T val=(*this)[0]; this->erase(this->begin()); return val; }else{ return T(); } }
		
		long indexOf(const T& value, int start=0) const { for(size_t i=(size_t)start; i < this->size(); ++i){ if((*this)[i] == value){ return i; } } return -1; }
		Array<T> slice(int start, int end=0) const { size_t length = this->size();
			if(start < 0){ start = length+start; }
			if(end <= 0){ end = length+end; } if(start < 0){ start = 0;}  
			if((size_t)end > length){ end = length; } if (start > end) start = end;
			Array<T> result; result.assign(this->begin() + start, this->begin() + end); return result; }
		void splice(int index, int count=1){ if(this->empty()) return; index = std::max(0, std::min(index, (int)this->size()-1)); 
			count = std::max(0, std::min(count, (int)this->size()-index)); this->erase(this->begin()+index, this->begin()+index+count); }
		Array<T>& concat(const Array<T>& arr2){ this->insert(this->end(), arr2.begin(), arr2.end()); return *this; }
		static Array<T> concat(const Array<T>& arr1, const Array<T>& arr2){ Array<T> ret = arr1; ret.concat(arr2); return ret; }
		void reverse(){ std::reverse(this->begin(),this->end()); }
		void minsize(size_t msize){ if(this->size() < msize){ this->resize(msize); } }
		T join(T& delim) const { return join_str<T>(delim); }
		T join(const std::string& delim) const { return join_str<T>(delim); }
		T join() const { T delim = T(); return join_str<T>(delim); }
		template <typename U>
		U _join(const std::string& delim) const { return join_str<U>(delim); }
			
		std::string cout() const { std::stringstream oss; oss << "["; if(this->size()>0){ oss << (*this)[0]; }
			for(size_t i=1;i<this->size();i++){ oss << ", " << (*this)[i]; } oss << "]"; return oss.str(); }
		std::string cout_str() const { std::stringstream oss; oss << "["; if(this->size()>0){ oss << "\"" << (*this)[0]; }
			for(size_t i=1;i<this->size();i++){ oss << "\", \"" << (*this)[i]; } if(this->size()>0){ oss << "\""; } oss << "]"; return oss.str(); }
		// Перегрузки операторов
        Array<T>& operator+=(const Array<T>& arr2){ this->concat(arr2); return *this; }
        Array<T> operator+(const Array<T>& arr2){ Array<T> result = *this; result.concat(arr2); return result; }
		//T& operator[](size_t index){ if(index >= this->size()){ this->resize(index + 1); } return std::vector<T>::operator[](index); }
		//const T& operator[](size_t index) const { if (index >= this->size()){ const_cast<Array<T>*>(this)->resize(index+1); } return std::vector<T>::operator[](index); }
		private:
			T join_buff(T& delimiter) const { if(this->empty()) return T(); T result = (*this)[0];
			for(size_t i = 1; i < this->size(); ++i){ result += delimiter; result += (*this)[i]; } return result; }
			template <typename U>
			U join_str(const std::string& delimiter) const { std::stringstream oss; if(this->size()>0){ oss << (*this)[0]; }
			for(size_t i=1;i<this->size();i++){ oss << delimiter << (*this)[i]; } return oss.str(); }
    };
    template <typename T>
    struct Array2D : Array<Array<T> > {};
	// Перегрузки операторов потока вывода для Array
	std::ostream& operator<<(std::ostream& os, const Array<std::string>& arr){ os << arr.cout_str(); return os; }
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Array<T>& arr){ os << arr.cout(); return os; }
	
	struct String : std::string {
		String() : std::string(){}
        String(const std::string& s) : std::string(s){}
        String(const char* cstr) : std::string(cstr){}
        String(const char* ptr, size_t len) : std::string(ptr, len){}

		int indexOf(const char* cptr, size_t len, size_t start) const {
            size_t pos = this->find(cptr, start, len); return (pos != std::string::npos) ? (int)pos : -1; }
        int indexOf(const char* cstr, size_t start = 0) const {
            size_t pos = this->find(cstr, start); return (pos != std::string::npos) ? (int)pos : -1; }
        int indexOf(const std::string& substr, size_t start = 0) const {
            size_t pos = this->find(substr, start); return (pos != std::string::npos) ? (int)pos : -1; }
        int indexOf(char c, size_t start = 0) const { size_t pos = this->find(c, start); return (pos != std::string::npos) ? static_cast<int>(pos) : -1; }
		bool includes(const std::string& substr) const { return indexOf(substr)>=0?true:false; }
        char back() const { return this->at(size()-1); }
	
        template <typename T>
		Array<T> _split(const char* delim, size_t len=0) const { if(len<=0){ len=std::strlen(delim); } Array<T> result;
			size_t start = 0; int end = this->indexOf(delim, len, start); if(end<0){ result.push_back(*this); return result; }
			while (end != -1){ result.push_back(this->substr(start, end - start)); start = end+len;
				end = this->indexOf(delim, len, start); } result.push_back(this->substr(start)); return result; }
		template <typename T>
		Array<T> _split(const std::string& delim) const noexcept { return _split<T>(delim.c_str(), delim.size()); }
		Array<String> split(const char* delim, size_t len=0) const noexcept { return _split<String>(delim, len); }
        Array<String> split(const std::string& delim) const noexcept { return _split<String>(delim.c_str(), delim.size()); }
        //Array<String> split(int count) const noexcept { return _split<String>(delim); }
		String slice(int start, int end=0) const { size_t length = this->size();
			if(start < 0){ start = length + start; } 
			if(end<=0){ end = length+end; } if(start < 0){ start = 0; }
			if((size_t)end > length){ end = length; } if(start > end) start = end;
			return String(this->substr(start, end - start)); }
		bool startsWith(const char* prefix, size_t len=0) const { if(len<=0){ len=std::strlen(prefix); } if(len==0||len > this->size()) return false;
			for(size_t i = 0; i < len; ++i){ if((*this)[i] != prefix[i]){ return false; } } return true; }
		bool startsWith(const String& prefix) const { return startsWith((prefix.size()>0)?(char*)&prefix[0]:NULL, prefix.size()); }
		bool endsWith(const char* suffix, size_t len=0) const { if(len<=0){ len=std::strlen(suffix); } size_t bsize=this->size(); if(len==0||len > bsize) return false;
			for(size_t i = 0; i < len; ++i){ if((*this)[bsize-len+i] != suffix[i]){ return false; } } return true; }
		bool endsWith(const String& suffix) const { return endsWith((suffix.size()>0)?(char*)&suffix[0]:NULL, suffix.size()); }
		String replace(const std::string& from, const std::string& to) const {
            std::string result=*this; size_t start_pos = 0; if(from.empty()){ return result; } 
			if((start_pos = result.find(from)) != std::string::npos){ result.replace(start_pos, from.length(), to); } return result; }
		String replaceAll(const std::string& from, const std::string& to) const {
            std::string result=*this; size_t start_pos = 0; if(from.empty()){ return result; }
            while((start_pos = result.find(from, start_pos)) != std::string::npos){
				result.replace(start_pos, from.length(), to); start_pos += to.length(); } return result; }
		struct RegExp; // not implemented
		String toLowerCase() const { String result(*this); for(size_t i = 0; i < result.size(); ++i){ 
			result[i] = static_cast<char>(std::tolower(result[i])); } return result; }
		String toUpperCase() const { String result(*this); for(size_t i = 0; i < result.size(); ++i){
            result[i] = static_cast<char>(std::toupper(result[i])); } return result; }
        static std::string toLowerCase(std::string str){ for(size_t i = 0; i < str.size(); ++i){ str[i] = std::tolower(str[i]); } return str; }
        static std::string toUpperCase(std::string str){ for(size_t i = 0; i < str.size(); ++i){ str[i] = std::toupper(str[i]); } return str; }
        String trim() const { size_t first = this->find_first_not_of(" \t\r\n\f\v"); size_t last = this->find_last_not_of(" \t\r\n\f\v");
            if(first == std::string::npos || last == std::string::npos){ return String(""); } return this->substr(first, last-first+1); }
    };
	// Специализированная перегрузка оператора потока вывода для типа Array<String>
	std::ostream& operator<<(std::ostream& os, const Array<String>& arr){ os << arr.cout_str(); return os; }
   
	struct Buffer : std::vector<unsigned char>{ //typedef std::vector<unsigned char> BfType;
        // === Конструкторы ===
        Buffer() : std::vector<unsigned char>(){} ~Buffer(){}
		Buffer(size_t size, unsigned char value=0) : std::vector<unsigned char>(size, value){} // Конструктор с параметром размера
		Buffer(const char* cstr){ assign(cstr, cstr+std::strlen(cstr)); } // Конструктор для c-строки
        Buffer(const void* ptr, size_t size){ assign((const char*)ptr, (const char*)ptr+size); } // Конструктор для массива байт с длинной.
		Buffer(const std::string& str, const char* type="str"){ _init(str, type); } // Конструктор для константной строки.
		Buffer(std::vector<unsigned char> vec) : std::vector<unsigned char>(vec){}
		Buffer(std::vector<unsigned char>::const_iterator first, std::vector<unsigned char>::const_iterator last) : std::vector<unsigned char>(first, last){} // Итераторы
		// == создание
		void _init(const std::string& str, const char* type="str"){
            if(strcmp(type, "hex")==0){ _fromHex(str.c_str(), str.size()); }else if(strcmp(type, "base64")==0){ (*this)=base64_decode(str); }
            else if(strcmp(type, "int")==0||strcmp(type, "number")==0){ _fromIntString(str); }else{ assign(str.begin(), str.end()); } }
        void _fromHex(const char* hexstr, size_t len=0){ if(len<=0){ len=std::strlen(hexstr); } len/=2; if (this->size()!=len){ resize(len); }
            for(size_t i = 0; i<len;++i){ (*this)[i] = htoc(hexstr+2*i); } }
        void _fromIntString(const std::string& str){
			this->clear(); Buffer temp(str.size()); for(size_t i = 0; i < str.size(); ++i){ temp[i] = str[i] - '0'; }
			while (!temp.empty() && !(temp.size() == 1 && temp[0] == 0)){ unsigned int remainder = 0;
				for(size_t i = 0; i < temp.size(); ++i){ unsigned int current = remainder * 10 + temp[i]; temp[i] = current / 256; remainder = current % 256; }
				this->insert(this->begin(), static_cast<unsigned char>(remainder));
				// Удаление ведущих нулей
				while (!temp.empty() && temp[0] == 0){ temp.erase(temp.begin()); }
			} }
		static Buffer from(const std::string& str, const char* type="str"){ return Buffer(str, type); }
		static Buffer from(unsigned long long numb){ Buffer buff(8); buff.writeInt64BE(numb); removeLeadingZeros(buff); return buff; }
		static Buffer fromHex(const char* hexstr, size_t len=0){ Buffer buff; buff._fromHex(hexstr, len); return buff; }
		static Buffer fromHex(const std::string& hexstr){ Buffer buff; buff._fromHex(hexstr.c_str(), hexstr.size()); return buff; }
		// == поиск
        // TODO: use "long"? Or use size_t and ncpp::npos?
        long indexOf(const char* delim, size_t len, size_t start) const {
			if(len==0 || this->size() < len || start >= this->size()){ return -1; }
			std::vector<size_t> lps(len, 0); size_t j = 0; size_t i = 1;
			
			while (i < len){
				if (delim[i] == delim[j]){ j++; lps[i] = j; i++; }
				else{ if (j != 0){ j = lps[j - 1]; }else { lps[i] = 0; i++; } } }

			i = start; j = 0;
			while (i < this->size()){
				if(delim[j] == (*this)[i]){ j++; i++; }
				if (j == len){ return i - j; } 
				else if (i < this->size() && delim[j] != (*this)[i]){
					if (j != 0){ j = lps[j - 1]; }else{ i++; } } } return -1; }
		long indexOf(const char* delim, size_t start=0) const { return indexOf(delim, std::strlen(delim), start); }
		long indexOf(const Buffer& delim, size_t start=0) const { return indexOf((delim.size()>0)?(char*)&delim[0]:NULL, delim.size(), start); }
		// == переобразования
		void push(unsigned char value){ this->push_back(value); }
		Array<Buffer> split(const char* delim, size_t len=0) const { if(len<=0){ len=std::strlen(delim); }
			Array<Buffer> parts; size_t start = 0; size_t end = indexOf(delim, len, start); if(end==std::string::npos){ parts.push_back(*this); return parts; }
			while (end != std::string::npos){ Buffer part; part.assign(begin() + start, begin() + end);
				parts.push_back(part); start = end + len; end = indexOf(delim, len, start); }
			Buffer part; part.assign(this->begin() + start, this->end()); parts.push_back(part); return parts; }
		Array<Buffer> split(const Buffer& delim) const { return split((delim.size()>0)?(char*)&delim[0]:NULL, delim.size()); }
		
		template <typename T>
		Array<T> _split(const char* delim, size_t len=0) const { if(len<=0){ len=std::strlen(delim); } Array<T> result;
			size_t start = 0; int end = this->indexOf(delim, len, start); if(end<0){ result.push_back(*this); return result; }
			while (end != -1){ result.push_back(this->slice(start, end - start)); start = end+len;
				end = this->indexOf(delim, len, start); } result.push_back(this->slice(start)); return result; }
		template <typename T>
		Array<T> _split(const std::string& delim) const { return _split<T>(delim.c_str(), delim.size()); }
		//Array<Buffer> split(int count) const; //TODO: сплит через N символов?
		Buffer slice(int start, int end=0) const { size_t length = this->size();
			if(start < 0){ start = length+start; }
			if(end <= 0){ end = length+end;} if(start < 0){ start = 0;} 
			if((size_t)end > length){ end = length; } if (start > end) start = end;
			Buffer result; result.assign(this->begin() + start, this->begin() + end); return result; }
		bool startsWith(const char* prefix, size_t len=0) const { if(len<=0){ len=std::strlen(prefix); } if(len==0||len > this->size()) return false;
			for(size_t i = 0; i < len; ++i){ if((*this)[i] != prefix[i]){ return false; } } return true; }
		bool startsWith(const Buffer& prefix) const { return startsWith((prefix.size()>0)?(char*)&prefix[0]:NULL, prefix.size()); }
		bool endsWith(const char* suffix, size_t len=0) const { if(len<=0){ len=std::strlen(suffix); } size_t bsize=this->size(); if(len==0||len > bsize) return false;
			for(size_t i = 0; i < len; ++i){ if((*this)[bsize-len+i] != suffix[i]){ return false; } } return true; }
		bool endsWith(const Buffer& suffix) const { return endsWith((suffix.size()>0)?(char*)&suffix[0]:NULL, suffix.size()); }
		void fill(unsigned char value){ std::vector<unsigned char>::assign(size(), value); }
		Buffer& concat(const Buffer& buff2){ this->insert(this->end(), buff2.begin(), buff2.end()); return *this; }
		void reverse(){ std::reverse(this->begin(),this->end()); }
		
		void read(void* dst, size_t size, size_t offset=0){ if(this->size() < offset+size){ size = this->size()-offset; }
			memcpy(dst, &(*this)[0]+offset, size); }
		void write(const void* src, size_t size, size_t offset){ if(this->size() < offset+size){ this->resize(offset+size); }
			memcpy(&(*this)[0]+offset, src, size); }
		void write(const char* cstr, size_t offset=0){ return write(cstr, std::strlen(cstr), offset); }
		void write(const Buffer& buff, size_t offset=0){ if(buff.empty()) return; return write(&buff[0], buff.size(), offset); }
		void write(const std::string& str, size_t offset=0){ if(str.empty()) return; return write(&str[0], str.size(), offset); }
		template <typename T>
		T readAny(size_t offset=0){ if(std::is_pointer<T>::value){ throw Err("readAny(T&): not accept pointers"); }
			T data; read((void*)&data, sizeof(data), offset); return data; }
		template <typename T>
		void writeAny(const T& data, size_t offset=0){ if(std::is_pointer<T>::value){ throw Err("writeAny(T&): not accept pointers"); }
			return write((void*)&data, sizeof(data), offset); }
		
		Array<char> BitsRead(size_t offset=0) const { return BitsRead(&(*this)[offset]); }
		void BitsWrite(const std::vector<char>& bits, size_t offset=0){ BitsWrite(&(*this)[offset], bits); }
		// == представления
		template <typename T>
		T readT_BE(size_t offset=0, int length=sizeof(T)) const { T value=readT_LE<T>(offset); std::reverse((char*)&value, (char*)&value+length); return value; }
		template <typename T>
		T readT_LE(size_t offset=0, int length=sizeof(T)) const { if(offset+length > this->size()){ throw std::out_of_range("readT_LE: out of range"); } 
			T value=T(); memcpy(&value, &(*this)[0]+offset, length); return value; }
		
		template <typename T>
		void writeT_BE(T value, size_t offset=0, int length=sizeof(T)){ writeT_LE<T>(value, offset); 
			std::reverse(&(*this)[0]+offset, &(*this)[0]+offset+length); }
		template <typename T>
		void writeT_LE(T value, size_t offset=0, int length=sizeof(T)){ if(this->size() < offset+length){ resize(offset+length); }
			memcpy(&(*this)[0]+offset, &value, length); }
			
		int readInt16BE(size_t offset=0) const { return readT_BE<int16_t>(offset); }
		int readInt16LE(size_t offset=0) const { return readT_LE<int16_t>(offset); }
		unsigned int readUInt16BE(size_t offset=0) const { return readT_BE<uint16_t>(offset); }
		unsigned int readUInt16LE(size_t offset=0) const { return readT_LE<uint16_t>(offset); }
		void writeInt16BE(int value, size_t offset=0){ writeT_BE<int16_t>(value, offset); }
		void writeInt16LE(int value, size_t offset=0){ writeT_LE<int16_t>(value, offset); }
		void writeUInt16BE(unsigned int value, size_t offset=0){ writeT_BE<uint16_t>(value, offset); }
		void writeUInt16LE(unsigned int value, size_t offset=0){ writeT_LE<uint16_t>(value, offset); }
		
		int readInt32BE(size_t offset=0) const { return readT_BE<int32_t>(offset); }
		int readInt32LE(size_t offset=0) const { return readT_LE<int32_t>(offset); }
		unsigned int readUInt32BE(size_t offset=0) const { return readT_BE<uint32_t>(offset); }
		unsigned int readUInt32LE(size_t offset=0) const { return readT_LE<uint32_t>(offset); }
		void writeInt32BE(int value, size_t offset=0){ writeT_BE<int32_t>(value, offset); }
		void writeInt32LE(int value, size_t offset=0){ writeT_LE<int32_t>(value, offset); }
		void writeUInt32BE(unsigned int value, size_t offset=0){ writeT_BE<uint32_t>(value, offset); }
		void writeUInt32LE(unsigned int value, size_t offset=0){ writeT_LE<uint32_t>(value, offset); }
		
		long long readInt64BE(size_t offset=0) const { return readT_BE<int64_t>(offset); }
		long long readInt64LE(size_t offset=0) const { return readT_LE<int64_t>(offset); }
		unsigned long long readUInt64BE(size_t offset=0) const { return readT_BE<uint64_t>(offset); }
		unsigned long long readUInt64LE(size_t offset=0) const { return readT_LE<uint64_t>(offset); }
		void writeInt64BE(long long value, size_t offset=0){ writeT_BE<int64_t>(value, offset); }
		void writeInt64LE(long long value, size_t offset=0){ writeT_LE<int64_t>(value, offset); }
		void writeUInt64BE(unsigned long long value, size_t offset=0){ writeT_BE<uint64_t>(value, offset); }
		void writeUInt64LE(unsigned long long value, size_t offset=0){ writeT_LE<uint64_t>(value, offset); }
			
		double readDoubleBE(size_t offset=0) const { return readT_BE<double>(offset); }
		double readDoubleLE(size_t offset=0) const { return readT_LE<double>(offset); }
		void writeDoubleBE(double value, size_t offset=0){ writeT_BE<double>(value, offset); }
		void writeDoubleLE(double value, size_t offset=0){ writeT_LE<double>(value, offset); }
		// === static funcs ===
		static Array<char> BitsRead(const unsigned char* ptr){ Array<char> bits(8);
			for(char indx = 0; indx < 8; ++indx){ bits[indx] = (*ptr >> (7 - indx)) & 1; } return bits; }
		static void BitsWrite(unsigned char* ptr, const std::vector<char>& bits){ *ptr=0;
			for(size_t indx = 0; indx < bits.size(); ++indx){ *ptr = (*ptr << 1) | bits[indx]; } }
		static Array<char> BitsRead(const void* ptr){ return BitsRead((const unsigned char*)ptr); }
		static void BitsWrite(void* ptr, const std::vector<char>& bits){ BitsWrite((unsigned char*)ptr, bits); }
		
		static int BitsToInt(const std::vector<char>& bits){ int numb = 0;
			for(size_t i = 0; i < bits.size(); ++i){ numb = (numb << 1) | bits[i]; } return numb; }
		static Array<char> IntToBits(int numb, size_t len=sizeof(int)*8){ Array<char> bits(len);	
			for(int i = len - 1; i >= 0; --i){ bits[i] = (numb >> i) & 1; } return bits; }
			
		static Buffer concat(const Array<Buffer>& buffs){ Buffer result; for(size_t i=0;i<buffs.size();i++){ result+=buffs[i]; } return result; }
		static Buffer concat(const Buffer& buff1, const Buffer& buff2){ return buff1+buff2; }
		static Buffer randBytes(int length); 

		static const std::string base64_chars;
		static String base64_encode(const Buffer& buffer){ if(buffer.size()<1){ return ""; }
			unsigned char const* bytes_to_encode = &buffer[0]; unsigned int in_len = buffer.size(); 
			std::string ret; int i = 0; int j = 0; unsigned char char_array_3[3]; unsigned char char_array_4[4];

			while (in_len--){ char_array_3[i++] = *(bytes_to_encode++);
				if(i == 3){
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;
					for (i = 0; (i < 4); i++){ ret += base64_chars[char_array_4[i]]; } i = 0; } }

			if(i){
				for (j = i; j < 3; j++){ char_array_3[j] = '\0'; }
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				for (j = 0; (j < i + 1); j++){ ret += base64_chars[char_array_4[j]]; }
				while ((i++ < 3)){ ret += '='; } } return ret; }
		static Buffer base64_decode(const std::string& encoded_string){
			int in_len = encoded_string.size(); int i = 0; int j = 0; int in_ = 0;
			unsigned char char_array_4[4], char_array_3[3]; Buffer ret;

			while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
				char_array_4[i++] = encoded_string[in_]; in_++;
				if (i == 4){
					for (i = 0; i < 4; i++){ char_array_4[i] = base64_chars.find(char_array_4[i]); }
					char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
					for (i = 0; (i < 3); i++){ ret.push_back(char_array_3[i]); } i = 0; } }

			if (i){ for(j = i; j < 4; j++){ char_array_4[j] = 0; }
				for (j = 0; j < 4; j++){ char_array_4[j] = base64_chars.find(char_array_4[j]); }
				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				for(j = 0; (j < i - 1); j++){ ret.push_back(char_array_3[j]); } } return ret; }
		
		// === ===
		static void removeLeadingZeros(Buffer& buff){ if(buff.size() <= 1) return; size_t firstNonZero = 0;
				while(firstNonZero < buff.size()-1 && buff[firstNonZero] == 0) ++firstNonZero;
				if(firstNonZero > 0){ buff.erase(buff.begin(), buff.begin() + firstNonZero); } }
		struct Math; // == арифметика (implemented in ncpp-bigint.cpp)
		unsigned int toInt() const noexcept { unsigned int value = 0; int sz=4; if(this->size()<4){ sz=this->size(); }
			for(int i = 0; i<sz; ++i){ value = (value << 8) | (*this)[i]; } return value; }
		unsigned long long toInt64() const noexcept { unsigned int value = 0; int sz=8; if(this->size()<8){ sz=this->size(); }
			for(int i = 0; i<sz; ++i){ value = (value << 8) | (*this)[i]; } return value; }
        String toString() const { return (this->empty())?String():String((char*)&(*this)[0], this->size()); }
		String toString(std::string type) const {
			if(type=="hex"){ return toHexString(); } else if(type=="base64"){ return base64_encode(*this); }
			else if(type=="raw"||type=="buff"){ return cout(); }
			else if(type=="int"||type=="number"||type=="dec"){ return toIntString(); }
			else{ return toString(); } }

        String toHexString() const { std::stringstream oss; char hex[3]; hex[2]=0; 
			for(size_t i = 0;i<(*this).size();i++){ ctoh((*this)[i], hex); oss << hex; } return oss.str(); }
		String toIntString() const { if(this->empty()){ return String("0"); }
			String result; Buffer temp(*this);
			while (!temp.empty() && !(temp.size() == 1 && temp[0] == 0)) {
				unsigned int remainder = 0; std::vector<unsigned char> newTemp; newTemp.reserve(temp.size());

				for (size_t i = 0; i < temp.size(); ++i){
					unsigned int current = remainder * 256 + temp[i];
					if (!newTemp.empty() || current / 10 > 0){ newTemp.push_back(current / 10); }
					remainder = current % 10;
				} result.insert(result.begin(), '0' + remainder); temp.swap(newTemp); } if(result.size()==0){result="0";} return result; }
    
		std::string cout() const { std::stringstream oss; oss << "<Buffer";
			size_t tmpsize=(*this).size(); if(tmpsize>=50){ tmpsize=50; } char hex[3]; hex[2]=0;
			for (size_t i = 0;i<tmpsize;i++){ ctoh((*this)[i], hex); oss << " " << hex; }
			if((*this).size()>tmpsize){ oss << " ... " << (*this).size()-tmpsize << " bytes more>"; }
			else{ oss << ">"; } return oss.str(); }
		// Перегрузки операторов
		Buffer& operator+=(const Buffer& buff2){ return this->concat(buff2); }
		Buffer operator+(const Buffer& buff2) const { Buffer nbuff = *this; nbuff+=buff2; return nbuff; }
		// == ==
		static unsigned char htoc(const char hex[2]){ char high = (hex[0] >= 'a' ? hex[0] - 'a' + 10 : hex[0] - '0');
			char low = (hex[1] >= 'a' ? hex[1] - 'a' + 10 : hex[1] - '0'); return ((high << 4) | low); }
		static void ctoh(unsigned char value, char hex[2]){ char high = (value >> 4) & 0x0F; char low = value & 0x0F;
			hex[0] = (high < 10) ? ('0' + high) : ('a' + (high - 10)); hex[1] = (low < 10) ? ('0' + low) : ('a' + (low - 10)); }
		#if __cplusplus >= 201103L
		static unsigned int htoi(const std::string& hexstr){ return (unsigned int)std::stoul(hexstr, nullptr, 16); }
		static unsigned long long htoll(const std::string& hexstr){ return std::stoull(hexstr, nullptr, 16); }
		#else
		static unsigned int htoi(const std::string& hexstr){ unsigned int value; std::stringstream ss(hexstr); ss >> std::hex >> value; return value; }
		static unsigned long long htoll(const std::string& hexstr){ unsigned long long value; std::stringstream ss(hexstr); ss >> std::hex >> value; return value; }
		#endif
		static std::string itoh(unsigned int value){ std::stringstream ss; ss << std::hex << value; return ss.str(); }
		static std::string itoh(unsigned long long value){ std::stringstream ss; ss << std::hex << value; return ss.str(); }
		private:
			std::string byteToHex(unsigned char value) const { const char hexDigits[] = "0123456789abcdef";
				std::string hex(2, '0'); hex[0] = hexDigits[(value >> 4) & 0xF]; hex[1] = hexDigits[value & 0xF]; return hex; }
			static bool is_base64(unsigned char c){ return (::isalnum(c) || (c == '+') || (c == '/')); }
	};
	const std::string Buffer::base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	// Перегрузки операторов для Buffer
	std::ostream& operator<<(std::ostream& os, const Buffer& buffer){ os << buffer.cout(); return os; }
	
	// Перегрузки метода Array::join для типов шаблона Array<T>
	template <>
	Buffer Array<Buffer>::join(Buffer& dbuff) const { return join_buff(dbuff); }
	template <> 
	Buffer Array<Buffer>::join(const std::string& delimiter) const { Buffer dbuff(delimiter); return join_buff(dbuff); }
	template <>
	Buffer Array<Buffer>::join() const { Buffer dbuff; return join_buff(dbuff); }

	unsigned long long xorshift128plus(){ static unsigned long long s[2]; static unsigned long long init=false;
		if(!init){ s[0] = GetTimestamp('m'); std::srand(s[0]); s[0]+=std::rand(); s[1] = s[0]+0x9e3779b97f4a7c15ULL; init=true; for(int i=0;i<5;i++){ xorshift128plus(); } }
		s[0] = s[1]; s[0] ^= s[0] << 23; s[1] ^= s[0] ^ (s[0] >> 17) ^ (s[1] >> 26); return s[1] + s[0]; }
	double random(){ return (xorshift128plus() >> 11) * (1.0 / 9007199254740991.0); }
	int randInt(int min, int max){ return floor(random() * (max - min + 1)) + min; }
	Buffer Buffer::randBytes(int length){ Buffer buff(length); for(size_t i=0;i<buff.size();i++){ buff[i]=randInt(0,255); } return buff; }
	String randStr(size_t length, String letters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"){ 
		String res; for(size_t i=0; i<length; ++i){res+=letters[floor(random()*letters.size())]; } return res; }
	
	template <typename K, typename V>
#if __cplusplus >= 201103L // C++11
	struct HashMap : std::unordered_map<K, V> { typedef std::unordered_map<K, V> MapType;
#else // or C++98
	struct HashMap : std::map<K, V> { typedef std::map<K, V> MapType;
#endif
		typedef typename MapType::iterator iterator; typedef typename MapType::const_iterator const_iterator;
		Array<K> keys() const { Array<K> keys;
			for(typename HashMap::const_iterator it = this->begin(); it != this->end(); ++it){ keys.push(it->first); } return keys; }
		bool has(const K& key) const { return this->find(key) != this->end(); }
		static HashMap<K, V>& assign(HashMap<K, V>& target, const HashMap<K, V>& map2){ 
			for(typename HashMap::const_iterator it = map2.begin(); it != map2.end(); ++it){ target[it->first]=it->second; } return target; }
		static HashMap<K, V>& assign(HashMap<K, V>& target, const Array<HashMap<K, V> >& maps){ for(size_t i=0;i<maps.size();i++){ target.assign(target, maps[i]); } return target; }
		std::string cout() const { std::stringstream ss; ss << "{" << std::setprecision(D_PRECISION);
			for(typename HashMap::const_iterator it = this->begin(); it != this->end(); ++it){
				ss << "\n  \"" << it->first << "\": " << it->second << ", "; } ss << "}"; return ss.str(); }
		std::string cout2() const { std::stringstream ss; for(typename HashMap::const_iterator it = this->begin(); it != this->end(); ++it){
				ss << it->first << ": " << it->second << "\n"; } return ss.str(); }
		String toJSON() const { std::stringstream ss("{"); ss << std::setprecision(D_PRECISION);
			for(typename HashMap::const_iterator it = this->begin(); it != this->end(); ++it){ if(it != this->begin()){ ss << ","; } 
				ss << "\"" << it->first << "\":" << it->second; } ss << "}"; return ss.str(); }
		String toString() const { return cout(); } 
		std::string type() const { return "map"; } 
		#if __cplusplus < 201103L
		const V& at(const K& key) const { typename HashMap::const_iterator it = this->find(key);
			if(it == this->end()){ throw std::out_of_range("Key not found in const HashMap::at()"); } return it->second; }
		#endif 
	};
	template <typename K, typename V>
	std::ostream& operator<<(std::ostream& os, const HashMap<K, V>& map){ os << map.cout(); return os; }
	
	template <typename K>
#if __cplusplus >= 201103L // C++11
	struct HashSet : std::unordered_set<K> { typedef std::unordered_set<K> MapType;
#else // or C++98
	struct HashSet : std::set<K> { typedef std::set<K> MapType;
#endif
		typedef typename MapType::iterator iterator; typedef typename MapType::const_iterator const_iterator;
		Array<K> keys() const { Array<K> keys;
			for(typename HashSet::const_iterator it = this->begin(); it != this->end(); ++it){ keys.push(*it); } return keys; }
		bool has(const K& key) const { return this->find(key) != this->end(); }
		inline void erase_ptr(K key){ this->erase(key); delete key; }
		void clear_ptr(){ for(typename HashSet::iterator it = this->begin(); it != this->end(); ++it){ if(*it!=NULL){ delete *it; } } this->clear(); }
		std::string cout() const { std::stringstream ss; ss << "{";
			for(typename HashSet::const_iterator it = this->begin(); it != this->end(); ++it){
				ss << *it << ", "; } ss << "}"; return ss.str(); }
		String toString() const { return cout(); }
		std::string type() const { return "set"; } };
	template <typename K>
	std::ostream& operator<<(std::ostream& os, const HashSet<K>& set){ os << set.cout(); return os; }
	
	struct StringMap : HashMap<std::string, String> {
		std::string cout() const { std::stringstream ss("{");
			for(StringMap::const_iterator it = this->begin(); it != this->end(); ++it){
				ss << "\n  \"" << it->first << "\": \"" << it->second << "\", "; } ss << "}"; return ss.str(); }
		String toJSON() const { std::stringstream ss("{");
			for(StringMap::const_iterator it = this->begin(); it != this->end(); ++it){ if(it != this->begin()){ ss << ","; } 
				ss << "\"" << it->first << "\":\"" << it->second << "\""; } ss << "}"; return ss.str(); }
		String toString() const { return toJSON(); }
	};
	
	typedef HashMap<std::string, double> DoubleMap;
	//typedef HashMap<std::string, unsigned long long> ULLMap; // UInt64Map?
	//typedef HashMap<std::string, long long> Int64Map; // IntLLMap?
	
	struct Date { long long timestamp; //char utc;
		Date() : timestamp(Date::now()){} ~Date(){} 
		Date(long long msecs, char type='m') : timestamp(msecs){ if(type=='s'){timestamp*=1000;} }
		Date(const String& dateStr) : timestamp(0){ _setDate(dateStr); }
		static long long now(){ return GetTimestamp('m'); }
		
		int getYear() const { long long days = timestamp/86400/1000; long long years1 = days/365;
			long long leapDays = (1970+years1)/4-(1970+years1)/100+(1970+years1)/400; return 1970+((days+leapDays)/365); } //365.2425
		int getMonth() const;
		int getDate() const;
		int getDay() const;
		int getDayWeek() const;
		
		std::string toDirectDate(char sp='.') const { int year, month, day, hour, minute; float seconds; _timestampToDate(year, month, day, hour, minute, seconds);
			std::stringstream r; r<<(day<10?"0":"")<<day<<sp<<(month<10?"0":"")<<month<<sp<<year<<" ";
			r<<(hour<10?"0":"")<<hour<<":"<<(minute<10?"0":"")<<minute<<":"<<(seconds<10?"0":"")<<seconds; return r.str(); }
		std::string toReverseDate(char sp='.') const { int year, month, day, hour, minute; float seconds; _timestampToDate(year, month, day, hour, minute, seconds);
			std::stringstream r; r<<year<<sp<<(month<10?"0":"")<<month+1<<sp<<(day<10?"0":"")<<day+1<<" ";
			r<<(hour<10?"0":"")<<hour<<":"<<(minute<10?"0":"")<<minute<<":"<<(seconds<10?"0":"")<<seconds; return r.str(); }
		std::string toString(char sp='.') const { return toReverseDate(sp); }
		std::string toString(const String& mode, char sp='.') const { if(mode=="reverse"){ return toReverseDate(); }
			else if(mode=="direct"){ return toDirectDate(); }else if(mode=="iso"||mode=="isostring"){ return toISOString(); }else{ return toReverseDate(); } }
		std::string toISOString() const { int year, month, day, hour, minute; float seconds; _timestampToDate(year, month, day, hour, minute, seconds);
			std::stringstream r; r<<year<<"-"<<(month<10?"0":"")<<month<<"-"<<(day<10?"0":"")<<day<<"T";
			r<<(hour<10?"0":"")<<hour<<":"<<(minute<10?"0":"")<<minute<<":"<<(seconds<10?"0":"")<<seconds<<"Z"; return r.str(); }
		
		Date& operator+=(const Date& other){ this->timestamp+=other.timestamp; return *this; }
		long long operator+(const Date& other) const { return this->timestamp+other.timestamp; }
		Date& operator+=(long long value){ this->timestamp+=value; return *this; }
		long long operator+(long long value) const { return this->timestamp+value; }
		
		Date& operator-=(const Date& other){ this->timestamp-=other.timestamp; return *this; }
		long long operator-(const Date& other) const { return this->timestamp-other.timestamp; }
		Date& operator-=(long long value){ this->timestamp-=value; return *this; }
		long long operator-(long long value) const { return this->timestamp-value; }
		private:
		bool _isGrigoryVisokos(int year) const { return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }
		int _GrigoryVisokosAbs(int year) const { return (year/4) - (year/100) + (year/400); }
		int _GrigoryVisokos(int startYear, int endYear) const { return _GrigoryVisokosAbs(endYear)-_GrigoryVisokosAbs(startYear); }
		int _MonthDays(int month, int year) const { const int daysOfMonth[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}; 
			int days=(month>0&&month<12)?daysOfMonth[month]:0; if(_isGrigoryVisokos(year) && month > 1){ days+=0; } return days; }
		void _parseDate(const String& dateStr, int& year, int& month, int& day, int& hour, int& minute, float& seconds){
			Array<String> darr = dateStr.replaceAll(".", "-").split("-"); darr.resize(3); bool isdirect=(darr[0].size()<=2&&darr[2].size()>2)?true:false;
			month=stoin(darr[1]); if(isdirect){ day=stoin(darr[0]); }else{ year=stoin(darr[0]); } darr = darr[2].replace("T"," ").split(" "); 
			if(isdirect){ year=stoin(darr[0]); }else{ day=stoin(darr[0]); } darr = darr[1].split(":"); 
			hour=stoin(darr[0]); minute=stoin(darr[1]); seconds=stoin(darr[2].split("Z")[0]);
			//std::cout << "_parseDate(darr): " << darr << std::endl;
			
			/*int argscnt = std::sscanf(dateStr.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &year, &month, &day, &hour, &minute, &seconds);
			if(argscnt<3){ std::cout << "(!) Date: sscanf parse failed: less 3 args parsed" << std::endl; return; }
			std::cout << "Date: sscanf parsed args: " << argscnt << std::endl;*/
			
			//std::cout << year << "; " << month << "; " << day << " " << hour << ":" << minute << ":" << seconds << " | Etalon: 1725065286957" << std::endl;
		}
		void _setDate(const String& dateStr){ // Предполагаются форматы: "YYYY-MM-DD HH:MM:SS", "DD-MM-YYYY HH:MM:SS"
			int year=0, month=0, day=0, hour=0, minute=0; float seconds=0; _parseDate(dateStr, year, month, day, hour, minute, seconds);
			timestamp = (long long)(year-1970) * 365 * 24 * 60 * 60 * 1000; // годы
			timestamp += (long long)_MonthDays(month, year) * 24 * 60 * 60 * 1000; // месяцы
			timestamp += (long long)(day-1+_GrigoryVisokos(1970, year)) * 24 * 60 * 60 * 1000; // дни
			timestamp += hour * 3600 * 1000; timestamp += minute * 60 * 1000; timestamp += seconds * 1000; }
		void _timestampToDate(int& year, int& month, int& day, int& hour, int& minute, float& seconds) const { month=0; year = getYear(); 
			int totalsecs = (timestamp/1000)-((year-1970)*365LL+_GrigoryVisokos(1970, year))*86400;
			//std::cout << "-- formula: (("<<year<<"-1970)*365+"<<(_GrigoryVisokos(1970, year))<<")*86400 = " << ((year-1970)*365LL+_GrigoryVisokos(1970, year))*86400 << std::endl;
			//std::cout << "totalsecs = " << totalsecs << std::endl;
			//std::cout << "_MonthDays("<<month<<", "<<year<<") = " << _MonthDays(month, year) << "(days="<< (float)totalsecs/86400 << "; totalsecs = " << totalsecs << std::endl;
			int totalDays=totalsecs/86400; while(totalDays > _MonthDays(month, year)&&month<11){ month++; } totalsecs-=_MonthDays(month, year)*86400;
			//std::cout << "2) _MonthDays("<<month<<", "<<year<<") = " << _MonthDays(month, year) << "(days="<< (float)totalsecs/86400 << "; totalsecs = " << totalsecs << std::endl;
			day = floor(totalsecs/86400); totalsecs-=day*86400; hour = floor(totalsecs/3600); totalsecs-=hour*3600; 
			minute = floor(totalsecs/60); totalsecs-=minute*60; seconds = totalsecs; }
	};
}
#if __cplusplus >= 201103L
namespace std { // Специализация struct hash<String> для std C++11
	template <>
	struct hash<ncpp::String> { size_t operator()(const ncpp::String& s) const noexcept { return hash<std::string>()(s); } }; }
#endif
