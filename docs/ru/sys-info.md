[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

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

Определения:
```cpp
namespace sys = system;
```

## ncpp::system::getUptime()
```cpp
long long getUptime(char type='s');
```
Возвращает время работы текущей системы.

## ncpp::system::os_info()
```cpp
StringMap os_info();
```
Возвращает базовую информацию об ОС в виде `StringMap`: `"OS"` - название ОС, `"Hostname"` - имя хоста.
Для Windows вернет дополнительное поле: `"NT"` - Версия ядра Windows.
Для Linux вернет дополнительное поле: `"Kernel"` - Версия ядра Linux.

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
ULLMap info();
```
Возвращает информацию о памяти системы в байтах.

### RAM::usage()
```cpp
ULLMap usage(int pid=-1);
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
String strUsage(const ULLMap& rinf=info());
```
Возвращает информацию о памяти системы в виде строки.

### RAM::strTotal()
```cpp
String strTotal(const ULLMap& rinf=info());
```
Возвращает общий объем установленной памяти в системы в виде строки.

## ncpp::system::Disk

### Disk::DiskInfo
```cpp
struct DiskInfo { String model; unsigned long long used, size; String label; };
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
ULLMap info();
```
Возвращает информацию о суммарной статистике использования памяти дисков в байтах.

### Disk::strUsage()
```cpp
String strUsage(const ULLMap& rinf=info());
```
Возвращает информацию о суммарной статистике использования памяти дисков в виде строки.

### Disk::strTotal()
```cpp
String strTotal(const ULLMap& rinf=info());
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
