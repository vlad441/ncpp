[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::hash](#ncpphash)
- [ncpp::HashMap](#ncpphashmap)
	- [HashMap.begin()](#hashmapbegin)
	- [HashMap.end()](#hashmapend)
	- [HashMap.empty()](#hashmapempty)
	- [HashMap.size()](#hashmapsize)
	- [HashMap.clear()](#hashmapclear)
	- [HashMap.insert()](#hashmapinsert)
	- [HashMap.find()](#hashmapfind)
	- [HashMap.erase()](#hashmaperase)
	- [HashMap.keys()](#hashmapkeys)
	- [HashMap.has()](#hashmaphas)
	- [HashMap.assign()](#hashmapassign)
	- [HashMap.cout()](#hashmapcout)
	- [HashMap.toJSON()](#hashmapjson)
	- [HashMap.toString()](#hashmaptostring)
	- [HashMap.type()](#hashmaptype)
	- [HashMap.at()](#hashmapat)
	
- [ncpp::HashSet](#ncpphashset)

## ncpp::hash
Набор перегруженных функций для вычисления хэша различных типов данных. Использует алгоритм **Fnv1a** для строк и буферов.

```cpp
uint32_t hash(T num);           // Для примитивных типов
uint32_t hash(const char* c);   // Для C-строк
uint32_t hash(const String& s); // Для ncpp::String
```
## ncpp::HashMap
`HashMap` — хэш-таблица, хранящая пары ключ-значение. Использует открытую адресацию с линейным пробированием (Linear Probing). Является легковесной альтернативой `std::unordered_map<T>` (C++11).

```cpp
HashMap();
HashMap(const HashMap& m);
```

Определения:
```cpp
typedef HashMap<String, String> StringMap;
typedef HashMap<String, double> DoubleMap;
typedef HashMap<String, unsigned long long> ULLMap;
typedef HashMap<String, long long> LLMap;
```

### HashMap.begin()
```cpp
ConstIter begin() const;
Iter begin();
```
Возвращает итератор, указывающий на первый занятый элемент таблицы.

### HashMap.end()
```cpp
ConstIter end() const;
Iter end();
```
Возвращает итератор, указывающий на элемент, следующий за последним.

### HashMap.empty()
```cpp
bool empty() const;
```
Возвращает `true`, если таблица не содержит элементов.

### HashMap.size()
```cpp
size_t size() const;
```
Возвращает текущее количество элементов в таблице.

### HashMap.clear()
```cpp
void clear();
```
Очищает таблицу, освобождая память и сбрасывая размер в 0.

### HashMap.insert()
```cpp
Pair<Iter, bool> insert(const K& key, const V& value = V());
```
Вставляет пару ключ-значение. Если ключ уже существует, вставка не производится, и возвращается `false` во втором поле пары.

### HashMap.find()
```cpp
Iter find(const K& key);
ConstIter find(const K& key) const;
```
Ищет элемент по ключу. Возвращает итератор на найденный элемент или `end()`, если ключ не найден.

### HashMap.erase()
```cpp
bool erase(const K& key);
Iter erase(Iter pos);
```
Удаляет элемент по ключу или по итератору.

### HashMap.keys()
```cpp
Array<K> keys() const;
```
Возвращает массив всех ключей, содержащихся в таблице.

### HashMap.has()
```cpp
bool has(const K& key) const;
```
Проверяет наличие ключа в таблице. Сокращение для `find(key) != end()`.

### HashMap.assign()
```cpp
static Derived& assign(Derived& target, const Derived& map2);
static Derived& assign(Derived& target, const Array<Derived>& maps);
```
Статические методы для копирования данных из одной или нескольких таблиц в целевую.

### HashMap.cout()
```cpp
String cout() const;
```
Возвращает форматированную строку с содержимым таблицы для отладочного вывода.

### HashMap.toJSON()
```cpp
String toJSON() const;
```
Сериализует содержимое таблицы в формат JSON-строки.

### HashMap.toString()
```cpp
String toString() const;
```
Аналог метода `cout()`.

### HashMap.type()
```cpp
String type() const;
```
Возвращает строковый идентификатор типа.

### HashMap.at()
```cpp
const V& at(const K& key) const;
```
Возвращает значение по ключу. Если ключ не найден, метод возвращает значение первого элемента таблицы.

## ncpp::HashSet
`HashSet` — хэш-таблица, хранящая только ключи. Является легковесной альтернативой `std::unordered_set<T>` (C++11).

```cpp
HashSet();
HashSet(const HashSet& s);
```

Все методы `HashSet` аналогичны `HashMap`.