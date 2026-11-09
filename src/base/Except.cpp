#ifndef NCPP_USESTL
void* operator new(size_t size, void* ptr) noexcept { return ptr; } //Определение для placement new
#endif
namespace ncpp { void _pErr(const char* c);

void (*onExcept)(const char* cstr, int errlvl, int type) = NULL;
void Except(const char* cstr, int errlvl, int type){
	if(onExcept){ onExcept(cstr, errlvl, type); return; } const char* prefix = "[?]";
	switch(errlvl){
		case ERR_FATAL:   prefix = "(!!FATAL): "; break;
		case ERR_ERROR:   prefix = "(E!) "; break;
		case ERR_WARNING: prefix = "(W!) ";  break;
        case ERR_INFO:    prefix = "(i) ";  break; } _pErr(prefix); _pErr(cstr); if(errlvl==0) exit(1); }
};