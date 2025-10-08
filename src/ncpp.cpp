// ncpp pre-v0.0.1-0; Primary entry point for direct compilation with gcc/clang.
// PD: No rights reserved; Released into the public domain.
#ifndef NCPP_CPP
#define NCPP_CPP
#include "ncpp.cpp.h"

// ======== Direct Include Sources ========
#include "ncpp-base.cpp"
#include "system/fs.cpp"
#include "ncpp-system.cpp"
#include "net/socket.cpp"
#include "net/unixsocket.cpp"
#include "net/http.cpp"
#include "system/thread.cpp"
#include "net/server.cpp"
#include "system/Console.cpp"
#include "ncpp-object.cpp"
#include "ncpp-bigint.cpp"
//#include "experimantal/ncpp-bigint_v2.cpp"
//#include "experimantal/SlabAllocator.cpp"
#include "ncpp-crypto.cpp"
#include "net/websocket.cpp"

#if defined(_WIN32) || defined(USE_GUI)
//#include "gui/gui.cpp"
#endif

#endif
