[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::CString](#ncppcstring)
- [ncpp::String](#ncppstring)
	- [String.stack()](#stringstack)
	- [String.c_str()](#stringc_str)
	- [String.data()](#stringdata)
	- [String.begin()](#stringbegin)
	- [String.end()](#stringend)
	- [String.empty()](#stringempty)
	- [String.size()](#stringsize)
	- [String.capacity()](#stringcapacity)
	- [String.length()](#stringlength)
	- [String.front()](#stringfront)
	- [String.back()](#stringback)
	- find
	- find_first_of
	- find_last_of
	- find_first_not_of
	- find_last_not_of
	- [String.indexOf()](#stringindexof)
	- [String.includes()](#stringincludes)
	- [String.lastIndexOf()](#stringlastindexof)
	- [String.slice()](#stringslice)
	- [String.substr()](#stringsubstr)
	- [String.split()](#stringsplit)
	- [String.splitTokens()](#stringsplittokens)
	- [String.splitParts()](#stringsplitparts)
	- [String.startsWith()](#stringstartswith)
	- [String.endsWith()](#stringendswith)
	- [String.toBool()](#stringtobool)
	- [String.reserve()](#stringreserve)
	- [String.resize()](#stringresize)
	- [String.insert()](#stringinsert)
	- [String.erase()](#stringerase)
	- [String.push_back()](#stringpush_back)
	- [String.pop_back()](#stringpop_back)
	- [String.clear()](#stringclear)
	- [String.shrink()](#stringshrink)
	- [String.push()](#stringpush)
	- [String.pop()](#stringpop)
	- [String.fill()](#stringfill)
	- [String.replace()](#stringreplace)
	- [String.replaceAll()](#stringreplaceall)
	- [String.toLowerCase()](#stringtolowercase)
	- [String.toUpperCase()](#stringtouppercase)
	- [String.trim()](#stringtrim)
	- [String.swap()](#stringswap)
	- [String::move()](#stringmove)
	- [String.steal()](#stringsteal)

## ncpp::CString	
`CString` — реализация "просмотра" буфера строки, указываюзая на внешнюю память для чтения. Является альтернативой `std::string_view` (C++17).

```cpp
CString();
CString(const char* c, size_t len);
CString(const char* c);
CString(const String& s);
CString(const char* first, const char* end);
template <typename T, typename D>
CString(const BaseString<T, D>& s);
template <size_t N>
CString(const char (&arr)[N]);
```

## ncpp::String
`String` — контейнер строки, являющийся легковесной альтернативой `std::string`. Использует стандартный С аллокатор (`malloc`/`realloc`/`free`).

```cpp
String(size_t len=0);
String(size_t len, char v);
String(const char* ptr, size_t len);
String(const char* cptr);
String(const String& s);
String(const char* begin, const char* end);
template <typename T, typename D>
String(const BaseString<T, D>& s);
template <size_t N>
String(const char (&arr)[N]);
```

Определения типов класса:
```cpp
typedef char* Iter;
typedef const char* CIter;
typedef CIter ConstIter;
typedef Iter iterator;
typedef CIter const_iterator;
#define NPOS (size_t)-1
```
	
### String.stack()
```cpp
template <typename A, size_t N>
String& stack(A (&arr)[N], bool ronly=false);
template <typename A>
String& stack(A* ptr, size_t len, bool ronly=false);
```
Позволяет использовать внешний буфер для строки.
`HEAP` - стандартный аллокатор, `STACK` - использование внешнего буфера, `STACK_ONLY` - запрет на аллокацию в куче.
Параметр `ronly` отвечает за запрет реаллокаций, при значении `true` будет установлен режим `STACK_ONLY`.

### String.c_str()
```cpp
const char* c_str() const;
```
Возвращает указатель на начало строки. Гарантирует наличие нуль-терминатора в конце строки.

### String.data()
```cpp
const char* data() const;
```
Алиас для `c_str()`.

### String.begin()
```cpp
const char* begin() const;
```
Возвращает итератор (указатель) на первый элемент.

### String.end()
```cpp
const char* end() const;
```
Возвращает итератор (указатель) на конец. (позиция за последним элементом в массиве)

### String.empty()
```cpp
bool empty() const;
```
Проверяет на наличие символов.

### String.size()
```cpp
size_t size() const;

```
Возвращают текущий логический размер (кол-во символов) строки.

### String.capacity()
```cpp
size_t capacity() const;
```
Возвращает размер выделенной памяти (буфера).

### String.length()
```cpp
size_t length() const;
```
Алиас для `size()`.

### String.indexOf()
```cpp
size_t indexOf(const char* delim, size_t len, size_t start) const;
size_t indexOf(const char* delim, size_t start=0) const;
size_t indexOf(char c, size_t start=0) const;
template <typename U, typename D> 
size_t indexOf(const BaseString<U, D>& s, size_t start=0) const;

```
Поиск первого вхождения подстроки или символа. 
Возвращает индекс начала вхождения или `(size_t)-1` (который `NPOS`), если ничего не найдено. Параметр start задает позицию, с которой начинать поиск.

### String.includes()
```cpp
bool includes(const char* c, size_t start=0) const;
bool includes(char c, size_t start=0) const;
template <typename U, typename D> 
bool includes(const BaseString<U, D>& s, size_t start=0) const;
```
Проверяет, содержит ли строка указанный символ или подстроку. Возвращает `true`, если вхождение найдено.

### String.lastIndexOf()
```cpp
size_t lastIndexOf(const char* delim, size_t len, size_t start=NPOS) const;
size_t lastIndexOf(const char* delim) const;
size_t lastIndexOf(char c) const;
```
Поиск последнего вхождения подстроки или символа. Просматривает строку справа налево. Возвращает индекс вхождения или `(size_t)-1` (который `NPOS`).

### String.slice()
```cpp
template <typename V>
V _slice(int start, int end = 0) const;
String slice(int start, int end = 0) const;
```
Возвращает новую строку, содержащую часть исходной. Поддерживает отрицательные индексы (отсчет с конца строки). 
Если `end` равен 0 или не указан, копируется часть до конца строки.

### String.substr()
```cpp
String substr(int start, int end = 0) const;
```
Алиас для метода `slice()`.

### String.split()
```cpp
template <typename V>
Array<V> _split(const char* delim, size_t len, int maxparts=-1) const;
Array<String> split(const char* delim, size_t len, int maxparts=-1) const;
Array<String> split(const char* delim) const;
Array<String> split(char delim) const;
template <typename U, typename D>
Array<String> split(const BaseString<U, D>& s) const;
```
Разбивает строку на массив строк `Array<String>` по указанному разделителю.

### String.splitTokens()
```cpp
template <typename V>
Array<V> _splitTokens(int limit=-1, const char* chars = " \t\r\n\f\v") const
Array<String> splitTokens(int limit = -1, const char* chars = " \t\r\n\f\v") const;
```
Разбивает строку на токены, используя любой из символов в `chars` как разделитель.
По умолчанию использует пробельные символы. Параметр `limit` ограничивает количество извлекаемых токенов.

### String.splitParts()
```cpp
template <typename V>
Array<V> _splitParts(int partSz) const;
Array<String> splitParts(int partSz) const;
```
Разрезает строку на равные части размером `partSz`. Последняя часть может быть короче остальных.

### String.startsWith()
```cpp
bool startsWith(const char* prefix, size_t len) const
bool startsWith(const char* prefix) const;
template <typename U, typename D>
bool startsWith(const BaseString<U, D>& prefix) const;
```
Проверяет, начинается ли строка с указанного префикса.

### String.endsWith()
```cpp
bool endsWith(const char* suffix, size_t len) const;
bool endsWith(const char* suffix) const;
template <typename U, typename D>
bool endsWith(const BaseString<U, D>& suffix) const;
```
Проверяет, заканчивается ли строка указанным суффиксом.

### String.toBool()
```cpp
bool toBool();
```
Пытается интерпретировать содержимое строки как логическое значение. Возвращает `true`, если строка равна `"true"`, `"1"` или `"True"`.

### String.reserve()
```cpp
String& reserve(size_t sz);
```
Резервирует память как минимум под `sz` символов (байт). Если текущая емкость (`capacity`) уже больше или равна `sz`, ничего не происходит. 
При переаллокации в режиме `HEAP` объем памяти обычно удваивается для оптимизации будущих вставок.

### String.resize()
```cpp
String& resize(size_t len);
```
Изменяет логический размер строки. Если новый размер `len` больше `capacity` - происходит `reserve()`. 
Если `len` меньше `capacity` - размер выделенной памяти остается без изменений, зануление не производится.

### String.insert()
```cpp
void insert(size_t pos, const char* dptr, size_t len);
void insert(size_t pos, size_t n, char c);
void insert(size_t pos, const char* cstr);
void insert(size_t pos, char v);
void insert(size_t pos, const String& s);
```
Вставляет данные в указанную позицию.

### String.erase()
```cpp
String& erase(size_t pos=0, size_t len = -1);
void erase(Iter first, Iter last);
void erase(Iter ipos);
```
Удаляет символы из строки. Можно указать диапазон по индексу и длине или с помощью итераторов.

### String.push_back()
```cpp
void push_back(char v);
```
Алиас для `push()`

### String.pop_back()
```cpp
void pop_back();
```
Алиас для `pop()`

### String.clear()
```cpp
String& clear();
```
Очищает строку, устанавливая её логический размер (длину) в `0`. Память при этом не освобождается (см. `shrink()`).

### String.shrink()
```cpp
String& shrink();
void shrink_to_fit();
```
Освобождает неиспользуемую зарезервированную память, подгоняя `capacity` под текущий `size`. (Работает только в режиме `HEAP`)

### String.push()
```cpp
String& push(const char* ptr, size_t len);
template <size_t N> 
String& push(const char (&arr)[N]);
String& push(const String& s);
String& push(const CString& cs);
String& push(char c);
```
Добавляет данные в конец строки. или извлечения последнего символа с его возвратом.

### String.pop()
```cpp
char pop()
```
Удаляет (уменьшает размер строки на `1`) символ из конца строки и возвращает его.

### String.fill()
```cpp
String& fill(char c);
```
Заполняет текущую строку (весь размер `size`) указанным символом.

### String.replace()
```cpp
String replace(const CString& from, const CString& to, bool all = false) const;
```
Возвращает новую строку, в которой первое вхождение `from` заменено на `to`. При параметре `all` = `true` делает аналогично `replaceAll()`

### String.replaceAll()
```cpp
String replaceAll(const CString& from, const CString& to) const;
```
Возвращает новую строку, в которой все вхождения `from` заменены на `to`.

### String.toLowerCase()
```cpp
String toLowerCase() const;
```
Возвращает копию строки, приведенную к нижнему регистру. Работает только с символами ASCII.

### String.toUpperCase()
```cpp
String toUpperCase() const;
```
Возвращает копию строки, приведенную к верхнему регистру. Работает только с символами ASCII.

### String.trim()
```cpp
String trim() const;
```
Возвращает копию строки без ведущих и завершающих пробельных символов.

### String.swap()
```cpp
void swap(String& other);
friend void swap(String& a, String& b);
```
Меняет содержимое двух массивов местами (путем обмена указателями и метаданными), избегая копирования.

### String::move()
```cpp
friend void move(String& dst, String& tmp);
```
C++11:
```cpp
friend void move(String& dst, String&& tmp);
```
C++98:
```cpp
friend void move(String& dst, const String& victim);
```
Реализация семантики перемещения. Позволяет передать владение ресурсами от одного объекта (жертвы) другому, оставляя исходный объект пустым, но валидным.
В C++11 и выше используются стандартные `rvalue` ссылки.

### String.steal()
C++11:
```cpp
String& steal(String& tmp);
String& steal(String&& tmp);
```
C++98:
```cpp
String& steal(const String& victim);
```
Метод-обертка для `move()`. "Крадет" (перемещает) содержимое `victim` и зануляет его, сохраняя валидным.

### -
```cpp
-
```
-