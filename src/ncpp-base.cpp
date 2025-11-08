#include "ncpp.cpp.h"
// ======== Base Include Sources ========
// -- Base --
#include "base/print.cpp" // Depends: None
#include "base/Array.cpp" // Depends: None
#include "base/String.cpp" // Depends: Array
#include "base/dtos.cpp" //Depends: String
#include "base/Buffer.cpp" //Depends: Array, String
#include "base/HashMap.cpp" //Depends: Array
#include "base/utils.cpp"

#ifdef NCPP_USESTL
#include "base/ostream.h"
#endif
