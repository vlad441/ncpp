namespace ncpp {

    // Функции хеширования (Fnv1a)
    uint32_t Fnv1a(const char* data, size_t len);
    uint64_t Fnv1a_64(const char* data, size_t len);

    // Перегрузки функции hash для различных типов
    template <typename T> uint32_t hash(T num);
    template <typename T> uint32_t hash(T* num);
    uint32_t hash(const char* c);
    uint32_t hash(const String& s);
    uint32_t hash(const CString& cs);

    // Вспомогательные структуры для хранения данных

    template <typename K>
    struct _KData {
        K first;
        _KData();
        _KData(K k);
        _KData(K k, K v);
    };

    // Основной шаблон хеш-таблицы (Linear Probing)
    template <typename K, typename V, typename D, typename Derived>
    struct _HashTable {
        typedef D* Iter;
        typedef const D* ConstIter;

        _HashTable(size_t capacity = 8, K empty_key = K());
        _HashTable(const _HashTable& other);
        ~_HashTable();

        // Итераторы
        Iter begin();
        Iter end();
        ConstIter begin() const;
        ConstIter end() const;

        // Основной API
        size_t size() const;
        size_t capacity() const;
        bool empty() const;
        void clear();

        void insert(const K& key, const V& value);
        void insert(const D& data);
        void assign(const _HashTable& other);
        void assign(const _HashTable& other, const _HashTable& other2);
        
        Iter find(const K& key);
        ConstIter find(const K& key) const;
        bool has(const K& key) const;
        
        void erase(const K& key);
        void erase(Iter it);

        V& operator[](const K& key);

        // Утилиты и экспорт
        static Derived concat(const Array<Derived>& maps);
        void erase_ptr(K key);
        
        String cout() const;
        String toJSON() const;
        String toString() const;
        String type() const;

        // Безопасный доступ
        const V& at(const K& key) const;

    protected:
        D* _buckets;
        size_t _size;
        size_t _capacity;
        K _empty_key;

        size_t _hash(const K& key) const;
        void _rehash(size_t new_capacity);
    };

    // Финальные структуры: HashMap и HashSet
    template <typename K, typename V>
    struct HashMap : _HashTable<K, V, Pair<K, V>, HashMap<K, V>> {
        HashMap();
        HashMap(const HashMap& m);
    };

    template <typename K>
    struct HashSet : _HashTable<K, K, _KData<K>, HashSet<K>> {
        HashSet();
        HashSet(const HashSet& m);
    };

}