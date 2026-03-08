namespace ncpp{ 
// Поток записи блокирует: вызывает lock() или pthread_rwlock_wrlock().
// Он видит, что счетчик читателей ненулевой.
// Писатель устанавливает флаг ожидания и переходит в режим ядра (через futex), засыпая.
// Самое важное: Установка этого флага блокирует все новые попытки ЧТЕНИЯ.
// aka std::shared_mutex (С++17)
	class SharedMutex { //for POSIX
	private:
		// Примитивы Pthreads
		pthread_mutex_t _mtx;
		pthread_cond_t m_cond_read;
		pthread_cond_t m_cond_write;
		
		// Счетчики состояния
		int _readers;
		int _writers_wait;
		bool _has_writer;
		
		// Вспомогательный класс для инициализации/очистки Pthreads (необходим из-за C++98)
		class ScopedMutexLock {
			pthread_mutex_t* m_mutex;
		public:
			ScopedMutexLock(pthread_mutex_t* m) : m_mutex(m) {
				pthread_mutex_lock(m_mutex);
			}
			~ScopedMutexLock() {
				pthread_mutex_unlock(m_mutex);
			}
		};

	public:
		SharedMutex() : _readers(0), _writers_wait(0), _has_writer(false) {
			// Инициализация примитивов
			pthread_mutex_init(&_mtx, NULL);
			pthread_cond_init(&m_cond_read, NULL);
			pthread_cond_init(&m_cond_write, NULL);
		}
		
		// Деструктор для очистки ресурсов
		~SharedMutex() {
			pthread_mutex_destroy(&_mtx);
			pthread_cond_destroy(&m_cond_read);
			pthread_cond_destroy(&m_cond_write);
		}

		// --- 1. Блокировка для ЧТЕНИЯ (Shared Lock) ---
		void lockRd() {
			// Используем RAII-подобную обертку для захвата управляющего мьютекса
			ScopedMutexLock lock(&_mtx);

			// Ждем, пока нет активного писателя И нет писателей, ожидающих
			while (_has_writer || (_writers_wait > 0)){
				// pthread_cond_wait атомарно освобождает мьютекс и блокирует поток.
				// После пробуждения он снова захватывает мьютекс.
				pthread_cond_wait(&m_cond_read, &_mtx);
			}

			_readers++;
		}

		// --- 2. Освобождение для ЧТЕНИЯ (Shared Unlock) ---
		void unlockRd() {
			ScopedMutexLock lock(&_mtx);
			_readers--;

			// Если это был последний читатель, нужно разбудить писателей
			if (_readers == 0) {
				pthread_cond_signal(&m_cond_write); // Будим одного ожидающего писателя
			}
		}

		// --- 3. Блокировка для ЗАПИСИ (Exclusive Lock) ---
		void lockWr() {
			ScopedMutexLock lock(&_mtx);
			_writers_wait++; 

			// Ждем, пока нет активного писателя И нет активных читателей
			while (_has_writer || (_readers > 0)) {
				pthread_cond_wait(&m_cond_write, &_mtx);
			}

			_has_writer = true;
			_writers_wait--;
		}

		// --- 4. Освобождение для ЗАПИСИ (Exclusive Unlock) ---
		void unlockWr() {
			ScopedMutexLock lock(&_mtx);
			_has_writer = false;

			// Писатель закончил. Будим всех ожидающих.
			if (_writers_wait > 0) {
				// Предпочтение писателю (будим одного ожидающего писателя)
				pthread_cond_signal(&m_cond_write); 
			} else {
				// Предпочтение читателям (будим всех ожидающих читателей)
				pthread_cond_broadcast(&m_cond_read); 
			}
		}
	};
	typedef SharedMutex RWLock;
}
