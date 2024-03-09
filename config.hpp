#pragma once

#ifndef NO_EXPORT
#define NO_EXPORT
#endif

constexpr inline short DEFAULT_VAL = -1;
constexpr inline int EXIT_PROGRAM = 0;
constexpr inline int RESTART_PROGRAM = 1;

//#if __cplusplus < 202004L
//#error ("This program requires C++20 or higher, please use a newer compiler.")
//#endif

#ifndef _MSC_VER
#warning ("This program was developed in Microsoft Visual Studio 2022, if you use another compiler, you may encounter errors.")
#elif _MSC_VER < 1935
#warning ("This program was developed in MSVC version 1935, please use the same version or higher, otherwise you may encounter errors.")
#endif
