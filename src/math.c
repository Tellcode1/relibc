#include "../include/math.h"
#include "../.cordic_atan_table.h"
#include "../include/attributes.h"
#include "../include/fenv.h"
#include "../include/floatfmt.h"
#include "../include/stdafx.h"
#include <stdbool.h>

fenv_t _re_fe_curr_env = _re_def_fe_env;

long double
copysignl(long double x, long double y)
{
  return x * ((y < 0.0L) ? -1.0L : 1.0L);
}

static inline double_t
factorial(double_t xint)
{
  if (xint <= 0.0F) { return 1.0F; }
  return xint * factorial(xint - 1.0);
}

static inline double_t
_pow(double_t xval, double_t xpow)
{
  const double_t orig_val = xval;
  for (double_t i = 0; i < xpow; i += 1.0)
  {
    xval *= orig_val;
  }
  return xval;
}

double_t
_re_atan(double_t xrads)
{
  if (RE_UNLIKELY(xrads == 0.0)) { return 0.0; }

  double_t result = 0.0;
  double_t term   = xrads; // x^1 / 1
  double_t sign   = 1.0;

  if (RE_UNLIKELY(xrads > 1.0)) { return M_PI_2 - _re_atan(1.0 / xrads); }
  else if (RE_UNLIKELY(xrads < -1.0)) { return -M_PI_2 - _re_atan(1.0 / xrads); }

  const double_t iterations = 32.0;

  for (double_t i = 0.0; i < iterations; i++)
  {
    double_t denom = 2.0 * i + 1.0; // 1,3,5,7
    result += sign * (term / denom);
    term *= xrads * xrads; // next power x^(n+2)
    sign = -sign;
  }

  return result;
}

static inline double_t
_wrap(double_t value, double_t min, double_t max)
{
  double_t range   = max - min;
  double_t wrapped = fmod(value - min, range);
  if (wrapped < 0.0) { wrapped += range; }
  return wrapped + min;
}

static inline float_t
_wrapf(float_t value, float_t min, float_t max)
{
  float_t range   = max - min;
  float_t wrapped = fmodf(value - min, range);
  if (wrapped < 0.0) { wrapped += range; }
  return wrapped + min;
}

static inline ldouble_t
_wrapl(ldouble_t value, ldouble_t min, ldouble_t max)
{
  ldouble_t range   = max - min;
  ldouble_t wrapped = fmodl(value - min, range);
  if (wrapped < 0.0) { wrapped += range; }
  return wrapped + min;
}

void
_re_cordic(double_t xrads, double_t* RE_RESTRICT o_sin, double_t* RE_RESTRICT o_cos)
{
  xrads = _wrap(xrads, -M_PI, M_PI);

  bool negate_sin = false, negate_cos = false;

  if (xrads < -M_PI_2l)
  {
    xrads += M_PI;
    negate_sin = true;
    negate_cos = true;
  }
  else if (xrads > M_PI_2l)
  {
    xrads -= M_PI;
    negate_sin = true;
    negate_cos = true;
  }

  double_t x = 1.0;
  double_t y = 0.0;
  double_t z = xrads;

  for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; i++)
  {
    double_t di     = copysign(1.0, z);
    double_t factor = 1.0 / (double_t)(1ULL << i);

    double_t x_new = x - di * y * factor;
    double_t y_new = y + di * x * factor;
    double_t z_new = z - di * (double_t)_cordic_atan_table[i];

    x = x_new;
    y = y_new;
    z = z_new;
  }

  x *= (double_t)_CORDIC_GAIN;
  y *= (double_t)_CORDIC_GAIN;

  if (negate_cos) x = -x;
  if (negate_sin) y = -y;

  if (o_cos) *o_cos = x;
  if (o_sin) *o_sin = y;
}

