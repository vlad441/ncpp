namespace ncpp{
#if __cplusplus >= 201103L //for C++11

#else //for C++98
    struct NullType {};
    // 1. Метафункции для поиска максимального размера (sizeof) в C++98
    template <size_t A, size_t B> struct MaxSize { static const size_t value = (A > B) ? A : B; };

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    struct MaxStorageSize {
        static const size_t value = MaxSize<sizeof(T1), 
            MaxSize<sizeof(T2), 
            MaxSize<sizeof(T3), 
            MaxSize<sizeof(T4), sizeof(T5)>::value>::value>::value>::value;
    };

    // 4. Метафункция хелпер для вызова деструкторов (работает по индексам)
    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    struct VariantDestructor {
        static void destroy(size_t index, void* buf){
            if(index == 0){ ((T1*)(buf))->~T1(); }
            else { VariantDestructor<T2, T3, T4, T5, NullType>::destroy(index-1, buf); } } };
    template <> struct VariantDestructor<NullType, NullType, NullType, NullType, NullType> { static void destroy(size_t, void*){} };

    // --- САМ ВАРИАНТ ---
    template <
        typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType
    >
    struct Variant {
        Variant() : _index(static_cast<size_t>(-1)) {}
        Variant(const T1& val) : _index(0){ new (_data.raw) T1(val); } // Конструктор для T1 (для примера)        
        Variant(const T2& val) : _index(1){ new (_data.raw) T2(val); } // Конструктор для T2
        ~Variant(){ if(_index != static_cast<size_t>(-1)){ VariantDestructor<T1, T2, T3, T4, T5>::destroy(_index, _data.raw); } }
		
        size_t index() const { return _index; }
        template <typename T> T& get(){ return *(T*)_data.raw; }
		private: union { char raw[MaxStorageSize<T1, T2, T3, T4, T5>::value];
            long long ll; double d; void* p; void (*pf)(); // ТОЛЬКО для определения выравнивания.
        } _data; size_t _index;
    };
#endif
}
