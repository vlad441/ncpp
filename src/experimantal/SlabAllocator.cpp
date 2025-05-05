namespace ncpp{
	template<typename T>
	struct SlabAllocator_v1 { size_t blocksize; int elemsize; struct Block { Block* next; }; Block* freeList;
		
		SlabAllocator_v1() : blocksize(4096), elemsize(sizeof(T)), freeList(NULL){};

		void* alloc(){ if (!freeList) refill(); Block* b = freeList; freeList = b->next; return b; }
		T* n(){ void* ptr = alloc(); return new (ptr) T(); }

		void dealloc(void* ptr){ Block* b = (Block*)ptr; b->next = freeList; freeList = b; }
		void free(T* ptr){ ptr->~T(); dealloc(ptr); }

		void refill(){ char* newBlock = (char*)malloc(blocksize); int bSize = blocksize / elemsize;
			for (int i = 0; i < bSize; ++i){ Block* b = (Block*)(newBlock + i * elemsize);
				b->next = freeList; freeList = b; }
		}
	};
	
	template<typename T>
	struct SlabAllocator_v2 { size_t blocksize; int elemsize; 
		struct Block { Block* next; char* data; int capacity; int offset; }; Block* first; Block* end;

		SlabAllocator_v2() : blocksize(4096), elemsize(sizeof(T)), first(NULL), end(NULL){}
		~SlabAllocator_v2(){ clear(); }

		void reserve(size_t size){ if(!end || end->offset + size > end->capacity) _newblock(size>blocksize ? size:blocksize); }
		void* alloc(int size){ reserve(size); void* ptr = end->data + end->offset; end->offset += size; return ptr; }
		T* New(){ return new (alloc(elemsize)) T(); }
		void dealloc(void* ptr, int size){} // Ничего не делаем — освобождение происходит разом
		void free(T* ptr){ ptr->~T(); } //dealloc(ptr, sizeof(T)); // не нужно
		void recreate(T* ptr){ ptr->~T(); new (ptr) T(); }
		
		void clear(){ Block* curr = first; while(curr){ Block* next = curr->next; free(curr); curr = next; } }

	private:
		void _newblock(int size){ char* raw = (char*)malloc(sizeof(Block) + size);
			Block* b = (Block*)raw; b->offset = 0; b->capacity = size; b->next = NULL; 
			b->data = raw + sizeof(Block); if(!first){ first = end = b; } else { end->next = b; end = b; } }
	};

}