void
_re_cordicf(float_t xrads, float_t* RE_RESTRICT o_sin, float_t* RE_RESTRICT o_cos)
{
  xrads = _wrapf(xrads, -(float_t)M_PI, (float_t)M_PI);

  bool negate_sin = false, negate_cos = false;

  if (xrads < -M_PI_2l)
  {
    xrads += (float_t)M_PI;
    negate_sin = true;
    negate_cos = true;
  }
  else if (xrads > M_PI_2l)
  {
    xrads -= (float_t)M_PI;
    negate_sin = true;
    negate_cos = true;
  }

  float_t x = 1.0F;
  float_t y = 0.0F;
  float_t z = xrads;

  for (unsigned long i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; i++)
  {
    float_t di     = copysignf(1.0F, z);
    float_t factor = 1.0F / (float_t)(1ULL << i);

    float_t x_new = x - di * y * factor;
    float_t y_new = y + di * x * factor;
    float_t z_new = z - di * (float_t)_cordic_atan_table[i];

    x = x_new;
    y = y_new;
    z = z_new;
  }

  x *= (float_t)_CORDIC_GAIN;
  y *= (float_t)_CORDIC_GAIN;

  if (negate_cos) x = -x;
  if (negate_sin) y = -y;

  if (o_cos) *o_cos = x;
  if (o_sin) *o_sin = y;
}

void
_re_cordicl(ldouble_t xrads, ldouble_t* RE_RESTRICT o_sin, ldouble_t* RE_RESTRICT o_cos)
{
  xrads = _wrapl(xrads, -M_PI_2l, M_PI_2l);

  bool negate_sin = false, negate_cos = false;

  if (xrads < -M_PI_2l)
  {
    xrads += M_PIl;
    negate_sin = true;
    negate_cos = true;
  }
  else if (xrads > M_PI_2l)
  {
    xrads -= M_PIl;
    negate_sin = true;
    negate_cos = true;
  }

  ldouble_t x = 1.0L;
  ldouble_t y = 0.0L;
  ldouble_t z = xrads;

  for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; i++)
  {
    ldouble_t di     = copysignl(1.0L, z);
    ldouble_t factor = 1.0L / (1ULL << i);

    ldouble_t x_new = x - di * y * factor;
    ldouble_t y_new = y + di * x * factor;
    ldouble_t z_new = z - di * _cordic_atan_table[i];

    x = x_new;
    y = y_new;
    z = z_new;
  }

  x *= _CORDIC_GAIN;
  y *= _CORDIC_GAIN;

  if (negate_cos) x = -x;
  if (negate_sin) y = -y;

  if (o_cos) *o_cos = x;
  if (o_sin) *o_sin = y;
}

void
_re_cordic_circular(double_t x_in, double_t y_in, double_t* out_magnitude, double_t* out_angle)
{
  double_t x = x_in;
  double_t y = y_in;
  double_t z = 0.0;

  for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; ++i)
  {
    double_t di = copysign(1.0, y);

    double_t x_new = x + di * y * (1.0 / (double_t)(1 << i));
    double_t y_new = y - di * x * (1.0 / (double_t)(1 << i));
    double_t z_new = z + di * (double_t)_cordic_atan_table[i];

    x = x_new;
    y = y_new;
    z = z_new;
  }

  *out_magnitude = x * (double_t)_CORDIC_GAIN; // multiply final x by gain to undo CORDIC scaling
  if (out_angle) *out_angle = z;
}

void
_re_cordic_circularf(float_t x_in, float_t y_in, float_t* out_magnitude, float_t* out_angle)
{
  float_t x = x_in;
  float_t y = y_in;
  float_t z = 0.0F;

  for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; ++i)
  {
    float_t di = copysignf(1.0F, y);

    float_t x_new = x + di * y * (1.0F / (float_t)(1 << i));
    float_t y_new = y - di * x * (1.0F / (float_t)(1 << i));
    float_t z_new = z + di * (float_t)_cordic_atan_table[i];

    x = x_new;
    y = y_new;
    z = z_new;
  }

  *out_magnitude = x * (float_t)_CORDIC_GAIN; // multiply final x by gain to undo CORDIC scaling
  if (out_angle) *out_angle = z;
}

