#pragma once


#include <stdint.h>

#ifndef DEFINED_BASIC_TYPES
typedef int8_t       s8 ;  //!< 8-bit signed integer.
typedef uint8_t      u8 ;  //!< 8-bit unsigned integer.
typedef int16_t      s16;  //!< 16-bit signed integer.
typedef uint16_t     u16;  //!< 16-bit unsigned integer.
typedef int32_t      s32;  //!< 32-bit signed integer.
typedef uint32_t     u32;  //!< 32-bit unsigned integer.
typedef int64_t      s64;  //!< 64-bit signed integer.
typedef uint64_t     u64;  //!< 64-bit unsigned integer.
typedef float        f32;  //!< 32-bit floating-point number.
typedef double       f64;  //!< 64-bit floating-point number.

// use this to avoid redefinition errors
#define DEFINED_BASIC_TYPES
#endif
