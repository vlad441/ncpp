#ifndef _WIN32
#include <pthread.h>
#endif
namespace ncpp {
	#ifdef _WIN32
	struct Mutex { //aka std::mutex (С++11)
		Mutex(){ InitializeCriticalSection(&_mtx); }
		~Mutex(){ DeleteCriticalSection(&_mtx); }
		void lock(){ EnterCriticalSection(&_mtx); }
		void unlock(){ LeaveCriticalSection(&_mtx); }
		CRITICAL_SECTION& native_handle(){ return _mtx; }
		private: CRITICAL_SECTION _mtx; };
	#else
	struct Mutex {
		Mutex(){ pthread_mutex_init(&_mtx, NULL); }
		~Mutex(){ pthread_mutex_destroy(&_mtx); }
		void lock(){ pthread_mutex_lock(&_mtx); }
		void unlock(){ pthread_mutex_unlock(&_mtx); }
		pthread_mutex_t& native_handle(){ return _mtx; }
		private: pthread_mutex_t _mtx; };
	#endif
	
	template<typename T>
	struct unique_lock { unique_lock(T& mtx) : _mtx(mtx){ _mtx.lock(); } ~unique_lock(){ _mtx.unlock(); } //UniqueLock?
		void lock(){ _mtx.lock(); } void unlock(){ _mtx.unlock(); }
		T* mutex() const { return &_mtx; } private: T& _mtx; };
	template<typename T> struct lock_guard { lock_guard(T& mtx) : _mtx(mtx){ _mtx.lock(); } ~lock_guard(){ _mtx.unlock(); } private: T& _mtx; }; //LockGuard?
	
	struct condition_variable;
	struct Thread { struct Id {
			#ifdef _WIN32
			DWORD _id; Id(DWORD id) : _id(id){} Id() : _id(0){} 
			bool operator==(const Id& other) const { return _id == other._id; }
			bool operator!=(const Id& other) const { return _id != other._id; }
			bool operator<=(const Id& other) const { return _id <= other._id; }
			bool operator>=(const Id& other) const { return _id >= other._id; }
			bool operator<(const Id& other) const { return _id < other._id; }
			bool operator>(const Id& other) const { return _id > other._id; }
			#else
			pthread_t _id; Id(const pthread_t& id) : _id(id){} Id(){} //Id(){ memset(&_id, 0, sizeof(pthread_t)); }
			bool operator==(const Id& other) const { return pthread_equal(_id, other._id) != 0; }
			bool operator!=(const Id& other) const { return pthread_equal(_id, other._id) == 0; }
			bool operator<=(const Id& other) const { return memcmp(&_id, &other._id, sizeof(pthread_t)) <= 0; }
			bool operator>=(const Id& other) const { return memcmp(&_id, &other._id, sizeof(pthread_t)) >= 0; }
			bool operator<(const Id& other) const { return memcmp(&_id, &other._id, sizeof(pthread_t)) < 0; }
			bool operator>(const Id& other) const { return memcmp(&_id, &other._id, sizeof(pthread_t)) > 0; }
			#endif	
		}; static int _stacklim; //aka std::thread (С++11)
	    Thread() : started(false), joined(false){}
		template<typename T>
	    Thread(void(*func)(T*), void* arg=NULL) : started(false), joined(false){ start((void*(*)(void*))func, arg); }
		Thread(void(*func)()) : started(false), joined(false){ start((void*(*)(void*))func, NULL); }
		~Thread(){ if(joinable()) detach(); }
		
		bool joinable() const { return started && !joined; }
		Id getId() const { return _thr; }
		#ifdef _WIN32
		void join(){ if(!joinable()){ return; } HANDLE hThread = OpenThread(SYNCHRONIZE, FALSE, _thr._id); WaitForSingleObject(hThread, INFINITE); CloseHandle(hThread); joined = true; }
		void detach(){ if(!joinable()){ return; } joined = true; }
		void kill(){ HANDLE hThread = OpenThread(THREAD_TERMINATE, FALSE, native_handle()); TerminateThread(hThread, 0); CloseHandle(hThread); }
			