void
_re_cordic_circularl(ldouble_t x_in, ldouble_t y_in, ldouble_t* out_magnitude, ldouble_t* out_angle)
{
  ldouble_t x = x_in;
  ldouble_t y = y_in;
  ldouble_t z = 0.0;

  for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; ++i)
  {
    ldouble_t di = copysignl(1.0L, y);

    ldouble_t x_new = x + di * y * (1.0L / (1 << i));
    ldouble_t y_new = y - di * x * (1.0L / (1 << i));
    ldouble_t z_new = z + di * _cordic_atan_table[i];

    x = x_new;
    y = y_new;
    z = z_new;
  }

  *out_magnitude = x * _CORDIC_GAIN; // multiply final x by gain to undo CORDIC scaling
  if (out_angle) { *out_angle = z; }
}

#define COS_LOW_THRESHOLD 1e-5

double_t
tan(double_t xrads)
{
  double_t sin_ = 0.0, cos_ = 0.0;
  _re_cordic(xrads, &sin_, &cos_);

  if (fabs(cos_) <= COS_LOW_THRESHOLD)
  {
    feraiseexcept(FE_INVALID);
    return INFINITY;
  }

  return sin_ / cos_;
}

float_t
tanf(float_t xrads)
{
  float_t sin_ = 0.0F, cos_ = 0.0F;
  _re_cordicf(xrads, &sin_, &cos_);

  if (fabsf(cos_) < (float_t)COS_LOW_THRESHOLD)
  {
    feraiseexcept(FE_INVALID);
    return INFINITY;
  }

  return sin_ / cos_;
}

ldouble_t
tanl(ldouble_t xrads)
{
  ldouble_t sin_ = 0.0L, cos_ = 0.0L;
  _re_cordicl(xrads, &sin_, &cos_);

  if (fabsl(cos_) < COS_LOW_THRESHOLD)
  {
    feraiseexcept(FE_INVALID);
    return INFINITY;
  }

  return sin_ / cos_;
}

int
_re_mask_fp_excepts(int excepts, int mask_or_unmask)
{
#if defined(__STDC_IEC_559__)

#  if defined(__x86_64__) || defined(__i386__)
  RE_ATTR_ALIGNED(16) uint16_t cw    = 0;
  RE_ATTR_ALIGNED(16) uint32_t mxcsr = 0;

  __asm__ volatile("fnstcw %0" : "=m"(cw));
  __asm__ volatile("stmxcsr %0" : "=m"(mxcsr));

  if (mask_or_unmask == FE_MASK_UNMASK) { cw &= ~(uint16_t)(excepts); }
  else { cw |= (uint16_t)(excepts); }

  uint32_t mxcsr_mask_bits = ((uint32_t)excepts) << 7;
  if (mask_or_unmask == FE_MASK_UNMASK) { mxcsr &= ~mxcsr_mask_bits; }
  else { mxcsr |= mxcsr_mask_bits; }

  __asm__ volatile("fldcw %0" : : "m"(cw));
  __asm__ volatile("ldmxcsr %0" : : "m"(mxcsr));
#  elif defined(__aarch64__) || defined(__arm__)
  uint32_t fpscr;
  asm volatile("vmrs %0, fpscr" : "=r"(fpscr));

  if (mask_or_unmask == FE_MASK_UNMASK) { cw &= ~(uint16_t)(excepts); }
  else { cw |= (uint16_t)(excepts); }

  uint32_t mxcsr_mask_bits = ((uint32_t)excepts) << 7;
  if (mask_or_unmask == FE_MASK_UNMASK) { mxcsr &= ~mxcsr_mask_bits; }
  else { mxcsr |= mxcsr_mask_bits; }

  __asm__ volatile("vmsr fpscr, %0" : : "r"(fpscr));
#  else
  return -1;
#  endif

#endif

  return 0;
}

