#ifdef __cplusplus
extern "C" {
#endif

#ifdef __x86_64__ //=== x86_64 Linux syscalls === https://filippo.io/linux-syscall-table/
inline long _syscall0(long n){ long ret; asm volatile ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory"); return ret; }
inline long _syscall1(long n, long a1){ long ret; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory"); return ret; }
inline long _syscall2(long n, long a1, long a2){ long ret; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2) : "rcx", "r11", "memory"); return ret; }
inline long _syscall3(long n, long a1, long a2, long a3){ long ret; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory"); return ret; }
inline long _syscall4(long n, long a1, long a2, long a3, long a4){ long ret; register long r10 asm("r10") = a4; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10) : "rcx", "r11", "memory"); return ret; }
inline long _syscall5(long n, long a1, long a2, long a3, long a4, long a5){ long ret; register long r10 asm("r10") = a4; register long r8 asm("r8") = a5; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory"); return ret; }
inline long _syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6){ long ret; register long r10 asm("r10") = a4; register long r8 asm("r8") = a5; register long r9 asm("r9") = a6; asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory"); return ret; }
#elif defined(__i386__) || defined(__i686__) //=== x86 32 bit Linux syscalls ===
inline long _syscall0(long n){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n) : "memory"); return r; }
inline long _syscall1(long n, long a1){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1) : "memory"); return r; }
inline long _syscall2(long n, long a1, long a2){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2) : "memory"); return r; }
inline long _syscall3(long n, long a1, long a2, long a3){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3) : "memory"); return r; }
inline long _syscall4(long n, long a1, long a2, long a3, long a4){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory"); return r; }
inline long _syscall5(long n, long a1, long a2, long a3, long a4, long a5){ long r; asm volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5) : "memory"); return r; }
inline long _syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6){ long r; asm volatile ("pushl %%ebp; movl %7, %%ebp; int $0x80; popl %%ebp" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5), "m"(a6) : "memory"); return r; }
#elif defined(__aarch64__) // === ARM64 Linux syscalls ===
inline long _syscall0(long n){ register long x8 asm("x8") = n; register long x0 asm("x0"); asm volatile ("svc #0" : "=r"(x0) : "r"(x8) : "memory"); return x0; }
inline long _syscall1(long n, long a1){ register long x8 asm("x8") = n; register long x0 asm("x0") = a1; asm volatile ("svc #0" : "+r"(x0) : "r"(x8) : "memory"); return x0; }
inline long _syscall2(long n, long a1, long a2){ register long x8 asm("x8") = n; register long x0 asm("x0") = a1; register long x1 asm("x1") = a2; asm volatile ("svc #0" : "+r"(x0) : "r"(x8), "r"(x1) : "memory"); return x0; }
inline long _syscall3(long n, long a1, long a2, long a3){ 
    register long x8 asm("x8") = n; register long x0 asm("x0") = a1; register long x1 asm("x1") = a2; register long x2 asm("x2") = a3;
    asm volatile ("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory"); return x0; }
