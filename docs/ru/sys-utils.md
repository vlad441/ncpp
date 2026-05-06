[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::GetTimestamp()](#ncppgettimestamp)
- [ncpp::xorshift128plus()](#ncppxorshift128plus)
- [ncpp::random()](#ncpprandom)
- [ncpp::randInt()](#ncpprandint)
- [ncpp::randStr()](#ncpprandstr)
- [ncpp::readline()](#ncppreadline)
- [ncpp::performance::start()](#ncppperformancestart)
- [ncpp::performance::now()](#ncppperformancenow)
- [ncpp::Result](#ncppresult)

## ncpp::GetTimestamp()
```cpp
long long GetTimestamp(char type='s'); // type: s - secs, m - msecs, u - usecs
```
Возвращает текущую временную метку (Unix Timestamp). По умолчанию в секундах.

## ncpp::xorshift128plus()
```cpp
unsigned long long xorshift128plus();
```
Быстрый генератор псевдослучайных чисел. Использует алгоритм xorshift128+ для генерации 64-битных значений. **Не рекомендуется** для криптографии.

## ncpp::random()
```cpp
double random();
```
Возвращает случайное вещественное число в диапазоне (0.0, 1.0). Использует алгоритм xorshift128+

## ncpp::randInt()
```cpp
int randInt(int min, int max);
```
Возвращает случайное целое число от `min` до `max`. Использует алгоритм xorshift128+

## ncpp::randStr()
```cpp
String randStr(size_t length, String letters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
```
Генерирует случайную строку заданной длины. Использует алгоритм xorshift128+

## ncpp::readline()
```cpp
String readline();
```
Считывает строку из стандартного ввода (stdin) до символа новой строки.

## ncpp::performance::start()
```cpp
void performance::start();
```
Инициализирует или сбрасывает точку начала отсчета.

## ncpp::performance::now()
```cpp
double performance::now();
```
Возвращает время в миллисекундах, прошедшее с момента вызова `start()` или с начала эпохи UNIX.

## ncpp::Result
```cpp
template <typename T, typename E = String> struct Result;
```
Шаблон `Result<T, E>` для возврата результата операции. Позволяет передавать объект данных вместе с кодом ошибки и поясняющим сообщением.