int
feraiseexcept(int excepts)
{
  _re_fe_curr_env.except |= excepts;

  int unmasked = excepts & ~_re_fe_curr_env.masked;

  if (unmasked)
  {
    volatile double x, y, z;

    if (excepts & FE_INVALID)
    {
      x = 0.0;
      y = 0.0;
      z = x / y; // 0.0 / 0.0 = NaN
      (void)z;
    }
    if (excepts & FE_DIVBYZERO)
    {
      x = 1.0;
      y = 0.0;
      z = x / y; // division by zero
      (void)z;
    }
    if (excepts & FE_OVERFLOW)
    {
      x = FLT_MAX;
      y = FLT_MAX;
      z = x * y; // overflow
      (void)z;
    }
    if (excepts & FE_UNDERFLOW)
    {
      x = FLT_MIN;
      y = FLT_MIN;
      z = x * y; // underflow
      (void)z;
    }
    if (excepts & FE_INEXACT)
    {
      x = 1.0;
      y = 3.0;
      z = x / y; // inexact result
      (void)z;
    }
  }

  return 0;
}

double_t
fmod(double_t x, double_t by)
{
  if (RE_UNLIKELY(isnan(x))) { return x; }
  if (RE_UNLIKELY(isnan(by))) { return by; }
  if (RE_UNLIKELY(by == 0.0))
  {
    feraiseexcept(FE_DIVBYZERO);
    return NAN;
  }

  return x - trunc(x / by) * by;
}

float_t
fmodf(float_t x, float_t by)
{
  if (RE_UNLIKELY(isnan(x))) { return x; }
  if (RE_UNLIKELY(isnan(by))) { return by; }
  if (RE_UNLIKELY(by == 0.0))
  {
    feraiseexcept(FE_DIVBYZERO);
    return NAN;
  }

  return x - truncf(x / by) * by;
}

ldouble_t
fmodl(ldouble_t x, ldouble_t by)
{
  if (RE_UNLIKELY(isnan(x))) { return x; }
  if (RE_UNLIKELY(isnan(by))) { return by; }
  if (RE_UNLIKELY(by == 0.0))
  {
    feraiseexcept(FE_DIVBYZERO);
    return NAN;
  }

  return x - truncl(x / by) * by;
}

double_t
exp(double_t x)
{
  volatile union
  {
    double_t d;
    uint64_t i;
  } cvt;

  double_t t  = x * M_LOG2E;
  double_t fi = floor(t);
  double_t f  = t - fi;
  int64_t  i  = (int64_t)fi;

  cvt.d = (0.3371894346 * f + 0.657636276) * f + 1.00172476;
  cvt.i += ((int64_t)i << 52);

  return cvt.d;
}

float_t
expf(float_t x)
{
  // https://stackoverflow.com/a/10792321
  volatile union
  {
    float_t      f;
    unsigned int i;
  } cvt;

  /* exp(x) = 2^i * 2^f; i = floor (log2(e) * x), 0 <= f <= 1 */
  float t  = x * (float_t)M_LOG2E;
  float fi = floorf(t);
  float f  = t - fi;
  int   i  = (int)fi;
  cvt.f    = (0.3371894346f * f + 0.657636276f) * f + 1.00172476f; /* compute 2^f */
  cvt.i += (i << 23);                                              /* scale by 2^i */
  return cvt.f;
}

ldouble_t
expl(ldouble_t x)
{
  ldouble_t t  = x * 1.4426950408889634074L; // log2(e)
  ldouble_t fi = floorl(t);
  ldouble_t f  = t - fi;
  int64_t   i  = (int64_t)fi;

  ldouble_t two_to_f = (0.3371894346L * f + 0.657636276L) * f + 1.00172476L;
  return ldexpl(two_to_f, i); // two_to_f * 2^i
}

double_t
pow(double_t base, double_t exp)
{
}

float_t
powf(float_t base, float_t exp)
{
}

ldouble_t
powl(ldouble_t base, ldouble_t exp)
{
}

double_t
ldexp(double_t x, int exp)
{
  if (RE_UNLIKELY(isnan(x) || x == 0.0 || isinf(x) || exp == 0)) { return x; }

  struct double_repr repr = re_break_double(x);
  repr.exponent += exp;
  return re_make_double(repr);
}

