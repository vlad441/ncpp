#ifdef _WIN32
HANDLE __defheap = GetProcessHeap();
void* malloc(size_t size){ return HeapAlloc(__defheap, 0, size); }
void* realloc(void* ptr, size_t size){ return HeapReAlloc(__defheap, 0, ptr, size);  }
//void free(void* ptr){}
#endif

