#include "../include/floatfmt.h"
#include "../include/string.h"
#include <stdio.h>

#define FLOAT_EXP_BIAS (FLT_MAX_EXP - 1)
#define DOUBLE_EXP_BIAS (DBL_MAX_EXP - 1)
#define LDBL_EXP_BIAS (LDBL_MAX_EXP - 1)

struct float_repr
re_break_float(float_t flt)
{
  uint32_t bits;
  memcpy(&bits, &flt, sizeof(bits));

  struct float_repr repr;
  repr.sign     = (uint8_t)((bits >> 31) & 1);
  int raw_exp   = (int)((bits >> 23) & 0xFF);
  repr.exponent = raw_exp - FLOAT_EXP_BIAS;
  repr.mantissa = bits & 0x7FFFFF;

  if (raw_exp != 0)
  {
    repr.mantissa |= 1ULL << 23;
  }
  return repr;
}

struct double_repr
re_break_double(double_t dbl)
{
  uint64_t bits;
  memcpy(&bits, &dbl, sizeof(bits));

  uint16_t raw_exp = (bits >> 52) & 0x7FF;

  struct double_repr repr;
  repr.sign     = (uint8_t)((bits >> 63) & 1);
  repr.exponent = (int)raw_exp - DOUBLE_EXP_BIAS;
  repr.mantissa = bits & 0xFFFFFFFFFFFFFULL;

  if (raw_exp != 0)
  {                              // subnormal or something idk man
    repr.mantissa |= 1ULL << 52; // add implicit leading 1
  }
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

  uint16_t exp  = (bits >> 52) & 0x7FF;
  repr.sign     = (uint8_t)((bits >> 63) & 1);
  repr.exponent = (int)exp - LDBL_EXP_BIAS;

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

  uint16_t exp  = raw.exp_sign & 0x7FFF;
  repr.sign     = (uint8_t)((raw.exp_sign >> 15) & 1);
  repr.exponent = (int)exp - LDBL_EXP_BIAS;

  if (exp == 0)
  {
    // subnormal/zero
    repr.mantissa = raw.mantissa;
  }
  else
  {
    // no need to add leading 1, x87 already has it
    repr.mantissa = raw.mantissa;
  }

#else
#  warning "Unsupported long double format on this platform."
#endif

  return repr;
}

float_t
re_make_float(struct float_repr repr)
{
  // IEEE-754: 1 sign bit | 8 exponent bits (biased) | 23 mantissa bits
  uint32_t u = 0;

  uint32_t biased_exp = (uint32_t)(repr.exponent + 127);
  u |= (repr.sign & 1) << 31;
  u |= (biased_exp & 0xFF) << 23;
  u |= (repr.mantissa & 0x7FFFFF); // 23 bits

  float f;
  memcpy(&f, &u, sizeof(f));
  return f;
}

double_t
re_make_double(struct double_repr repr)
{
  uint64_t u = 0;

  uint64_t biased_exp = (uint64_t)(repr.exponent + 1023);
  u |= ((uint64_t)(repr.sign & 1)) << 63;
  u |= (biased_exp & 0x7FF) << 52;
  u |= (repr.mantissa & 0xFFFFFFFFFFFFF); // 52 bits

  double d;
  memcpy(&d, &u, sizeof(d));
  return d;
}

ldouble_t
re_make_long_double(struct long_double_repr repr)
{
#if defined(__x86_64__) || defined(__i386__)
  // x87 80-bit long double: 1-bit sign, 15-bit biased exponent, 1 explicit integer bit + 63-bit fraction

  struct
  {
    uint64_t mantissa; // 64-bit mantissa, includes integer bit (MSB)
    uint16_t se;       // sign + exponent: [15:1] = exponent, [0] = sign
  } raw = { 0 };

  uint16_t biased_exp = (uint16_t)(repr.exponent + LDBL_EXP_BIAS);
  raw.mantissa        = repr.mantissa; // already includes integer bit at bit 63
  raw.se              = (repr.sign << 15) | (biased_exp & 0x7FFF);

  ldouble_t result;
  memcpy(&result, &raw, sizeof(result));
  return result;

#elif (LDBL_MANT_DIG == DBL_MANT_DIG && LDBL_MAX_EXP == DBL_MAX_EXP)

  struct double_repr drep = {
    .sign     = repr.sign,
    .exponent = repr.exponent,
    .mantissa = repr.mantissa & 0xFFFFFFFFFFFFF // truncate to 52 bits
  };
  return re_make_double(drep);

#else
#  error "Unsupported long double format"
#endif
}
