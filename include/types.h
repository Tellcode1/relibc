#ifndef RE_TYPES_H_INCLUDED_
#define RE_TYPES_H_INCLUDED_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

  typedef uint64_t u64;
  typedef uint32_t u32;
  typedef uint16_t u16;
  typedef uint8_t  u8;
  typedef int8_t   sbyte;
  typedef uint8_t  ubyte;

  typedef unsigned char uchar;

  typedef int64_t i64;
  typedef int32_t i32;
  typedef int16_t i16;
  typedef int8_t  i8;

  // They ARE 32 and 64 bits by IEEE-754 but aren't set by the standard
  // But there is a 99.9% chance that they will be
  typedef float  f32;
  typedef double f64;

#ifdef __cplusplus
}
#endif

#endif // RE_TYPES_H_INCLUDED_
