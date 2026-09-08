//RB tree Map implementation O(logN)
template <typename K, typename V>
//struct pair { K first; V second; K& key; V& value; pair(const K& v1=K(), const V& v2=V()) : first(v1), second(v2), key(first), value(second){} };
struct pair { K first; V second; pair(const K& v1=K(), const V& v2=V()) : first(v1), second(v2){} };

template <typename K, typename V>
struct _KVData { K first; V second; _KVData(const K& k, const V& v) : first(k), second(v){}; 
	K& key(){ return first; } const K& key() const { return first; }
	V& value(){ return second; } const V& value() const { return second; } };
template <typename K>
struct _KData { K first; _KData(const K& k, const K& v) : first(k){}; 
	K& key(){ return first; } const K& key() const { return first; }
	K& value(){ return first; } const K& value() const { return first; } };

template <typename K, typename V, typename KVData>
class _RB_tree { enum Color { RED, BLACK };
	struct RBNode { KVData data; char color; RBNode* parent; RBNode* left; RBNode* right;
		RBNode(const K& k, const V& v) : data(k,v), color(RED), parent(NULL), left(NULL), right(NULL){} };
	RBNode *root, *nmin, *nmax; size_t _len;
	template <typename ValT>
	struct _iterator { RBNode* _node;
		_iterator() : _node(NULL){}
		_iterator(RBNode* ptr) : _node(ptr){}

		ValT& operator*() const { return _node->data; }
		ValT* operator->() const { return &_node->data; }
		
		bool operator==(const _iterator& other) const { return _node == other._node; }
		bool operator!=(const _iterator& other) const { std::cout << "-- debug operator!=: " << _node << ", " << other._node << "\n";
			return _node != other._node; }

		_iterator& operator++(){ _node = nextNode(_node); return *this; }
		_iterator& operator--(){ _node = prevNode(_node); return *this; }
	};
	public:
	_RB_tree() : root(NULL), nmin(NULL), nmax(NULL), _len(0){}
	_RB_tree(const _RB_tree& rb){ _copy(rb); };
	//template <class InputIterator>  map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
	~_RB_tree(){ clear(); }
	
	typedef _iterator<const KVData> const_iterator;
	struct iterator : _iterator<KVData>{ iterator(RBNode* ptr){ this->_node=ptr; }
		operator const_iterator() const { return const_iterator(this->_node); } };
	
	const_iterator begin() const { return nmin; }
	iterator begin(){ return nmin; }
	const_iterator end() const { return NULL; }
	iterator end(){ return NULL; }
	bool empty() const { return _len<=0; }
	size_t size() const { return _len; }
	void clear(){ if(!root) return;
		vector<RBNode*> stack; stack.push_back(root);
		while(!stack.empty()){ RBNode* node = stack.back(); stack.pop_back();
			if(node->left)  stack.push_back(node->left);
			if(node->right) stack.push_back(node->right); 
			delete node; } root = nmin = nmax = NULL; _len = 0; }
	
	iterator find(const K& key){ RBNode *parent=NULL, *node=_find(key, parent); return node==NULL?end():node; }
	const_iterator find(const K& key) const { RBNode *parent=NULL, *node=_find(key, parent); return node==NULL?end():node; }
	
	pair<iterator,bool> insert(const pair<K,V>& val){ 
		RBNode *parent = NULL, *current = _find(val.first, parent); if(current) return pair<iterator,bool>(current, false);
		return pair<iterator,bool>(_insert(val.first, val.value(), parent), true); }
	//iterator insert(iterator position, const pair<K,V>& val);
	
	void erase(iterator pos){ pos._node?_erase(pos._node):0; return; }
	size_t erase(const K& key){ RBNode *parent = NULL; RBNode *z = _find(key, parent); return !z?0:_erase(z); }
    void erase(iterator first, iterator last);
	
	string _tree() const { string str="root: "; str+=root->data.first.c_str(); str+=" | left: "; str+=root->left?root->left->data.first.c_str():"NULL";
		str+=" | right: "; str+=root->right?root->right->data.first.c_str():"NULL"; str+="\n";
		str+=_printTree(root, "", true); return str; }

