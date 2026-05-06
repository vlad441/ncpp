[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Buffer](#ncppbuffer)
	- [Buffer.stack()](#bufferstack)
	- [Buffer.data()](#bufferdata)
	- [Buffer.begin()](#bufferbegin)
	- [Buffer.end()](#bufferend)
	- [Buffer.empty()](#bufferempty)
	- [Buffer.size()](#buffersize)
	- [Buffer.capacity()](#buffercapacity)
	- [Buffer.length()](#bufferlength)
	- [Buffer.front()](#bufferfront)
	- [Buffer.back()](#bufferback)
	- [Buffer.indexOf()](#bufferindexof)
	- [Buffer.includes()](#bufferincludes)
	- [Buffer.lastIndexOf()](#bufferlastindexof)
	- [Buffer.slice()](#bufferslice)
	- [Buffer.split()](#buffersplit)
	- [Buffer.splitTokens()](#buffersplittokens)
	- [Buffer.splitParts()](#buffersplitparts)
	- [Buffer.startsWith()](#bufferstartswith)
	- [Buffer.endsWith()](#bufferendswith)
	- [Buffer.toBool()](#buffertobool)
	- [Buffer.from()](#bufferfrom)
    - [Buffer.fromHex()](#bufferfromhex)
    - [Buffer.fromBase64()](#bufferfrombase64)
    - [Buffer.fromInt()](#bufferfromint)
    - [Buffer.assign()](#bufferassign)
	- [Buffer.reserve()](#bufferreserve)
	- [Buffer.resize()](#bufferresize)
	- [Buffer.insert()](#bufferinsert)
	- [Buffer.erase()](#buffererase)
	- [Buffer.push_back()](#bufferpush_back)
	- [Buffer.pop_back()](#bufferpop_back)
	- [Buffer.push_front()](#bufferpush_front)
    - [Buffer.pop_front()](#bufferpop_front)
	- [Buffer.clear()](#bufferclear)
	- [Buffer.shrink()](#buffershrink)
	- [Buffer.push()](#bufferpush)
    - [Buffer.pop()](#bufferpop)
    - [Buffer.fill()](#bufferfill)
    - [Buffer.reverse()](#bufferreverse)
    - [Buffer.read()](#bufferread)
    - [Buffer.write()](#bufferwrite)
    - [Buffer.readAny()](#bufferreadany)
    - [Buffer.writeAny()](#bufferwriteany)
    - [Buffer.BitsRead()](#bufferbitsread)
    - [Buffer.BitsWrite()](#bufferbitswrite)
    - [Buffer.readT_BE()](#bufferreadt_be)
    - [Buffer.readT_LE()](#bufferreadt_le)
    - [Buffer.writeT_BE()](#bufferwritet_be)
    - [Buffer.writeT_LE()](#bufferwritet_le)
    - [Buffer.readInt16BE()](#bufferreadint16be)
    - [Buffer.readInt16LE()](#bufferreadint16le)
    - [Buffer.readUInt16BE()](#bufferreaduint16be)
    - [Buffer.readUInt16LE()](#bufferreaduint16le)
    - [Buffer.writeInt16BE()](#bufferwriteint16be)
    - [Buffer.writeInt16LE()](#bufferwriteint16le)
    - [Buffer.writeUInt16BE()](#bufferwriteuint16be)
    - [Buffer.writeUInt16LE()](#bufferwriteuint16le)
    - [Buffer.readInt32BE()](#bufferreadint32be)
    - [Buffer.readInt32LE()](#bufferreadint32le)
    - [Buffer.readUInt32BE()](#bufferreaduint32be)
    - [Buffer.readUInt32LE()](#bufferreaduint32le)
    - [Buffer.writeInt32BE()](#bufferwriteint32be)
    - [Buffer.writeInt32LE()](#bufferwriteint32le)
    - [Buffer.writeUInt32BE()](#bufferwriteuint32be)
    - [Buffer.writeUInt32LE()](#bufferwriteuint32le)
    - [Buffer.readInt64BE()](#bufferreadint64be)
    - [Buffer.readInt64LE()](#bufferreadint64le)
    - [Buffer.readUInt64BE()](#bufferreaduint64be)
    - [Buffer.readUInt64LE()](#bufferreaduint64le)
    - [Buffer.writeInt64BE()](#bufferwriteint64be)
    - [Buffer.writeInt64LE()](#bufferwriteint64le)
    - [Buffer.writeUInt64BE()](#bufferwriteuint64be)
    - [Buffer.writeUInt64LE()](#bufferwriteuint64le)
    - [Buffer.readDoubleBE()](#bufferreaddoublebe)
    - [Buffer.readDoubleLE()](#bufferreaddoublele)
    - [Buffer.writeDoubleBE()](#bufferwritedoublebe)
    - [Buffer.writeDoubleLE()](#bufferwritedoublele)
    - [Buffer::BitsToInt()](#bufferbitsint)
    - [Buffer::IntToBits()](#bufferinttobits)
    - [Buffer.concat()](#bufferconcat)
    - [Buffer.randBytes()](#bufferrandbytes)
    - [Buffer::base64_encode()](#bufferbase64_encode)
    - [Buffer::base64_decode()](#bufferbase64_decode)
    - [Buffer::increment()](#bufferincrement)
    - [Buffer::decrement()](#bufferdecrement)
    - [Buffer::removeLeadingZeros()](#bufferremoveleadingzeros)
    - [Buffer::removeLastZeros()](#bufferremovelastzeros)
    - [Buffer.toInt()](#buffertoint)
    - [Buffer.toInt64()](#buffertoint64)
    - [Buffer.toString()](#buffertostring)
    - [Buffer.toHex()](#buffertohex)
    - [Buffer.toBase64()](#buffertobase64)
    - [Buffer.toIntString()](#buffertointstring)
    - [Buffer.cout()](#buffercout)
    - [Buffer::htoc()](#bufferhtoc)
    - [Buffer::ctoh()](#bufferctoh)
    - [Buffer::htoll()](#bufferhtoll)
    - [Buffer::lltoh()](#bufferlltoh)
    - [Buffer.swap()](#bufferswap)
    - [Buffer::move()](#buffermove)
    - [Buffer.steal()](#buffersteal)
	
## ncpp::Buffer
`Buffer` — контейнер необработанных байт, структура данных идентична массиву `unsigned char` (`Array<unsigned char>`). Использует стандартный С аллокатор (`malloc`/`realloc`/`free`).

```cpp
Buffer(size_t len=0);
Buffer(size_t len, unsigned char v);
Buffer(const void* ptr, size_t len);
Buffer(const char* cptr);
Buffer(const Buffer& b);
Buffer(const void* begin, const void* end);
template <size_t N>
Buffer(const unsigned char (&arr)[N]);
template <typename T, typename D>
Buffer(const BaseString<T, D>& s);
```

Определения типов класса:
```cpp
typedef unsigned char* Iter;
typedef const unsigned char* CIter;
typedef CIter ConstIter;
typedef Iter iterator;
typedef CIter const_iterator;
#define NPOS (size_t)-1
```
	
### Buffer.stack()
```cpp
template <typename A, size_t N>
Buffer& stack(A (&arr)[N], bool ronly=false);
template <typename A>
Buffer& stack(A* ptr, size_t len, bool ronly=false);
```
Позволяет использовать внешний буфер для этого класса.
`HEAP` - стандартный аллокатор, `STACK` - использование внешнего буфера, `STACK_ONLY` - запрет на аллокацию в куче.
Параметр `ronly` отвечает за запрет реаллокаций, при значении `true` будет установлен режим `STACK_ONLY`.

### Buffer.data()
```cpp
const unsigned char* data() const;
```
Возвращает указатель на начало буфера.

### Buffer.begin()
```cpp
const unsigned char* begin() const;
```
Возвращает итератор (указатель) на первый элемент.

### Buffer.end()
```cpp
const unsigned char* end() const;
```
Возвращает итератор (указатель) на конец. (позиция за последним элементом в массиве)

### Buffer.empty()
```cpp
bool empty() const;
```
Проверяет на наличие байт.

### Buffer.size()
```cpp
size_t size() const;

```
Возвращают текущий логический размер (кол-во байт) буфера.

### Buffer.capacity()
```cpp
size_t capacity() const;
```
Возвращает размер выделенной памяти (буфера).

### Buffer.indexOf()
```cpp
size_t indexOf(const char* delim, size_t len, size_t start) const;
size_t indexOf(const char* delim, size_t start=0) const;
size_t indexOf(char c, size_t start=0) const;
template <typename U, typename D> 
size_t indexOf(const BaseString<U, D>& s, size_t start=0) const;

```
Поиск первого вхождения подстроки или символа.
Возвращает индекс начала вхождения или `(size_t)-1` (который `NPOS`), если ничего не найдено. Параметр start задает позицию, с которой начинать поиск.

### Buffer.includes()
```cpp
bool includes(const char* c, size_t start=0) const;
bool includes(char c, size_t start=0) const;
template <typename U, typename D> 
bool includes(const BaseString<U, D>& s, size_t start=0) const;
```
Проверяет, содержит ли буфер указанный символ или подстроку. Возвращает `true`, если вхождение найдено.

### Buffer.lastIndexOf()
```cpp
size_t lastIndexOf(const char* delim, size_t len, size_t start=NPOS) const;
size_t lastIndexOf(const char* delim) const;
size_t lastIndexOf(char c) const;
```
Поиск последнего вхождения подстроки или символа. Просматривает буфер справа налево. Возвращает индекс вхождения или `(size_t)-1` (который `NPOS`).

### Buffer.slice()
```cpp
template <typename V>
V _slice(int start, int end = 0) const;
Buffer slice(int start, int end = 0) const;
```
Возвращает новый буфер, содержащий часть исходной. Поддерживает отрицательные индексы (отсчет с конца буфера). 
Если `end` равен 0 или не указан, копируется часть до конца буфера.

### Buffer.split()
```cpp
template <typename V>
Array<V> _split(const char* delim, size_t len, int maxparts=-1) const;
Array<Buffer> split(const char* delim, size_t len, int maxparts=-1) const;
Array<Buffer> split(const char* delim) const;
Array<Buffer> split(char delim) const;
template <typename U, typename D>
Array<Buffer> split(const BaseString<U, D>& s) const;
```
Разбивает буфер на массив `Array<Buffer>` по указанному разделителю.

### Buffer.splitTokens()
```cpp
template <typename V>
Array<V> _splitTokens(int limit=-1, const char* chars = " \t\r\n\f\v") const
Array<Buffer> splitTokens(int limit = -1, const char* chars = " \t\r\n\f\v") const;
```
Разбивает буфер на токены, используя любой из символов в `chars` как разделитель.
По умолчанию использует пробельные символы. Параметр `limit` ограничивает количество извлекаемых токенов.

### Buffer.splitParts()
```cpp
template <typename V>
Array<V> _splitParts(int partSz) const;
Array<Buffer> splitParts(int partSz) const;
```
Разрезает буфер на равные части размером `partSz`. Последняя часть может быть короче остальных.

### Buffer.startsWith()
```cpp
bool startsWith(const char* prefix, size_t len) const
bool startsWith(const char* prefix) const;
template <typename U, typename D>
bool startsWith(const BaseString<U, D>& prefix) const;
```
Проверяет, начинается ли буфер с указанного префикса.

### Buffer.endsWith()
```cpp
bool endsWith(const char* suffix, size_t len) const;
bool endsWith(const char* suffix) const;
template <typename U, typename D>
bool endsWith(const BaseString<U, D>& suffix) const;
```
Проверяет, заканчивается ли буфер указанным суффиксом.

### Buffer.toBool()
```cpp
bool toBool();
```
Пытается интерпретировать содержимое буфера как логическое значение. Возвращает `true`, если буфер равнен `"true"`, `"1"` или `"True"`.

### Buffer.from()
```cpp
static Buffer from(const CString& cs);
static Buffer from(const CString& str, const CString& type);
```
Статические методы для создания буфера из строки. Параметр `type` определяет способ интерпретации строки:
`"str"` (по умолчанию) — прямое копирование символов.
`"hex"` — декодирование из Hex-строки.
`"base64"` — декодирование из Base64.
`"int"` / `"number"` — создание буфера, представляющего число из десятичной строки.

### Buffer.fromHex()
```cpp
static Buffer fromHex(const char* hex, size_t len);
static Buffer fromHex(const CString& hex);
```
Частный случай `from()`. Создает буфер, декодируя шестнадцатеричную строку.

### Buffer.fromBase64()
```cpp
static Buffer fromBase64(const CString& cs);
```
Частный случай `from()`. Создает буфер, декодируя строку формата Base64.

### Buffer.fromInt()
```cpp
static Buffer fromInt(unsigned long long numb);
static Buffer fromInt(const CString& cs);
```
Частный случай `from()`. Создает буфер из целого числа или числовой строки. Результат представляет число в Big-Endian формате с удаленными ведущими нулями.

### Buffer.assign()
```cpp
void assign(const void* begin, const void* end);
```
Заменяет содержимое буфера данными из диапазона памяти.

### Buffer.reserve()
```cpp
Buffer& reserve(size_t sz);
```
Резервирует память как минимум под `sz` символов (байт). Если текущая емкость (`capacity`) уже больше или равна `sz`, ничего не происходит. 
При переаллокации в режиме `HEAP` объем памяти обычно удваивается для оптимизации будущих вставок.

### Buffer.resize()
```cpp
Buffer& resize(size_t len);
```
Изменяет логический размер буфера. Если новый размер `len` больше `capacity` - происходит `reserve()`. 
Если `len` меньше `capacity` - размер выделенной памяти остается без изменений, зануление не производится.

### Buffer.insert()
```cpp
void insert(size_t pos, const void* dptr, size_t len);
void insert(size_t pos, size_t n, unsigned char c);
void insert(size_t pos, const char* cstr);
void insert(size_t pos, unsigned char v);
void insert(size_t pos, const Buffer& bf);
```
Вставляет данные в указанную позицию.

### Buffer.erase()
```cpp
Buffer& erase(size_t pos=0, size_t len = -1);
void erase(Iter first, Iter last);
void erase(Iter ipos);
```
Удаляет байты из буфера. Можно указать диапазон по индексу и длине или с помощью итераторов.

### Buffer.push_back()
```cpp
void push_back(unsigned char v);
```
Алиас для `push()`

### Buffer.pop_back()
```cpp
void pop_back();
```
Алиас для `pop()`

### Buffer.push_front()
```cpp
void push_front(unsigned char v);
```
Добавляет байт в начало буфера, сложность `O(n)`.

### Buffer.pop_front()
```cpp
void pop_front();
```
Удаляет первый байт из буфера, сложность `O(n)`.

### Buffer.clear()
```cpp
Buffer& clear();
```
Очищает буфер, устанавливая его логический размер (длину) в `0`. Память при этом не освобождается (см. `shrink()`).

### Buffer.shrink()
```cpp
Buffer& shrink();
void shrink_to_fit();
```
Освобождает неиспользуемую зарезервированную память, подгоняя `capacity` под текущий `size`. (Работает только в режиме `HEAP`)

### Buffer.push()
```cpp
template <size_t N>
Buffer& push(const unsigned char (&arr)[N]);
Buffer& push(const void* ptr, size_t len);
Buffer& push(const Buffer& buff);
Buffer& push(const String& s);
Buffer& push(const CString& cs);
Buffer& push(unsigned char v);
```
Добавляет данные в конец буфера.

### Buffer.pop()
```cpp
unsigned char pop();
```
Удаляет (уменьшает размер буфера на `1`) символ из конца буфера и возвращает его.

### Buffer.fill()
```cpp
Buffer& fill(unsigned char v);
```
Заполняет весь текущий размер буфера указанным значением.

### Buffer.reverse()
```cpp
void reverse();
```
Разворачивает порядок байт в буфере.

### Buffer.read()
```cpp
void read(void* dst, size_t size, size_t offset=0);
```
Копирует `size` байт из буфера (начиная с `offset`) в целевой указатель `dst`.

### Buffer.write()
```cpp
void write(const void* src, size_t size, size_t offset);
void write(const char* cstr, size_t offset=0);
void write(const Buffer& buff, size_t offset=0);
template <typename T, typename D>
void write(const BaseString<T, D>& s, size_t offset=0);
```
Записывает данные в буфер по указанному смещению. Если данных больше, чем текущий размер, размер будет автоматически расширен.

### Buffer.readAny()
```cpp
template <typename T>
T readAny(size_t offset=0);
```
Читает значение произвольного типа `T` по смещению. Не принимает указатели.

### Buffer.writeAny()
```cpp
template <typename T>
void writeAny(const T& data, size_t offset=0);
```
Записывает значение произвольного типа `T` по смещению.

### Buffer.BitsRead()
```cpp
Buffer BitsRead(size_t offset=0) const;
static Buffer BitsRead(const void* ptr);
```
Читает 8 бит (1 байт) и возвращает буфер из 8 элементов, где каждый элемент равен 0 или 1.

### Buffer.BitsWrite()
```cpp
void BitsWrite(const Buffer& bits, size_t offset=0);
static void BitsWrite(void* ptr, const Buffer& bits);
```
Записывает биты из буфера (массив нулей и единиц) в один байт по указанному адресу/смещению.

### Buffer.readT_BE()
```cpp
template <typename T>
T readT_BE(size_t offset=0, int length=sizeof(T)) const;
```
Метод чтения типизированных данных в `BE` (Big-Endian) порядке байт.

### Buffer.readT_LE()
```cpp
template <typename T>
T readT_LE(size_t offset=0, int length=sizeof(T)) const;
```
Метод чтения типизированных данных в `LE` (Little-Endian) порядке байт.

### Buffer.writeT_BE()
```cpp
template <typename T>
void writeT_BE(T value, size_t offset=0, int length=sizeof(T));
```
Запись типизированных данных в `BE` (Big-Endian) порядке байт.

### Buffer.writeT_LE()
```cpp
template <typename T>
void writeT_LE(T value, size_t offset=0, int length=sizeof(T));
```
Запись типизированных данных в `LE` (Little-Endian) порядке байт.

### Buffer.readInt16BE()
```cpp
int readInt16BE(size_t offset=0) const;
```
-

### Buffer.readInt16LE()
```cpp
int readInt16LE(size_t offset=0) const;
```
-

### Buffer.readUInt16BE()
```cpp
unsigned int readUInt16BE(size_t offset=0) const;
```
-

### Buffer.readUInt16LE()
```cpp
unsigned int readUInt16LE(size_t offset=0) const;
```
-

### Buffer.writeInt16BE()
```cpp
void writeInt16BE(int value, size_t offset=0);
```
-

### Buffer.writeInt16LE()
```cpp
void writeInt16LE(int value, size_t offset=0);
```
-

### Buffer.writeUInt16BE()
```cpp
void writeUInt16BE(unsigned int value, size_t offset=0);
```
-

### Buffer.writeUInt16LE()
```cpp
void writeUInt16LE(unsigned int value, size_t offset=0);
```
-

### Buffer.readInt32BE()
```cpp
int32_t readInt32BE(size_t offset=0) const;
```
-

### Buffer.readInt32LE()
```cpp
int32_t readInt32LE(size_t offset=0) const;
```
-

### Buffer.readUInt32BE()
```cpp
uint32_t readUInt32BE(size_t offset=0) const;
```
-

### Buffer.readUInt32LE()
```cpp
uint32_t readUInt32LE(size_t offset=0) const;
```
-

### Buffer.writeInt32BE()
```cpp
void writeInt32BE(int value, size_t offset=0);
```
-

### Buffer.writeInt32LE()
```cpp
void writeInt32LE(int value, size_t offset=0);
```
-

### Buffer.writeUInt32BE()
```cpp
void writeUInt32BE(unsigned int value, size_t offset=0);
```
-

### Buffer.writeUInt32LE()
```cpp
void writeUInt32LE(unsigned int value, size_t offset=0);
```
-

### Buffer.readInt64BE()
```cpp
long long readInt64BE(size_t offset=0) const;
```
-

### Buffer.readInt64LE()
```cpp
long long readInt64LE(size_t offset=0) const;
```
-

### Buffer.readUInt64BE()
```cpp
unsigned long long readUInt64BE(size_t offset=0) const;
```
-

### Buffer.readUInt64LE()
```cpp
unsigned long long readUInt64LE(size_t offset=0) const;
```
-

### Buffer.writeInt64BE()
```cpp
void writeInt64BE(long long value, size_t offset=0);
```
-

### Buffer.writeInt64LE()
```cpp
void writeInt64LE(long long value, size_t offset=0);
```
-

### Buffer.writeUInt64BE()
```cpp
void writeUInt64BE(unsigned long long value, size_t offset=0);
```
-

### Buffer.writeUInt64LE()
```cpp
void writeUInt64LE(unsigned long long value, size_t offset=0);
```
-

### Buffer.readDoubleBE()
```cpp
double readDoubleBE(size_t offset=0) const;
```
-

### Buffer.readDoubleLE()
```cpp
double readDoubleLE(size_t offset=0) const;
```
-

### Buffer.writeDoubleBE()
```cpp
void writeDoubleBE(double value, size_t offset=0);
```
-

### Buffer.writeDoubleLE()
```cpp
void writeDoubleLE(double value, size_t offset=0);
```
-

### Buffer.BitsToInt()
```cpp
static int BitsToInt(const void* bits, size_t len);
static int BitsToInt(const Buffer& bits);
```
Преобразует массив бит (где каждый элемент равен 0 или 1) в целое число `int`.

### Buffer.IntToBits()
```cpp
static void IntToBits(void* dst, int numb, int bitlen);
static Buffer IntToBits(int numb, size_t bitlen=sizeof(int)*8);
```
Преобразует целое число в массив бит (0 и 1).

### Buffer.concat()
```cpp
static Buffer concat(const Array<Buffer>& buffs);
static Buffer concat(const Buffer& buff1, const Buffer& buff2);
```
Объединяет несколько буферов в один новый.

### Buffer.randBytes()
```cpp
static Buffer randBytes(int length);
```
Генерирует случайные байты заданной длины. Используется быстрый, криптографически нестойкий алгоритм. **Не рекомендуется** для криптографии.

### Buffer::base64_encode()
```cpp
static String base64_encode(unsigned char const* bptr, size_t len);
```
Статический метод для кодирования сырых байт в строку формата `Base64`.

### Buffer::base64_decode()
```cpp
static Buffer base64_decode(const CString& encoded_string);
```
Статический метод для декодирования строки `Base64` обратно в `Buffer`.

### Buffer::increment()
```cpp
static void increment(Buffer& buff, unsigned int value=1, bool insert = false);
```
Трактует содержимое буфера как большое число в формате Big-Endian и увеличивает его на `value`.
Если `insert` равен `true`, при переполнении размер буфера будет увеличен (добавлен старший байт).

### Buffer::decrement()
```cpp
static void decrement(Buffer& buff, unsigned int value=1, bool erase = false);
```
Трактует содержимое буфера как большое число в формате Big-Endian и уменьшает его на `value`.
Если `erase` равен `true`, ведущие нулевые байты будут удалены после операции.

### Buffer::removeLeadingZeros()
```cpp
static void removeLeadingZeros(Buffer& buff);
```
Удаляет ведущие нулевые байты в начале буфера. Оставляет хотя бы один байт, если весь буфер состоит из нулей.

### Buffer::removeLastZeros()
```cpp
static void removeLastZeros(Buffer& buff);
```
Удаляет нулевые байты в конце буфера.

### Buffer.toInt()
```cpp
unsigned int toInt() const noexcept;
```
Интерпретирует первые байты буфера (до размера `int`) как целое число в формате Big-Endian и возвращает его.

### Buffer.toInt64()
```cpp
unsigned long long toInt64() const noexcept;
```
Интерпретирует первые байты буфера (до размера `long long`) как 64-битное целое число в формате Big-Endian.

### Buffer.toString()
```cpp
String toString() const;
String toString(const CString& type) const;
```
Преобразует содержимое буфера в строку. Параметр `type` позволяет выбрать формат: `"hex"`, `"base64"`, `"int"/"dec"`, (десятичное число) или `"raw"`.

### Buffer.toHex()
```cpp
String toHex() const;
```
Преобразует содержимое буфера в шестнадцатеричную строку (hex).

### Buffer.toBase64()
```cpp
String toBase64() const;
```
Преобразует содержимое буфера в строку формата Base64.

### Buffer.toIntString()
```cpp
String toIntString() const;
```
Интерпретирует буфер как очень большое число (BigInt) и возвращает его десятичное строковое представление.

### Buffer.cout()
```cpp
String cout(size_t osize=50) const;
```
Возвращает отладочную строку вида `<Buffer 00 1a 2b ...>`, содержащую байты в виде hex. Параметр `osize` ограничивает количество выводимых байт.

### Buffer::htoc()
```cpp
static unsigned char htoc(const char hex[2]);
```
Преобразует два hex-символа в один байт.

### Buffer::ctoh()
```cpp
static void ctoh(char hex[2], unsigned char value);
```
Преобразует байт в два hex-символа.

### Buffer::htoll()
```cpp
static unsigned long long htoll(const char* hex);
static unsigned long long htoll(const String& str);
```
Преобразует hex-строку в число `unsigned long long`.

### Buffer::lltoh()
```cpp
static String lltoh(unsigned long long value);
```
Преобразует число `unsigned long long` в hex-строку.

### Buffer.swap()
```cpp
void swap(Buffer& other);
friend void swap(Buffer& a, Buffer& b);
```
Меняет содержимое двух массивов местами (путем обмена указателями и метаданными), избегая копирования.

### Buffer::move()
```cpp
friend void move(Buffer& dst, Buffer& tmp);
```
C++11:
```cpp
friend void move(Buffer& dst, Buffer&& tmp);
```
C++98:
```cpp
friend void move(Buffer& dst, const Buffer& victim);
```
Реализация семантики перемещения. Позволяет передать владение ресурсами от одного объекта (жертвы) другому, оставляя исходный объект пустым, но валидным.
В C++11 и выше используются стандартные `rvalue` ссылки.

### Buffer.steal()
C++11:
```cpp
Buffer& steal(Buffer& tmp);
Buffer& steal(Buffer&& tmp);
```
C++98:
```cpp
Buffer& steal(const Buffer& victim);
```
Метод-обертка для `move()`. "Крадет" (перемещает) содержимое `victim` и зануляет его, сохраняя валидным.