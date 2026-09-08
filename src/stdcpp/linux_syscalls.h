#ifdef __cplusplus
extern "C" {
#endif

#ifdef __x86_64__ //x86_64 syscalls https://filippo.io/linux-syscall-table/
inline long _syscall0(long n){ long ret; asm volatile ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory"); return ret; }
inline long _syscall1(long n, long a1){ long ret; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory"); return ret; }
inline long _syscall2(long n, long a1, long a2){ long ret; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2) : "rcx", "r11", "memory"); return ret; }
inline long _syscall3(long n, long a1, long a2, long a3){ long ret; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory"); return ret; }
inline long _syscall4(long n, long a1, long a2, long a3, long a4){ long ret; register long r10 asm("r10") = a4; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10) : "rcx", "r11", "memory"); return ret; }
inline long _syscall5(long n, long a1, long a2, long a3, long a4, long a5){ long ret; register long r10 asm("r10") = a4; register long r8 asm("r8") = a5; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory"); return ret; }
inline long _syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6){ long ret; register long r10 asm("r10") = a4; register long r8 asm("r8") = a5; register long r9 asm("r9") = a6; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory"); return ret; }
#elif defined(__i386__) || defined(__i686__) //x86 32 bit syscalls
inline long _syscall0(long n){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n) : "memory"); return r; }
inline long _syscall1(long n, long a1){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1) : "memory"); return r; }
inline long _syscall2(long n, long a1, long a2){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2) : "memory"); return r; }
inline long _syscall3(long n, long a1, long a2, long a3){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3) : "memory"); return r; }
inline long _syscall4(long n, long a1, long a2, long a3, long a4){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory"); return r; }
inline long _syscall5(long n, long a1, long a2, long a3, long a4, long a5){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5) : "memory"); return r; }
inline long _syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6){ long r; asm volatile ("pushl %%ebp; movl %7, %%ebp; int $0x80; popl %%ebp" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5), "m"(a6) : "memory"); return r; }
#endif

#ifdef __x86_64__ //x86_64 syscall table
#define SYS_read 0
#define SYS_write 1
#define SYS_open 2
#define SYS_close 3
#define SYS_stat 4
#define SYS_fstat 5
#define SYS_lstat 6
#define SYS_poll 7
#define SYS_lseek 8
#define SYS_mmap 9
#define SYS_mprotect 10
#define SYS_munmap 11
#define SYS_exit 60
#elif defined(__i386__) || defined(__i686__) //x86 32 bit syscall table
#define SYS_restart_syscall 0
#define SYS_exit 1
#define SYS_fork 2
#define SYS_read 3
#define SYS_write 4
#define SYS_open 5
#define SYS_close 6
#define SYS_waitpid 7
#define SYS_creat 8
#define SYS_link 9
#define SYS_unlink 10
// ...
#define SYS_lseek 19
#define SYS_stat 106    // Это современный stat (sys_stat64 обычно 195)
#define SYS_fstat 108
#define SYS_lstat 107
#define SYS_mmap 90     // Или 192 для mmap2 (смещение в страницах)
#define SYS_munmap 91
#define SYS_mprotect 125
#define SYS_poll 168
#endif

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

ssize_t read(int fd, void* buf, size_t count){ return (ssize_t)_syscall3(SYS_read, (long)fd, (long)buf, (long)count); }
ssize_t write(int fd, const void* buf, size_t count){ return (ssize_t)_syscall3(SYS_write, (long)fd, (long)buf, (long)count); }
int open(const char* pathname, int flags, int mode){ return (int)_syscall3(SYS_open, (long)pathname, (long)flags, (long)mode); }
int close(int fd){ return (int)_syscall1(SYS_close, (long)fd); }

// 4, 5, 6: stat, fstat, lstat -> (!) структура stat различается между архитектурами!
int stat(const char* pathname, void* statbuf){ return (int)_syscall2(SYS_stat, (long)pathname, (long)statbuf); }
int fstat(int fd, void* statbuf){ return (int)_syscall2(SYS_fstat, (long)fd, (long)statbuf); }
int lstat(const char* pathname, void* statbuf){ return (int)_syscall2(SYS_lstat, (long)pathname, (long)statbuf); }

int poll(void* fds, unsigned long nfds, int timeout){ return (int)_syscall3(SYS_poll, (long)fds, (long)nfds, (long)timeout); }
off_t lseek(int fd, off_t offset, int whence){ return (off_t)_syscall3(SYS_lseek, (long)fd, (long)offset, (long)whence); }

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset){
#ifdef __x86_64__
    return (void*)_syscall6(SYS_mmap, (long)addr, (long)length, (long)prot, (long)flags, (long)fd, (long)offset);
#elif defined(__i386__) || defined(__i686__)
    return (void*)_syscall6(SYS_mmap, (long)addr, (long)length, (long)prot, (long)flags, (long)fd, (long)(offset >> 12)); // На i386 mmap2 (192) ожидает смещение в страницах (offset >> 12)
#endif
}

int munmap(void* addr, size_t length){ return (int)_syscall2(SYS_munmap, (long)addr, (long)length); }
	
#ifdef __cplusplus
}
#endif

