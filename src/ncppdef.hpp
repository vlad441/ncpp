// ncpp: tech header file. Entry point for compilation from object files. Using only for direct compilation or build libs.
#pragma once
#ifndef NCPP_H_DEF
#define NCPP_H_DEF
#define NCPP_VER "v0.0.1-0-dev"
//#define NCPP_VER_MAJOR 0
//#define NCPP_VER_MINOR 0
//#define NCPP_VER_PATCH 1
//#define NCPP_VERSION(major, minor, patch) ((major << 16) | (minor << 8) | (patch))
//#define NCPP_VER_CODE NCPP_VERSION(NCPP_VER_MAJOR, NCPP_VER_MINOR, NCPP_VER_PATCH)

/*#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // 0x0400 - Win 95; 0x0410 - Win 98; 0x0500 - Win 2000; 0x0501 - WinXP; 0x0601 - Win7; 0x0602 - Win8; 0x0A00 - Win10;
#endif*/	

#include <stdlib.h> // C lib
#include <string.h> // strlen, memcpy, etc.
#include <stdint.h> // int types: int32_t, int64_t, etc.
#include <math.h>
#include <ctype.h> // isdigit, isalpha, isalnum, etc.
#include <errno.h> // errno: UNIX get last error;
#include <limits.h> // INT_MAX, LLONG_MAX, ULLONG_MAX, PATH_MAX constant, etc.

#ifdef _WIN32 // == Windows Headers ==
#define WIN32_LEAN_AND_MEAN  // Исключает редко используемые компоненты из заголовков Windows
#define NOSERVICE            // Исключает API служб
#define NOMCX                // Исключает API модемов
#define NOIME                // Исключает API ввода символов (иероглифы и т.д.)
#else // == Linux Headers ==
#include <unistd.h> // Linux POSIX API (write(), close(), getpid, _fd, etc...)
#include <sys/types.h> //pid_t, uid_t, gid_t, off_t
#include <sys/socket.h>
#include <netdb.h> // for getaddrinfo
#include <arpa/inet.h> // struct in_addr, inet_pton, inet_ntop
#include <spawn.h> // for posix_spawn
#include <signal.h> // signals kill, etc.
#include <sys/time.h> // time api: gettimeofday
#include <fcntl.h> // for socket descryptor settings (non-bloking mode, etc).
#endif

namespace ncpp { void print(const char *cptr); const char* version(); void print(long long v); } //Only declaration.

#define NPOS (size_t)-1
#if __cplusplus >= 201103L
#else //C++98
#define noexcept throw()
#define nullptr NULL
#define decltype __typeof__
//#define unique_ptr auto_ptr
#endif

//#ifndef NCPP_LIB_USE
#ifdef NCPP_USESTL
#include <new> //WTF? placement new без него никак?
#else
void* operator new(size_t size, void* ptr) noexcept; //Объявление сигнатуры для placement new
//#include "stdcpp/libsupc.cpp" //Only for Linux
#endif //NCPP_USESTL
//#endif //NCPP_LIB_USE
//#include "experimental/initializer_list.hpp"
//#define NCPP_COUT_COLORED

#if defined(__GNUC__) && __GNUC__ < 4
#include "sys/gcc3_def.h"
#endif

#ifndef NCPP_LIB_BUILD
void _ncpp_check_ver(){	if(strcmp(NCPP_VER, ncpp::version())==0) return;
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
}
#endif // NCPP_LIB_BUILD
#endif // NCPP_H_DEF end
