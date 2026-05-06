[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Console](#ncppconsole)
    - [Console.log()](#consolelog)
    - [Console.input()](#consoleinput)
    - [Console.setColor()](#consolesetcolor)
    - [Console.setBGColor()](#consolesetbgcolor)
    - [Console.setFont()](#consolesetfont)
    - [Console.reset()](#consolereset)
    - [Console.clear()](#consoleclear)
    - [Console.clearLn()](#consoleclearln)
    - [Console.setColorEx()](#consolesetcolorex)
	
## ncpp::Console
`Console` — вспомогательный класс для работы с системной консолью.
Обеспечивает кроссплатформенный вывод текста, чтение данных и управление цветами.

### Console.log()
```cpp
void log(const char* c);
void log(const CString& cs);
```
Выводит строку в консоль, автоматически добавляя символ переноса строки `\n` в конце.

### Console.input()
```cpp
String input();
```
Считывает строку из стандартного потока ввода до символа новой строки.

### Console.setColor()
```cpp
Console& setColor(char color);
```
Устанавливает цвет текста. Принимает значения от 0 до 7 (BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE).

### Console.setBGColor()
```cpp
Console& setBGColor(char color);
```
Устанавливает цвет фона консоли. Использует ту же палитру от 0 до 7.

### Console.setFont()
```cpp
Console& setFont(char font);
```
Устанавливает стиль шрифта:
`0`: Сброс всех атрибутов.
`1`: Жирный или яркий текст.
Другие коды (2-4) поддерживаются преимущественно в Unix-системах (курсив, подчеркивание).

### Console.reset()
```cpp
Console& reset();
```
Сбрасывает все цветовые атрибуты и настройки шрифта к исходному состоянию.

### Console.clear()
```cpp
Console& clear();
```
Полная очистка экрана консоли.

### Console.clearLn()
```cpp
Console& clearLn();
```
Очищает текущую строку, на которой находится курсор, и возвращает его в начало строки `\r`.

### Console.setColorEx()
```cpp
Console& setColorEx(unsigned char color);
Console& setColorEx(unsigned char r, unsigned char g, unsigned char b);
```
Расширенная установка цвета.