//libsupc++ (-lsupc++): Minimal support C++ runtime.
void* operator new(size_t size, void* ptr) noexcept { return ptr; } //Определение сигнатуры для placement new
void* operator new(size_t size) noexcept { void* ptr = malloc(size); if(ptr!=NULL) return ptr; else { ncpp::print("(!) operator new(size_t) error: Out of memory"); exit(1); } }
void operator delete(void* ptr) noexcept { free(ptr); }

#ifndef _WIN32
//Инициализация статических локальных переменных: Заглушки для однопоточной среды.
extern "C" int __cxa_guard_acquire(long* guard_object){ if(*guard_object == 0){ return 1; } return 0; }
extern "C" void __cxa_guard_release(long* guard_object){ *guard_object = 1; }

//Исключения: Заглушки для аварийного завершения программы (Если не установлен флаг -fno-exceptions)
//extern "C" void __cxa_call_unexpected(){ ncpp::print("(!) noexcept throw() unexpected exception: Terminate..."); exit(1); }
//extern "C" void __gxx_personality_v0(){ ncpp::print("(!) __gxx_personality_v0() called: Exceptions disabled: Terminate..."); exit(1); }
#endif