	string _printTree(RBNode* node, const string& prefix, bool isLeft) const { if(!node) return "";
		string res=prefix; if(isLeft) res+="└──"; else res+="├──";
		res+=node->data.first.c_str(); res+=" ("; res+=node->color == RED? "R" : "B"; res+=")\n";

		if(node->left || node->right){
			res+=_printTree(node->right, prefix + (isLeft ? "    " : "│   "), false);
			res+=_printTree(node->left,  prefix + (isLeft ? "    " : "│   "), true); } return res; }
	
	int _depth(const K& key) const { RBNode* current = root; int depth = 0;
		while (current){
			if(key < current->data.first){ current = current->left; }
			else if(key > current->data.first){ current = current->right; }
			else{ return depth; } depth++; } return NULL; }
	
	_RB_tree& operator=(const _RB_tree<K, V, KVData>& rb){ _copy(rb); return *this; }
	//const V& operator[](const K& key) const { RBNode *parent = NULL, *current = _find(key, parent); if(current) return current->data.value(); }
	V& operator[](const K& key){ RBNode *parent = NULL, *current = _find(key, parent); 
		if(current) return current->data.value(); return _insert(key, V(), parent)->data.value(); }
	
	private:
		RBNode* _alloc(const K& key, const V& v){ return new RBNode(key, v); }
	
		RBNode* _find(const K& key, RBNode*& parent) const { RBNode* current = root; parent = NULL;
			while(current){
				if(key < current->data.first){ parent = current; current = current->left; }
				else if(key > current->data.first){ parent = current; current = current->right; }
				else{ return current; } } return NULL; }
				
		RBNode* _insert(const K& key, const V& val, RBNode*& parent){
			RBNode* newNode = new RBNode(key, val); newNode->parent = parent;

			if(!parent){ root = newNode; }
			else if(key < parent->data.first){ parent->left = newNode; } 
			else{ parent->right = newNode; }

			_insertFixup(newNode); ++_len; if(!nmin||key < nmin->data.first) nmin = newNode; 
			if(!nmax||key > nmax->data.first) nmax = newNode; return newNode; }
				
		bool _erase(RBNode* z){ RBNode *y = z; RBNode *x = NULL; RBNode *x_parent = NULL; char y_original_color = y->color;

			if(!z->left){ x = z->right; _transplant(z, z->right); x_parent = z->parent; }
			else if(!z->right){ x = z->left; _transplant(z, z->left); x_parent = z->parent; } 
			else{ y = minNode(z->right); y_original_color = y->color; x = y->right; 
				if(y->parent == z){ if(x) x->parent = y; x_parent = y; } 
				else{ _transplant(y, y->right); y->right = z->right; y->right->parent = y; x_parent = y->parent; }
				_transplant(z, y); y->left = z->left; y->left->parent = y; y->color = z->color; }

			delete z; --_len; if(y_original_color == BLACK) _eraseFixup(x, x_parent); 
			if(z == nmin) nmin = minNode(root); if(z == nmax) nmax = maxNode(root); return true; }

		static RBNode* nextNode(RBNode* node){ if(!node) return NULL;
			// 1. Если есть правое поддерево — successor там
			if (node->right){ node = node->right; while (node->left) node = node->left; return node; }
			// 2. Иначе идем вверх, пока не поднимемся из левого поддерева
			RBNode* parent = node->parent; while (parent && node == parent->right){ node = parent; parent = parent->parent; } return parent; }
		
		static RBNode* prevNode(RBNode* node){ if (!node) return NULL;
			// 1. Если есть левое поддерево — предшественник там
			if (node->left){ node = node->left; while (node->right) node = node->right; return node; }
			// 2. Иначе идем вверх, пока не поднимемся из правого поддерева
			RBNode* parent = node->parent; while(parent && node == parent->left){ node = parent; parent = parent->parent; } return parent; }
			
		static RBNode* minNode(RBNode* node){ if(!node) return NULL; while(node->left) node = node->left; return node; }
		static RBNode* maxNode(RBNode* node){ if(!node) return NULL; while(node->right) node = node->right; return node; }
		
		void rotateLeft(RBNode* x){ RBNode* y = x->right;
			x->right = y->left; if(y->left) y->left->parent = x;
			y->parent = x->parent;
			if(!x->parent) root = y;
			else if (x == x->parent->left) x->parent->left = y;
			else x->parent->right = y;
			y->left = x; x->parent = y; }

