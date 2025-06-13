#include "floatfmt.h"
#include <string.h>

#define FLOAT_EXP_BIAS (FLT_MAX_EXP - 1)
#define DOUBLE_EXP_BIAS (DBL_MAX_EXP - 1)
#define LDBL_EXP_BIAS (LDBL_MAX_EXP - 1)

struct float_repr
re_break_float(float_t flt)
{
  uint32_t bits;
  memcpy(&bits, &flt, sizeof(bits));

  struct float_repr repr;
  repr.sign     = (int)(bits >> 31) & 1;
  repr.exponent = (int)((bits >> 23) & 0xFF) - FLOAT_EXP_BIAS;
  repr.mantissa = bits & 0x7FFFFF;
  return repr;
}

struct double_repr
re_break_double(double_t dbl)
{
  uint64_t bits;
  memcpy(&bits, &dbl, sizeof(bits));

  struct double_repr repr;
  repr.sign     = (int)(bits >> 63) & 1;
  repr.exponent = (int)((bits >> 52) & 0x7FF) - DOUBLE_EXP_BIAS;
  repr.mantissa = bits & 0xFFFFFFFFFFFFFULL;
  return repr;
}

struct long_double_repr
re_break_long_double(ldouble_t ldbl)
{
  struct long_double_repr repr;

#if LDBL_MANT_DIG == DBL_MANT_DIG && LDBL_MAX_EXP == DBL_MAX_EXP
  // IEEE-754 double
  uint64_t bits;
  memcpy(&bits, &ldbl, sizeof(bits));

  uint16_t exp = (bits >> 52) & 0x7FF;
  repr.sign       = (bits >> 63) & 1;
  repr.exponent   = (int)exp - DOUBLE_EXP_BIAS;

  if (exp == 0)
  {
    // subnormal/zero
    repr.mantissa = bits & 0xFFFFFFFFFFFFFULL;
  }
  else
  {
    // add implicit leading 1
    repr.mantissa = (1ULL << 52) | (bits & 0xFFFFFFFFFFFFFULL);
  }

#elif LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384
  // 80-bit
  struct
  {
    uint64_t mantissa;
    uint16_t exp_sign;
  } raw;
  memcpy(&raw, &ldbl, sizeof(raw));

  uint16_t exp = raw.exp_sign & 0x7FFF;
  repr.sign       = (raw.exp_sign >> 15) & 1;
  repr.exponent   = (int)exp - LDBL_EXP_BIAS;

  if (exp == 0)
  {
    // subnormal/zero
    repr.mantissa = raw.mantissa;
  }
  else
  {
    // no need to add leading 1, x87 already has it
    repr.mantissa = raw.mantissa;
    if ((repr.mantissa >> 63) == 0) { repr.mantissa |= 1ULL << 63; }
  }

#else
#  warning "Unsupported long double format on this platform."
#endif

  return repr;
}

float_t   re_make_float(struct float_repr repr);
double_t  re_make_double(struct double_repr repr);
ldouble_t re_make_long_double(struct long_double_repr repr);