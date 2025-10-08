namespace ncpp{
template<bool, typename T = void> struct enable_if {};
template<typename T> struct enable_if<true, T> { typedef T Type; };

template <bool B1, bool B2> struct _or { static const bool V = false; }; // По умолчанию false
template <> struct _or<true, true> { static const bool V = true; };
template <> struct _or<true, false> { static const bool V = true; };
template <> struct _or<false, true> { static const bool V = true; };

template <typename T> struct is_pointer { static const bool V = false; };
template <typename T> struct is_pointer<T*> { static const bool V = true; typedef T type; };

template<typename T> struct is_integer { static const bool V = false; };
template<> struct is_integer<char> { static const bool V = true; typedef T type; };
template<> struct is_integer<signed char> { static const bool V = true; typedef T type; };
template<> struct is_integer<unsigned char> { static const bool V = true; typedef T type; };
template<> struct is_integer<short> { static const bool V = true; typedef T type; };
template<> struct is_integer<unsigned short> { static const bool V = true; typedef T type; };
template<> struct is_integer<int> { static const bool V = true; typedef T type; };
template<> struct is_integer<unsigned int> { static const bool V = true; typedef T type; };
template<> struct is_integer<long> { static const bool V = true; typedef T type; };
template<> struct is_integer<unsigned long> { static const bool V = true; typedef T type; };
template<> struct is_integer<long long> { static const bool V = true; typedef T type; };
template<> struct is_integer<unsigned long long> { static const bool V = true; typedef T type; };

template<typename T> struct is_float { static const bool V = false; };
template<> struct is_float<float> { static const bool V = true; typedef T type; };
template<> struct is_float<double> { static const bool V = true; typedef T type; };
template<> struct is_float<long double> { static const bool V = true; typedef T type; };

template<typename T> struct is_number { static const bool V = _or<is_integer<T>::V, is_float<T>::V>::V; };
template<typename T> struct is_pod { static const bool V = _or<is_number<T>::V, is_pointer<T>::V>::V; };
template<> struct is_pod<bool> { static const bool V = true; };
template<> struct is_pod<wchar_t> { static const bool V = true; };

#if __cplusplus >= 201103L // C++11
template<typename T> struct remove_reference { typedef T type; };
template<typename T> struct remove_reference<T&> { typedef T type; };
template<typename T> struct remove_reference<T&&> { typedef T type; };
// ncpp::move(T&&)
template<typename T> typename remove_reference<T>::type&& move(T&& arg) noexcept { return static_cast<typename remove_reference<T>::type&&>(arg); }
#endif

//== SFINAE Example
//template<typename T, class SFINAE = typename is_integer<T>::type>
//void foo(T val);
}