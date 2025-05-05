[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Stream](#ncppstream)
- [ncpp::fs::writeFile](#ncppfswritefile)
- [ncpp::fs::appendFile](#ncppfsappendfile)
- [ncpp::fs::readFile](#ncppfsreadFile)
- [ncpp::fs::readLines](#ncppfsreadLines)
- [ncpp::fs::readLine](#ncppfsreadLine)
- [ncpp::fs::ConfigRead](#ncppfsconfigread)
- [ncpp::fs::ConfigWrite](#ncppfsconfigwrite)
- [ncpp::fs::stat](#ncppfsstat)
- [ncpp::fs::isDir](#ncppfsisdir)
- [ncpp::fs::copy](#ncppfscopy)
- [ncpp::fs::rename](#ncppfsrename)
- [ncpp::fs::unlink](#ncppfsunlink)
- [ncpp::fs::mkdir](#ncppfsmkdir)
- [ncpp::fs::rmdir](#ncppfsrmdir)
- [ncpp::fs::chmod](#ncppfschmod)
- [ncpp::fs::chown](#ncppfschown)
- [ncpp::fs::exists](#ncppfsexists)
- [ncpp::fs::rm](#ncppfsrm)
- [ncpp::fs::readDir](#ncppfsreaddir)
- [ncpp::fs::dirname](#ncppfsdirname)
- [ncpp::fs::FStream](#ncppbuffer)
	- [FStream.read()](#fstreamread)
	- [FStream.write()](#fstreamwrite)
	- [FStream::createReadStream()](#fstreamcreatereadstream)
	- [FStream::createWriteStream()](#fstreamcreatewritestream)
- [ncpp::fs::createReadStream()](#ncppfscreatereadstream)
- [ncpp::fs::createWriteStream()](#ncppfscreatewritestream)

## ncpp::Stream
```cpp
struct Stream { bool destroyed;
	Stream();
	size_t read(Buffer* rbuff, size_t count=0);
	Buffer read(size_t count=0);
	void write(const Buffer& wrbuff);
	void pipe(Stream& stream2);
	void pipe(Stream* stream2);
	void pipestart();
	void destroy();
	void close();
	void (*OnData)(const Buffer& data);
};
```
Метаструктура которая может наследоваться для описания концепции потока.

## ncpp::fs::writeFile
```cpp
bool writeFile(const std::string& path, const Buffer& data);
};
```
Записывает буфер в новый файл. Если файл существует - перезаписывает. Если файл не существует - создает его.

## ncpp::fs::appendFile
```cpp
bool appendFile(const std::string& path, const Buffer& data);
};
```
Дозаписывает буфер в конец файла. Если файл не существует - создает его.

## ncpp::fs::readFile
```cpp
Buffer readFile(const std::string& path);
};
```
Считывает файл и возвращает буффер с содержанием файла.

## ncpp::fs::readLines
```cpp
Array<String> readLines(const std::string& path);
};
```
Считывает файл в виде массива строк. Позволяет считывать виртуальные файлы (например, файлы ядра linux) без фиксированого размера.

## ncpp::fs::readLine
```cpp
String readLine(const std::string& path);
};
```
Считывает только первую строку из файла и возвращает ее. Также как и `readLines()`, позволяет считывать виртуальные файлы (например, файлы ядра linux) без фиксированого размера.

## ncpp::fs::ConfigRead
```cpp
StringMap ConfigRead(const std::string& path, bool unescape=false, std::string delim="=");
};
```
Считывает конфиг из файла виде `StringMap`. Параметр `unescape` указывает, стоит ли раскрывать кавычки `"` значений в конфиге.

### Пример синтаксиса конфига:
```conf
#Комментарии игнорируются
param1=nani deska
param2="test value 2"
param3=441
# ...
```

## ncpp::fs::ConfigWrite
```cpp
bool ConfigWrite(const std::string& path, StringMap config, std::string delim="=");
};
```
Записывает конфиг в виде `StringMap` в файл. [Пример синтаксиса конфига](#примерсинтаксисаконфига)

## ncpp::fs::stat
```cpp
DoubleMap stat(const std::string& path);
};
```
Возвращает свойства файла. Если файл не существует, будет возвращена пустая структура.

## ncpp::fs::isDir
```cpp
bool isDir(const DoubleMap& stat);
bool isDir(const std::string& path);
};
```
Проверяет, является ли указаный путь директорией. Возвращает свойства файла. Если файл не существует, будет возвращена пустая структура.

## ncpp::fs::copy
```cpp
bool copy(const std::string& src, const std::string& dst);
};
```
Копирует файл из директории `src` в директорию `dst`.

## ncpp::fs::rename
```cpp
bool rename(const std::string& oldpath, const std::string& newpath);
};
```
Переименовывает файл. По сути является эквивалентом `mv` (move), т.к. переименование в файловой системе это перемещение.

## ncpp::fs::unlink
```cpp
bool unlink(const std::string& path);
};
```
Удаляет файл. В Unix подобных системах файл будет отмечен удаленным только если будут удалены все символические ссылки на него.

## ncpp::fs::mkdir
```cpp
bool mkdir(const std::string& path);
};
```
Создает директорию.

## ncpp::fs::rmdir
```cpp
bool rmdir(const std::string& path);
};
```
Удаляет директорию.

## ncpp::fs::chmod
```cpp
bool chmod(const std::string& path, int mode);
};
```
Меняет права файла в unix стиле. На `Windows` не реализован и всегда возвращает `false`. `mode` ожидается в восьмиричном представлении, например `0755`.

## ncpp::fs::chown
```cpp
bool chown(const std::string& path, const std::string& uowner, const std::string& ugroup="");
};
```
Меняет владельца файла.

## ncpp::fs::exists
```cpp
bool exists(const std::string& path);
};
```
Проверяет существование файла/директории.

## ncpp::fs::rm
Алиас [ncpp::fs::unlink](#ncppfsunlink).

## ncpp::fs::readDir
```cpp
Array<String> readDir(const std::string& path);
};
```
Считывает директорию и возвращает список имен файлов в ней.

## ncpp::fs::readDir
```cpp
std::string dirname(const std::string& path);
};
```
Переобразовывает путь к файлу в путь текущей директории для этого файла (по сути обрезает название файла).
Считывает директорию и возвращает список имен файлов в ней.

## ncpp::fs::FStream
```cpp
FStream(const std::string& path, std::ios_base::openmode mode);
```
Наследует от `ncpp::Stream`. Предоставляет поток для работы с текущим дескриптором файла (текущим открытым файлом).

### FStream.read()
```cpp
int read(unsigned char* ptr, size_t size);
int read(Buffer* rbuff);
Buffer read(int size=-1);
```
Выполняет чтение из дескриптора файла начиная с последней позиции предыдущего чтения (потоковое чтение).
Параметр `size` позволяет указать, сколько байт прочитать из текущего файлового дескриптора (потока).
Если `size` не указан или `size<=0`, будут считаны все оставшиеся байты до конца файла (потока).

### FStream.write()
```cpp
void write(const unsigned char* ptr, size_t size);
void write(const Buffer& wrbuff);
```
Записывает байты в поток (файл).

### FStream::createReadStream()
```cpp
static FStream createReadStream(const std::string& path);
```
Создает новый поток чтения файла и возвращает экземпляр `FStream`.

### FStream::createWriteStream()
```cpp
static FStream createWriteStream(const std::string& path);
```
Создает новый поток записи файла и возвращает экземпляр `FStream`.

## ncpp::fs::createReadStream()
Алиас [FStream::createReadStream()](#fstreamcreatereadstream)

## ncpp::fs::createWriteStream()
Алиас [FStream::createWriteStream()](#fstreamcreatewritestream)