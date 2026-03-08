// ncpp pre-v0.0.1-0; Primary entry point for direct compilation with gcc/clang. (Unity Build style)
// PD: No rights reserved; Released into the public domain.
#ifndef NCPP_CPP
#define NCPP_CPP
#include "ncppdef.hpp"

// ======== Direct Include Sources ========
#include "ncpp-base.cpp"
#include "sys/utils.cpp"
#include "sys/fs.cpp"
#include "sys/info.cpp"
#include "sys/process.cpp"
#include "sys/Date.cpp"
#include "sys/thread.cpp"
#include "sys/Console.cpp"

#ifndef NOUSE_NET
#include "net/socket.cpp"
#include "net/unixsocket.cpp"
#include "net/http.cpp"
#include "net/server.cpp"
#endif

#ifndef NOUSE_OBJECT
#include "structs/object.cpp"
#endif

#include "structs/bigint.cpp"
//#include "experimental/bigint_old.cpp"
//#include "experimental/Allocators.cpp"
//#include "experimental/utf8.cpp"

#ifndef NOUSE_CRYPTO
#include "ncpp-crypto.cpp"
#ifndef NOUSE_NET
#include "net/websocket.cpp"
#endif
#endif

#if defined(USE_GUI) //|| defined(_WIN32)
#include "gui/gui.cpp"
#endif

#if defined(_WIN32) || defined(USE_GUI)
#include "sys/mk-input.cpp"
#endif

#endif