		DWORD native_handle() const { return _thr._id; }
		static unsigned hardware_concurrency(){ SYSTEM_INFO sysinfo; GetSystemInfo(&sysinfo); return sysinfo.dwNumberOfProcessors; }
		//static Id getTID(){ return Id(GetCurrentThreadId()); }
		//static unsigned long _getNativeTID(){ return GetCurrentThreadId(); }
		private: 
			void start(void*(*func)(void*), void* arg=NULL){ HANDLE handle = CreateThread(NULL, _stacklim, (LPTHREAD_START_ROUTINE)func, arg, 0, &_thr._id);
				if(handle == NULL){ Except("winapi: Thread create error\n"); } CloseHandle(handle); started = true; }
		#else
		void join(){ if(!joinable()){ return; } pthread_join(_thr._id, NULL); joined = true; }
		void detach(){ if(!joinable()){ return; } pthread_detach(_thr._id); joined = true; }
		void kill(){ pthread_cancel(native_handle()); }
			
		pthread_t native_handle() const { return _thr._id; }
		static unsigned hardware_concurrency(){ return sysconf(_SC_NPROCESSORS_ONLN); }
		//static Id getTID(){ return Id(pthread_self()); }
		//static unsigned long _getNativeTID(){ return syscall(SYS_gettid); }
		private:
		    void start(void*(*func)(void*), void* arg=NULL){ bool ok=false; if(_stacklim<=0){ ok=pthread_create(&_thr._id, NULL, func, arg)==0; }
				else{ pthread_attr_t attr; pthread_attr_init(&attr); pthread_attr_setstacksize(&attr, _stacklim); ok=pthread_create(&_thr._id, &attr, func, arg)==0; }
				if(!ok){ Except("pthread: Thread create error\n"); } started = true; }
		#endif
		Id _thr; bool started; bool joined; public:
			//void _prokladka(){}
			//struct _Args { T arg1, T arg2, T arg3 };
			
		//Bug 58909 - C++11's condition variables fail with static linking (-static -pthread)
		//https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58909
		struct ConditionVariable; typedef condition_variable CondV; struct Signal;
	}; int Thread::_stacklim=0;
	//uint32_t hash(const Thread::Id& id){ return Fnv1a(c, strlen(c)); }
	
	#ifdef _WIN32
	struct Semaphore { Semaphore(int count = 0){ _semaphore = CreateSemaphore(NULL, count, count, NULL); }
		//Semaphore(){ _semaphore = CreateSemaphore(NULL, 0, INT_MAX, NULL); }
		~Semaphore(){ CloseHandle(_semaphore); }
		void acquire(){ WaitForSingleObject(_semaphore, INFINITE); }
		void release(){ ReleaseSemaphore(_semaphore, 1, NULL); } private: HANDLE _semaphore; };
	#else
	struct Semaphore { Semaphore(int count = 0) : _count(count){ pthread_mutex_init(&mtx, NULL); pthread_cond_init(&cond, NULL); }
		~Semaphore(){ pthread_mutex_destroy(&mtx); pthread_cond_destroy(&cond); }
		void acquire(){ pthread_mutex_lock(&mtx); while(_count == 0){ pthread_cond_wait(&cond, &mtx); } pthread_mutex_unlock(&mtx); }
		void release(){ pthread_mutex_lock(&mtx); _count++; pthread_cond_signal(&cond); pthread_mutex_unlock(&mtx); }
		private: pthread_mutex_t mtx; pthread_cond_t cond; int _count; };
	#endif
	
	#if defined(_WIN32) && _WIN32_WINNT >= 0x0600 // >= Windows Vista
	struct condition_variable {
        condition_variable(){ InitializeConditionVariable(&cond); }
		
