#ifndef NCPP_H_BASE
#define NCPP_H_BASE
//#include "ncpp.cpp.hpp"
// ======== Base Module Include Sources ========
#include "base/print.h" // Depends: None
#include "base/utils0.hpp" //Depends: None
#include "base/Array.hpp" // Depends: utils0
#include "base/String.hpp" // Depends: Array
#include "base/dtos.hpp" //Depends: utils0, String
#include "base/Buffer.hpp" //Depends: utils0, Array, String, dtos
#include "base/HashMap.hpp" //Depends: Array
#include "base/utils.hpp" //Depends: String
#include "base/Date.hpp" //Depends: utils

#ifdef NCPP_USESTL
#include "base/ostream.hpp"
#endif

#endif
