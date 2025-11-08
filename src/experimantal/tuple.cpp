namespace ncpp{ 
	struct NullType {}; // Тип-заглушка для конца списка
	// Базовый класс для пустого кортежа
	template <class T, class Rest> struct Typle; // Основной шаблон (без реализации)
	template <> struct Typle<NullType, NullType> {}; // Пустой кортеж, ничего не хранит
	
	template <typename T1, typename T2, typename T3> struct Typle {}; // заглушка для 3-х типов
	template <typename T1, typename T2, typename T3, typename T4> struct Typle {}; // заглушка для 4-х типов
	template <typename T1, typename T2, typename T3, typename T4, typename T5> struct Typle {}; // заглушка для 5-х типов
}
