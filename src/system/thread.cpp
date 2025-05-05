//#if __cplusplus >= 202002L // C++20 или новее
//#elif __cplusplus >= 201103L // не старее C++11 и не новее C++20
#if __cplusplus >= 201103L && !defined(NO_USE_STDTHREADS) // C++11 или новее
#include <thread>
#include <mutex>
#include <condition_variable>
namespace ncpp {
	struct Semaphore { Semaphore(int count = 0) : _count(count){} // std::counting_semaphore C++20?
		void acquire(){ std::unique_lock<std::mutex> lock(mtx); while(_count == 0){ cv.wait(lock); } _count--; }
		void release(){ std::unique_lock<std::mutex> lock(mtx); _count++; cv.notify_one(); }
		private: std::mutex mtx; std::condition_variable cv; int _count; };
	/*int test(){ std::vector<std::thread> threads;
		for(int i = 0; i < 10; ++i){ threads.emplace_back(access_database, i); } // Создаем и запускаем потоки
		for(auto& th : threads){ th.join(); } return 0; } // Ожидаем завершения всех потоков */
#else // C++ 98 or NO_USE_STDTHREADS
namespace std {
	#ifdef _WIN32
	struct mutex {
		mutex(){ InitializeCriticalSection(&_mtx); }
		~mutex(){ DeleteCriticalSection(&_mtx); }
		void lock(){ EnterCriticalSection(&_mtx); }
		void unlock(){ LeaveCriticalSection(&_mtx); }
		CRITICAL_SECTION& native_handle(){ return _mtx; }
		private: CRITICAL_SECTION _mtx; };
	#else
	#include <pthread.h>
	struct mutex {
		mutex(){ pthread_mutex_init(&_mtx, NULL); }
		~mutex(){ pthread_mutex_destroy(&_mtx); }
		void lock(){ pthread_mutex_lock(&_mtx); }
		void unlock(){ pthread_mutex_unlock(&_mtx); }
		pthread_mutex_t& native_handle(){ return _mtx; }
		private: pthread_mutex_t _mtx; };
	#endif
	
	template<typename T>
	struct unique_lock { unique_lock(T& mtx1) : mtx(mtx1){ mtx.lock(); } ~unique_lock(){ mtx.unlock(); } 
		void lock(){ mtx.lock(); } void unlock(){ mtx.unlock(); }
		T* mutex() const { return &mtx; } private: T& mtx; };
	template<typename T>
	struct lock_guard { lock_guard(T& mtx1) : mtx(mtx1){ mtx.lock(); } ~lock_guard(){ mtx.unlock(); } private: T& mtx; };
	
	#if defined(_WIN32) && _WIN32_WINNT >= 0x0600
	struct condition_variable {
        condition_variable(){ InitializeConditionVariable(&cond); }

        void wait(mutex& mtx){ CRITICAL_SECTION _mtx=mtx.native_handle(); SleepConditionVariableCS(&cond, &_mtx, INFINITE); }
        void wait(unique_lock<std::mutex>& lock){ wait(*lock.mutex()); }
        void notify_one(){ WakeConditionVariable(&cond); }
        void notify_all(){ WakeAllConditionVariable(&cond); }
        private: CONDITION_VARIABLE cond; };
	#elif _WIN32
	struct condition_variable {
		condition_variable() : event(CreateEvent(NULL, FALSE, FALSE, NULL)){}
		~condition_variable(){ CloseHandle(event); }

		void wait(mutex& mtx){ mtx.unlock(); WaitForSingleObject(event, INFINITE); mtx.lock(); }
		void wait(unique_lock<std::mutex>& lock){ wait(*lock.mutex()); }
		void notify_one(){ SetEvent(event); } void notify_all(){ SetEvent(event); } // Упрощённо = notify_one()
		private: HANDLE event; };
	#else
	struct condition_variable {
		condition_variable(){ pthread_cond_init(&cond, NULL); }
		~condition_variable(){ pthread_cond_destroy(&cond); }

		void wait(mutex& mtx){ pthread_cond_wait(&cond, &mtx.native_handle()); }
		void wait(unique_lock<std::mutex>& lock){ wait(*lock.mutex()); }
		void notify_one(){ pthread_cond_signal(&cond); }
		void notify_all(){ pthread_cond_broadcast(&cond); }
		protected: pthread_cond_t cond; };
	#endif

	struct thread { static int _stacklim;
	    thread() : started(false), joined(false){}
		template<typename T>
	    thread(void(*func)(T*), void* arg=NULL) : started(false), joined(false){ start((void*(*)(void*))func, arg); }
		thread(void(*func)()) : started(false), joined(false){ start((void*(*)(void*))func, NULL); }
		
		bool joinable() const { return started && !joined; }
		#ifdef _WIN32
		void join(){ if(!joinable()){ throw std::runtime_error("thread already joined"); }
			HANDLE handle = OpenThread(SYNCHRONIZE, FALSE, id); WaitForSingleObject(handle, INFINITE); CloseHandle(handle); joined = true; }
		void detach(){ if(!joinable()){ throw std::runtime_error("thread not joinable"); } joined = true; }
			
