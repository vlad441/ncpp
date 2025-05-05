namespace std {
	struct mutex {
		void lock();
		void unlock();
		pthread_mutex_t native_handle() const; };
	
	template<typename T>
	struct unique_lock;
	template<typename T>
	struct lock_guard;
	
	struct condition_variable {
		condition_variable(){ pthread_cond_init(&cond, NULL); }
		~condition_variable(){ pthread_cond_destroy(&cond); }

		void wait(mutex& mtx){ pthread_mutex_t _mtx=mtx.native_handle(); pthread_cond_wait(&cond, &_mtx); }
		void wait(unique_lock<std::mutex>& lock){ wait(*lock.mutex()); }
		void notify_one(){ pthread_cond_signal(&cond); }
		void notify_all(){ pthread_cond_broadcast(&cond); } };

	struct thread {
	    thread() : started(false), joined(false){}
		thread(void(*func)()) : started(false), joined(false){ start((void*(*)(void*))func, NULL); }
		template<typename T>
	    thread(void(*func)(T*), T* arg=NULL) : started(false), joined(false){ start((void*(*)(void*))func, arg); }
		
		bool joinable() const { return started && !joined; }
		#ifdef _WIN32
		void join(){ if(!joinable()){ throw std::runtime_error("thread already joined"); }
			HANDLE handle = OpenThread(SYNCHRONIZE, FALSE, id); WaitForSingleObject(handle, INFINITE); CloseHandle(handle); joined = true; }
		void detach(){ if(!joinable()){ throw std::runtime_error("thread not joinable"); } joined = true; }
			
		unsigned int get_id() const { return id; } DWORD native_handle() const { return id; }
		static unsigned hardware_concurrency(){ SYSTEM_INFO sysinfo; GetSystemInfo(&sysinfo); return sysinfo.dwNumberOfProcessors; }
		DWORD id; private: bool started; bool joined;
			void start(void*(*func)(void*), void* arg=NULL){ HANDLE handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, &id);
				if(handle == NULL){ throw std::runtime_error("thread create error"); } CloseHandle(handle); started = true; }
		#else
		void join(){ if(!joinable()){ throw std::runtime_error("thread is not joinable"); }
			pthread_join(id, NULL); joined = true; }
		void detach(){ if (!joinable()){ throw std::runtime_error("thread detach is not avalaible"); }
			pthread_detach(id); joined = true; }
			
		unsigned int get_id() const { return id; } pthread_t native_handle() const { return id; }
		static unsigned hardware_concurrency(){ return sysconf(_SC_NPROCESSORS_ONLN); }
		pthread_t id; private: bool started; bool joined;
		    void start(void*(*func)(void*), void* arg=NULL){ if (pthread_create(&id, NULL, func, arg) != 0){
				throw std::runtime_error("Ошибка создания потока"); } started = true; }
		#endif
			//void _prokladka(){}
			//struct _Args { T arg1, T arg2, T arg3 };
	};
	
	
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
