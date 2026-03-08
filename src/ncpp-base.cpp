//#include "ncpp.cpp.hpp"
// ======== Base Module: Include Sources ========
#include "base/print.cpp" // Depends: None
#include "base/utils0.cpp" //Depends: None
#include "base/Array.cpp" // Depends: utils0
#include "base/String.cpp" // Depends: Array
#include "base/dtos.cpp" //Depends: utils0, String
#include "base/Buffer.cpp" //Depends: utils0, Array, String, dtos
#include "base/HashMap.cpp" //Depends: Array

#ifdef NCPP_USESTL
#include "base/ostream.hpp"
#endif
