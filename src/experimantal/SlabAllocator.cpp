namespace ncpp{
	template<typename T>
	struct SlabAllocator { struct Block { Block* next; T* data; size_t offset; }; 
		
		SlabAllocator(size_t bsize=10) : _sizeT(sizeof(T)), blocksize(bsize), first(NULL), end(NULL){ freePtrs.reserve(bsize); }
		SlabAllocator(size_t bsize, unsigned int sizeT) : _sizeT(sizeT), blocksize(bsize), first(NULL), end(NULL){ freePtrs.reserve(bsize); }
		~SlabAllocator(){ clear(); }
		
		T* alloc(){ if(!freePtrs.empty()) freePtrs.pop(); if(!end || end->offset >= blocksize) _newblock();
			T* ptr = (T*)((char*)end->data+end->offset*_sizeT); end->offset++;  return ptr;
			//T* ptr = end->data+end->offset++; return ptr; 
		}
		void dealloc(T* ptr){ freePtrs.push(ptr); }

		T* New(){ return new (alloc()) T(); }
		void Delete(T* ptr){ ptr->~T(); dealloc(ptr); } //dealloc(ptr, _sizeT); // не применяется
		
		bool hasCache(){ return !freePtrs.empty(); }
		void clear(){ Block* curr = first; while(curr){ Block* next = curr->next; free(curr); curr = next; } }
		size_t blocksCnt(){ size_t count=0; Block* curr = first; while(curr){ Block* next = curr->next; curr = next; ++count; } return count; }

	private: unsigned int _sizeT; size_t blocksize; Block* first; Block* end; Array<T*> freePtrs; //HashSet<T*> elems; //Массив занятых указателей?
		void _newblock(){ Block* b = (Block*)malloc(sizeof(Block)+blocksize*_sizeT);
			b->offset = 0; b->next = NULL; b->data = (T*)((char*)b+sizeof(Block)); 
			if(!first){ first = end = b; } else { end->next = b; end = b; } }
		//void _clearBlock(){  } //Операция деструктора всех объектов в блоке не реализована
	};
	
	struct BucketAlloc { //BinningAllocator
		BucketAlloc(size_t lim=16) : _pool16(lim, 16), _pool32(lim, 32), _pool64(lim, 64){}

		void* alloc(size_t size){
			if(size <= 16){ return _pool16.alloc(); }
			else if(size <= 32){ return _pool32.alloc(); } 
			else if(size <= 64){ return _pool64.alloc(); }
			else { return malloc(size); } }

		void dealloc(char* p, size_t size){
			if (size <= 16){ _pool16.dealloc(p); } 
			else if (size <= 32){ _pool32.dealloc(p); } 
			else if (size <= 64){ _pool64.dealloc(p); }
			else { free(p); } }
		private: SlabAllocator<char> _pool16, _pool32, _pool64;
	};
}
