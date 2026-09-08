namespace ncpp{ //namespace allocators;
//#define HEADER_SZ ((sizeof(size_t)+sizeof(void*)-1) & ~(sizeof(void*)-1))
//#define ALIGN_SZ sizeof(void*)
	
#ifdef _WIN32
	bool sysMprotect(void* ptr, size_t len=4096, bool read=1, bool write=1, bool exec=1){ DWORD prot = PAGE_NOACCESS;
		if(write){ 
			if(exec){ prot = (read) ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_WRITECOPY; }
			else{ prot = (read) ? PAGE_READWRITE : PAGE_WRITECOPY; }
		}else if(exec){ prot = (read) ? PAGE_EXECUTE_READ : PAGE_EXECUTE; }else if(read){ prot = PAGE_READONLY; }
		SYSTEM_INFO sysInfo; GetSystemInfo(&sysInfo); unsigned int pageSz = sysInfo.dwPageSize;
		ptr = (void*)((uintptr_t)ptr & ~(pageSz - 1)); DWORD oldProt; if(!VirtualProtect(ptr, len, prot, &oldProt)) return false; return true; }

    void* sysMalloc(size_t size, bool read=1, bool write=1, bool exec=0){ //static const PAGESZ = 4096; // SYSTEM_INFO si; GetSystemInfo(&si); //Запрос размера страницы памяти.
		DWORD flProtect=exec?(PAGE_EXECUTE_READWRITE):PAGE_READWRITE; void* ptr = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, flProtect);
		if(ptr==NULL){ Except("SysMalloc: VirtualAlloc() failed with error: -"); return NULL; } return ptr; } //DWORD err = GetLastError(); //+dtos(err)
    void sysFree(void* ptr){ if(!ptr) return; VirtualFree(ptr, 0, MEM_RELEASE); }
