namespace ncpp{ template <size_t N, typename Tuple_T> struct _TupleRetT; // Вспомогательная структура для поиска типа по индексу N
#if __cplusplus >= 201103L //for C++11
	template <typename... Args> struct Tuple; template <> struct Tuple<> {};
	
	template <typename Head, typename... Tail> struct Tuple<Head, Tail...> : Tuple<Tail...> { 
		Head _val; Tuple() : Tuple<Tail...>(), _val(){} Tuple(Head head, Tail... tail) : Tuple<Tail...>(tail...), _val(head){} };
	
	template <typename Head, typename... Tail> // Если N == 0, мы нашли нужный уровень наследования -> Возвращаем значение текущего класса.
	struct _TupleRetT<0, Tuple<Head, Tail...>> { typedef Head return_type; static return_type& get(Tuple<Head, Tail...>& t){ return t._val; } };

	template <size_t N, typename Head, typename... Tail> // Если N > 0, мы рекурсивно поднимаемся к базовому классу (отбрасывая Head)
	struct _TupleRetT<N, Tuple<Head, Tail...>> { // Тип берем у базового класса, уменьшив N на 1
		typedef typename _TupleRetT<N-1, Tuple<Tail...>>::return_type return_type;
		static return_type& get(Tuple<Head, Tail...>& t){ return _TupleRetT<N - 1, Tuple<Tail...>>::get(t); } };

	// Красивая внешняя функция для пользователя
	template <size_t N, typename... Args>
	typename _TupleRetT<N, Tuple<Args...>>::return_type& get(Tuple<Args...>& t){ return _TupleRetT<N, Tuple<Args...>>::get(t); }
#else //for C++98
#define _TYPLE_T Tuple<T1, T2, T3, T4, T5>
#define _TYPLE_RT Tuple<T2, T3, T4, T5, NullType>
//#define _TYPLE_ARGS typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType
	struct NullType {}; // Маркер пустоты
	template <
		typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType
		//typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType,
	> // Ограничение: Реализация кортежа в С++98 может иметь только ограниченное количество аргументов. (Тут до 5).
	// Наследуемся от "сдвинутого" кортежа (Рекурсивное наследование)
	struct Tuple : _TYPLE_RT { T1 _val; 
		Tuple() : _TYPLE_RT(), _val(){} Tuple(T1 p1, T2 p2=T2(), T3 p3=T3(), T4 p4=T4(), T5 p5=T5()) : _TYPLE_RT(p2, p3, p4, p5), _val(p1){} }; 
	// Пустой кортеж — база рекурсии, ничего не хранит и конструктор пустой
	template <> struct Tuple<NullType, NullType, NullType, NullType, NullType> { Tuple(){} Tuple(NullType, NullType, NullType, NullType, NullType){} };
	
	template <typename T1, typename T2, typename T3, typename T4, typename T5> // База рекурсии геттера: нашли нужный индекс (N == 0)
	struct _TupleRetT<0, _TYPLE_T > { typedef T1 return_type; static return_type& get(_TYPLE_T& t){ return t._val; } };

	// Шаг рекурсии геттера: уменьшаем N и передаем базовый тип (сдвинутый Tuple)
	template <size_t N, typename T1, typename T2, typename T3, typename T4, typename T5>
	struct _TupleRetT<N, _TYPLE_T > { // Рекурсивно вытаскиваем тип из Базового класса (где всё сдвинуто влево)
		typedef typename _TupleRetT<N-1, _TYPLE_RT >::return_type return_type;
		static return_type& get(_TYPLE_T& t){ return _TupleRetT<N-1, _TYPLE_RT >::get(static_cast<_TYPLE_RT&>(t)); } };

	// Внешняя функция для пользователя
	template <size_t N, typename T1, typename T2, typename T3, typename T4, typename T5>
	typename _TupleRetT<N, _TYPLE_T >::return_type& get(_TYPLE_T& t){ return _TupleRetT<N, _TYPLE_T >::get(t); }
#undef _TYPLE_T
#undef _TYPLE_RT
//#undef _TYPLE_ARGS
#endif
}
