#ifndef _RE_FLOATING_POINT_FORMAT_H
#define _RE_FLOATING_POINT_FORMAT_H

#include "attributes.h"
#include "float.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define FLT_MANTISSA_SIZE (FLT_MANT_DIG)
#define DBL_MANTISSA_SIZE (DBL_MANT_DIG)
#define LDBL_MANTISSA_SIZE (LDBL_MANT_DIG)

// the minus one is for the sign bit. These are in bits by the way
#define FLT_EXPONENT_SIZE ((sizeof(float) * 8) - FLT_MANTISSA_SIZE - 1)
#define DBL_EXPONENT_SIZE ((sizeof(double) * 8) - DBL_MANTISSA_SIZE - 1)
#define LDBL_EXPONENT_SIZE ((sizeof(long double) * 8) - LDBL_MANTISSA_SIZE - 1)

#define FLT_MANTISSA_OFFSET (1)
#define DBL_MANTISSA_OFFSET (1)
#define LDBL_MANTISSA_OFFSET (1)

#define FLT_EXPONENT_OFFSET (1 + FLT_MANTISSA_SIZE)
#define DBL_EXPONENT_OFFSET (1 + DBL_EXPONENT_SIZE)
#define LDBL_EXPONENT_OFFSET (1 + LDBL_EXPONENT_SIZE)

  struct float_repr
  {
    uint8_t  sign : (1);                     // 0 -> positive, 1 -> negative
    int64_t exponent : (FLT_EXPONENT_SIZE + 1); // unbiased raw bits
    uint32_t mantissa : (FLT_MANTISSA_SIZE); // raw bits + explicit leading 1, divide by 2 ^ FLT_MANTISSA_SIZE to get fractional value.
  };

  struct double_repr
  {
    uint8_t  sign : (1);                     // 0 -> positive, 1 -> negative
    int64_t exponent : (DBL_EXPONENT_SIZE + 1); // unbiased raw bits
    uint64_t mantissa : (DBL_MANTISSA_SIZE); // raw bits + explicit leading 1, divide by 2 ^ DBL_MANTISSA_SIZE to get fractional value.
  };

  struct long_double_repr
  {
    uint8_t  sign : (1);
    int64_t exponent : (LDBL_EXPONENT_SIZE + 1); // unbiased raw bits
    uint64_t mantissa : (LDBL_MANTISSA_SIZE); // always has an explicit leading 1 bit, divide by 2 ^ LDBL_MANTISSA_SIZE to get fractional value.
  };

  extern RE_ATTR_CONST struct float_repr       re_break_float(float_t flt);
  extern RE_ATTR_CONST struct double_repr      re_break_double(double_t dbl);
  extern RE_ATTR_CONST struct long_double_repr re_break_long_double(ldouble_t ldbl);

  extern RE_ATTR_CONST float_t   re_make_float(struct float_repr repr);
  extern RE_ATTR_CONST double_t  re_make_double(struct double_repr repr);
  extern RE_ATTR_CONST ldouble_t re_make_long_double(struct long_double_repr repr);

#ifdef __cplusplus
}
#endif

#endif //_RE_FLOATING_POINT_FORMAT_H
