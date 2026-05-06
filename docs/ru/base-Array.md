[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Array](#ncpparray)
	- [Array.stack()](#arraystack)
	- [Array.data()](#arraydata)
	- [Array.begin()](#arraybegin)
	- [Array.end()](#arrayend)
	- [Array.empty()](#arrayempty)
	- [Array.size()](#arraysize)
	- [Array.capacity()](#arraycapacity)
	- [Array.reserve()](#arrayreserve)
	- [Array.resize()](#arrayresize)
    - [Array.assign](#arrayassign)
    - [Array.insert](#arrayinsert)
    - [Array.erase](#arrayerase)
    - [Array.push_back](#arraypush_back)
    - [Array.pop_back](#arraypop_back)
    - [Array.push_front](#arraypush_front)
    - [Array.pop_front](#arraypop_front)
    - [Array.front](#arrayfront)
    - [Array.back](#arrayback)
    - [Array.clear](#arrayclear)
    - [Array.shrink](#arrayshrink)
    - [Array.indexOf](#arrayindexof)
    - [Array.fill](#arrayfill)
    - [Array.splice](#arraysplice)
    - [Array.push()](#arraypush)
    - [Array.pop()](#arraypop)
    - [Array.concat](#arrayconcat)
    - [Array.reverse](#arrayreverse)
    - [Array.join](#arrayjoin)
    - [Array.cout](#arraycout)
    - [Array.swap](#arrayswap)
    - [Array::move](#arraymove)
    - [Array.steal](#arraysteal)
	
## ncpp::Array
`Array<T>` — динамический массив, являющийся легковесной альтернативой `std::vector<T>`. Использует стандартный С аллокатор (`malloc`/`realloc`/`free`).

```cpp
Array(size_t n=0, const T& val=T());
Array(const Array<T>& arr);
Array(const T* begin, const T* end);
template <size_t N> Array(const T (&arr)[N]);
```

Определения типов класса:
```cpp
typedef T* Iter;
typedef const T* CIter; 
typedef CIter ConstIter; 
typedef Iter iterator; 
typedef CIter const_iterator;
```

### Array.stack()
```cpp
template <typename A, size_t N> Array<T>& stack(A (&arr)[N], bool ronly=false);
template <typename A> Array<T>& stack(A* ptr, size_t len, bool ronly=false);
```
Позволяет использовать внешний буфер для массива.
`HEAP` - стандартный аллокатор, `STACK` - использование внешнего буфера, `STACK_ONLY` - запрет на аллокацию в куче.
Параметр `ronly` отвечает за запрет реаллокаций, при значении `true` будет установлен режим `STACK_ONLY`.

### Array.data()
```cpp
T* data() const;
```
Возвращает указатель на первый элемент. Если массив пуст - вернет `NULL`.

### Array.begin()
```cpp
T* begin() const;
```
Возвращает итератор (указатель) на первый элемент.

### Array.end()
```cpp
T* end() const;
```
Возвращает итератор (указатель) на конец. (позиция за последним элементом в массиве)

### Array.empty()
```cpp
bool empty() const;
```
Проверяет на наличие элементов.

### Array.size()
```cpp
size_t size() const;
```
Возвращает кол-во элементов в массиве.

### Array.capacity()
```cpp
size_t capacity() const;
```
Возвращает кол-во элементов, под которые зарезервирована память.

### Array.reserve()
```cpp
Array<T>& reserve(size_t len)
```
Резервирует память как минимум под `len` элементов. Если текущая емкость (`capacity`) уже больше или равна `len`, ничего не происходит. 
При переаллокации в режиме `HEAP` объем памяти обычно удваивается для оптимизации будущих вставок.

### Array.resize()
```cpp
Array<T>& resize(size_t len, const T& val = T())
```
Изменяет логический размер массива. Если новый размер больше текущего, в конец добавляются элементы со значением `val`. Если меньше — лишние элементы удаляются.

### Array.assign()
```cpp
void assign(const T* begin, const T* end);
void assign(size_t n, const T& val);
```
Присваивает новое содержимое массива. В первом варианте копирует элементы из диапазона `[begin, end]`. Во втором — заполняет массив `n` копиями значения `val`.

### Array.insert()
```cpp
void insert(Iter ipos, const T* dptr, size_t len);
void insert(Iter ipos, size_t n, const T& v);
void insert(Iter ipos, const T& v);
void insert(Iter ipos, const Array& other);
```
Вставляет элементы в позицию `ipos`. Если указатель `ipos` некорректен или равен `NULL`, вставка происходит в конец. 
Метод автоматически расширяет емкость массива и сдвигает существующие элементы вправо.

### Array.erase()
```cpp
Iter erase(Iter first, Iter last);
Iter erase(Iter ipos);
void erase(size_t pos, size_t count = 1);
```
Удаляет элементы из массива. Можно передать диапазон итераторов/конкретный итератор/индекс `pos` и количество элементов `count`. Оставшиеся элементы сдвигаются влево, логический размер `size` уменьшается.

### Array.push_back()
```cpp
void push_back(const T& val);
```
Алиас для `push()`. Добавляет элемент в конец массива.

### Array.pop_back()
```cpp
void pop_back();
```
Алиас для `pop()`. Удаляет последний элемент массива.

### Array.push_front()
```cpp
void push_front(const T& v);
```
Вставляет элемент в начало массива. Все существующие элементы сдвигаются вправо, сложность `O(n)`.

### Array.pop_front()
```cpp
void pop_front();
```
Удаляет первый элемент массива. Все последующие элементы сдвигаются влево, сложность `O(n)`.

### Array.front()
```cpp
T& front();
const T& front() const;
```
Возвращает ссылку на первый элемент массива.

### Array.back()
```cpp
T& back();
const T& back() const;
```
Возвращает ссылку на последний элемент массива.

### Array.clear()
```cpp
Array<T>& clear();
```
Очищает массив, вызывая деструкторы для всех элементов. Логический размер становится равным `0`, выделенная память `capacity` не освобождается.

### Array.shrink()
```cpp
Array<T>& shrink();
void shrink_to_fit();
```
Освобождает неиспользуемую зарезервированную память, подгоняя `capacity` под текущий `size`. (Работает только в режиме `HEAP`)

### Array.indexOf()
```cpp
size_t indexOf(const T& value, size_t start = 0) const;
```
Выполняет линейный поиск значения `value`, начиная с индекса `start`. Возвращает индекс первого найденного вхождения или `(size_t)-1` (который `NPOS`), если элемент не найден.

### Array.fill()
```cpp
void fill(const T& val);
```
Присваивает значение `val` всем существующим элементам массива.

### Array.splice()
```cpp
void splice(size_t pos, size_t count = 1);
```
Алиас для `erase()`. Удаляет `count` элементов, начиная с индекса `pos`.

### Array.push()
```cpp
Array<T>& push(const T (&arr)[N]);
Array<T>& push(const T* ptr, size_t len);
Array<T>& push(const T& val);
```
Вставляет элемент в конец массива.

### Array.pop()
```cpp
T pop();
```
Удаляет элемент из конца массива и возвращает его.

### Array.concat()
```cpp
Array<T>& concat(const Array<T>& arr2);
static Array<T> concat(const Array<T>& arr1, const Array<T>& arr2);
```
Объединяет массивы. Метод экземпляра добавляет `arr2` в конец текущего массива. 
Статический метод создает и возвращает новый массив, содержащий элементы обоих входных массивов.

### Array.reverse()
```cpp
void reverse();
```
Инвертирует порядок элементов в массиве.

### Array.join()
```cpp
template <typename U> T join(const U& delim) const;
```
Объединяет элементы массива в одну строку (или другой составной тип `T`), используя разделитель `delim`. Требует, чтобы тип `T` поддерживал оператор `+=`.

### Array.cout()
```cpp
String cout() const;
```
Возвращает строковое представление массива (обычно для целей отладки или вывода в консоль).

### Array.swap()
```cpp
void swap(Array& other);
friend void swap(Array& a, Array& b);
```
Меняет содержимое двух массивов местами (путем обмена указателями и метаданными), избегая копирования.

### Array::move()
```cpp
friend void move(Array& dst, Array& tmp);
```
C++11:
```cpp
friend void move(Array& dst, Array&& tmp);
```
C++98:
```cpp
friend void move(Array& dst, const Array& victim);
```
Реализация семантики перемещения. Позволяет передать владение ресурсами от одного объекта (жертвы) другому, оставляя исходный объект пустым, но валидным.
В C++11 и выше используются стандартные `rvalue` ссылки.

### Array.steal()
C++11:
```cpp
Array& steal(Array& tmp);
Array& steal(Array&& tmp);
```
C++98:
```cpp
Array& steal(const Array& victim);
```
Метод-обертка для `move()`. "Крадет" (перемещает) содержимое `victim` и зануляет его, сохраняя валидным.