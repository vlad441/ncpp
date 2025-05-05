[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Semaphore](#ncppsemaphore)
	- [Semaphore.acquire()](#semaphoreacquire)
	- [Semaphore.release()](#semaphorerelease)
- [ncpp::Thread](#ncppthread)
	- [Thread.join()](#threadjoin)
	- [Thread.detach()](#threaddetach)
	- [Thread.kill()](#threadkill)
	- [Thread::Signal](#threadsignal)
		- [Signal.wait()](#signalwait)
		- [Signal.notify()](#signalnotify)
- [ncpp::Timers](#ncpptimers)
	- [Timers::clear()](#timersclear)
- [ncpp::setInterval](#ncppsetinterval)
- [ncpp::setTimeout](#ncppsettimeout)

## ncpp::Semaphore
```cpp
Semaphore(int count=0);
```

### Semaphore.acquire()
```cpp
void acquire();
```
Ожидает, пока не станет доступен слот семафора, и затем захватывает его.
Если ни один слот не доступен (значение равно 0), вызывающий поток будет заблокирован пока другой поток не вызовет `release()` и не освободит слот.

### Semaphore.release()
```cpp
void release();
```
Освобождает слот семафора. Если ранее какой-либо поток был заблокирован вызовом `acquire()`, то он будет разбужен и получит доступ к ресурсу.

## ncpp::Thread
```cpp
Thread();
```
`C++11`: 
```cpp
template<class F, class... Args>
Thread(F&& f, Args&&... args);
```
`C++98`: 
```cpp
template<typename T>
Thread(void(*func)(T*), void* arg=NULL);
Thread(void(*func)());
```

### Thread.join()
```cpp
void join();
```
Ожидает завершение потока.

### Thread.detach()
```cpp
void detach();
```
Отсоединяется от потока, делая его независисмым.

### Thread.kill()
```cpp
void kill();
```
Убивает поток. Не является безопасным, т.к. вызывает неопределенное поведение при попытке его завершения извне.

### Thread::Signal

#### Signal.wait()
```cpp
void wait();
```
Блокирует поток до наступления события, которое возникает при вызове `notify()`.

#### Signal.notify()
```cpp
void notify();
```
Отправляет событие пробуждения для потока, ожидающего `wait()`.

## ncpp::Timers

### Timers::clear()
```cpp
void clear();
```
Очищает все созданные таймеры.

## ncpp::setInterval
```cpp
template<typename T>
void setInterval(void(*func)(T*), int msec, void* arg=NULL);
void setInterval(void(*func)(), int msec);
```
Создает таймер, вызывающий функцию по указателю через равный интервал времени в миллисекундах.

## ncpp::setTimeout
```cpp
template<typename T>
void setTimeout(void(*func)(T*), int msec, void* arg=NULL);
void setTimeout(void(*func)(), int msec);
```
Создает одноразовый таймер, который один раз вызовет функцию по указателю через указанный промежуток времени, после чего будет уничтожен.