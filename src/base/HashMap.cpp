//Rigtorp HashMap Modified (Linear Probing) O(~1)
//MIT: https://github.com/rigtorp/HashMap
namespace ncpp {
uint32_t Fnv1a(const char* data, size_t len){ uint32_t hash = 2166136261u;
    for(size_t i = 0; i < len; ++i) hash ^= (uint8_t)data[i], hash *= 16777619u; return hash; }
uint64_t Fnv1a_64(const char* data, size_t len){ uint64_t hash = 14695981039346656037ULL;
    for(size_t i = 0; i < len; ++i){ hash ^= (uint8_t)data[i]; hash *= 1099511628211ULL; } return hash; }

template <typename T> uint32_t hash(T num){ return (uint32_t)num; } //return (uint32_t)(num ^ (num >> 16) ^ (num >> 32) ^ (num >> 48)); // Простой бит-миксинг
template <typename T> uint32_t hash(T* num){ return (size_t)num; }
uint32_t hash(const char* c){ return Fnv1a(c, strlen(c)); }
uint32_t hash(const String& s){ return Fnv1a(s.c_str(), s.size()); }
uint32_t hash(const CString& cs){ return Fnv1a(cs.c_str(), cs.size()); }
uint32_t hash(const Buffer& bf){ return Fnv1a((const char*)bf.data(), bf.size()); }

//template <>
//struct hash<int>{ size_t operator()(int val) const { return val; } }

template <typename K, typename V>
struct Pair { K first; V second; Pair(const K& k=K(), const V& v=V()) : first(k), second(v){}
	K& key(){ return first; } const K& key() const { return first; }
	V& value(){ return second; } const V& value() const { return second; } };

template <typename K>
struct _KData { K first; _KData(const K& k, const K& v) : first(k){}; 
	K& key(){ return first; } const K& key() const { return first; }
	K& value(){ return first; } const K& value() const { return first; }
	operator K&() { return first; } operator const K&() const { return first; }
};

#define MAX_LOAD_FACTOR 0.7f //Original - 0.5f
template <typename K, typename V, typename KVData, typename Derived>
class _HashTable {
    struct Entry { KVData data; bool occupied;
        Entry() : data(K(), V()), occupied(false){}
        Entry(const K& key, const V& val) : data(key, val), occupied(true){} };
    Array<Entry> table; size_t _size;

    size_t _hash(const K& key) const { return hash(key); }

    void _rehash(){ Array<Entry> old = table; table.clear();
        table.resize(old.size()*2+1); _size = 0;
        for(size_t i = 0; i < old.size(); ++i){ if(old[i].occupied) insert(old[i].data.key(), old[i].data.value()); } }
        
    void _del_reinsert(size_t idx){ size_t next = (idx + 1) % table.size();
        while(table[next].occupied){ Entry temp = table[next];
			table[next].occupied = false; --_size;
            insert(temp.data.key(), temp.data.value());
            next = (next + 1) % table.size(); } }
    
    template <typename ValT, typename ArrEntryT, typename IterDerived>
    struct _Iter { size_t idx; ArrEntryT* tbl;
		_Iter(ArrEntryT* table, size_t idx1=0) : idx(idx1), tbl(table){}

		ValT& operator*() const { return (*tbl)[idx].data; }
		ValT* operator->() const { return &(*tbl)[idx].data; }
		
		bool operator==(const _Iter& other) const { return idx == other.idx && tbl == other.tbl; }
		bool operator!=(const _Iter& other) const { return idx != other.idx || tbl != other.tbl; }

		IterDerived& operator++(){ do{ ++idx; }while(idx < tbl->size() && !(*tbl)[idx].occupied); return (IterDerived&)*this; }
		IterDerived& operator--(){ if(idx == 0) return *this; do { --idx; }while(idx > 0 && !(*tbl)[idx].occupied); return (IterDerived&)*this; }
	};

public: typedef _HashTable MapT; //typedef _HashTable<K, V, KVData, Derived> MapT; //Для использования внутри зависимого шаблона полная специализация не требуется.
	// == STL similar api ===
    _HashTable() : _size(0){ table.resize(8); }
    
    //typedef _Iter<const KVData, const Array<Entry>> ConstIter;
	struct ConstIter : _Iter<const KVData, const Array<Entry>, ConstIter> {
		ConstIter(const Array<Entry>* table, size_t idx1=0) : _Iter<const KVData, const Array<Entry>, ConstIter>(table, idx1){} };
    struct Iter : _Iter<KVData, Array<Entry>, Iter> {
		Iter(Array<Entry>* table, size_t idx1=0) : _Iter<KVData, Array<Entry>, Iter>(table, idx1){}
		operator ConstIter() const { return ConstIter(this->tbl, this->idx); } };
	typedef Iter iterator; typedef ConstIter const_iterator;
    
