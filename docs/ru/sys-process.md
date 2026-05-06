[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::execShell()](#ncppexecshell)
- [ncpp::system::ProcessInfo](#ncppsystemprocessinfo)
- [ncpp::system::ProcessOpts](#ncppsystemprocessopts)
- [ncpp::system::Process](#ncppsystemprocess)
	- [Process::start()](#processstart)
	- [Process::exec()](#processexec)
	- [Process.signal()](#processsignal)
	- [Process.kill()](#processkill)
	- [Process.term()](#processterm)
	- [Process::getpid()](#processgetpid)
	- [Process.isrun()](#processisrun)
	- [Process::GetExePath()](#processgetexepath)
	- [Process::GetProcInfo()](#processgetprocinfo)
	- [Process::GetList()](#processgetlist)

## ncpp::execShell()
```cpp
String execShell(const CString& cmd, Stream* stream=NULL);
```
Выполняет команду оболочки в cmd/терминале, ждет выполнения и возвращает `String`.
В случае, если указан `Stream*`, результат будет выводиться в него, а `String` будет возвращен пустым.

## ncpp::system::ProcessInfo
```cpp
struct ProcessInfo { int pid; int threads; int parent; int priority; String name; char state; int nice; size_t virt; size_t rss; };
```
Структура для хранения информации о процессе.

## ncpp::system::ProcessOpts
```cpp
struct ProcessOpts { char detach; bool getpid; bool StdOut; bool StdIn; };
```
Структура для хранения информации о параметрах запуска процесса.

## ncpp::system::Process

### Process::start()
```cpp
static Process start(const CString& execpath, const ProcessOpts& opts);
static Process start(const CString& execpath, char detach=1);
```
Запускает процесс. `detach` определяет уровень "отделения", т.е. независимости процесса: 
`0` - дочерний процесс.
`1` - независимый без консоли. 
`2` - независимый с собственной оболочкой (если возможно).

### Process::exec()
```cpp
static bool exec(const CString& execcmd, char detach=0);
```
Выполняет команду оболочки в cmd/терминале, но **не ждет** ее завершения.

### Process.signal()
```cpp
static bool signal(int pid, int id);
bool signal(int id);
```
Отправляет сигнал процессу. (наиболее актуально для unix систем).

### Process.kill()
```cpp
static bool kill(int pid);
bool kill();
```
Убивает процесс **немедленно**. (На сколько это возможно в данной ОС).

### Process.term()
```cpp
static bool term(int pid); 
bool term();
```
Отправляет процессу сигнал завершения.

### Process::getpid()
```cpp
inline static int getpid();
```
Возвращает `pid` текущего процесса.

### Process.isrun()
```cpp
bool isrun() const;
```
Проверяет, запущен ли процесс.

### Process::GetExePath()
```cpp
static String GetExePath(int pid=-1);
```
Возвращает полный путь к исполняемому файлу процесса по `pid`.
Если `pid` не указан, возвращает для текущего процесса.

### Process::GetProcInfo()
```cpp
static ProcessInfo GetProcInfo(int pid);
```
Возвращает информацию о процессе по `pid`.

### Process::GetList()
```cpp
static Array<ProcessInfo> GetList();
```
Возвращает список запущенных процессов в системе.







	
	
	

	




