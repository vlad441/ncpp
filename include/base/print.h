void print(const char* cptr, size_t len);
#ifdef __cplusplus
namespace ncpp{ using ::print;
	void print(const char* cstr);
	const char* version();
	
	void print(long long v);
}
#endif