        void wait(Mutex& mtx){ CRITICAL_SECTION& _mtx=mtx.native_handle(); SleepConditionVariableCS(&cond, &_mtx, INFINITE); }
        void wait(unique_lock<Mutex>& lock){ wait(*lock.mutex()); }
        void notify_one(){ WakeConditionVariable(&cond); }
        void notify_all(){ WakeAllConditionVariable(&cond); }
        private: CONDITION_VARIABLE cond; };
	#elif _WIN32 // Windows XP
	struct condition_variable { condition_variable() : _wait_cnt(0){}
		void wait(Mutex& usr_mtx){ _int_mtx.lock(); _wait_cnt++; _int_mtx.unlock();
			usr_mtx.unlock(); _sem.acquire(); usr_mtx.lock(); }
		void wait(unique_lock<Mutex>& lock){ wait(*lock.mutex()); }

		void notify_one(){ _int_mtx.lock(); if(_wait_cnt > 0){ _wait_cnt--; _sem.release(); } _int_mtx.unlock(); }
		void notify_all(){ _int_mtx.lock(); int rel_cnt = _wait_cnt; _wait_cnt = 0; _int_mtx.unlock(); 
			for(int i = 0; i < rel_cnt; ++i){ _sem.release(); } }
		private: Semaphore _sem; Mutex _int_mtx; volatile int _wait_cnt; };
	#else
	struct condition_variable {
		condition_variable(){ pthread_cond_init(&cond, NULL); }
		~condition_variable(){ pthread_cond_destroy(&cond); }

		void wait(Mutex& mtx){ pthread_cond_wait(&cond, &mtx.native_handle()); }
		void wait(unique_lock<Mutex>& lock){ wait(*lock.mutex()); }
		void notify_one(){ pthread_cond_signal(&cond); }
		void notify_all(){ pthread_cond_broadcast(&cond); }
		protected: pthread_cond_t cond; };
	#endif
		
	struct Thread::Signal { Signal() : _count(0){} //Auto-Reset Event 1P:1C
		void wait(){ unique_lock<Mutex> lock(mtx); while(_count == 0){ cv.wait(lock); } _count = 0; }
		void notify(){ unique_lock<Mutex> lock(mtx); if(_count == 0){ _count = 1; cv.notify_one(); } }
		
		//void wait(){ unique_lock<Mutex> lock(mtx); while(!signaled){ cv.wait(lock); } signaled = false; }
		//void notify(){ unique_lock<Mutex> lock(mtx); if(!signaled){ signaled = true; cv.notify_one(); } }
		private: condition_variable cv; Mutex mtx; char _count; };
	
	template<typename T>
	struct Atomic { Atomic(T value = 0) : _value(value){} //aka std::atomic (С++11), memory order = seq_cst
	#if defined(_WIN32) && !defined(_WIN64) // only WIN 32 bit
		T load() const { return InterlockedCompareExchange(const_cast<volatile LONG*>(&_value), 0, 0); }
		void store(T value) { InterlockedExchange(const_cast<volatile LONG*>(&_value), value); }
		T fetch_add(T increment){ return InterlockedExchangeAdd(const_cast<volatile LONG*>(&_value), increment); }
		T fetch_sub(T decrement){ return InterlockedExchangeAdd(const_cast<volatile LONG*>(&_value), -decrement); }
		bool compare_exchange(T expected, T desired){ return InterlockedCompareExchange(const_cast<volatile LONG*>(&_value), desired, expected) == expected; }
	#else
		T load() const { return __sync_val_compare_and_swap(const_cast<volatile T*>(&_value), 0, 0); }
		void store(T value){ __sync_lock_test_and_set(&_value, value); }
		T fetch_add(T increment){ return __sync_fetch_and_add(&_value, increment); }
		T fetch_sub(T decrement){ return __sync_fetch_and_sub(&_value, decrement); }
		bool compare_exchange(T expected, T desired){ return __sync_bool_compare_and_swap(&_value, expected, desired); }
	#endif
		Atomic<T>& operator=(const Atomic<T>& other){ if(this != &other){ store(other.load()); } return *this; }
		Atomic<T>& operator=(T value){ store(value); return *this; }
		
