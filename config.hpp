#pragma once

// This file `config.hpp` has pre-defined macros and constants 
//		that are used throughout the program.
// If the file is included in a source file, 
//		it MUST be included first.
// Otherwise, errors such as linker error may occur.
#ifdef _MSC_VER

#include <yvals_core.h>

#ifndef _WIN32
#error ("This program is designed to run on Windows, please use a Windows machine.")
#endif

#if !_HAS_CXX23
#error ("This program requires C++23 or higher, please use a newer compiler.")
#endif

#ifdef __cpp_lib_modules
#if __cpp_lib_modules < 202207L
#error ("This program requires C++ modules TS 202207 or higher, please use a newer compiler.")
#endif
#endif

#else
#warning("This program was developed in Microsoft Visual Studio 2022, if you use another compiler, you may encounter errors.")
#endif

#ifndef NO_EXPORT
#define NO_EXPORT
#endif

#ifndef DEFINITION
#define DEFINITION
#endif

import "limits.hpp";
import "core.hpp";

extern inline int realMain([[maybe_unused]] const int& argc, [[maybe_unused]] char* argv[]);