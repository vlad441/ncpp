// ncpp - (testincludes)
#include <iostream>
#include <string>
#include <vector> // for dynamic arrays
#include <cstring> // for std::strlen, memset and memcpy
#include <sstream> // for std::stringstream
#include <cstdlib> // for atoi и atof, etc...
#include <fstream> // for file operations
#include <math.h> // for matan
#include <ctime> // for std::time(0)
#include <stdint.h> // for int64_t, etc int types...
#include <stdexcept> // std::runtime_error
#include <map>
#include <algorithm> // std::reverse
//#include <iomanip> // for std::hex
//#include <cstdio> // for std::sprintf
// == sockets ==
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h> //pid_t, uid_t, gid_t, off_t
#include <sys/socket.h>
#include <netdb.h> // for getaddrinfo
#include <arpa/inet.h> // struct in_addr, inet_pton, inet_ntop
#include <unistd.h> // Linux POSIX API (close socket, getpid, etc...)
#include <spawn.h> // for posix_spawn
#include <signal.h> // signals kill, etc.
#include <sys/time.h> // unix time api
#include <cerrno> // errno: UNIX get last error;
#endif
// ==== C++ 11 includes ====
#if __cplusplus >= 201103L // C++11 или новее
#include <thread>
#include <unordered_map>
#include <chrono> //time api
#endif

#ifdef _WIN32
#else
    void Sleep(int msec){ usleep(msec*1000); }
#endif
