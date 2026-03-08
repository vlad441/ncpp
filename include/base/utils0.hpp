namespace ncpp {
    // Вспомогательные алгоритмы и математика
    template <typename T> void reverse(T* begin, T* end);
    template <typename N> N min(N num1, N num2);
    template <typename N> N max(N num1, N num2);

    //template<typename I> char _dtos(char str[27], I num, char sep) noexcept;
    //template<typename I> void _printNum(I val, char sep = 0);
    //template<typename I> void _printNum128(I val, char sep = 0);
	
	template <typename K, typename V>
    struct Pair { K first; V second; Pair(const K& k = K(), const V& v = V()); K& key(); const K& key() const; V& value(); const V& value() const; };
}