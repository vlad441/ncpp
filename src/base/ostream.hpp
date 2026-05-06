namespace ncpp {
	template <typename T, typename D>
	std::ostream& operator<<(std::ostream& os, const BaseString<T, D>& s){ os << s.data(); return os; }
	// Перегрузки операторов потока вывода для Array
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Array<T>& arr){ os << arr.cout(); return os; }
	//std::ostream& operator<<(std::ostream& os, const Array<std::string>& arr){ os << arr.cout(); return os; }
	//std::ostream& operator<<(std::ostream& os, const Array<String>& arr){ os << arr.cout(); return os; }
	// Перегрузки операторов для Buffer
	std::ostream& operator<<(std::ostream& os, const Buffer& buffer){ os << buffer.cout(); return os; }
	
	template <typename K, typename V>
	std::ostream& operator<<(std::ostream& os, const HashMap<K, V>& map){ os << map.cout(); return os; }
	
	template <typename K>
	std::ostream& operator<<(std::ostream& os, const HashSet<K>& set){ os << set.cout(); return os; }
}