		Atomic<T>& operator+=(T increment){ fetch_add(increment); return *this; }
		Atomic<T>& operator-=(T decrement){ fetch_sub(decrement); return *this; }
		Atomic<T>& operator++(){ fetch_add(1); return *this; } // prefix: ++a
		Atomic<T> operator++(int){ return Atomic<T>(fetch_add(1)); } // postfix: a++
		Atomic<T>& operator--(){ fetch_sub(1); return *this; }
		Atomic<T> operator--(int){ return Atomic<T>(fetch_sub(1)); }
		bool operator==(const Atomic<T>& other) const { return load() == other.load(); }
		bool operator==(T other) const { return load() == other; }
		operator T() const { return load(); }
		private: volatile T _value; };
	
	template<typename T>
	struct SPtr { // aka std::shared_ptr (С++11)
		//SPtr() : _ptr(NULL), ctrl(NULL){}
		explicit SPtr(T* p = NULL) : _ptr(p), ctrl(p?new CtrlBlock(1):NULL){}
		SPtr(const SPtr<T>& other) : _ptr(other._ptr), ctrl(other.ctrl){ if(ctrl) ++(ctrl->rcount); }
		~SPtr(){ release(); }
		
		//SPtr<T>& operator=(T* p){ reset(p); return *this; }
		SPtr<T>& operator=(const SPtr<T>& other){ if(this == &other) return *this;
			release(); _ptr = other._ptr; ctrl = other.ctrl; if(ctrl) ++(ctrl->rcount); return *this; }
			
		T& operator*() const { return *_ptr; }
		T* operator->() const { return _ptr; }
		bool operator==(const SPtr<T>& other) const { return _ptr==other._ptr; }
		bool operator==(T* p) const { return _ptr==p; }
		
		T* get() const { return _ptr; }
		void reset(T* p = NULL){ if(_ptr == p) return; release(); _ptr = p; ctrl = p?new CtrlBlock(1):NULL; }
		void release(){ if(ctrl!=NULL && ctrl->rcount.fetch_sub(1) == 1){ delete _ptr; delete ctrl; _ptr = NULL; ctrl = NULL; } }
		int use_count() const { return ctrl?ctrl->rcount.load():0; }
		
		#if __cplusplus >= 201103L
		//SPtr(SPtr&& other);
		//SPtr& operator=(SPtr&& other);
		//friend void move(SPtr& dst, SPtr&& tmp){ move(dst, (SPtr&)tmp); }
		#else
		//friend void move(SPtr& dst, const SPtr& victim){ move(dst, (SPtr&)victim); }
		#endif

		private: struct CtrlBlock { CtrlBlock(int count=0) : rcount(count){} Atomic<int> rcount; }; T* _ptr; CtrlBlock* ctrl; };
	#if __cplusplus >= 201103L
	template<typename T> using SharedPtr = SPtr<T>;
	#else
	#define SharedPtr SPtr //template<typename T> struct SharedPtr : SPtr<T>{}; 
	#endif
	
	template<typename T>
	struct UPtr { //aka std::unique_ptr (С++11)
		//UPtr() : ptr(new T()){}
		explicit UPtr(T* p = NULL) : _ptr(p){}
		~UPtr(){ delete _ptr; }

		#if __cplusplus >= 201103L
		UPtr(const UPtr&) = delete; UPtr& operator=(const UPtr&) = delete;
		
		UPtr(UPtr&& other) : _ptr(other._ptr){ other._ptr = NULL; }
		UPtr& operator=(UPtr&& other){ reset(other.release()); return *this; }
		
		UPtr& steal(UPtr& tmp){ move(*this, tmp); return *this; }
		UPtr& steal(UPtr&& tmp){ move(*this, tmp); return *this; }
		friend void move(UPtr& dst, UPtr&& tmp){ move(dst, (UPtr&)tmp); }
		#else
		private: UPtr(const UPtr&); UPtr& operator=(const UPtr&); public:
		UPtr& steal(const UPtr& victim){ move(*this, (UPtr&)victim); return *this; }
		friend void move(UPtr& dst, const UPtr& victim){ move(dst, (UPtr&)victim); }
		#endif

