namespace ncpp{ struct Console {
	void log(const char* c){ String ss(c); ss+='\n'; print(ss); }
	void log(CString cs){ String ss(cs); ss+='\n'; print(ss); }
	String input(){ return readline(); }
	
	Console& operator+=(const char* c){ print(c); return *this; }
	template <typename T, typename D>
	Console& operator+=(const BaseString<T, D>& s){ print(s); return *this; }
	
	Console& operator<<(const char* c){ print(c); return *this; }
	Console& operator<<(long long num){ print(dtos(num)); return *this; }
	Console& operator<<(const Buffer& buff){ print(buff.cout()); return *this; }
	template <typename T, typename D> Console& operator<<(const BaseString<T, D>& s){ print(s); return *this; }
	template <typename T> Console& operator<<(const Array<T>& arr){ print(arr.cout()); return *this; }
	template <typename K, typename V> Console& operator<<(const HashMap<K, V>& obj){ print(obj.cout()); return *this; }
	template <typename K> Console& operator<<(const HashSet<K>& obj){ print(obj.cout()); return *this; }
	Console& operator<<(const Object& obj){ print(obj.cout()); return *this; }
	
	Console& operator>>(String& s){ s=readline(); return *this; }
	Console& operator>>(int& i){ i=stoin(readline()); return *this; }
	Console& operator>>(long long& ll){ ll=stolln(readline()); return *this; }
	Console& operator>>(double& d){ d=stodn(readline()); return *this; }
	Console& operator>>(float& f){ f=stofn(readline()); return *this; }
}; }