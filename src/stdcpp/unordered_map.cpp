//Rigtorp HashMap Modified (Linear Probing) O(~1)
//MIT: https://github.com/rigtorp/HashMap

uint32_t Fnv1a(const char* data, size_t len){ uint32_t hash = 2166136261u;
    for(size_t i = 0; i < len; ++i) hash ^= (uint8_t)data[i], hash *= 16777619u; return hash; }
    
uint64_t Fnv1a_64(const char* data, size_t len){ uint64_t hash = 14695981039346656037ULL;
    for(size_t i = 0; i < len; ++i){ hash ^= (uint8_t)data[i]; hash *= 1099511628211ULL; } return hash; }

template <typename T>
uint32_t hash(T num){ return num; }
uint32_t hash(const char* c){ return Fnv1a(c, strlen(c)); }
uint32_t hash(const string& str){ return Fnv1a(str.c_str(), str.size()); }

//template <>
//struct hash<int>{ size_t operator()(int val) const { return val; } }

#define MAX_LOAD_FACTOR 0.6f //Original - 0.5f
template <typename K, typename V, typename KVData>
class _HashTable {
    struct Entry { KVData data; bool occupied;
        Entry() : data(K(), V()), occupied(false){}
        Entry(const K& key, const V& val) : data(key, val), occupied(true){} };
    vector<Entry> table; size_t _size;

    size_t _hash(const K& key) const { return hash(key); }

    void _rehash(){ vector<Entry> old = table; table.clear();
        table.resize(old.size()*2+1); _size = 0;
        for (size_t i = 0; i < old.size(); ++i){ if(old[i].occupied) insert(old[i].data.key(), old[i].data.value()); } }
        
    void _del_reinsert(size_t idx){ size_t next = (idx + 1) % table.size();
        while(table[next].occupied){ Entry temp = table[next];
			table[next].occupied = false; --_size;
            insert(temp.data.key(), temp.data.value());
            next = (next + 1) % table.size(); } }
    
    template <typename ValT>
    struct _iterator { size_t idx; vector<Entry>* tbl;
		_iterator(vector<Entry>* table, size_t idx1=0) : idx(idx1), tbl(table){}

		ValT& operator*() const { return (*tbl)[idx].data; }
		ValT* operator->() const { return &(*tbl)[idx].data; }
		
		bool operator==(const _iterator& other) const { return idx == other.idx; }
		bool operator!=(const _iterator& other) const { return idx != other.idx; }

		_iterator& operator++(){ do{ ++idx; }while(idx < tbl->size() && !(*tbl)[idx].occupied); return *this; }
		_iterator& operator--(){ if(idx == 0) return *this; do { --idx; }while(idx > 0 && !(*tbl)[idx].occupied); return *this; }
	};

public:
    _HashTable() : _size(0){ table.resize(8); }
    
    typedef _iterator<const KVData> const_iterator;
    struct iterator : _iterator<KVData> {
		iterator(vector<Entry>* table, size_t idx1=0) : _iterator<KVData>(table, idx1){}
		operator const_iterator() const { return const_iterator(this->tbl, this->idx); } };
    
	const_iterator begin() const { size_t i = 0; while(i < table.size() && !table[i].occupied) ++i; return const_iterator(&table, i); }
	iterator begin(){ size_t i = 0; while(i < table.size() && !table[i].occupied) ++i; return iterator(&table, i); }
	const_iterator end() const { return const_iterator(&table, table.size()); }
	iterator end(){ return iterator(&table, table.size()); }
	bool empty() const { return _size<=0; }
	size_t size() const { return _size; }
	void clear(){ table.clear(); _size=0; }

    pair<iterator,bool> insert(const K& key, const V& value){
		std::cout << "debug: " << (float)(_size+1) << "/" << table.size() << std::endl;
        if((float)(_size+1) / table.size() > MAX_LOAD_FACTOR) _rehash();
        size_t idx = _hash(key) % table.size();
        while(table[idx].occupied){
            if(table[idx].data.key() == key) return pair<iterator,bool>(end(),false);
            idx = (idx + 1) % table.size(); }
        table[idx] = Entry(key, value); ++_size; return pair<iterator,bool>(iterator(&table, idx), true); }

    iterator find(const K& key){ size_t idx = _hash(key) % table.size(); size_t start = idx;
        while(table[idx].occupied){
            if(table[idx].data.key() == key) return iterator(&table, idx);
            idx = (idx + 1) % table.size();
            if (idx == start) break;
        } return end(); }
    
    bool erase(const K& key){
		size_t idx = _hash(key) % table.size(); size_t start = idx;
        while(table[idx].occupied){
            if(table[idx].data.key() == key){
                table[idx].occupied = false; --_size; _del_reinsert(idx); return true; }
            idx = (idx + 1) % table.size();
            if (idx == start) break;
        }
        return false;
    }
    
    //_RB_tree& operator=(const _RB_tree<K, V, KVData>& rb){ _copy(rb); return *this; }
	//const V& operator[](const K& key) const;
	V& operator[](const K& key){ iterator it = find(key); if(it!=end()) return it->value(); return insert(key, V()).first->value(); }
};

template <typename K, typename V>
struct unordered_map : _HashTable<K, V, _KVData<K,V> >{ unordered_map() : _HashTable<K, V, _KVData<K,V> >(){} 
	unordered_map(const unordered_map& m) : _HashTable<K, V, _KVData<K,V> >(m){} };
template <typename K>
struct unordered_set : _HashTable<K, K, _KData<K> >{ unordered_set() : _HashTable<K, K, _KData<K> >(){} 
	unordered_set(const unordered_set& s) : _HashTable<K, K, _KData<K> >(s){}; };