		void rotateRight(RBNode* y){ RBNode* x = y->left;
			y->left = x->right; if(x->right) x->right->parent = y;
			x->parent = y->parent; 
			if(!y->parent) root = x;
			else if (y == y->parent->left) y->parent->left = x;
			else y->parent->right = x;
			x->right = y; y->parent = x; }
		
		void _transplant(RBNode* u, RBNode* v){
			if(!u->parent){ root = v; } // u был корнем
			else if(u == u->parent->left){ u->parent->left = v; } // u был левым ребёнком
			else{ u->parent->right = v; } // u был правым ребёнком
			if(v){ v->parent = u->parent; } // Обновляем родителя у v
		}
		
		void _insertFixup(RBNode* node){
			while (node != root && node->parent->color == RED){
				RBNode* parent = node->parent; RBNode* grandparent = parent->parent;
				
				if(parent == grandparent->left){ RBNode* uncle = grandparent->right;

					if(uncle && uncle->color == RED){ // Случай 1: дядя красный
						parent->color = BLACK; uncle->color = BLACK; grandparent->color = RED; node = grandparent; } 
					else{
						if(node == parent->right){ node = parent; rotateLeft(node); } // Случай 2: узел — правый потомок
						// Случай 3: узел — левый потомок
						parent->color = BLACK; grandparent->color = RED; rotateRight(grandparent); }
				}else{ RBNode* uncle = grandparent->left;

					if(uncle && uncle->color == RED){
						parent->color = BLACK; uncle->color = BLACK;
						grandparent->color = RED; node = grandparent; }
					else{
						if (node == parent->left){ node = parent; rotateRight(node); }
						parent->color = BLACK; grandparent->color = RED; rotateLeft(grandparent); }
				}
			} root->color = BLACK; }
		
		void _eraseFixup(RBNode* x, RBNode* parent){
			while((x != root) && (!x || x->color == BLACK)){
				if(x == (parent ? parent->left : NULL)){ RBNode* w = parent->right;
					if(w && w->color == RED){ w->color = BLACK; parent->color = RED; rotateLeft(parent); w = parent->right; }

					// Случай 2: оба потомка брата чёрные
					if((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)){
						w->color = RED; x = parent; parent = x->parent; } 
					else{ // Случай 3: правый чёрный, левый красный
						if(!w->right || w->right->color == BLACK){
							if (w->left) w->left->color = BLACK; w->color = RED; rotateRight(w); w = parent->right; }

						// Случай 4: правый красный
						w->color = parent->color; parent->color = BLACK; if(w->right) w->right->color = BLACK;
						rotateLeft(parent); x = root; }
				}else{ RBNode* w = parent->left; // Зеркальное отражение
					if(w && w->color == RED){ w->color = BLACK; parent->color = RED; rotateRight(parent); w = parent->left; }

					if((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)){
						w->color = RED; x = parent; parent = x->parent; } 
					else{
						if (!w->left || w->left->color == BLACK){
							if(w->right) w->right->color = BLACK; w->color = RED; rotateLeft(w); w = parent->left; }

						w->color = parent->color; parent->color = BLACK;
						if(w->left) w->left->color = BLACK; rotateRight(parent); x = root;
					}
				}
			} if(x) x->color = BLACK; }
			
		void _copy(const _RB_tree& other){ clear(); if(!other.root) return;
			root = _copyNode(NULL, other.root); _len = other._len; nmin = minNode(root); nmax = maxNode(root); }
		
		RBNode* _copyNode(RBNode* parent, RBNode* src){ if(!src) return NULL;
			RBNode* node = _alloc(src->data.first, src->data.value()); node->color = src->color; node->parent = parent;
			node->left  = _copyNode(node, src->left); node->right = _copyNode(node, src->right); return node; }

};

template <typename K, typename V>
struct map : _RB_tree<K, V, _KVData<K,V> >{ map() : _RB_tree<K, V, _KVData<K,V> >(){} map(const map& m) : _RB_tree<K, V, _KVData<K,V> >(m){} };
template <typename K>
struct set : _RB_tree<K, K, _KData<K> >{ set() : _RB_tree<K, K, _KData<K> >(){} set(const set& s) : _RB_tree<K, K, _KData<K> >(s){}; };
