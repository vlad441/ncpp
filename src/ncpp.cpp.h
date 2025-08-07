// ncpp: tech header file. Entry point for compilation from object files. Using only for direct compilation or build libs.
#ifndef NCPP_H
#define NCPP_H
#define NCPP_VER "v0.0.1-0-rev1"
namespace ncpp { void print(const char *cptr); const char* version(); } //Only declaration.

/*#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // 0x0501 - WinXP; 0x0601 - Win7; 0x0602 - Win8; 0x0A00 - Win10;
#endif*/

#if defined(_WIN32) && (defined(__x86_64__) || defined(__aarch64__) || defined(__LP64__)) // win 64 bit
	//typedef long long int_t; typedef long long ssize_t;
#elif defined(_WIN32)
    //typedef int int_t; typedef int ssize_t;
#endif

#include <stdlib.h> // C lib
#include <string.h> // strlen, memcpy, etc.
#include <stdint.h> // int types: int32_t, int64_t, etc.
#include <math.h>
#include <ctype.h> // isdigit, isalpha, isalnum, etc.
#include <errno.h> // errno: UNIX get last error;

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>  // for AcceptEx
//#include <windows.h>
#else
#include <unistd.h> // Linux POSIX API (write(), close(), getpid, _fd, etc...)
#include <sys/types.h> //pid_t, uid_t, gid_t, off_t
#include <sys/socket.h>
#include <netdb.h> // for getaddrinfo
#include <arpa/inet.h> // struct in_addr, inet_pton, inet_ntop
#include <spawn.h> // for posix_spawn
#include <signal.h> // signals kill, etc.
#include <sys/time.h> // time api: gettimeofday
#include <fcntl.h> // for socket descryptor settings (non-bloking mode, etc).
#include <limits.h> //PATH_MAX constant
#endif

// === polyfills ===
#define NPOS (size_t)-1

#if __cplusplus < 201103L
#define noexcept throw()
#define nullptr NULL
//#define unique_ptr auto_ptr
#endif

#ifdef NCPP_USESTL
#include <new> //WTF? placement new без него никак?
#else
void* operator new(size_t size, void* ptr) noexcept { return ptr; } //Определение сигнатуры для placement new
//#include "stdcpp/libsupc.cpp"
#endif

#if defined(_WIN32) && defined(__GNUC__) && __GNUC__ < 4
#include "system/gcc3_winxpdef.h"
#endif

void _ncpp_check_ver(){	if(strcmp(NCPP_VER, ncpp::version())!=0){
	ncpp::print(""\
	"█████████████████████████████████████████████████████████████████████████\n"\
	"██                                                                   ██\n"\
	"██  (!) Предупреждение: Несоответствие версий ncpp!                  ██\n"\
	"██                                                                   ██\n"\
	"██  Обнаружено расхождение между ожидаемой версией NCPP и            ██\n"\
	"██  версией, с которой скомпилирована библиотека.                    ██\n"\
	"██                                                                   ██\n"\
	"██  Ожидаемая версия (NCPP_VER): "); ncpp::print(NCPP_VER); ncpp::print("\n"\
	"██  Фактическая версия (ncpp::version()): "); ncpp::print(ncpp::version()); ncpp::print("\n"\
	"██                                                                   ██\n"\
	"██  Это может привести к непредсказуемому поведению,                 ██\n"\
	"██  сбоям или уязвимостям безопасности.                              ██\n"\
	"██                                                                   ██\n"\
	"██  РЕШЕНИЕ:                                                         ██\n"\
	"██  1. Убедитесь, что все компоненты (исходный код, библиотеки,      ██\n"\
	"██     заголовочные файлы) используют одну и ту же версию NCPP.      ██\n"\
	"██  2. Перекомпилируйте проект, используя соответствующую версию     ██\n"\
	"██     библиотеки или обновите исходный код до актуальной версии.    ██\n"\
	"██                                                                   ██\n"\
	"█████████████████████████████████████████████████████████████████████████\n");
	// exit(1);
} }

#endif // NCPP_H end
