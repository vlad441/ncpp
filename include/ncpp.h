// ncpp pre-v0.0.0; Main header file.
// CC0 License: No rights reserved; Released into the public domain under CC0.
#ifndef NCPP_H
#define NCPP_H
#define NCPP_VER "v0.0.0-0"

#if __cplusplus < 201103L
// C++ 98 polyfills ====
#define nullptr NULL
#define noexcept throw()
#define unique_ptr auto_ptr
#endif

// ======== Include Interfaces ========
#include "ncpp-base.h"

#endif // NCPP_H
