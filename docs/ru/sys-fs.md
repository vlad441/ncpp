[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::FStream](#ncpphashmap)
- [FStream.isOpen()](#fstreamisopen)
    - [FStream.is_open()](#fstreamis_open)
    - [FStream.open()](#fstreamopen)
    - [FStream.pos()](#fstreampos)
    - [FStream.setPos()](#fstreamsetpos)
    - [FStream.size()](#fstreamsize)
    - [FStream.close()](#fstreamclose)
    - [FStream.read()](#fstreamread)
    - [FStream.write()](#fstreamwrite)
    - [FStream.readEOF()](#fstreamreadeof)
    - [FStream.readline()](#fstreamreadline)
    - [FStream.readLines()](#fstreamreadlines)
    - [FStream.swap()](#fstreamswap)
    - [FStream.move()](#fstreammove)
    - [FStream.steal()](#fstreamsteal)
	
- [ncpp::fs::writeFile()](#ncppfswritefile)
- [ncpp::fs::appendFile()](#ncppfsappendfile)
- [ncpp::fs::readFile()](#ncppfsreadfile)
- [ncpp::fs::readLines()](#ncppfsreadlines)
- [ncpp::fs::readFstLine()](#ncppfsreadfstline)
- [ncpp::fs::ConfigRead()](#ncppfsconfigread)
- [ncpp::fs::ConfigWrite()](#ncppfsconfigwrite)
- [ncpp::fs::stat()](#ncppfsstat)
- [ncpp::fs::isDir()](#ncppfsisdir)
- [ncpp::fs::copy()](#ncppfscopy)
- [ncpp::fs::rename()](#ncppfsrename)
- [ncpp::fs::unlink()](#ncppfsunlink)
- [ncpp::fs::mkdir()](#ncppfsmkdir)
- [ncpp::fs::rmdir()](#ncppfsrmdir)
- [ncpp::fs::chmod()](#ncppfschmod)
- [ncpp::fs::chown()](#ncppfschown)
- [ncpp::fs::exists()](#ncppfsexists)
- [ncpp::fs::readDir()](#ncppfsreaddir)
- [ncpp::fs::dirname()](#ncppfsdirname)

## ncpp::FStream
Класс для работы с файловыми потоками. Поддерживает кроссплатформенную работу (WinAPI/POSIX).

### FStream::isOpen()
```cpp
bool isOpen();
bool is_open();
```
Возвращает `true`, если файл успешно открыт и дескриптор валиден.

### FStream::open()
```cpp
bool open(const CString& fpath, char mode);
```
Открывает файл по пути fpath. Режимы `mode`:
`FStream::IO_READ` — чтение (существующий файл).
`FStream::IO_WRITE` — запись (создает новый или перезаписывает).
`FStream::IO_APPEND` — дозапись (курсор в конец файла).

### FStream::pos()
```cpp
size_t pos();
size_t tellg();
```
Возвращает текущую позицию курсора в файле.

### FStream::setPos()
```cpp
void setPos(size_t pos);
void seekg(size_t pos);
```
-

### FStream::size()
```cpp
size_t size();
```
Возвращает общий размер файла в байтах.

### FStream::read()
```cpp
int read(char* ptr, size_t size);
int read(Buffer* rbuff);
Buffer read();
Buffer read(int len);
```
Считывает данные из файла. Версия без параметров считывает файл целиком от текущей позиции до конца. Возвращает кол-во записанных байт или -1 при ошибке.

### FStream::readEOF()
```cpp
Buffer readEOF();
```
Считывает все оставшиеся данные из потока до достижения конца файла (EOF).

### FStream::readline()
```cpp
bool readline(String& line, bool once=false);
```
Считывает одну строку до символа `\n`. Если `once` установлен в `true`, позиция курсора не сдвигается автоматически для следующего чтения.

### FStream::readLines()
```cpp
size_t readLines(Array<String>& lines, int cnt);
```
Считывает указанное количество строк `cnt` в массив `lines`. Возвращает кол-во прочитанных строк.

### FStream::write()
```cpp
int write(const char* ptr, size_t size);
int write(const Buffer& wrbuff);
int write(const char* c);
```
Записывает данные в файл. Возвращает кол-во записанных байт или -1 при ошибке.

### FStream.swap()
```cpp
void swap(FStream& other);
friend void swap(FStream& a, FStream& b);
```
Меняет содержимое двух массивов местами (путем обмена указателями и метаданными), избегая копирования.

### FStream::move()
```cpp
friend void move(FStream& dst, FStream& tmp);
```
C++11:
```cpp
friend void move(FStream& dst, FStream&& tmp);
```
C++98:
```cpp
friend void move(FStream& dst, const FStream& victim);
```
Реализация семантики перемещения. Позволяет передать владение ресурсами от одного объекта (жертвы) другому, оставляя исходный объект пустым, но валидным.
В C++11 и выше используются стандартные `rvalue` ссылки.

### FStream.steal()
C++11:
```cpp
FStream& steal(FStream& tmp);
FStream& steal(FStream&& tmp);
```
C++98:
```cpp
FStream& steal(const FStream& victim);
```
Метод-обертка для `move()`. "Крадет" (перемещает) содержимое `victim` и зануляет его, сохраняя валидным.

## ncpp::fs::writeFile()
```cpp
bool writeFile(const CString& path, const Buffer& data);
```
Полностью перезаписывает файл данными из `data`.

## ncpp::fs::appendFile()
```cpp
bool appendFile(const CString& path, const Buffer& data);
```
Добавляет данные `data` в конец файла.

## ncpp::fs::readFile()
```cpp
Buffer readFile(const CString& path);
```
Считывает всё содержимое файла в `Buffer`.

## ncpp::fs::readLines()
```cpp
Array<String> readLines(const CString& path);
```
Считывает файл и разбивает его на массив строк.

## ncpp::fs::readFstLine()
```cpp
String readFstLine(const CString& path);
```
Быстрое считывание только первой строки файла.

## ncpp::fs::ConfigRead()
```cpp
StringMap ConfigRead(const CString& path, bool unescape=false, const CString& delim="=");
```
Читает конфигурационный файл формата `key=value`. Пропускает строки, начинающиеся с `#` (комментарии)

## ncpp::fs::ConfigWrite()
```cpp
bool ConfigWrite(const CString& path, StringMap config, String delim="=");
```
Записывает `StringMap` в файл в формате конфигурации `key=value`.

## ncpp::fs::stat()
```cpp
ULLMap stat(const CString& path);
```
Возвращает информацию о файле.

## ncpp::fs::isDir()
```cpp
bool isDir(const CString& path);
bool isDir(const ULLMap& stat);
```
Проверяет, является ли путь директорией.

## ncpp::fs::exists()
```cpp
bool exists(const CString& path);
```
Проверяет существование файла или директории.

## ncpp::fs::copy()
```cpp
bool copy(const CString& src, const CString& dst);
```
Копирует файл из `src` в `dst`.

## ncpp::fs::rename()
```cpp
bool rename(const CString& oldpath, const CString& newpath);
```
Переименовывает или перемещает файл/директорию.

## ncpp::fs::unlink()
```cpp
bool unlink(const CString& path);
bool rm(const CString& path);
```
Удаляет файл / иноду на него. В Linux системах удаляет только ссылку на файл.

## ncpp::fs::mkdir()
```cpp
bool mkdir(const CString& path);
```
Создает директорию. Возвращает `true`, если директория создана или уже существует.

## ncpp::fs::rmdir()
```cpp
bool rmdir(const CString& path);
```
Удаляет пустую директорию.

## ncpp::fs::chmod()
```cpp
bool chmod(const CString& path, int mode);
```
Изменяет права доступа к файлу (только POSIX, на Windows всегда возвращает `false`). 
Параметр `mode` принимается в восьмиричной системе (например `0711`)

## ncpp::fs::chown()
```cpp
bool chown(const CString& path, const CString& uowner, const CString& ugroup="");
```
Изменяет владельца файла.

## ncpp::fs::readDir()
```cpp
Array<String> readDir(const CString& path);
```
Возвращает список имен файлов и папок в указанной директории.

## ncpp::fs::dirname()
```cpp
String dirname(const CString& path);
```
Возвращает путь к родительской директории (отсекает имя файла).