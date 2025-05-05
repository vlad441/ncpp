[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Buffer::Math](#ncppbuffermath)
	- [Math::increment()](#mathincrement)
	- [Math::decrement()](#mathdecrement)
	- [Math::multiply_single()](#mathmultiply_single)
	- [Math::divide_single()](#mathdivide_single)
	- [Math::add()](#mathadd)
	- [Math::subtract()](#mathsubtract)
	- [Math::multiply()](#mathmultiply)
	- [Math::divide()](#mathdivide)
	- [Math::mod()](#mathmod)
	- [Math::compare()](#mathcompare)
- [ncpp::BigInt](#ncppbigint)
	
## ncpp::Buffer::Math
Эта структура содержит функции, которые работают с буфером как с представлением BE (Big-Endian) целого положительного числа.

### Math::increment()
```cpp
static void increment(Buffer& buffer, unsigned int value=1, bool insert = false);
```
Инкрементирует буффер на указанный размер в диапазоне `int`.

### Math::decrement()
```cpp
static void decrement(Buffer& buffer, unsigned int value=1, bool erase = false);
```
Уменьшает буффер на указанный размер в диапазоне `int`.

### Math::multiply_single()
```cpp
static void multiply_single(Buffer& buffer, unsigned int value, bool insert = true);
```
Изменяет текущий буфер, умножая себя на число в диапазоне `int`.

### Math::divide_single()
```cpp
static unsigned int divide_single(Buffer& buffer, unsigned int value, bool erase = true);
```
Изменяет текущий буфер, деля себя на число в диапазоне `int`. Возвращает остаток от деления.

### Math::add()
```cpp
static Buffer add(const Buffer& a, const Buffer& b, bool resize = true);
```
Суммирует два буфера и возвращает новый буфер.

### Math::subtract()
```cpp
static Buffer subtract(const Buffer& a, const Buffer& b, bool resize = true);
```
Вычитает два буфера и возвращает новый буфер.

### Math::multiply()
```cpp
static Buffer multiply(const Buffer& a, const Buffer& b, bool resize = true);
```
Умножает два буфера и возвращает новый буфер.

### Math::divide()
```cpp
static Buffer divide(const Buffer& a, const Buffer& b, bool resize = true);
```
Делит два буфера и возвращает новый буфер.

### Math::divide()
```cpp
static Buffer mod(const Buffer& a, const Buffer& b, bool resize = true);
```
Выполняет операцию извлечения остатка от деления из двух буферов и возвращает новый буфер.

### Math::compare()
```cpp
static int compare(const Buffer& a, const Buffer& b);
```
Сравнивает два буфера и возвращает `1`, `0` или `-1`.

## ncpp::BigInt
```cpp
BigInt();
BigInt(const char* cstr);
BigInt(int value);
BigInt(unsigned int value);
BigInt(long long value);
BigInt(const Buffer& other);
```
Эта структура является представлением большого положительного целого числа.