inline long _syscall4(long n, long a1, long a2, long a3, long a4){ 
    register long x8 asm("x8") = n; register long x0 asm("x0") = a1; register long x1 asm("x1") = a2; 
    register long x2 asm("x2") = a3; register long x3 asm("x3") = a4; 
    asm volatile ("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3) : "memory"); return x0; }
inline long _syscall5(long n, long a1, long a2, long a3, long a4, long a5){ 
    register long x8 asm("x8") = n; register long x0 asm("x0") = a1; register long x1 asm("x1") = a2; 
    register long x2 asm("x2") = a3; register long x3 asm("x3") = a4; register long x4 asm("x4") = a5; 
    asm volatile ("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4) : "memory"); return x0; }
inline long _syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6){ 
    register long x8 asm("x8") = n; register long x0 asm("x0") = a1; register long x1 asm("x1") = a2; register long x2 asm("x2") = a3; 
    register long x3 asm("x3") = a4; register long x4 asm("x4") = a5; register long x5 asm("x5") = a6; 
    asm volatile ("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "memory"); return x0; }
#elif defined(__riscv) && (__riscv_xlen == 64) // === RISCV64 Linux syscalls ===
inline long _syscall0(long n){ register long a7 asm("a7") = n; register long a0 asm("a0"); asm volatile ("ecall" : "=r"(a0) : "r"(a7) : "memory"); return a0; }
inline long _syscall1(long n, long a1){ register long a7 asm("a7") = n; register long a0 asm("a0") = a1; asm volatile ("ecall" : "+r"(a0) : "r"(a7) : "memory"); return a0; }
inline long _syscall2(long n, long a1, long a2){ register long a7 asm("a7") = n; register long a0 asm("a0") = a1; register long a1_reg asm("a1") = a2; asm volatile ("ecall" : "+r"(a0) : "r"(a7), "r"(a1_reg) : "memory"); return a0; }
inline long _syscall3(long n, long a1, long a2, long a3){
    register long a7 asm("a7") = n; register long a0 asm("a0") = a1; register long a1_reg asm("a1") = a2; register long a2_reg asm("a2") = a3;
    asm volatile ("ecall" : "+r"(a0) : "r"(a7), "r"(a1_reg), "r"(a2_reg) : "memory"); return a0; }
inline long _syscall4(long n, long a1, long a2, long a3, long a4){ 
    register long a7 asm("a7") = n; register long a0 asm("a0") = a1; register long a1_reg asm("a1") = a2; 
    register long a2_reg asm("a2") = a3; register long a3_reg asm("a3") = a4; 
    asm volatile ("ecall" : "+r"(a0) : "r"(a7), "r"(a1_reg), "r"(a2_reg), "r"(a3_reg) : "memory"); return a0; }
inline long _syscall5(long n, long a1, long a2, long a3, long a4, long a5){ 
    register long a7 asm("a7") = n; register long a0 asm("a0") = a1; register long a1_reg asm("a1") = a2; 
    register long a2_reg asm("a2") = a3; register long a3_reg asm("a3") = a4; register long a4_reg asm("a4") = a5; 
    asm volatile ("ecall" : "+r"(a0) : "r"(a7), "r"(a1_reg), "r"(a2_reg), "r"(a3_reg), "r"(a4_reg) : "memory"); return a0; }
inline long _syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6){ 
    register long a7 asm("a7") = n; register long a0 asm("a0") = a1; register long a1_reg asm("a1") = a2; register long a2_reg asm("a2") = a3;
	register long a3_reg asm("a3") = a4; register long a4_reg asm("a4") = a5; register long a5_reg asm("a5") = a6;
    asm volatile ("ecall" : "+r"(a0) : "r"(a7), "r"(a1_reg), "r"(a2_reg), "r"(a3_reg), "r"(a4_reg), "r"(a5_reg) : "memory"); return a0; }
#endif

#ifdef __x86_64__ //=== x86_64 syscall table ===
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

#elif defined(__i386__) || defined(__i686__) // === x86 32 bit syscall table ===
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
#define SYS_mmap 90 //mmap на x32 принимает указатель на структуру
#define SYS_mmap2 192 // mmap2 принимает аргументы отдельно
#define SYS_munmap 91
#define SYS_mprotect 125
#define SYS_poll 168
#else //========= asm-generic/unistd.h (ARM, RISCV, etc.) =========
#define SYS_io_setup 0
#define SYS_io_destroy 1
#define SYS_io_submit 2
#define SYS_io_cancel 3
#define SYS_io_getevents 4
#define SYS_setxattr 5
#define SYS_lsetxattr 6
#define SYS_fsetxattr 7
#define SYS_getxattr 8
#define SYS_lgetxattr 9
#define SYS_fgetxattr 10
#define SYS_listxattr 11
#define SYS_llistxattr 12
#define SYS_flistxattr 13
#define SYS_removexattr 14
#define SYS_lremovexattr 15
#define SYS_fremovexattr 16
#define SYS_getcwd 17
#define SYS_dup 23
#define SYS_dup3 24
#define SYS_fcntl 25
#define SYS_ioctl 29
#define SYS_flock 32
#define SYS_mknodat 33
#define SYS_mkdirat 34
#define SYS_unlinkat 35
#define SYS_symlinkat 36
#define SYS_linkat 37
#define SYS_umount2 39
#define SYS_mount 40
#define SYS_pivot_root 41
#define SYS_statfs 43
#define SYS_fstatfs 44
#define SYS_truncate 45
#define SYS_ftruncate 46
#define SYS_fallocate 47
#define SYS_faccessat 48
#define SYS_chdir 49
#define SYS_fchdir 50
#define SYS_chroot 51
#define SYS_fchmod 52
#define SYS_fchmodat 53
#define SYS_fchownat 54
#define SYS_fchown 55
#define SYS_openat 56
#define SYS_close 57
#define SYS_vhangup 58
#define SYS_pipe2 59
#define SYS_quotactl 60
#define SYS_getdents64 61
#define SYS_lseek 62
#define SYS_read 63
#define SYS_write 64
#define SYS_readv 65
#define SYS_writev 66
#define SYS_pread64 67
#define SYS_pwrite64 68
#define SYS_preadv 69
#define SYS_pwritev 70
#define SYS_sendfile 71
#define SYS_pselect6 72
#define SYS_ppoll 73
#define SYS_readlinkat 78
#define SYS_fstatat 79
#define SYS_fstat 80
#define SYS_sync 81
#define SYS_fsync 82
#define SYS_fdatasync 83
#define SYS_exit 93
#define SYS_mprotect 226
#define SYS_munmap 215
#define SYS_mmap 222
#define SYS_poll 168
#endif

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

__attribute__((noreturn, cold, unused)) void exit(int status){ _syscall1(SYS_exit, status); }
ssize_t read(int fd, void* buf, size_t count){ return (ssize_t)_syscall3(SYS_read, (long)fd, (long)buf, (long)count); }
ssize_t write(int fd, const void* buf, size_t count){ return (ssize_t)_syscall3(SYS_write, (long)fd, (long)buf, (long)count); }
int open(const char* pathname, int flags, int mode){ return (int)_syscall3(SYS_open, (long)pathname, (long)flags, (long)mode); }
int close(int fd){ return (int)_syscall1(SYS_close, (long)fd); }

#ifdef __x86_64__ // 4, 5, 6: stat, fstat, lstat -> (!) структура stat различается между архитектурами! //asm-generic: fstatat?
int stat(const char* pathname, void* statbuf){ return (int)_syscall2(SYS_stat, (long)pathname, (long)statbuf); }
int fstat(int fd, void* statbuf){ return (int)_syscall2(SYS_fstat, (long)fd, (long)statbuf); }
int lstat(const char* pathname, void* statbuf){ return (int)_syscall2(SYS_lstat, (long)pathname, (long)statbuf); }
#endif

int poll(void* fds, unsigned long nfds, int timeout){ return (int)_syscall3(SYS_poll, (long)fds, (long)nfds, (long)timeout); }
off_t lseek(int fd, off_t offset, int whence){ return (off_t)_syscall3(SYS_lseek, (long)fd, (long)offset, (long)whence); }

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset){
#if !defined(__x86_64__) && (defined(__i386__) || defined(__i686__)) //x86 32 bit
    return (void*)_syscall6(SYS_mmap2, (long)addr, (long)length, (long)prot, (long)flags, (long)fd, (long)(offset >> 12)); // На i386 mmap2 (192) ожидает смещение в страницах (offset >> 12)
#else
	return (void*)_syscall6(SYS_mmap, (long)addr, (long)length, (long)prot, (long)flags, (long)fd, (long)offset);
#endif
}

int munmap(void* addr, size_t length){ return (int)_syscall2(SYS_munmap, (long)addr, (long)length); }
	
#ifdef __cplusplus
}
#endif

