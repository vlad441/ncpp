// Список нужных функций (в отдельном файле или макросе)
#define X11_FUNCTIONS \
    X(XOpenDisplay) \
    X(XCreateSimpleWindow) \
    X(XNextEvent) \
    X(XCloseDisplay)

//#define decltype __typeof__
// Объявляем указатели (>=C++11): Берет объявление из .h
#define X(name) typedef decltype(name)* PFN_##name; PFN_##name name;
X11_FUNCTIONS
#undef X

// Загружаем одним махом
void load_x11(void* handle) {
    #define X(name) name = (PFN_##name)DGET_ADDR(handle, #name);
    X11_FUNCTIONS
    #undef X
}