#else
	bool sysMprotect(void* ptr, size_t len=4096, bool read=1, bool write=1, bool exec=1){ int prot=0; 
		if(read) prot |= PROT_READ; if(write) prot |= PROT_WRITE; if(exec) prot |= PROT_EXEC; if(prot==0){ prot = PROT_NONE; }
		unsigned int pageSz = sysconf(_SC_PAGE_SIZE); ptr = (void*)((uintptr_t)ptr & ~(pageSz-1));
		if(mprotect(page_start, len, prot)!=0) return false; return true; }
		
    void* sysMalloc(size_t size, bool read=1, bool write=1, bool exec=0){ //static const PGSIZE = 4096; //sysconf(_SC_PAGE_SIZE)
        size_t sz = size+sizeof(size_t); int prot=PROT_READ | PROT_WRITE; if(exec) prot |= PROT_EXEC; //use size+sizeof(void*);?
		void* ptr = mmap(NULL, sz, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if(ptr == MAP_FAILED){ Except("SysMalloc: mmap() failed with error: -"); return NULL; } //+dtos(errno)
        *(size_t*)ptr = sz; return (char*)ptr+sizeof(size_t); }
    void sysFree(void* ptr){ if(!ptr) return; ptr = (char*)ptr-sizeof(size_t); munmap(ptr, *(size_t*)ptr); }
#endif

	struct Allocator { void* alloc(size_t size){ return ::malloc(size); } void dealloc(void* ptr){ ::free(ptr); } };
	
	template<void* (*_allocFN)(size_t) = malloc, void (*_freeFN)(void*) = free> //Arena: Trivially Destructible only
	struct ArenaAlloc { struct Block { Block* next; void* data; unsigned int offset, capacity; };
		ArenaAlloc(size_t bsize=4096-8, size_t pageSz=4096) : blockSz(bsize), first(NULL), end(NULL){} 
		~ArenaAlloc() { clear(); }
		void setBlockSz(size_t bsize){ blockSz=bsize; }

		void* alloc(size_t sz, char align=sizeof(void*)){ if(end){ end->offset = (end->offset+align-1) & ~(align-1); } //выравнивание
			if(!end || end->offset + sz > end->capacity) _newblock(sz > blockSz ? sz : blockSz);
			void* ptr = (char*)end->data + end->offset; end->offset += sz; return ptr; }
		template<typename U> U* New(){ return ::new (alloc(sizeof(U))) U(); }
		void clear(){ Block* curr = first; while(curr){ Block* next = curr->next; _freeFN(curr); curr = next; } first = end = NULL; }
		size_t blocksCnt(){ size_t count=0; Block* curr = first; while(curr){ Block* next = curr->next; curr = next; ++count; } return count; }

	private: size_t blockSz; Block* first; Block* end; //bool nonTriviallyDestructible;
		void _newblock(size_t sz){ Block* b = (Block*)_allocFN(sizeof(Block)+sz); if(!b){ Except("ArenaAlloc: _newblock() alloc fail"); return; }
			b->offset = 0; b->capacity = sz; b->next = NULL; b->data = (void*)((char*)b+sizeof(Block));
			if(!first){ first = end = b; }else{ end->next = b; end = b; } }
		private: ArenaAlloc(const ArenaAlloc&); ArenaAlloc& operator=(const ArenaAlloc&); public: //Скрытие копирования.
	};
	
	template<typename T, void* (*_allocFN)(size_t) = malloc, void (*_freeFN)(void*) = free> //void* (*_realloc)(void* ptr, size_t size);
	struct SlabAlloc { struct Block { Block* next; T* data; unsigned int offset; }; //PoolAlloc/SlabAlloc
		
		SlabAlloc(size_t elems=10) : _sizeT(ncpp::max(sizeof(T), sizeof(void*))), blockSz(elems), first(NULL), end(NULL), freeListHead(NULL){}
		SlabAlloc(size_t elems, unsigned int sizeT) : _sizeT(ncpp::max(sizeT, sizeof(void*))), blockSz(elems), first(NULL), end(NULL), freeListHead(NULL){}
		~SlabAlloc(){ clear(); }
		
		//Intrusive Free List: sizeof(T) >= sizeof(void*)
		void* alloc(){ if(freeListHead){ T* ptr = freeListHead; freeListHead = *(T**)ptr; return ptr; }
			if(!end || end->offset >= blockSz) _newblock();
			T* ptr = (T*)((char*)end->data + end->offset * _sizeT); end->offset++; return ptr; }
		void dealloc(void* ptr){ if(!ptr) return; *(T**)ptr = freeListHead; freeListHead = ptr; }
		
		//Usage Array Pointers
		// void* alloc(){ if(!freePtrs.empty()) return freePtrs.pop(); if(!end || end->offset >= blockSz) _newblock();
			// T* ptr = (T*)((char*)end->data+end->offset*_sizeT); end->offset++; return ptr; }
		// void dealloc(void* ptr){ freePtrs.push(ptr); }
		
		//Old Intrusive Free List
		//T* alloc(){ if (!nextFreeBlock) return _newblock(); T* result = nextFreeBlock; nextFreeBlock = *(T**)result; return result; }
		//void dealloc(T* ptr){ *(T**)ptr = nextFreeBlock; nextFreeBlock = ptr; }

		T* New(){ return new (alloc()) T(); }
		void Delete(T* ptr){ ptr->~T(); dealloc(ptr); } //dealloc(ptr, _sizeT); // не применяется
		
		void clear(){ Block* curr = first; while(curr){ Block* next = curr->next; _freeFN(curr); curr = next; } }
		size_t blocksCnt(){ size_t count=0; Block* curr = first; while(curr){ Block* next = curr->next; curr = next; ++count; } return count; }
		
		SlabAlloc& fitBlockSz(size_t targetSz=4096-8, size_t pageSz=4096){ if(first) return *this; size_t overhead = sizeof(Block)+sizeof(size_t);
			targetSz=((targetSz+pageSz-1)/pageSz)*pageSz; //ceil align по размеру pageSz
			blockSz=(targetSz-overhead)/_sizeT; blockSz = (blockSz>0)?blockSz:1; return *this; }

	private: unsigned int _sizeT; size_t blockSz; Block* first; Block* end; T* freeListHead;  //Array<T*, _allocFN, _freeFN> freePtrs;
		void _newblock(){ Block* b = (Block*)_allocFN(sizeof(Block)+blockSz*_sizeT); if(!b){ Except("SlabAlloc: _newblock() alloc fail"); return; }
			b->offset = 0; b->next = NULL; b->data = (T*)((char*)b+sizeof(Block)); 
			if(!first){ first = end = b; }else{ end->next = b; end = b; } }
		//void _clearBlock(){  } //Операция деструктора всех объектов в блоке не реализована
		//(char*)this + sizeof(Block);
		//(char*)this + sizeof(Block);
	};
	
	template<void* (*_allocFN)(size_t) = malloc, void (*_freeFN)(void*) = free> //void* (*_realloc)(void* ptr, size_t size);
	struct FreeListAlloc { struct Block { Block* next; void* data; unsigned int offset, capacity; }; 
		struct FreeElem { FreeElem* next; size_t size; };
		FreeListAlloc(size_t bsize=4096-8, size_t pageSz=4096) : blockSz(bsize), first(NULL), end(NULL), freeListHead(NULL){}
		~FreeListAlloc(){ clear(); }
		
		//Intrusive Free List: sz >= sizeof(void*)
		void* getFreeElem(size_t sz){ FreeElem* prev = NULL; FreeElem* curr = freeListHead;
			while(curr){
				if(curr->size >= sz){ if(prev){ prev->next = curr->next; }else{ freeListHead = curr->next; } return (void*)curr; }
				prev = curr; curr = curr->next; } return NULL; }
		void* alloc(size_t size, char align=sizeof(FreeElem)){ size_t sz = size+sizeof(size_t); void* ptr=getFreeElem(sz); if(ptr) return ptr;
			if(end){ end->offset = (end->offset+align-1) & ~(align-1); } //выравнивание 
			if(!end || end->offset + sz > end->capacity) _newblock(sz > blockSz ? sz : blockSz);
			ptr = (char*)end->data + end->offset; end->offset += sz;
			*(size_t*)ptr = sz; return (char*)ptr+sizeof(size_t); }
		void dealloc(void* ptr){ if(!ptr) return; ptr = (void*)((char*)ptr-sizeof(size_t)); size_t sz=*(size_t*)ptr;
			FreeElem* node = (FreeElem*)ptr; node->next = freeListHead; node->size = sz; freeListHead = node; }
		
		template<typename U> U* New(){ return ::new (alloc(sizeof(U))) U(); }
		template<typename U> void Delete(U* ptr){ ptr->~U(); dealloc(ptr); }
		
		void clear(){ Block* curr = first; while(curr){ Block* next = curr->next; _freeFN(curr); curr = next; } }
		size_t blocksCnt(){ size_t count=0; Block* curr = first; while(curr){ Block* next = curr->next; curr = next; ++count; } return count; }

	private: size_t blockSz; Block* first; Block* end; FreeElem* freeListHead;
		void _newblock(size_t sz){ Block* b = (Block*)_allocFN(sizeof(Block)+sz); if(!b){ Except("FreeListAlloc: _newblock() alloc fail"); return; }
			b->offset = 0; b->capacity = sz; b->next = NULL; b->data = (void*)((char*)b+sizeof(Block));
			if(!first){ first = end = b; }else{ end->next = b; end = b; } }
	};
	
	template<void* (*_allocFN)(size_t) = malloc, void (*_freeFN)(void*) = free>
	struct BucketAlloc { //BinningAllocator/Segregated Free Lists
		BucketAlloc(size_t bSz=16) : _p16(bSz, 16), _p32(bSz, 32), _p64(bSz, 64), _p128(bSz, 128), _p256(bSz, 256), _p512(bSz, 512){}

		void* alloc(size_t size){ size_t sz = size+sizeof(size_t); void* ptr=NULL;
			if(sz <= 16){ ptr = _p16.alloc(); } else if(sz <= 32){ ptr = _p32.alloc(); } 
			else if(sz <= 64){ ptr = _p64.alloc(); } else if(sz <= 128){ ptr = _p128.alloc(); }
			else if(sz <= 256){ ptr = _p256.alloc(); } else if(sz <= 512){ ptr = _p512.alloc(); }
			else{ ptr = _allocFN(sz); } 
			*(size_t*)ptr = sz; return (char*)ptr+sizeof(size_t); }

		void dealloc(void* ptr){ char* p = (char*)ptr-sizeof(size_t); size_t sz=*(size_t*)ptr;
			if(sz <= 16){ _p16.dealloc(p); } else if(sz <= 32){ _p32.dealloc(p); } 
			else if(sz <= 64){ _p64.dealloc(p); } else if(sz <= 128){ _p128.dealloc(p); }
			else if(sz <= 256){ _p256.dealloc(p); } else if(sz <= 512){ _p512.dealloc(p); }
			else{ _freeFN(p); } }
		private: SlabAlloc<char, _allocFN, _freeFN> _p16, _p32, _p64, _p128, _p256, _p512;
	};
	
	//template<typename T> struct SlotMap {} //Индексная карта?
}