		T& operator*() const { return *_ptr; }
		T* operator->() const { return _ptr; }
		bool operator==(const UPtr<T>& other) const { return _ptr==other._ptr; }
		bool operator==(T* p) const { return _ptr==p; }
		
		T* get() const { return _ptr; }
		void reset(T* p = NULL){ if(_ptr == p) return; delete _ptr; _ptr = p; }
		T* release() noexcept { T* tmp = _ptr; _ptr = NULL; return tmp; }
		void swap(UPtr& other) noexcept { T* tmp=_ptr; _ptr=other._ptr; other._ptr=tmp; }
		
		friend void swap(UPtr& a, UPtr& b) noexcept { a.swap(b); }
		friend void move(UPtr& dst, UPtr& src) noexcept { dst.reset(src.release()); }
		//friend void move(UPtr& dst, UPtr& src) noexcept { if (&dst == &src) return; if(dst._ptr) delete dst._ptr; dst._ptr=src._ptr; src._ptr=nullptr; }
		private: T* _ptr; };
	#if __cplusplus >= 201103L
	template<typename T> using UniquePtr = UPtr<T>;
	#else
	#define UniquePtr UPtr //template<typename T> struct UniquePtr : UPtr<T>{};
	#endif
	
	namespace Timers { Mutex _ThrMtx; bool _run=false; Thread _TimerThr; int _tMaxIdx=0; static void _handler();
		struct Timer { void(*func)(void*); long long msec; long long last; bool once; void* arg; }; typedef HashMap<int, Timer> TimersL;
		TimersL tList;
		template<typename F>
		int add(F func, int msec, bool once=false, void* arg=NULL){ Timer timer; timer.func=(void(*)(void*))func; 
			timer.arg=(void*)arg; timer.msec=msec; timer.last=GetTimestamp('m'); timer.once=once; 
			_ThrMtx.lock(); bool empty=tList.empty(); tList[_tMaxIdx]=timer; /*tList.push(timer);*/ int idx=_tMaxIdx++; _ThrMtx.unlock();
			if(empty){ _TimerThr=Thread(_handler); /*_TimerThr.detach();*/ } return idx; }
		
		static void _handler(){ _run=true;
			while(!tList.empty()){ long long minTime = 9223372036854775807LL; long long now = GetTimestamp('m');
				for(TimersL::Iter it = tList.begin(); it != tList.end(); ++it){ Timer& t = it->second;
					long long timeLeft = t.msec-(now-t.last); if(timeLeft < minTime){ minTime = timeLeft; } }
				
				if(minTime>0&&minTime!=LLONG_MAX){ Sleep((int)minTime); } now = GetTimestamp('m');
				for(TimersL::Iter it = tList.begin(); it != tList.end(); ){ if(!_run){ _ThrMtx.lock(); tList.clear(); _ThrMtx.unlock(); break; } 
					Timer& t = it->second;
					if((now-t.last) >= t.msec){ t.func(t.arg); t.last += t.msec;
						if(t.once){ _ThrMtx.lock(); it = tList.erase(it); _ThrMtx.unlock(); continue; } } ++it; }
			} }
		void clear(){ _run=false; _tMaxIdx=0; }
		void wait(){ if(_TimerThr.joinable()) _TimerThr.join(); }
		void detach(){ if(_TimerThr.joinable()) _TimerThr.detach(); }
		void Delete(int idx){ _ThrMtx.lock(); tList.erase(idx); if(tList.empty()){ _tMaxIdx=0; } _ThrMtx.unlock(); }
	}
	
	void clearInterval(int id){ Timers::Delete(id); }
	template<typename T>
	int setInterval(void(*func)(T*), int msec, void* arg=NULL){ return Timers::add(func, msec, false, arg); };
	int setInterval(void(*func)(), int msec){ return Timers::add(func, msec, false); };
	template<typename T>
	int setTimeout(void(*func)(T*), int msec, void* arg=NULL){ return Timers::add(func, msec, true, arg); };
	int setTimeout(void(*func)(), int msec){ return Timers::add(func, msec, true); };
	
}