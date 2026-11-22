/*extern "C" { extern char __file_size[]; extern char __header_size[]; extern char __entry_point[]; }
//#pragma pack(push, 1)
struct __attribute__((packed)) Header
{
    unsigned char e_ident[4] = { 0x7F, 'E', 'L', 'F' };
    unsigned char EI_CLASS = 2;
    unsigned char EI_DATA = 1;
    unsigned char EI_VERSION = 1;
    unsigned char EI_OSABI = 0;
    unsigned char EI_ABIVERSION = 0;
    unsigned char Padding[7] = { 0, 0, 0, 0, 0, 0, 0 };

    unsigned short int e_type = 2;
    unsigned short int e_machine = 0x3E;
    unsigned int e_version = 1;
    unsigned long long e_entry = (unsigned long long)__entry_point;
    unsigned long long e_phoff = 64;
    unsigned long long e_shoff = 0;
    unsigned int e_flags = 0;
    unsigned short int e_ehsize = 64;
    unsigned short int e_phentsize = 56;
    unsigned short int e_phnum = 1;
    unsigned short int e_shentsize = 0;
    unsigned short int e_shnum = 0;
    unsigned short int e_shstrndx = 0;

    unsigned int p_type = 1;
    unsigned int p_flags = 5;
    unsigned long long p_offset = 0;
    unsigned long long p_vaddr = 0x400000;
    unsigned long long p_paddr = 0x400000;
    unsigned long long p_filesz = (unsigned long)__file_size;
    unsigned long long p_memsz = (unsigned long)__file_size;
    unsigned long long p_align = 0x200000;
};
//#pragma pack(pop)

constexpr Header header __attribute__((section(".header")));
*/


inline void Print(const char* str, int len)
{
    /*#if defined(__linux__) && defined(__x86_64__)
        asm volatile ("syscall" : : "a"(1), "D"(1), "S"(str), "d"(len) : "rcx", "r11", "memory");
    #elif defined(__linux__) && defined(__i386__)
        asm volatile ("int $0x80" : : "a"(4), "b"(1), "c"(str), "d"(len) : "memory");
    #endif*/
}
inline void ExitPeocess(int code)
{
    #if defined(__linux__) && defined(__x86_64__)
        asm volatile ("syscall" : : "a"(60), "D"(code) : "memory");
    #elif defined(__linux__) && defined(__i386__)
        asm volatile ("int $0x80" : : "a"(1L), "b"(code) : "memory");
    #endif
}
extern "C" void _start()
{
    //unsigned char hw[13] = { 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n' };
    //asm volatile ("syscall" : : "a"(1), "D"(1), "S"(hw), "d"(12) : "rcx", "r11", "memory");
    //asm volatile ("syscall" : : "a"(60), "D"(0) : "memory");
    static const char msg[] = "hello world!\n";
    Print(msg, 13);
    ExitPeocess(0);
}


