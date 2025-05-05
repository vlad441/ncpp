// ncpp: tech header file. Entry point for compilation from object files. Using only for direct compilation or build libs.
/*#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // 0x0501 - WinXP; 0x0601 - Win7; 0x0A00 - Win10;
#endif*/

#ifndef NCPP_H
#define NCPP_H
#define NCPP_VER "v0.0.0-0"

#if defined(_WIN32) && (defined(__x86_64__) || defined(__aarch64__) || defined(__LP64__)) // win 64 bit
	//typedef long long int_t;
	//typedef long long ssize_t;
#elif defined(_WIN32)
    //typedef int int_t;
    //typedef int ssize_t;
#endif

//#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <map>
#include <set>
#include <sstream> // for std::stringstream
#include <cstring> // for std::strlen, memcpy, memset, etc../
#include <cstdlib> // for size_t, atoi и atof, etc...
//--#include <cstddef> // only type def? size_t, NULL, etc...
#include <ctime> // for std::time(0)
#include <stdint.h> // for int64_t, etc types...
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::reverse
#include <limits.h> // types max values
#include <iomanip> // for std::hex, std::setprecision
//#include <cstdio> // for printf, std::sprintf
// == sockets ==
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>  // for AcceptEx
//#define popen(cmd, mode) _popen(cmd, mode)
//#define pclose(fp) _pclose(fp)
#else
#include <sys/types.h> //pid_t, uid_t, gid_t, off_t
#include <sys/socket.h>
#include <netdb.h> // for getaddrinfo
#include <arpa/inet.h> // struct in_addr, inet_pton, inet_ntop
#include <unistd.h> // Linux POSIX API (write(), close(), getpid, _fd, etc...)
#include <spawn.h> // for posix_spawn
#include <signal.h> // signals kill, etc.
#include <sys/time.h> // unix time api
#include <cerrno> // errno: UNIX get last error;
#include <fcntl.h> // for socket descryptor settings(non-bloking mode, etc).
#include <memory>  // smart pointers (shared_ptr, unique_ptr, etc).
#endif
// ==== C++ 11 includes ====
#if __cplusplus >= 201103L
#include <unordered_map>
#include <unordered_set>
#include <chrono> //time api
#include <functional> //std::function
#else
// C++ 98 polyfills ====
#define nullptr NULL
#define noexcept throw()
#define unique_ptr auto_ptr
#endif
// === defines ===
//#define DEF_BUFF_SIZE 4096

#if defined(_WIN32) && defined(__GNUC__) && __GNUC__ < 4
#include "system/gcc3_winxpdef.h"
#endif

#endif // NCPP_H
