struct CString { 
	CString() : ptr(NULL), len(0){}
	CString(const char* s) : ptr(s), len(std::strlen(s)){}
	//CString(const void* ptr, int len) : ptr(ptr), len(len){}
	CString(const std::string& s) : ptr(s.c_str()), len(s.size()){}
	size_t size() const { return len; }
	const char* c_str() const { return ptr; }
	bool operator==(const char* c) const { return std::strncmp(ptr, c, (size_t)len)==0; }
	bool operator==(const std::string& s) const { return std::strncmp(ptr, s.c_str(), (size_t)len)==0; }
	private: const char* ptr; int len; }