[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Mutex](#ncppmutex)
    - [Mutex.lock()](#mutexlock)
    - [Mutex.unlock()](#mutexunlock)
    - [Mutex.native_handle()](#mutexnative_handle)
- [ncpp::unique_lock](#ncppunique_lock)
    - [unique_lock.lock()](#unique_locklock)
    - [unique_lock.unlock()](#unique_lockunlock)
    - [unique_lock.mutex()](#unique_lockmutex)
- [ncpp::lock_guard](#ncpplock_guard)
- [ncpp::Thread](#ncppthread)
    - [Thread.joinable()](#threadjoinable)
    - [Thread.join()](#threadjoin)
    - [Thread.detach()](#threaddetach)
    - [Thread.kill()](#threadkill)
    - [Thread.getId()](#threadgetid)
    - [Thread::hardware_concurrency()](#threadhardware_concurrency)
- [ncpp::Thread::Signal](#ncppthreadsignal)
    - [Signal.wait()](#signalwait)
    - [Signal.notify()](#signalnotify)
- [ncpp::Semaphore](#ncppsemaphore)
    - [Semaphore.acquire()](#semaphoreacquire)
    - [Semaphore.release()](#semaphorerelease)
- [ncpp::condition_variable](#ncppcondition_variable)
    - [condition_variable.wait()](#condition_variablewait)
    - [condition_variable.notify_one()](#condition_variablenotify_one)
    - [condition_variable.notify_all()](#condition_variablenotify_all)
- [ncpp::Atomic](#ncppatomic)
    - [Atomic.load()](#atomicload)
    - [Atomic.store()](#atomicstore)
    - [Atomic.fetch_add()](#atomicfetch_add)
    - [Atomic.fetch_sub()](#atomicfetch_sub)
    - [Atomic.compare_exchange()](#atomiccompare_exchange)
- [ncpp::SharedPtr](#ncppsharedptr)
    - [SharedPtr.get()](#sharedptrget)
    - [SharedPtr.use_count()](#sharedptruse_count)
    - [SharedPtr.reset()](#sharedptrreset)
- [ncpp::UniquePtr](#ncppuniqueptr)
    - [UniquePtr.get()](#uniqueptrget)
    - [UniquePtr.reset()](#uniqueptrreset)
    - [UniquePtr.release()](#uniqueptrrelease)
    - [UniquePtr.swap()](#uniqueptrswap)
    - [UniquePtr.move()](#uniqueptrmove)
    - [UniquePtr.steal()](#uniqueptrsteal)
- [ncpp::clearInterval()](#ncppclearinterval)
- [ncpp::setInterval()](#ncppsetinterval)
- [ncpp::setTimeout()](#ncppsettimeout)

## ncpp::Mutex
`Mutex` — легковесная альтернатива `std::mutex` (С++11).

```cpp
Mutex();
```

### Mutex.lock()
```cpp
void lock();
```
Блокирует мьютекс. Если он уже захвачен другим потоком, текущий поток приостанавливает выполнение до освобождения.

### Mutex.unlock()
```cpp
void unlock();
```
Освобождает мьютекс, позволяя другим потокам захватить его.

### Mutex.native_handle()
```cpp
#ifdef _WIN32
CRITICAL_SECTION& native_handle();
#else
pthread_mutex_t& native_handle();
#endif
```
Возвращает ссылку на нативный дескриптор системы.

## ncpp::unique_lock
RAII-обертка над `Mutex` для управления мьютексом с возможностью ручного управления блокировкой. aka `std::unique_lock` (С++11)

```cpp
template<typename T> unique_lock(T& mtx); //Mutex& mtx
```

### unique_lock.lock()
```cpp
void lock();
```
Захватывает связанный мьютекс.

### unique_lock.unlock()
```cpp
void unlock();
```
Освобождает связанный мьютекс.

### unique_lock.mutex()
```cpp
T* mutex() const;
```
Возвращает указатель на объект мьютекса.

## ncpp::lock_guard
Простейшая RAII-обертка над `Mutex`. Захватывает мьютекс в конструкторе и освобождает в деструкторе. aka `std::lock_guard` (С++11)

```cpp
template<typename T> lock_guard(T& mtx); //Mutex& mtx
```

## ncpp::Thread
`Thread` — предоставляет возможность кроссплатформенного создания потока. Альтернатива `std::thread` (С++11).

```cpp
Thread();
template<typename T>
Thread(void(*func)(T*), void* arg=NULL);
Thread(void(*func)());
```

### Thread.joinable()
```cpp
bool joinable() const;
```
Возвращает `true`, если поток был запущен и еще не был присоединен `join()` или отсоединен `detach()`.

### Thread.join()
```cpp
void join();
```
Блокирует текущий поток до завершения выполнения потока, представленного объектом.

### Thread.detach()
```cpp
void detach();
```
Разрывает связь между объектом и потоком. Поток продолжает выполнение в фоновом режиме.

### Thread.kill()
```cpp
void kill();
```
Принудительно завершает выполнение потока. (Использовать с осторожностью).

### Thread.getId()
```cpp
Thread::Id getId() const;
```
Возвращает обертку идентификатора потока `Thread::Id`.

### Thread::hardware_concurrency()
```cpp
static unsigned hardware_concurrency();
```
Возвращает количество доступных аппаратных ядер процессора.

## ncpp::Thread::Signal
`Thread::Signal` — упрощенный механизм сигнализации (Auto-Reset Event) для взаимодействия 1P:1C.

### Signal.wait()
```cpp
void wait();
```
Ожидает получения сигнала. Если сигнал уже был отправлен, сбрасывает его и продолжает выполнение.

### Signal.notify()
```cpp
void notify();
```
Отправляет сигнал ожидающему потоку.

## ncpp::Semaphore
`Semaphore` - механизм синхронизации для ограничения доступа к ресурсам.

```cpp
Semaphore(int count = 0)
```

### Semaphore.acquire()
```cpp
void acquire();
```
Уменьшает счетчик семафора. Если счетчик равен 0, поток блокируется.

### Semaphore.release()
```cpp
void release();
```
Увеличивает счетчик семафора и пробуждает один из ожидающих потоков.

## ncpp::condition_variable
`condition_variable` - переменная условия для синхронизации потоков на основе предикатов. aka `std::condition_variable` (C++11)

### condition_variable.wait()
```cpp
void wait(Mutex& mtx);
void wait(unique_lock<Mutex>& lock);
```
Атомарно освобождает мьютекс и блокирует поток до получения уведомления.

### condition_variable.notify_one()
```cpp
void notify_one();
```
Разблокирует один из потоков, ожидающих на этой переменной.

### condition_variable.notify_all()
```cpp
void notify_all();
```
Разблокирует все потоки, ожидающие на этой переменной.

### ncpp::Atomic
`Atomic` - обеспечивает атомарность операций. Гарантирует `memory_order_seq_cst`.

### Atomic.load()
```cpp
T load() const;
```
Атомарно считывает текущее значение.

### Atomic.store()
```cpp
void store(T value);
```
Атомарно записывает новое значение.

### Atomic.fetch_add()
```cpp
T fetch_add(T increment);
```
Атомарно прибавляет значение и возвращает старое значение.

### Atomic.fetch_sub()
```cpp
T fetch_sub(T decrement);
```
Атомарно вычитает значение и возвращает старое значение.

### Atomic.compare_exchange()
```cpp
bool compare_exchange(T expected, T desired);
```
Сравнивает текущее значение с `expected`. Если они равны, заменяет текущее на `desired` и возвращает `true`.

## ncpp::SharedPtr
`SharedPtr` - умный указатель, поведение которого приближено к `std::shared_ptr` (С++11).

```cpp
SharedPtr();
explicit SharedPtr(T* p);
SharedPtr(const SharedPtr<T>& other);
```

Определения:
```cpp
#if __cplusplus >= 201103L
template<typename T> using SPtr = SharedPtr<T>;
#else
template<typename T> struct SPtr : SharedPtr<T>{};
#endif
```

### SharedPtr.get()
```cpp
T* get() const;
```
Возвращает сырой указатель.

### SharedPtr.use_count()
```cpp
int use_count() const;
```
Возвращает текущее количество владельцев объекта.

### SharedPtr.reset()
```cpp
void reset(T* p = NULL);
```
Заменяет управляемый объект. Если старый объект больше не имеет владельцев, он удаляется.

## ncpp::UniquePtr
`SharedPtr` - уникальный указатель, поведение которого приближено к `std::unique_ptr` (С++11).

```cpp
UniquePtr();
explicit UniquePtr(T* p = nullptr);
```

Определения:
```cpp
#if __cplusplus >= 201103L
template<typename T> using UPtr = UniquePtr<T>;
#else
template<typename T> struct UPtr : UniquePtr<T>{};
#endif
```

### UniquePtr.get()
```cpp
T* get() const;
```
Возвращает сырой указатель.

### UniquePtr.reset()
```cpp
void reset(T* p = nullptr);
```
Удаляет текущий объект и берет под управление новый.

### UniquePtr.release()
```cpp
T* release() noexcept;
```
Отказывается от владения объектом и возвращает указатель на него без удаления.

### UniquePtr.swap()
```cpp
void swap(UniquePtr& other) noexcept;
friend void swap(UniquePtr& a, UniquePtr& b) noexcept;
```
Обменивает содержимое двух указателей.

### UniquePtr::move()
```cpp
friend void move(UniquePtr& dst, UniquePtr& tmp);
```
C++11:
```cpp
friend void move(UniquePtr& dst, UniquePtr&& tmp);
```
C++98:
```cpp
friend void move(UniquePtr& dst, const UniquePtr& victim);
```
Реализация семантики перемещения. Позволяет передать владение ресурсами от одного объекта (жертвы) другому, оставляя исходный объект пустым, но валидным.
В C++11 и выше используются стандартные `rvalue` ссылки.

### UniquePtr.steal()
C++11:
```cpp
UniquePtr& steal(UniquePtr& tmp);
UniquePtr& steal(UniquePtr&& tmp);
```
C++98:
```cpp
UniquePtr& steal(const UniquePtr& victim);
```
Метод-обертка для `move()`. "Крадет" (перемещает) содержимое `victim` и зануляет его, сохраняя валидным.

## ncpp::clearInterval()
```cpp
void clearInterval(int id);
```
Останавливает и удаляет таймер по его `id`.

## ncpp::setInterval()
```cpp
template<typename T>
int setInterval(void(*func)(T*), int msec, void* arg = NULL);
int setInterval(void(*func)(), int msec);
```
Запускает выполнение функции каждые `msec` миллисекунд.

## ncpp::setTimeout()
```cpp
template<typename T>
int setTimeout(void(*func)(T*), int msec, void* arg = NULL);
int setTimeout(void(*func)(), int msec);
```
Запускает выполнение функции один раз через `msec` миллисекунд.