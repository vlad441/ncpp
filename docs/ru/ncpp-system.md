[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::excmd()](#ncppexcmd)
- [ncpp::system::getUptime()](#ncppsystemgetuptime)
- [ncpp::system::os_info()](#ncppsystemos_info)
- [ncpp::system::getUptimeStr()](#ncppsystemgetuptimestr)
- [ncpp::system::CPU](#ncppsystemcpu)
	- [CPU::info()](#cpuinfo)
	- [CPU::CPUTime](#cpucputime)
	- [CPU::model()](#cpumodel)
	- [CPU::getTotalTime()](#cpugettotaltime)
	- [CPU::getCoresTime()](#cpugetcorestime)
	- [CPU::getUsageTime()](#cpugetusagetime)
	- [CPU::calcLoad()](#cpucalcload)
	- [CPU::getLoad()](#cpugetload)
	- [CPU::getAvgLoad()](#cpugetavgload)
	- [CPU::usage()](#cpuusage)
- [ncpp::system::RAM](#ncppsystemram)
	- [RAM::info()](#raminfo)
	- [RAM::usage()](#ramusage)
	- [RAM::usage_rss()](#ramusage_rss)
	- [RAM::strUsage()](#ramstrusage)
	- [RAM::strTotal()](#ramstrtotal)
- [ncpp::system::Disk](#ncppsystemdisk)
	- [Disk::DiskInfo](#diskdiskinfo)
	- [Disk::drives()](#diskdrives)
	- [Disk::logical()](#disklogical)
	- [Disk::models()](#diskmodels)
	- [Disk::info()](#diskinfo)
	- [Disk::strUsage()](#diskstrusage)
	- [Disk::strTotal()](#diskstrtotal)
- [ncpp::system::GPU](#ncppsystemgpu)
	- [GPU::list()](#gpulist)
	- [GPU::model()](#gpumodel)
- [ncpp::system::info()](#ncppsysteminfo)
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
- [ncpp::system::performance](#ncppsystemperformance)
	- [performance::start()](#performancestart)
	- [performance::now()](#performancenow)

## ncpp::excmd()
```cpp
String excmd(const std::string& cmd, Stream* stream=NULL);
```
Выполняет команду оболочки в cmd/терминале, ждет выполнения и возвращает `String`.
В случае, если указан `Stream*`, результат будет выводиться в него, а `String` будет возвращен пустым.

## ncpp::system::getUptime()
```cpp
long long getUptime(char type='s');
```
Возвращает время работы текущей системы.

## ncpp::system::os_info()
```cpp
StringMap os_info();
```
Возвращает базовую информацию об ОС в виде `StringMap`: "OS" - название ОС, "Hostname" - имя хоста.
Для Windows вернет дополнительное поле: "NT" - Версия ядра Windows.
Для Linux вернет дополнительное поле: "Kernel" - Версия ядра Linux.

## ncpp::system::getUptimeStr()
```cpp
String getUptimeStr();
```
Возвращает время работы текущей системы в виде строки.

## ncpp::system::CPU

### CPU::info()
```cpp
StringMap info();
```
Возвращает информацию о процессоре в виде `StringMap`.

## CPU::CPUTime
```cpp
struct CPUTime { long long user; long long nice; long long system; long long idle; long long iowait; long long irq; long long softirq; };
```
Структура для хранения процессорного времени: время работы и простоя в unix стиле.

### CPU::model()
```cpp
String model();
```
Возвращает имя модели процессора.

### CPU::getTotalTime()
```cpp
CPUTime getTotalTime();
```
Возвращает суммарное процессорное время всех ядер.

### CPU::getCoresTime()
```cpp
Array<CPUTime> getCoresTime();
```
Возвращает массив с процессорным временем каждого ядра.
Для Windows не реализовано и вернет пустой массив.

### CPU::getUsageTime()
```cpp
CPUTime getUsageTime(int pid = -1);
```
Возвращает процессорное время ядра для указанного процесса по `pid`.
Если `pid` не указан, возвращает для текущего процесса.

### CPU::calcLoad()
```cpp
double calcLoad(CPUTime& prevTime, const CPUTime& currTime);
Array<double> calcLoad(Array<CPUTime>& prevCoresTime, const Array<CPUTime>& CoresTime);
```
Возвращает разницу отработанного процессорного времени.

### CPU::getLoad()
```cpp
Array<double> getLoad();
```
Сравнивает процессорное время каждого ядра между предыдущим и текущим вызовом `getLoad()`. Возвращает % активного времени за этот период.

### CPU::getAvgLoad()
```cpp
double getAvgLoad();
```
Сравнивает суммарное процессорное время между предыдущим и текущим вызовом `getAvgLoad()`. Возвращает % активного времени за этот период.

### CPU::calcLoad()
```cpp
double usage(int pid=-1);
```
Возвращает % "нагрузки" процесса. 
Сравнивает процессорное время ядра для указанного процесса по `pid` между предыдущим и текущим вызовом `usage()`, выполняя `calcLoad()` для этого ядра.
Если `pid` не указан, возвращает для текущего процесса.

## ncpp::system::RAM

### RAM::info()
```cpp
DoubleMap info();
```
Возвращает информацию о памяти системы в байтах.

### RAM::usage()
```cpp
DoubleMap usage(int pid=-1);
```
Возвращает статистику использования памяти в байтах для процесса по `pid`.
Если `pid` не указан, возвращает для текущего процесса.

### RAM::usage_rss()
```cpp
size_t usage_rss(int pid=-1);
```
Возвращает использование памяти `rss` в байтах для процесса по `pid`.
Если `pid` не указан, возвращает для текущего процесса.

### RAM::strUsage()
```cpp
String strUsage(const DoubleMap& rinf=info());
```
Возвращает информацию о памяти системы в виде строки.

### RAM::strTotal()
```cpp
String strTotal(const DoubleMap& rinf=info());
```
Возвращает общий объем установленной памяти в системы в виде строки.

## ncpp::system::Disk

### Disk::DiskInfo
```cpp
struct DiskInfo { std::string model; unsigned long long used, size; std::string label; };
```
Структура для хранения информации о диске или разделах.

### Disk::drives()
```cpp
Array<DiskInfo> drives();
```
Возвращает массив с информацией об аппаратных носителях.

### Disk::logical()
```cpp
Array<DiskInfo> logical();
```
Возвращает массив с информацией о логических разделах дисков.

### Disk::models()
```cpp
Array<String> models();
```
Возвращает массив с названиями моделей аппаратных носителей.

### Disk::info()
```cpp
DoubleMap info();
```
Возвращает информацию о суммарной статистике использования памяти дисков в байтах.

### Disk::strUsage()
```cpp
String strUsage(const DoubleMap& rinf=info());
```
Возвращает информацию о суммарной статистике использования памяти дисков в виде строки.

### Disk::strTotal()
```cpp
String strTotal(const DoubleMap& rinf=info());
```
Возвращает суммарный объем дисков в системе в виде строки.

## ncpp::system::GPU

### GPU::list()
```cpp
Array<String> list();
```
Возвращает массив с названиями моделей видеокарт.

### GPU::model()
```cpp
String model();
```
Возвращает название модели первой видеокарты в списке.

## ncpp::system::info()
```cpp
StringMap info();
```
Консолидированная информация о системе в стиле `neofetch`.

## ncpp::system::ProcessInfo
```cpp
struct ProcessInfo { int pid; int threads; int parent; int priority; std::string name; char state; int nice; size_t virt; size_t rss; };
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
static Process start(const std::string& execpath, const ProcessOpts& opts);
static Process start(const std::string& execpath, char detach=1);
```
Запускает процесс. `detach` определяет уровень "отделения", т.е. независимости процесса: 
0 - дочерний процесс.
1 - независимый без консоли. 
2 - независимый с собственной оболочкой (если возможно).

### Process::exec()
```cpp
static bool exec(const std::string& execcmd, char detach=0);
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
static std::string GetExePath(int pid=-1);
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

## ncpp::performance

### performance::start()
```cpp
void start();
```
Установить точку отсчета замера производительности. (по умолчанию, точка отсчета может идти с момента запуска программы или с начала эпохи unix);

### performance::now()
```cpp
double now();
```
Вывести текущее время в миллисекундах, прошедшее с точки отсчета.







	
	
	

	




