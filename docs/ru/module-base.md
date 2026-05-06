[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [Array](base-Array.md) (≈ `std::vector<T>`)
- [String](base-String.md) (≈ `std::string`)
- [Buffer](base-Buffer.md) (≈ `Array<unsigned char>`)
- [HashMap](base-HashMap.md) (≈ `std::unordered_map<T>`)

- [ncpp::print](ncppprint)
- [ncpp::reverse()](#ncppreverse)
- [ncpp::min()](#ncppmin)
- [ncpp::max()](#ncppmax)
- [ncpp::swap()](#ncppswap)
- [ncpp::Pair](#ncpppair)
- [ncpp::onExcept()](#ncpponexcept)
- [ncpp::IsInt()](#ncppisint)
- [ncpp::IsFloat()](#ncppisfloat)
- [ncpp::IsHex()](#ncppishex)
- [ncpp::dtos()](#ncppdtos)
- [ncpp::isNaN()](#ncppisnan)
- [ncpp::stoin()](#ncppstoin)
- [ncpp::stolln()](#ncppstolln)
- [ncpp::stoulln()](#ncppstoulln)
- [ncpp::stodn()](#ncppstodn)
- [ncpp::stofn()](#ncppstofn)

## ncpp::print()
```cpp
void print(const char *cptr, size_t len);
void print(const char *cstr);
void print(const String& str);
void print(long long v);
```
Выводит текст в консоль. Поддерживает перегрузки для отображения `cout()` стандартных контейнеров.

### ncpp::reverse()
```cpp
template <typename T> void reverse(T* begin, T* end);
```
Инвертирует порядок элементов в диапазоне от `begin` до `end`.

### ncpp::min()
```cpp
template <typename N> N min(N num1, N num2);
```
Возвращает минимальное из двух значений.

### ncpp::max()
```cpp
template <typename N> N max(N num1, N num2);
```
Возвращает минимальное из двух значений.

### ncpp::swap()
```cpp
template <typename T> void swap(T& a, T& b);
```
Обменивает значения двух переменных.

### ncpp::Pair
```cpp
template <typename K, typename V> struct Pair { K first; V second; };
```
Простая структура для хранения пары связанных значений (ключ/значение).

### ncpp::onExcept()
```cpp
void (*onExcept)(const char* cstr, int errlvl, int type) = NULL;
```
Коллбек для обработки исключений.

### ncpp::IsInt()
```cpp
bool IsInt(const char* cstr, size_t len);
```
-

### ncpp::IsFloat()
```cpp
bool IsFloat(const char* cstr, size_t len);
```
-

### ncpp::IsHex()
```cpp
bool IsHex(const char* cstr, size_t len);
```
-

### ncpp::isNaN()
```cpp
char isNaN(double d);
```
Проверяет число на "не-число" (NaN) или бесконечность. Возвращает: 1 для `NaN`, 2 для `Inf`, 3 для `-Inf`, 0 в остальных случаях.

### ncpp::dtos()
```cpp
template<typename I> char dtos(char str[27], I num, char sep) noexcept;
template<typename I> String dtos(I num, char sep=0);
String dtos(double d, char precision=-1);
String dtos(float num, char precision=-1);
```
Преобразует число (целое или с плавающей запятой) в строку. Позволяет указывать разделитель разрядов (`sep`) для целых чисел или точность (`precision`) для дробных.

### ncpp::stoin()
```cpp
int stoin(const String& s);
```
Преобразует строку в целое число `int`.

### ncpp::stolln()
```cpp
long long stolln(const String& s);
```
Преобразует строку в целое число `long long`.

### ncpp::stoulln()
```cpp
unsigned long long stoulln(const String& s);
```
Преобразует строку в целое число `unsigned long long`.

### ncpp::stodn()
```cpp
double stodn(const String& s);
```
Преобразует строку в число с плавающей запятой `double`.

### ncpp::stofn()
```cpp
float stofn(const String& s);
```
Преобразует строку в число с плавающей запятой `float`.