		unsigned int get_id() const { return id; } DWORD native_handle() const { return id; }
		static unsigned hardware_concurrency(){ SYSTEM_INFO sysinfo; GetSystemInfo(&sysinfo); return sysinfo.dwNumberOfProcessors; }
		DWORD id; private: bool started; bool joined;
			void start(void*(*func)(void*), void* arg=NULL){ HANDLE handle = CreateThread(NULL, _stacklim, (LPTHREAD_START_ROUTINE)func, arg, 0, &id);
				if(handle == NULL){ throw std::runtime_error("thread create error"); } CloseHandle(handle); started = true; }
		#else
		void join(){ if(!joinable()){ throw std::runtime_error("thread is not joinable"); }
			pthread_join(id, NULL); joined = true; }
		void detach(){ if (!joinable()){ throw std::runtime_error("thread detach is not avalaible"); }
			pthread_detach(id); joined = true; }
			
		unsigned int get_id() const { return id; } pthread_t native_handle() const { return id; }
		static unsigned hardware_concurrency(){ return sysconf(_SC_NPROCESSORS_ONLN); }
		pthread_t id; private: bool started; bool joined;
		    void start(void*(*func)(void*), void* arg=NULL){ bool ok=false; if(_stacklim<=0){ ok=pthread_create(&id, NULL, func, arg)==0; }
				else{ pthread_attr_t attr; pthread_attr_init(&attr); pthread_attr_setstacksize(&attr, _stacklim); ok=pthread_create(&id, &attr, func, arg)==0; }
				if(!ok){ throw std::runtime_error("pthread: Create thread error."); } started = true; }
		#endif
			//void _prokladka(){}
			//struct _Args { T arg1, T arg2, T arg3 };
	}; int thread::_stacklim=0;
	
	
	#if __cplusplus < 201103L // === Только C++98, C++03
	template<typename T>
	#ifdef _WIN32
	struct atomic { atomic(T value = 0) : value_(value){}
		T load() const { return InterlockedCompareExchange(const_cast<volatile LONG*>(&value_), 0, 0); }
		void store(T value) {InterlockedExchange(const_cast<volatile LONG*>(&value_), value); }
		T fetch_add(T increment) {return InterlockedExchangeAdd(const_cast<volatile LONG*>(&value_), increment); }
		T fetch_sub(T decrement){return InterlockedExchangeAdd(const_cast<volatile LONG*>(&value_), -decrement); }
		bool compare_exchange(T expected, T desired){ return InterlockedCompareExchange(const_cast<volatile LONG*>(&value_), desired, expected) == expected; }
	#else
	struct atomic { atomic(T value = 0) : value_(value){}
		T load() const {return __sync_val_compare_and_swap(const_cast<volatile T*>(&value_), 0, 0);}
		void store(T value) { __sync_lock_test_and_set(&value_, value); }
		T fetch_add(T increment) { return __sync_fetch_and_add(&value_, increment); }
		T fetch_sub(T decrement){ return __sync_fetch_and_sub(&value_, decrement); }
		bool compare_exchange(T expected, T desired){ return __sync_bool_compare_and_swap(&value_, expected, desired); }
	#endif
		atomic<T>& operator+=(T increment){ fetch_add(increment); return *this; }
		atomic<T>& operator-=(T decrement){ fetch_sub(decrement); return *this; }
		atomic<T>& operator++(){ fetch_add(1); return *this; } // prefix: ++a
		atomic<T> operator++(int){ return fetch_add(1); } // postfix: a++
		atomic<T>& operator--(){ fetch_sub(1); return *this; }
		atomic<T> operator--(int){ return fetch_sub(1); }
		atomic<T>& operator=(const atomic<T>& other){ if(this != &other){ store(other.load()); } return *this; }
		atomic<T>& operator=(T value){ store(value); return *this; }
		bool operator==(T other) const { return load() == other; }
		private: volatile T value_; };
	
	template<typename T>
	struct shared_ptr { // std::shared_ptr С++11 polyfill
		shared_ptr() : ptr(NULL), ctrl(new CtrlBlock()){}
		explicit shared_ptr(T* p) : ptr(p), ctrl(new CtrlBlock(1)){}
		shared_ptr(const shared_ptr<T>& other) : ptr(other.ptr), ctrl(other.ctrl){ if(ptr){ ++(ctrl->rcount); } }
		~shared_ptr(){ release(); }
		T* get() const { return ptr; }
		int use_count() const { return ctrl?ctrl->rcount.load():0; }
		void reset(T* p = NULL){ release(); ptr = p; ctrl = new CtrlBlock(1); }

		shared_ptr<T>& operator=(const shared_ptr<T>& other){
			if(this != &other){ release(); ptr = other.ptr; ctrl = other.ctrl; if(ptr){ ++(ctrl->rcount); } } return *this; }
		T& operator*() const { return *ptr; }
		T* operator->() const { return ptr; }
		bool operator==(const shared_ptr<T>& other) const { return ptr==other.ptr; }
		bool operator==(T* p) const { return ptr==p; }