float_t
ldexpf(float_t x, int exp)
{
  if (RE_UNLIKELY(isnan(x) || x == 0.0F || isinf(x) || exp == 0)) { return x; }

  struct float_repr repr = re_break_float(x);
  repr.exponent += exp;
  return re_make_float(repr);
}

ldouble_t
ldexpl(ldouble_t x, int exp)
{
  if (RE_UNLIKELY(isnan(x) || x == 0.0L || isinf(x) || exp == 0)) { return x; }

  struct long_double_repr repr = re_break_long_double(x);
  repr.exponent += exp;
  return re_make_long_double(repr);
}

double_t
frexp(double_t num, int* exp)
{
  if (RE_UNLIKELY(num == 0.0))
  {
    if (RE_LIKELY(exp)) { *exp = 0; }
    return 0.0;
  }

  struct double_repr repr = re_break_double(num);

  if (RE_LIKELY(exp)) { *exp = repr.exponent + 1; }

  // repr.sign     = repr.sign;
  repr.exponent = -1; // -1 exponent = 0.5 ≤ m < 1.0
  repr.mantissa = repr.mantissa;
  return re_make_double(repr);
}

float_t
frexpf(float_t num, int* exp)
{
  if (RE_UNLIKELY(num == 0.0F))
  {
    if (RE_LIKELY(exp)) { *exp = 0; }
    return 0.0F;
  }

  struct float_repr repr = re_break_float(num);

  if (RE_LIKELY(exp)) { *exp = repr.exponent + 1; }

  // repr.sign     = repr.sign;
  repr.exponent = -1; // -1 exponent = 0.5 ≤ m < 1.0
  repr.mantissa = repr.mantissa;
  return re_make_float(repr);
}

ldouble_t
frexpl(ldouble_t num, int* exp)
{
  if (RE_UNLIKELY(num == 0.0L))
  {
    if (RE_LIKELY(exp)) { *exp = 0; }
    return 0.0L;
  }

  struct long_double_repr repr = re_break_long_double(num);

  if (RE_LIKELY(exp)) { *exp = repr.exponent + 1; }

  // repr.sign     = repr.sign;
  repr.exponent = -1; // -1 exponent = 0.5 ≤ m < 1.0
  repr.mantissa = repr.mantissa;
  return re_make_long_double(repr);
}

double_t
log(double_t x)
{
  if (RE_UNLIKELY(x < 0.0)) { return NAN; }
  else if (RE_UNLIKELY(x == 0.0)) { return -INFINITY; }
  else if (RE_UNLIKELY(isinf(x))) { return copysign(INFINITY, x); }

  int      exponent            = 0;
  double_t fractional_mantissa = frexp(x, &exponent); // 0.5 <= fractional_mantissa <= 1.0
  fractional_mantissa *= 2.0;                         // bring fractional_mantissa to 1.0 <= fractional_mantissa <= 2.0
  exponent--;                                         // subtract the exponent by 1 so we aren't actually modifying the number, just how we chose to represent it

  // Use the derived formula: ln (x) = ln(m) + exponent * ln(2), where m is the fractional mantissa and exponent is the integral exponent of x
  // You can derive the formula from the IEEE754 floating point representation: x = m * 2 ^ exponent.

  // Use horner's method on a mercator series to calculate ln(m) or the natural logarithm of the fractional mantissa
  // the series falls off in accuracy near its bounds so that's an issue.
  double_t z         = (fractional_mantissa - 1) / (fractional_mantissa + 1);
  double_t z_squared = z * z;

  const int iterations = 8;

  double_t result = 1.0 / (double_t)(2 * iterations + 1);
  for (int i = iterations - 1; i >= 0; i--) // its - 1 because we seeded the first term manually
  {
    result = (1.0 / (double_t)((2 * i) + 1)) + z_squared * result;
  }

  double_t ln_fractional_mantissa = 2.0 * z * result;         // ln(m)
  return ln_fractional_mantissa + (double_t)exponent * M_LN2; // ln(m) + exponent * ln(2)
}

