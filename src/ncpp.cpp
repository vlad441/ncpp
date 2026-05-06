// ncpp (Node C++). Primary entry point for direct compilation with GCC/Clang. (Unity Build style)
// PD: No rights reserved. Released into the Public Domain.
// Project Site: http://ncpp.art | Project in GitHub: https://github.com/vlad441/ncpp
#ifndef NCPP_CPP
#define NCPP_CPP
#include "ncppdef.hpp"

// ======== Direct Include Sources ========
// --- Base ---
#include "base/print.cpp" // Depends: None
#include "base/utils0.cpp" //Depends: None
#include "base/Array.cpp" // Depends: utils0
#include "base/String.cpp" // Depends: utils0, Array
#include "base/dtos.cpp" //Depends: utils0, String
#include "base/Buffer.cpp" //Depends: utils0, Array, String, dtos
#include "base/HashMap.cpp" //Depends: utils0, Array

// #ifdef NCPP_USESTL
// #include "base/ostream.hpp"
// #endif
// --- ---
#ifndef NCPP_BASE_ONLY

#include "sys/utils.cpp"
#include "sys/fs.cpp"
#include "sys/info.cpp"
#include "sys/process.cpp"
#include "sys/Date.cpp"
#include "sys/thread.cpp"
#include "sys/Console.cpp"

#ifndef NCPP_SYS_ONLY

#ifndef NOUSE_NET
#include "net/socket.cpp"
#include "net/unixsocket.cpp"
#include "net/http.cpp"
#include "net/server.cpp"
#endif

#ifndef NOUSE_OBJECT
#include "structs/Object.cpp"
#endif

#include "structs/BigInt.cpp"
//#include "experimental/bigint_old.cpp"

#ifdef USE_EXPERIMENTAL
#include "experimental/Allocators.cpp"
#include "experimental/utf8.cpp"

#ifndef NOUSE_MEDIA
#include "experimental/media/audio.cpp"
#include "experimental/media/images.cpp"
#endif
#include "experimental/gl/gl-engine.cpp"
#endif

#ifndef NOUSE_CRYPTO
#include "ncpp-crypto.cpp"
#ifndef NOUSE_NET
#include "net/websocket.cpp"
#endif
#endif

#if defined(USE_GUI) //|| defined(_WIN32)
#include "gui/gui.cpp"
#endif

#endif //ends NCPP_SYS_ONLY

#if defined(_WIN32) || defined(USE_GUI)
#include "sys/mk-input.cpp"
#endif

#endif //ends NCPP_BASE_ONLY
#endif //ends NCPP_CPP
