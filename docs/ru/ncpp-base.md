[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::stolln()](#ncppstolln)
- [ncpp::print()](#ncppprint)
- [ncpp::dtos()](#ncppdtos)
- [ncpp::stoin()](#ncppstolln)
- [ncpp::stodn()](#ncppstolln)
- [ncpp::stofn()](#ncppstolln)
- [ncpp::GetTimestamp()](#ncppgettimestamp)
- [ncpp::Sleep()](#ncppsleep)
- [ncpp::IsInt()](#ncppisint)
- [ncpp::Err](#ncpperr)
- [ncpp::Buffer](#ncppbuffer)
	- [Buffer::from()](#bufferfrom)
	- [Buffer::fromHex()](#bufferfromhex)
	- [Buffer.indexOf()](#bufferindexof)
	- [Buffer.push()](#bufferpush)
	- [Buffer.split()](#buffersplit)
	- [Buffer.slice()](#bufferslice)
	- [Buffer.startsWith()](#bufferstartswith)
	- [Buffer.endsWith()](#bufferendswith)
	- [Buffer.fill()](#bufferfill)
	- [Buffer.concat()](#bufferconcat)
	- [Buffer.reverse()](#bufferreverse)
	- [Buffer.read()](#bufferread)
	- [Buffer.write()](#bufferwrite)
	- [Buffer.readAny()](#bufferreadany)
	- [Buffer.writeAny()](#bufferwriteAny)
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
	- [Buffer::BitsToInt()](#bufferbitstoint)
	- [Buffer::IntToBits()](#bufferinttobits)
	- [Buffer::randBytes()](#bufferrandbytes)
	static void removeLeadingZeros(Buffer& buff);
	struct Math;
	- [Buffer.toInt()](#buffertoint)
	- [Buffer.toInt64()](#buffertoint64)
	- [Buffer.toString()](#buffertostring)
	- [Buffer.toHexString()](#buffertohexstring)
	- [Buffer.toIntString()](#buffertointstring)
	- [Buffer.cout()](#buffercout)
	static unsigned char htoc(const char hex[2]);
	static void ctoh(unsigned char value, char hex[2]);
	static std::string itoh(unsigned int value);
	static std::string itoh(unsigned long long value);
- [ncpp::String](#ncppstring)
	- [String.indexOf()](#stringindexof)
	- [String.includes()](#stringincludes)
	- [String.back()](#stringback)
	- [String.split()](#stringsplit)
	- [String.slice()](#stringslice)
	- [String.startsWith()](#stringstartswith)
	- [String.endsWith()](#stringendswith)
	- [String.replace()](#stringreplace)
	- [String.replaceAll()](#stringreplaceall)
	- [String.toLowerCase()](#stringtolowercase)
	- [String.toUpperCase()](#stringtouppercase)
	- [String.trim()](#stringtrim)
- [ncpp::Array](#ncpparray)
	- [Array.push()](#arraypush)
	- [Array.pop()](#arraypop)
	- [Array.unshift()](#arrayunshift)
	- [Array.shift()](#arrayshift)
	- [Array.indexOf()](#arrayindexof)
	- [Array.slice()](#arrayslice)
	- [Array.splice()](#arraysplice)
	- [Array.concat()](#arrayconcat)
	- [Array.reverse()](#arrayreverse)
	- [Array.minsize()](#arrayminsize)
	- [Array.join()](#arrayjoin)
	- [Array.cout()](#arraycout)
- [ncpp::random()](#ncpprandom)
- [ncpp::randInt()](#ncpprandint)
- [ncpp::randStr()](#ncpprandstr)
- [ncpp::HashMap](#ncpphashmap)
	- [HashMap.keys()](#hashmapkeys)
	- [HashMap.has()](#hashmaphas)
	- [HashMap::assign()](#hashmapassign)
	- [HashMap.cout()](#hashmapcout)
	- [HashMap.toJSON()](#hashmaptojson)
	- [HashMap.toString()](#hashmaptostring)
	- [HashMap.type()](#hashmaptype)
- [ncpp::HashSet](#ncpphashset)
	- [HashSet.keys()](#hashsetkeys)
	- [HashSet.has()](#hashsethas)
	inline void erase_ptr(K key);
	void clear_ptr();
	- [HashSet.cout()](#hashsetcout)
	- [HashSet.toString()](#hashsettoString)
	- [HashSet.type()](#hashsettype)
- [ncpp::StringMap](#ncppstringmap)
- [ncpp::DoubleMap](#ncppdoublemap)
- [ncpp::Date](#ncppdate)
	- [Date::now()](#datenow)
	int getYear() const;
	int getMonth() const;
	int getDate() const;
	int getDay() const;
	int getDayWeek() const;
	- [Date.toDirectDate()](#datetodirectdate)
	- [Date.toReverseDate()](#datetoreversedate)
	- [Date.toString()](#datetostring)
	- [Date.toISOString()](#datetoisostring)

## ncpp::stolln()
```cpp
long long stolln(const std::string& str) noexcept;
```
Переобразует строчное представление числа в `int64`. В cлучае исключения возвращает **0**.

## ncpp::print()
```cpp
void print(const char *cptr, size_t len);
void print(const char *cstr);
void print(const std::string& str);
```
Выводит текст в консоль.

## ncpp::dtos()
```cpp
std::string dtos(double num, int precision=-1) noexcept;
template<typename T>
std::string dtos(T num) noexcept;
```
Переобразует число в строку.

## ncpp::stoin()
```cpp
int stoin(const std::string& str) noexcept;
```
Переобразует строчное представление числа в `int`. В cлучае исключения возвращает **0**.

## ncpp::stodn()
```cpp
double stodn(const std::string& str) noexcept;
```
Переобразует строчное представление числа в `double`. В cлучае исключения возвращает **0**.

## ncpp::stofn()
```cpp
float stofn(const std::string& str) noexcept;
```
Переобразует строчное представление числа в `float`. В cлучае исключения возвращает **0**.

## ncpp::GetTimestamp()
```cpp
long long GetTimestamp(char type='m');
```
Возвращает текущую метку времени в unix формате. `type`: s - секунды, m - миллисекунды, u - микросекунды.

## ncpp::Sleep()
```cpp
void Sleep(unsigned int msec);
```
Приостанавливает текущий поток на `msec` миллисекунд. На Windows реализация опускается, т.к. она уже объявлена в `windows.h`.
Под WinAPI есть полифил `usleep` из POSIX.




## ncpp::IsInt()
```cpp
bool IsInt(const std::string& str);
```
Проверяет, является ли строка представлением целого числа.

## ncpp::Err
Наследует от `std::exception`.
```cpp
Err();
Err(const std::string& msg, int errcode=0);
Err(int errcode);
```
Пользователький класс для исключений.
	

## ncpp::Buffer

`Buffer` наследует от `std::vector<unsigned char>`

```cpp
Buffer();
Buffer(size_t size, unsigned char value=0);
Buffer(const char* cstr);
Buffer(const void* ptr, size_t size);
Buffer(const std::string& str, const char* type="str")
Buffer(std::vector<unsigned char> vec);
```
Допустимые значения для `std::string type`: "str"|"string", "int", "hex", "base64";

### Buffer::from()
```cpp
static Buffer from(const std::string& str, const char* type="str");
static Buffer from(unsigned long long numb);
```
Статический метод, создающий новый экземпляр `Buffer` из входных данных этой функции.
Строковый вариант перегрузки вызывает конструктор `Buffer(const std::string& str, std::string type);`
Числовой вариант перегрузки копирует число в BE формате в новый буфер.

### Buffer::fromHex()
```cpp
static Buffer fromHex(const char* hexstr, size_t len=0);
static Buffer fromHex(const std::string& hexstr);
```
Создает новый экземпляр `Buffer`, в который записывает байты соответствующие шестнадцатеричному представлению, переданному в виде строки.

### Buffer.indexOf()
```cpp
long indexOf(const char* delim, size_t len, size_t start) const;
long indexOf(const char* delim, size_t start=0) const;
long indexOf(const Buffer& delim, size_t start=0) const;
```
Находит индекс первого вхождения по параметру разделителя, начиная с позиции `start`. В случае отсутствия совпадений возвращает **-1**.

### Buffer.push()
```cpp
void push(unsigned char value);
```
Вставляет байт в представлении `unsigned char` в конец.

### Buffer.split()
```cpp
Array<Buffer> split(const char* delim, size_t len=0) const;
Array<Buffer> split(const Buffer& delim) const;
```
Разбивает текущий буфер на части по разделителю.

### Buffer.slice()
```cpp
Buffer slice(int start, int end=0) const;
```
Возвращает подстроку начиная со `start` заканчивая `end`. Поддерживает отрицательные индексы, идентичен [аналогичному методу](https://developer.mozilla.org/ru/docs/Web/JavaScript/Reference/Global_Objects/String/slice) из JS.

### Buffer.startsWith()
```cpp
bool startsWith(const char* prefix, size_t len=0) const;
bool startsWith(const Buffer& prefix) const;
```
Проверяет, начинается ли буфер с указанного префикса.

### Buffer.endsWith()
```cpp
bool endsWith(const char* suffix, size_t len=0) const;
bool endsWith(const Buffer& suffix) const;
```
Проверяет, заканчивается ли буфер на указанный суффикс.

### Buffer.fill()
```cpp
void fill(unsigned char value);
```
Заполнить все байты буфера новым значением в представлении `unsigned char`.

### Buffer.concat()
```cpp
Buffer& concat(const Buffer& buff2);
static Buffer concat(const Array<Buffer>& buffs);
static Buffer concat(const Buffer& buff1, const Buffer& buff2);
```
Объединить (конкатенировать) новый буффер с текущим экземпляром буфера.
Статическая версия функции выполняет конкатенацию двух или массива буферов и возвращает новый объединенный буфер.

### Buffer.reverse()
```cpp
void reverse();
```
Инвертировать все байты в этом буфере.

### Buffer.read()
```cpp
void read(void* dst, size_t size, size_t offset=0);
```
Считывает `size` байт начиная с `offset` и записывает их начиная с позиции указателя `dst` в памяти.

### Buffer.write()
```cpp
void write(const void* src, size_t size, size_t offset);
void write(const char* cstr, size_t offset=0);
void write(const Buffer& buff, size_t offset=0);
void write(const std::string& str, size_t offset=0);
```
Базовый вариант сигнатуры функции записывает `size` байт начиная с `offset`, считывает их начиная с позиции указателя `src` в памяти.
Гарантирует что размер текущего буфера будет достаточен для записи, расширяя его при необходимости. Однако проверить безопасность пределов по указателю `src` невозможно.
Имеет перегрузки для более удобного использования.

### Buffer.readAny()
```cpp
template <typename T>
T readAny(size_t offset=0);
```
Шаблонная функция которая считывает из текущего буфера начиная с `offset` в размере `sizeof(T)`, записывает результат в `T` и возвращает его.

### Buffer.writeAny()
```cpp
template <typename T>
void writeAny(const T& data, size_t offset=0);
```
Шаблонная функция которая записывает в текущий буфер значение `T` начиная с `offset` в размере `sizeof(T)`.

### Buffer.BitsRead()
```cpp
Array<char> BitsRead(size_t offset=0) const;
static Array<char> BitsRead(const unsigned char* ptr);
static Array<char> BitsRead(const void* ptr);
```
Из текущего буфера считывает из байта по позиции `offset` биты, записывает их в  `Array<char>` и возвращает их.
Статическая версия функции считывает биты из произвольного байта по указателю.

### Buffer.BitsWrite()
```cpp
void BitsWrite(const std::vector<char>& bits, size_t offset=0);
static void BitsWrite(unsigned char* ptr, const std::vector<char>& bits);
static void BitsWrite(void* ptr, const std::vector<char>& bits);
```
Записывает в текущий буфер биты из `Array<char>` в байт по позиции `offset`.
Статическая версия функции записывает биты в произвольный байт по указателю.

### Buffer.readT_BE()
```cpp
template <typename T>
T readT_BE(size_t offset=0, int length=sizeof(T)) const;
```
Шаблонный метод для считывания чисел в Big-Endian (BE) порядке байт. Считывает байты начиная с `offset` равным размеру указанного типа `sizeof(T)`. Возвращает тип указанный в шаблоне, например `int numb = readT_BE<int>()`;

### Buffer.readT_LE()
```cpp
template <typename T>
T readT_LE(size_t offset=0, int length=sizeof(T)) const;
```
Шаблонный метод для считывания чисел в Little-Endian (LE) порядке байт. Считывает байты начиная с `offset` равным размеру указанного типа `sizeof(T)`. Возвращает тип указанный в шаблоне, например `int numb = readT_LE<int>()`;
	
### Buffer.writeT_BE()
```cpp
template <typename T>
void writeT_BE(T value, size_t offset=0, int length=sizeof(T));
```
Шаблонный метод для записи чисел в Big-Endian (BE) порядке байт. Записывает байты начиная с `offset` равным размеру указанного типа `sizeof(T)`. Использует тип указанный в шаблоне, например `writeT_BE<int>(441)`;

### Buffer.writeT_LE()
```cpp
template <typename T>
void writeT_LE(T value, size_t offset=0, int length=sizeof(T));
```
Шаблонный метод для записи чисел в Little-Endian (LE) порядке байт. Записывает байты начиная с `offset` равным размеру указанного типа `sizeof(T)`. Использует тип указанный в шаблоне, например `writeT_LE<int>(441)`;

### Buffer.readInt16BE()
```cpp
int readInt16BE(size_t offset=0) const;
```
Считывает `int` в Big-Endian (BE) порядке байт начиная с `offset` в размере **2 байта** (16 бит). Является эквивалентом `readT_BE<int16_t>`;

### Buffer.readInt16LE()
```cpp
int readInt16LE(size_t offset=0) const;
```
Считывает `int` в Little-Endian (LE) порядке байт начиная с `offset` в размере **2 байта** (16 бит). Является эквивалентом `readT_LE<int16_t>`;

### Buffer.readUInt16BE()
```cpp
unsigned int readUInt16BE(size_t offset=0) const;
```
Считывает `int` в Big-Endian (BE) порядке байт начиная с `offset` в размере **2 байта** (16 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `readT_BE<uint16_t>`;

### Buffer.readUInt16LE()
```cpp
unsigned int readUInt16LE(size_t offset=0) const;
```
Считывает `int` в Little-Endian (LE) порядке байт начиная с `offset` в размере **2 байта** (16 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `readT_LE<uint16_t>`;

### Buffer.writeInt16BE()
```cpp
void writeInt16BE(int value, size_t offset=0);
```
Записывает `int` в Big-Endian (BE) порядке байт начиная с `offset` в размере **2 байта** (16 бит). Является эквивалентом `writeT_BE<int16_t>`;

### Buffer.writeInt16LE()
```cpp
void writeInt16LE(int value, size_t offset=0);
```
Записывает `int` в Little-Endian (LE) порядке байт начиная с `offset` в размере **2 байта** (16 бит). Является эквивалентом `writeT_LE<int16_t>`;

### Buffer.writeUInt16BE()
```cpp
void writeUInt16BE(unsigned int value, size_t offset=0);
```
Записывает `int` в Big-Endian (BE) порядке байт начиная с `offset` в размере **2 байта** (16 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `writeT_BE<uint16_t>`;

### Buffer.writeUInt16LE()
```cpp
void writeUInt16LE(unsigned int value, size_t offset=0);
```
Записывает `int` в Little-Endian (LE) порядке байт начиная с `offset` в размере **2 байта** (16 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `writeT_LE<uint16_t>`;

### Buffer.readInt32BE()
```cpp
int readInt32BE(size_t offset=0) const;
```
Считывает `int` в Big-Endian (BE) порядке байт начиная с `offset` в размере **4 байта** (32 бит). Является эквивалентом `readT_BE<int32_t>`;

### Buffer.readInt32LE()
```cpp
int readInt16LE(size_t offset=0) const;
```
Считывает `int` в Little-Endian (LE) порядке байт начиная с `offset` в размере **4 байта** (32 бит). Является эквивалентом `readT_LE<int32_t>`;

### Buffer.readUInt32BE()
```cpp
unsigned int readUInt16BE(size_t offset=0) const;
```
Считывает `int` в Big-Endian (BE) порядке байт начиная с `offset` в размере **4 байта** (32 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `readT_BE<uint32_t>`;

### Buffer.readUInt32LE()
```cpp
unsigned int readUInt16LE(size_t offset=0) const;
```
Считывает `int` в Little-Endian (LE) порядке байт начиная с `offset` в размере **4 байта** (32 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `readT_LE<uint32_t>`;

### Buffer.writeInt32BE()
```cpp
void writeInt16BE(int value, size_t offset=0);
```
Записывает `int` в Big-Endian (BE) порядке байт начиная с `offset` в размере **4 байта** (32 бит). Является эквивалентом `writeT_BE<int32_t>`;

### Buffer.writeInt32LE()
```cpp
void writeInt16LE(int value, size_t offset=0);
```
Записывает `int` в Little-Endian (LE) порядке байт начиная с `offset` в размере **4 байта** (32 бит). Является эквивалентом `writeT_LE<int32_t>`;

### Buffer.writeUInt32BE()
```cpp
void writeUInt16BE(unsigned int value, size_t offset=0);
```
Записывает `int` в Big-Endian (BE) порядке байт начиная с `offset` в размере **4 байта** (32 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `writeT_BE<uint32_t>`;

### Buffer.writeUInt32LE()
```cpp
void writeUInt16LE(unsigned int value, size_t offset=0);
```
Записывает `int` в Little-Endian (LE) порядке байт начиная с `offset` в размере **4 байта** (32 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `writeT_LE<uint32_t>`;

### Buffer.readInt64BE()
```cpp
long long readInt64BE(size_t offset=0) const;
```
Считывает число в Big-Endian (BE) порядке байт начиная с `offset` в размере **8 байт** (64 бит). Является эквивалентом `readT_BE<int64_t>`;

### Buffer.readInt64LE()
```cpp
long long readInt64LE(size_t offset=0) const;
```
Считывает число в Little-Endian (LE) порядке байт начиная с `offset` в размере **8 байт** (64 бит). Является эквивалентом `readT_LE<int64_t>`;

### Buffer.readUInt64BE()
```cpp
unsigned long long readUInt64BE(size_t offset=0) const;
```
Считывает число в Big-Endian (BE) порядке байт начиная с `offset` в размере **8 байт** (64 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `readT_BE<uint64_t>`;

### Buffer.readUInt64LE()
```cpp
unsigned long long readUInt64LE(size_t offset=0) const;
```
Считывает число в Little-Endian (LE) порядке байт начиная с `offset` в размере **8 байт** (64 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `readT_LE<uint64_t>`;

### Buffer.writeInt64BE()
```cpp
void writeInt64BE(long long value, size_t offset=0);
```
Записывает число в Big-Endian (BE) порядке байт начиная с `offset` в размере **8 байт** (64 бит). Является эквивалентом `writeT_BE<int64_t>`;

### Buffer.writeInt64LE()
```cpp
void writeInt64LE(long long value, size_t offset=0);
```
Записывает число в Little-Endian (LE) порядке байт начиная с `offset` в размере **8 байт** (64 бит). Является эквивалентом `writeT_LE<int64_t>`;

### Buffer.writeUInt64BE()
```cpp
void writeUInt64BE(unsigned long long value, size_t offset=0);
```
Записывает число в Big-Endian (BE) порядке байт начиная с `offset` в размере **8 байт** (64 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `writeT_BE<uint64_t>`;

### Buffer.writeUInt64LE()
```cpp
void writeUInt64LE(unsigned long long value, size_t offset=0);
```
Записывает число в Little-Endian (LE) порядке байт начиная с `offset` в размере **8 байт** (64 бит), интерпретируя значение как **беззнаковое**. Является эквивалентом `writeT_LE<uint64_t>`;

### Buffer.readDoubleBE()
```cpp
double readDoubleBE(size_t offset=0) const;
```
Считывает `double` в Big-Endian (BE) порядке байт начиная с `offset` в размере **8 байт** (64 бит). Является эквивалентом `readT_BE<double>`;

### Buffer.readDoubleLE()
```cpp
double readDoubleLE(size_t offset=0) const;
```
Считывает `double` в Little-Endian (LE) порядке байт начиная с `offset` в размере **8 байт** (64 бит). Является эквивалентом `readT_LE<double>`;

### Buffer.writeDoubleBE()
```cpp
void writeDoubleBE(double value, size_t offset=0);
```
Записывает `double` в Big-Endian (BE) порядке байт начиная с `offset` в размере **8 байт** (64 бит). Является эквивалентом `writeT_BE<double>`;

### Buffer.writeDoubleLE()
```cpp
void writeDoubleLE(double value, size_t offset=0);
```
Записывает `double` в Little-Endian (LE) порядке байт начиная с `offset` в размере **8 байт** (64 бит). Является эквивалентом `writeT_LE<double>`;

### Buffer::BitsToInt()
```cpp
static int BitsToInt(const std::vector<char>& bits);
```
Статический метод, записывающий биты в число.

### Buffer::IntToBits()
```cpp
static Array<char> IntToBits(int numb, size_t len=sizeof(int)*8);
```
Статический метод, считывающий биты из числа.

### Buffer::randBytes()
```cpp
static Buffer randBytes(int len); 
```
Создает новый `Buffer` заполненный случайными байтами. Используется простой и быстрый алгоритм, который **не является** криптографически стойким.
Для криптографической стойкости используйте метод `crypto::randomBytes()` из модуля [ncpp-crypto](ncpp-crypto.md)

### Buffer.toInt()
```cpp
unsigned int toInt() const;
```
Конвертирует до первых 4-х байт в 32 битное число из BE порядка байт.

### Buffer.toInt64()
```cpp
unsigned long long toInt64() const;
```
Конвертирует до первых 8-ми байт в 64 битное число из BE порядка байт.

### Buffer.toString()
```cpp
String toString() const;
String toString(std::string type) const;
```
Переобразует текущий буфер в строку.
Допустимые значения для `std::string type`: "str"|"string", "int", "hex", "base64";

### Buffer.toHexString()
```cpp
String toHexString() const;
```
Переобразует текущий буфер строчное шестнадцатиричное представление байт.

### Buffer.toIntString()
```cpp
String toIntString() const;
```
Переобразует текущий буфер в строчное представление десятичного числа.

### Buffer.cout()
```cpp
std::string cout() const;
```
Выводит содержание текущего буфера в читаемой форме.


## ncpp::String

`String` наследует от `std::string`

```cpp
String();
String(const std::string& s);
String(const char* cstr);
String(const char* ptr, size_t len);
```

### String.indexOf()
```cpp
int indexOf(const char* cptr, size_t len, size_t start) const;
int indexOf(const char* cstr, size_t start = 0) const const;
int indexOf(const std::string& substr, size_t start = 0) const const;
int indexOf(char c, size_t start = 0) const;
```
Находит индекс первого вхождения по параметру разделителя, начиная с позиции `start`. В случае отсутствия совпадений возвращает **-1**.

### String.includes()
```cpp
bool includes(const std::string& substr) const;
```
Проверить, включает ли текущая строка указанную подстроку.

### String.back()
```cpp
char back() const;
```
Вернуть последний символ строки.

### String.split()
```cpp
Array<String> split(const char* delim, size_t len=0) const noexcept;
Array<String> split(const std::string& delim) const noexcept;
```
Разбивает текущий буфер на части по разделителю.

### String.slice()
```cpp
String slice(int start, int end=0) const;
```
Возвращает подстроку начиная со `start` заканчивая `end`. Поддерживает отрицательные индексы, идентичен [аналогичному методу](https://developer.mozilla.org/ru/docs/Web/JavaScript/Reference/Global_Objects/String/slice) из JS.

### String.startsWith()
```cpp
bool startsWith(const char* prefix, size_t len=0) const;
bool startsWith(const String& prefix) const;
```
Проверяет, начинается ли строка с указанного префикса.

### String.endsWith()
```cpp
bool endsWith(const char* suffix, size_t len=0) const;
bool endsWith(const String& suffix) const;
```
Проверяет, заканчивается ли строка на указанный суффикс.

### String.replace()
```cpp
String replace(const std::string& from, const std::string& to) const;
```
Заменяет первое вхождение подстроки на другую подстроку, возвращает новую строку.

### String.replaceAll()
```cpp
String replaceAll(const std::string& from, const std::string& to) const;
```
Заменяет все вхождения подстроки на другую подстроку, возвращает новую строку.

### String.toLowerCase()
```cpp
String toLowerCase() const;
```
Заменяет на нижний регистр и возвращает новую строку. Заменяет только ASCII символоы.

### String.toUpperCase()
```cpp
String toUpperCase() const;
```
Заменяет на верхний регистр и возвращает новую строку. Заменяет только ASCII символы.

### String.trim()
```cpp
String trim() const;
```
Удаляет пробелы с обоих концов этой строки и возвращает новую строку, идентичен [аналогичному методу](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/trim) из JS.


## ncpp::Array

`Array<T>` наследует от `std::vector<T>`

```cpp
Array();
Array(size_t size);
Array(const T* ptr, size_t size);
Array(const std::vector<T>& vec);
```

### Array.push()
```cpp
void push(T value);
```
Вставляет элемент в конец массива.

### Array.pop()
```cpp
T pop();
```
Удаляет элемент из конца массива и возвращает его.

### Array.unshift()
```cpp
void unshift(T value);
```
Вставляет элемент в начало массива.

### Array.shift()
```cpp
T shift();
```
Удаляет первый элемент из массива и возвращает его.

### Array.indexOf()
```cpp
long indexOf(const T& value, int start=0) const;
```
Находит индекс первого вхождения по параметру разделителя, начиная с позиции `start`. В случае отсутствия совпадений возвращает **-1**.

### Array.slice()
```cpp
Array<T> slice(int start, int end=0) const;
```
Возвращает массив начиная со `start` заканчивая `end`. Поддерживает отрицательные индексы, идентичен [аналогичному методу](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/slice) из JS.

### Array.splice()
```cpp
void splice(int index, int count=1);
```
Удаляет элементы из массива начиная со `index` в количестве заканчивая `count`.

### Array.concat()
```cpp
Array<T>& concat(const Array<T>& arr2)
static Array<T> concat(const Array<T>& arr1, const Array<T>& arr2);
```
Объединить (конкатенировать) новый массив с текущим экземпляром массива.
Статическая версия функции выполняет конкатенацию двух массивов и возвращает новый объединенный буфер.

### Array.reverse()
```cpp
void reverse();
```
Инвертировать все элементы в этом массиве.

### Array.minsize()
```cpp
void minsize(size_t msize);
```
Обеспечить минимальный размер массива: `resize()` будет выполнен только если `size() < msize`;

### Array.join()
```cpp
T join(T& delimiter) const;
T join(const std::string& delim) const;
T join() const;
```
Объединяет все элементы массива в один элемент с разделителем. (Преимущественно, в строку).

### Array.cout()
```cpp
std::string cout() const;
std::string cout_str() const;
```
Выводит содержание текущего массива в читаемой форме.

## ncpp::random()
```cpp
double random();
```
Возвращает псевдослучайное число с плавающей запятой в диапазоне от 0 до 1, аналогичен [random()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/random) из JS.

## ncpp::randInt()
```cpp
int randInt(int min, int max);
```
Возвращает псевдослучайное целое числ в диапазоне от `min` до `max`.

## ncpp::randInt()
```cpp
String randStr(size_t length, String letters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
```
Возвращает псевдослучайную строку с длинной `length`.


## ncpp::HashMap

`C++98`: Наследует от `std::map<K, V>`
`C++11`: Наследует от `std::unordered_map<K, V>`

### HashMap.keys()
```cpp
Array<K> keys() const;
```
Возвращает массив всех ключей текущего экземпляра `HashMap`.

### HashMap.has()
```cpp
bool has(K key) const;
```
Проверяет наличие элемента по ключу в текущем экземпляре `HashMap`.

### HashMap::assign()
```cpp
template <typename K, typename V>
static HashMap<K, V>& assign(HashMap<K, V>& target, const HashMap<K, V>& map2);
static HashMap<K, V>& assign(HashMap<K, V>& target, const Array<HashMap<K, V> >& maps);
```
Выполняет слияние одного или более экземпляров `HashMap` в `target`.

### HashMap.cout()
```cpp
std::string cout() const;
```
Выводит содержание текущего `HashMap` в читаемой форме.

### HashMap.toJSON()
```cpp
String toJSON() const;
```
Возвращает содержание текущего `HashMap` в представлении JSON строки.

### HashMap.toString()
```cpp
String toString() const; 
```
Алиас для `cout()`.

### HashMap.type()
```cpp
std::string type() const;
```
Возвращает текущий тип контейнера.


## ncpp::HashSet

`C++98`: Наследует от `std::set<K>`
`C++11`: Наследует от `std::unordered_set<K>`

### HashSet.keys()
```cpp
Array<K> keys() const;
```
Возвращает массив всех ключей текущего экземпляра `HashSet`.

### HashSet.has()
```cpp
bool has(K key) const;
```
Проверяет наличие элемента по ключу в текущем экземпляре `HashSet`.

### HashSet.cout()
```cpp
std::string cout() const;
```
Выводит содержание текущего `HashSet` в читаемой форме.

### HashSet.toString()
```cpp
String toString() const; 
```
Алиас для `cout()`.

### HashSet.type()
```cpp
std::string type() const;
```
Возвращает текущий тип контейнера.


## ncpp::StringMap
Наследует от `HashMap<std::string, String>`.

## ncpp::DoubleMap
Является `typedef HashMap<std::string, double> DoubleMap`.

## ncpp::Date

```cpp
Date();
Date(long long msecs, char type='m');
Date(const String& dateStr);
```

### Date::now()
```cpp
static long long now();
```
Возвращает текущее время в миллисекундах.

### Date.toDirectDate()
```cpp
std::string toDirectDate(char sp='.') const;
```
Вывести время в формате `DD.MM.YYYY HH:MM`.

### Date.toReverseDate()
```cpp
std::string toReverseDate(char sp='.') const;
```
Вывести время в формате `YYYY.MM.DD HH:MM`.

### Date.toString()
```cpp
std::string toString(char sp='.')
std::string toString(const String& mode, char sp='.')
```
По умолчанию выводит `toReverseDate()`.

### Date.toISOString()
```cpp
std::string toISOString() const;
```
Вывести время в формате стандарта `ISO 8601`.