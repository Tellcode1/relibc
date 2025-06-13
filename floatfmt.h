#ifndef _RE_FLOATING_POINT_FORMAT_H
#define _RE_FLOATING_POINT_FORMAT_H

#include <stdint.h>

#include "float.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define FLT_MANTISSA_SIZE (FLT_MANT_DIG - 1)
#define DBL_MANTISSA_SIZE (DBL_MANT_DIG - 1)
#define LDBL_MANTISSA_SIZE (LDBL_MANT_DIG - 1)

// the minus one is for the sign bit
#define FLT_EXPONENT_SIZE ((sizeof(float) * 8) - FLT_MANTISSA_SIZE - 1)
#define DBL_EXPONENT_SIZE ((sizeof(double) * 8) - DBL_MANTISSA_SIZE - 1)
#define LDBL_EXPONENT_SIZE ((sizeof(long double) * 8) - LDBL_MANTISSA_SIZE - 1)

  struct float_repr
  {
    int      sign : (1);                     // 0 or 1
    int      exponent : (FLT_EXPONENT_SIZE); // unbiased
    uint32_t mantissa : (FLT_MANTISSA_SIZE); // raw bits
  };

  struct double_repr
  {
    int      sign : (1);                     // 0 or 1
    int      exponent : (DBL_EXPONENT_SIZE); // unbiased
    uint64_t mantissa : (DBL_MANTISSA_SIZE); // raw bits
  };

  struct long_double_repr
  {
    int      sign : (1);
    uint64_t exponent : (LDBL_EXPONENT_SIZE); // unbiased
    uint64_t mantissa : (LDBL_MANTISSA_SIZE); // always has an explicit leading 1 bit
  };

  extern __attribute__((const)) struct float_repr       re_break_float(float_t flt);
  extern __attribute__((const)) struct double_repr      re_break_double(double_t dbl);
  extern __attribute__((const)) struct long_double_repr re_break_long_double(ldouble_t ldbl);

  extern __attribute__((const)) float_t   re_make_float(struct float_repr repr);
  extern __attribute__((const)) double_t  re_make_double(struct double_repr repr);
  extern __attribute__((const)) ldouble_t re_make_long_double(struct long_double_repr repr);

#ifdef __cplusplus
}
#endif

#endif //_RE_FLOATING_POINT_FORMAT_H
