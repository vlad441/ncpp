[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::sysMalloc()](#ncppsysmalloc)
- [ncpp::sysFree()](#ncppsysfree)
- [ncpp::sysMprotect()](#ncppsysmprotect)
- [ncpp::GetTimestamp()](#ncppgettimestamp)
- [ncpp::xorshift128plus()](#ncppxorshift128plus)
- [ncpp::random()](#ncpprandom)
- [ncpp::randInt()](#ncpprandint)
- [ncpp::randStr()](#ncpprandstr)
- [ncpp::readline()](#ncppreadline)
- [ncpp::performance::start()](#ncppperformancestart)
- [ncpp::performance::now()](#ncppperformancenow)
- [ncpp::Result](#ncppresult)

## ncpp::sysMalloc()
```cpp
void* sysMalloc(size_t size, bool exec=0);
```
Выделяет страницы памяти ОС (обычно, размер страницы `4кб`)
> Заметка: В некоторых средах (например `Linux`, привет тупому `mmap`) реальный выделенный размер будет `size + sizeof(size_t)` байт из-за нужды хранить размер `size_t` в блоке выделенной памяти.
Из-за чего при запросе ровно 4096 байт - будет прибавлено `sizeof(size_t)` и будет выделено 2 страницы памяти замеcто 1, что является одной из форм внутренней фрагментации.

## ncpp::sysFree()
```cpp
void sysFree(void* ptr);
```
Возвращает выделенные страницы памяти ОС.

## ncpp::sysMprotect()
```cpp
bool sysMprotect(void* ptr, size_t len=4096, bool read=1, bool write=1, bool exec=1);
```
Измняет модификаторы защиты выделенных страниц памяти. Например, разрешить ее выполнение.

## ncpp::GetTimestamp()
```cpp
long long GetTimestamp(char type='s'); // type: s - secs, m - msecs, u - usecs
```
Возвращает текущую временную метку (Unix Timestamp). По умолчанию в секундах.

## ncpp::xorshift128plus()
```cpp
unsigned long long xorshift128plus();
```
Быстрый генератор псевдослучайных чисел. Использует алгоритм `xorshift128+` для генерации 64-битных значений. **Не рекомендуется** для криптографии.

## ncpp::random()
```cpp
double random();
```
Возвращает случайное вещественное число в диапазоне (0.0, 1.0). Использует алгоритм `xorshift128+`

## ncpp::randInt()
```cpp
int randInt(int min, int max);
```
Возвращает случайное целое число от `min` до `max`. Использует алгоритм `xorshift128+`

## ncpp::randStr()
```cpp
String randStr(size_t length, const CString& letters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
```
Генерирует случайную строку заданной длины. Использует алгоритм `xorshift128+`

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