		private: struct CtrlBlock { CtrlBlock(int count=0) : rcount(count){} std::atomic<int> rcount; }; T* ptr; CtrlBlock* ctrl;
			void release(){ if(ptr!=NULL && ctrl->rcount.fetch_sub(1) <= 1){ delete ptr; delete ctrl; ptr = NULL; ctrl = NULL; } } };
	#endif
}

namespace ncpp {
	#ifdef _WIN32
	struct Semaphore { Semaphore(int count = 0){ semaphore = CreateSemaphore(NULL, count, count, NULL); }
		~Semaphore(){ CloseHandle(semaphore); }
		void acquire(){ WaitForSingleObject(semaphore, INFINITE); }
		void release(){ ReleaseSemaphore(semaphore, 1, NULL); } private: HANDLE semaphore; };
	#else
	struct Semaphore { Semaphore(int count = 0) : _count(count){ pthread_mutex_init(&mtx, NULL); pthread_cond_init(&cond, NULL); }
		~Semaphore(){ pthread_mutex_destroy(&mtx); pthread_cond_destroy(&cond); }
		void acquire(){ pthread_mutex_lock(&mtx); while(_count == 0){ pthread_cond_wait(&cond, &mtx); } pthread_mutex_unlock(&mtx); }
		void release(){ pthread_mutex_lock(&mtx); _count++; pthread_cond_signal(&cond); pthread_mutex_unlock(&mtx); }
		private: pthread_mutex_t mtx; pthread_cond_t cond; int _count; };
	#endif
#endif
	//struct Promise {} //std::promise?
	struct Thread : std::thread { //typedef void (*TArgFunc)(void*);
		Thread() : std::thread(){}
		#if __cplusplus >= 201103L
		template<class F, class... Args>
	    Thread(F&& f, Args&&... args) : std::thread(f, args...){}
		#else
		template<typename T>
	    Thread(void(*func)(T*), void* arg=NULL) : std::thread(func, arg){}
		Thread(void(*func)()) : std::thread(func){}
		#endif
		~Thread(){ if(joinable()){ detach(); } }
		
		void join(){ if(joinable()){ std::thread::join(); } }
		void detach(){ if(joinable()){ std::thread::detach(); } }
		void kill(){
		#ifdef _WIN32
			HANDLE hThread = OpenThread(THREAD_TERMINATE, FALSE, native_handle()); TerminateThread(hThread, 0); CloseHandle(hThread);
		#else
			pthread_cancel(native_handle());
		#endif
		}
		//Bug 58909 - C++11's condition variables fail with static linking (-static -pthread)
		//https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58909 
		struct Signal { std::condition_variable cv; std::mutex mtx; bool signaled;
			Signal() : signaled(false){}
			void wait(){ if(!signaled){ std::unique_lock<std::mutex> lock(mtx); cv.wait(lock); } signaled = false; }
			void notify(){ signaled = true; cv.notify_one(); } };
	};
	
	
	namespace Timers { bool _run=false; std::thread _TimerThr; static void _handler();
		struct Timer { void(*func)(void*); long long msec; Date last; bool once; void* arg; }; std::vector<Timer> List;
		template<typename F>
		void add(F func, int msec, bool once=false, void* arg=NULL){ Timer timer; timer.func=(void(*)(void*))func; 
			timer.arg=(void*)arg; timer.msec=msec; timer.last=Date::now(); timer.once=once; bool empty=List.empty();
			List.push_back(timer); if(empty){ _TimerThr=std::thread(_handler); _TimerThr.detach(); } }
		
		static void _handler(){ _run=true; //std::cout << "Timers debug: Thread created." << std::endl;
			//if(List.size()>=1){ std::cout << "; List[0].msec=" << List[0].msec << "; List[0].once=" << List[0].once << std::endl; }else{ std::cout << std::endl; }
			while(!List.empty()){ long long minTime = 9223372036854775807LL; Date now = Date::now(); 
				for(size_t i=0; i<List.size(); ++i){ long long timeLeft = List[i].msec-(now-List[i].last);
					if(timeLeft < minTime){ minTime = timeLeft; } }
				
				if(minTime > 0){ Sleep((int)minTime); } now = Date::now();
				for(std::vector<Timer>::iterator it = List.begin(); it != List.end(); ){ if(!_run){ List.clear(); break; } Timer& timer = *it;
					if((now-timer.last) >= timer.msec){ timer.func(timer.arg); timer.last += timer.msec;
						if(timer.once){ it = List.erase(it); continue; } } ++it; } 
			} }
		void clear(){ _run=false; }
		void wait(){ if(_TimerThr.joinable()) _TimerThr.join(); } //_TimerThr.detach();
	}
	
	template<typename T>
	void setInterval(void(*func)(T*), int msec, void* arg=NULL){ Timers::add(func, msec, false, arg); };
	void setInterval(void(*func)(), int msec){ Timers::add(func, msec, false); };
	template<typename T>
	void setTimeout(void(*func)(T*), int msec, void* arg=NULL){ Timers::add(func, msec, true, arg); };
	void setTimeout(void(*func)(), int msec){ Timers::add(func, msec, true); };
}
