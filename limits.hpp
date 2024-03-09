#pragma once

// due to the bug in the current version of the compiler 
// that missing together C++ module and ordinary headers,
// I have to redefine some macro in the `limits.h`.

constexpr static inline int CHAR_BIT = 8;
constexpr static inline int SCHAR_MIN = (-128);
constexpr static inline int SCHAR_MAX = 127;
constexpr static inline int UCHAR_MAX = 0xff;

#ifndef _CHAR_UNSIGNED
    constexpr static inline int CHAR_MIN = SCHAR_MIN;
    constexpr static inline int CHAR_MAX = SCHAR_MAX;
#else
    constexpr static inline int CHAR_MIN = 0;
    constexpr static inline int CHAR_MAX = UCHAR_MAX;
#endif

constexpr static inline int MB_LEN_MAX = 5;
constexpr static inline int SHRT_MIN = (-32768);
constexpr static inline int SHRT_MAX = 32767;
constexpr static inline int USHRT_MAX = 0xffff;
constexpr static inline int INT_MIN = (-2147483647 - 1);
constexpr static inline int INT_MAX = 2147483647;
constexpr static inline unsigned int UINT_MAX = 0xffffffff;
constexpr static inline long LONG_MIN = (-2147483647L - 1);
constexpr static inline long LONG_MAX = 2147483647L;
constexpr static inline unsigned long ULONG_MAX = 0xffffffffUL;
constexpr static inline long long LLONG_MAX = 9223372036854775807i64;
constexpr static inline long long LLONG_MIN = (-9223372036854775807i64 - 1);
constexpr static inline unsigned long long ULLONG_MAX = 0xffffffffffffffffui64;

constexpr static inline int _I8_MIN = (-127i8 - 1);
constexpr static inline int _I8_MAX = 127i8;
constexpr static inline int _UI8_MAX = 0xffui8;

constexpr static inline int _I16_MIN = (-32767i16 - 1);
constexpr static inline int _I16_MAX = 32767i16;
constexpr static inline int _UI16_MAX = 0xffffui16;

constexpr static inline int _I32_MIN = (-2147483647i32 - 1);
constexpr static inline int _I32_MAX = 2147483647i32;
constexpr static inline unsigned int _UI32_MAX = 0xffffffffui32;

constexpr static inline long long _I64_MIN = (-9223372036854775807i64 - 1);
constexpr static inline long long _I64_MAX = 9223372036854775807i64;
constexpr static inline unsigned long long _UI64_MAX = 0xffffffffffffffffui64;

#ifndef SIZE_MAX
    // SIZE_MAX definition must match exactly with stdint.h for modules support.
    #ifdef _WIN64
        constexpr static inline unsigned long long SIZE_MAX = 0xffffffffffffffffui64;
    #else
        constexpr static inline unsigned int SIZE_MAX = 0xffffffffui32;
    #endif
#endif

#if __STDC_WANT_SECURE_LIB__
    #ifndef RSIZE_MAX
        constexpr static inline size_t RSIZE_MAX = (SIZE_MAX >> 1);
    #endif
#endif