float_t
logf(float_t x)
{
  if (RE_UNLIKELY(x < 0.0F)) { return NAN; }
  else if (RE_UNLIKELY(x == 0.0F)) { return -INFINITY; }
  else if (RE_UNLIKELY(isinf(x))) { return copysignf(INFINITY, x); }

  int     exponent            = 0;
  float_t fractional_mantissa = frexpf(x, &exponent); // 0.5 <= fractional_mantissa <= 1.0
  fractional_mantissa *= 2.0F;                        // bring fractional_mantissa to 1.0 <= fractional_mantissa <= 2.0
  exponent--;                                         // subtract the exponent by 1 so we aren't actually modifying the number, just how we chose to represent it

  // Use the derived formula: ln (x) = ln(m) + exponent * ln(2), where m is the fractional mantissa and exponent is the integral exponent of x
  // You can derive the formula from the IEEE754 floating point representation: x = m * 2 ^ exponent.

  // Use horner's method on a mercator series to calculate ln(m) or the natural logarithm of the fractional mantissa
  // the series falls off in accuracy near its bounds so that's an issue.
  float_t z         = (fractional_mantissa - 1) / (fractional_mantissa + 1);
  float_t z_squared = z * z;

  const int iterations = 4;

  float_t result = 1.0 / (float_t)(2 * iterations + 1);
  for (int i = iterations - 1; i >= 0; i--) // its - 1 because we seeded the first term manually
  {
    result = (1.0F / (float_t)((2 * i) + 1)) + z_squared * result;
  }

  float_t ln_fractional_mantissa = 2.0F * z * result;        // ln(m)
  return ln_fractional_mantissa + (float_t)exponent * M_LN2; // ln(m) + exponent * ln(2)
}

ldouble_t
logl(ldouble_t x)
{
  if (RE_UNLIKELY(x < 0.0F)) { return NAN; }
  else if (RE_UNLIKELY(x == 0.0F)) { return -INFINITY; }
  else if (RE_UNLIKELY(isinf(x))) { return copysignl(INFINITY, x); }

  int       exponent            = 0;
  ldouble_t fractional_mantissa = frexpl(x, &exponent); // 0.5 <= fractional_mantissa <= 1.0
  fractional_mantissa *= 2.0F;                          // bring fractional_mantissa to 1.0 <= fractional_mantissa <= 2.0
  exponent--;                                           // subtract the exponent by 1 so we aren't actually modifying the number, just how we chose to represent it

  // Use the derived formula: ln (x) = ln(m) + exponent * ln(2), where m is the fractional mantissa and exponent is the integral exponent of x
  // You can derive the formula from the IEEE754 floating point representation: x = m * 2 ^ exponent.

  // Use horner's method on a mercator series to calculate ln(m) or the natural logarithm of the fractional mantissa
  // the series falls off in accuracy near its bounds so that's an issue.
  ldouble_t z         = (fractional_mantissa - 1) / (fractional_mantissa + 1);
  ldouble_t z_squared = z * z;

  const int iterations = 16;

  ldouble_t result = 1.0 / (ldouble_t)(2 * iterations + 1);
  for (int i = iterations - 1; i >= 0; i--) // its - 1 because we seeded the first term manually
  {
    result = (1.0F / (ldouble_t)((2 * i) + 1)) + z_squared * result;
  }

  ldouble_t ln_fractional_mantissa = 2.0F * z * result;        // ln(m)
  return ln_fractional_mantissa + (ldouble_t)exponent * M_LN2; // ln(m) + exponent * ln(2)
}

double_t
log10(double_t x)
{
  return log(x) / M_LN10;
}

float_t
log10f(float_t x)
{
  return logf(x) / (float_t)M_LN10;
}

ldouble_t
log10l(ldouble_t x)
{
  return logl(x) / M_LN10l;
}

extern double_t  log2(double_t x);
extern float_t   log2f(float_t x);
extern ldouble_t log2l(ldouble_t x);