	ConstIter begin() const { size_t i = 0; while(i < table.size() && !table[i].occupied) ++i; return ConstIter(&table, i); }
	Iter begin(){ size_t i = 0; while(i < table.size() && !table[i].occupied) ++i; return Iter(&table, i); }
	ConstIter end() const { return ConstIter(&table, table.size()); }
	Iter end(){ return Iter(&table, table.size()); }
	bool empty() const { return _size<=0; }
	size_t size() const { return _size; }
	void clear(){ table.clear(); _size=0; }

    Pair<Iter,bool> insert(const K& key, const V& value=V()){
		//String debug("== debug: "); debug << dtos((float)(_size+1), 2) << "/" << table.size() << "\n"; print(debug);
        if((float)(_size+1) / table.size() > MAX_LOAD_FACTOR) _rehash();
        size_t idx = _hash(key) % table.size();
        while(table[idx].occupied){
            if(table[idx].data.key() == key) return Pair<Iter,bool>(end(),false);
            idx = (idx + 1) % table.size(); }
        table[idx] = Entry(key, value); ++_size; return Pair<Iter,bool>(Iter(&table, idx), true); }

	ConstIter find(const K& key) const { size_t idx = _hash(key) % table.size(); size_t start = idx;
        while(table[idx].occupied){
            if(table[idx].data.key() == key) return ConstIter(&table, idx);
            idx = (idx + 1) % table.size();
            if (idx == start) break; } return end(); }
	Iter find(const K& key){ ConstIter cit = static_cast<const _HashTable*>(this)->find(key);
		return Iter(const_cast<Array<Entry>*>(cit.tbl), cit.idx); }
    
    bool erase(const K& key){
		size_t idx = _hash(key) % table.size(); size_t start = idx;
        while(table[idx].occupied){
            if(table[idx].data.key() == key){ table[idx].occupied = false; --_size; _del_reinsert(idx); return true; }
            idx = (idx + 1) % table.size();
            if (idx == start) break; } return false; }
	
	Iter erase(Iter pos){ if(pos == end() || pos.tbl != &table) return end(); 
		size_t idx = pos.idx; if(!table[idx].occupied) return ++pos;
		table[idx].occupied = false; --_size; _del_reinsert(idx); return ++pos; }
    
	V& operator[](const K& key){ Iter it = find(key); if(it!=end()) return it->value(); return insert(key, V()).first->value(); }
	// == ==
	
	//typedef typename MapType::iterator iterator; typedef typename MapType::const_iterator const_iterator;
	Array<K> keys() const { Array<K> keys; for(typename MapT::ConstIter it = begin(); it != end(); ++it){ keys.push(it->first); } return keys; }
	bool has(const K& key) const { return this->find(key) != this->end(); }
	static Derived& assign(Derived& target, const Derived& map2){
		for(ConstIter it = map2.begin(); it != map2.end(); ++it){ target[it->first]=it->second; } return target; }
	static Derived& assign(Derived& target, const Array<Derived>& maps){ for(size_t i=0;i<maps.size();i++){ target.assign(target, maps[i]); } return target; }
	void erase_ptr(K key){ erase(key); delete key; }
	String cout() const { String ss("{");
		for(ConstIter it = this->begin(); it != this->end(); ++it){ ss << "\n  \"" << it->first << "\": " << it->second << ", "; } ss+="}"; return ss; }
	String toJSON() const { String ss("{"); for(ConstIter it = this->begin(); it != this->end(); ++it){ ss << "\"" << it->first << "\":" << it->second<<","; }
		ss.resize(ss.size()-1); ss << "}"; return ss; }
	String toString() const { return cout(); }
	String type() const { return "map"; }
	// unsafe
	const V& at(const K& key) const { ConstIter it = this->find(key); if(it == this->end()){ return begin()->second; } return it->second; }
	//const V& operator[](const K& key) const;
};

template <typename K, typename V> //HashMap ≈ std::unordered_map
struct HashMap : _HashTable<K, V, Pair<K,V>, HashMap<K,V> >{ HashMap() : _HashTable<K, V, Pair<K,V>, HashMap<K,V> >(){} 
	HashMap(const HashMap& m) : _HashTable<K, V, Pair<K,V>, HashMap<K,V> >(m){} };
template <typename K>
struct HashSet : _HashTable<K, K, _KData<K>, HashSet<K> >{ HashSet() : _HashTable<K, K, _KData<K>, HashSet<K> >(){}
	HashSet(const HashSet& s) : _HashTable<K, K, _KData<K>, HashSet<K> >(s){}; };
	
typedef HashMap<String, String> StringMap;
typedef HashMap<String, double> DoubleMap;
//typedef HashMap<String, unsigned long long> ULLMap; // UInt64Map?
//typedef HashMap<String, long long> Int64Map; // IntLLMap?
}