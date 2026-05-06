[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::BigInt](#ncppbigint)
    - [BigInt::fromInt()](#bigintfromint)
    - [BigInt::fromHex()](#bigintfromhex)
    - [BigInt::fromBuffLE()](#bigintfrombuffle)
    - [BigInt::fromBuffBE()](#bigintfrombuffbe)
    - [BigInt.set()](#bigintset)
    - [BigInt.size()](#bigintsize)
    - [BigInt.bits()](#bigintbits)
    - [BigInt.IsEven()](#bigintiseven)
    - [BigInt.IsZero()](#bigintiszero)
    - [BigInt.negate()](#bigintnegate)
    - [BigInt.abs()](#bigintabs)
    - [BigInt.toBuffLE()](#biginttobuffle)
    - [BigInt.toBuffBE()](#biginttobuffbe)
    - [BigInt.toBuff()](#biginttobuff)
    - [BigInt.toIntString()](#biginttointstring)
    - [BigInt.toHex()](#biginttohex)
    - [BigInt.toString()](#biginttostring)
    - [BigInt.rand()](#bigintrand)
    - [BigInt.safeRandom()](#bigintsaferandom)
    - [BigInt::compare()](#bigintcompare)
    - [BigInt::pow()](#bigintpow)
    - [BigInt::powMod()](#bigintpowmod)

## ncpp::BigInt
`BigInt` — реализация длинной арифметики для работы с целыми числами произвольной длины.

```cpp
BigInt();
BigInt(const CString& value, const CString& type="dec");
BigInt(const char* cstr);
BigInt(const String& s);
BigInt(unsigned int i);
BigInt(unsigned long long ll);
BigInt(int i);
BigInt(long long ll);
BigInt(const Buffer& bf); //Buffer BE int
```

### BigInt::fromInt()
```cpp
static BigInt fromInt(unsigned long long ll);
static BigInt fromInt(const CString& cs);
```
Создает структуру из стандартного 64-битного целого числа или его строкового представления.

### BigInt::fromHex()
```cpp
static BigInt fromHex(const CString& cs);
```
Создает структуру из шестнадцатеричной строки.

### BigInt::fromBuffLE()
```cpp
static BigInt fromBuffLE(const Buffer& bf);
```
Создает число из сырого буфера байт в формате Little-Endian.

### BigInt::fromBuffBE()
```cpp
static BigInt fromBuffBE(const Buffer& bf);
```
Создает число из сырого буфера байт в формате Big-Endian.

### BigInt.set()
```cpp
void set(unsigned int i);
void set(unsigned long long ll);
void set(int i);
void set(long long ll);
void set(const CString& cs);
void set(const Buffer& bf);
```
Устанавливает значение числа из стандартного целочисленного типа или его представления.

### BigInt.size()
```cpp
size_t size() const;
```
Возвращает размер в байтах.

### BigInt.bits()
```cpp
size_t bits() const;
```
Возвращает размер в битах.

### BigInt.IsEven()
```cpp
bool IsEven() const;
```
Проверяет, является ли число четным.

### BigInt.IsZero()
```cpp
bool IsZero() const;
```
Возвращает `true`, если число равно нулю.

### BigInt.negate()
```cpp
BigInt& negate();
```
Инвертирует знак числа.

### BigInt.abs()
```cpp
BigInt abs() const;
```
Возвращает абсолютное значение числа (устанавливая знак на положительный).

### BigInt.toBuffLE()
```cpp
Buffer toBuffLE() const;
```
Экспорт числа в байтовый буфер в LE (Little-Endian) порядке.

### BigInt.toBuffBE()
```cpp
Buffer toBuffBE() const;
```
Экспорт числа в байтовый буфер в BE (Big-Endian) порядке.

### BigInt.toBuff()
```cpp
Buffer toBuff() const;
```
Экспорт числа в байтовый буфер, по умолчанию использует Big-Endian (BE).

### BigInt.toIntString()
```cpp
String toIntString() const;
```
Преобразование числа в строкое представление числа.

### BigInt.toHex()
```cpp
String toHex() const;
```
Преобразование числа в шестнадцатеричную строку.

### BigInt.toString()
```cpp
String toString() const;
String toString(const CString& type) const;
```
Преобразование числа в строку. По умолчанию `toIntString()`.

### BigInt.rand()
```cpp
static BigInt rand(size_t bits);
```
Генерирует случайное число заданной битовой длины.

### BigInt.safeRandom()
```cpp
static BigInt safeRandom(size_t bits);
```
Генерирует криптографически безопасное (насколько позволяет системный PRNG) случайное число заданной битовой длины.

### BigInt::compare()
```cpp
static char compare(const BigInt& a, const BigInt& b);
```
Сравнивает два числа. Возвращает `1` (больше), `-1` (меньше) или `0` (равно).

### BigInt::pow()
```cpp
static BigInt pow(BigInt base, BigInt exp);
```
Возводит число в степень.

### BigInt::powMod()
```cpp
static BigInt powMod(BigInt base, BigInt exp, const BigInt& mod);
```
Вычисляет `base^exp % mod`.