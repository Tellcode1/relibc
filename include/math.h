#ifndef _RELIBC_MATH_H
#define _RELIBC_MATH_H

#include "float.h"
#include "floatfmt.h"
#include "stdafx.h"
#include "stdint.h"

#ifndef __STDC_IEC_559__
#  error RElib depends on IEE-754 floating point numbers. Sorry for the inconvenience.
#endif

/**
 * Whether isnan and isinf always return false under -ffast-math.
 * If false, a compiler error is generated instead.
 */
#ifndef _RELIB_MATH_NAN_AND_INF_CHECKS_RETURN_FALSE
#  define _RELIB_MATH_NAN_AND_INF_CHECKS_RETURN_FALSE true
#endif

#define RE_HEADER_FN static inline

#if (__STDC_VERSION__ >= 199901L)
#  define RE_TYPECAST(to_cast, var_or_type_to_cast_to) ((__typeof__(var_or_type_to_cast_to))to_cast)
#else
#  define RE_TYPECAST(to_cast, var_or_type_to_cast_to)
#endif

#if defined(__has_attribute) && __has_attribute(const)
#  define RE_CONST_FN __attribute__((const))
#else
#  define RE_CONST_FN
#endif

#define M_E 2.718281828459045       // The value of euler's number
#define M_LOG2E 1.442695040889      // The log base 2 of e
#define M_LOG10E 0.43429448190325   // The log base 10 of e
#define M_LN2 0.69314718055995      // The natural log (log base e) of 2
#define M_LN10 2.302585092994       // The natural log of 10
#define M_PI 3.14159265358979       // The constant pi
#define M_PI_2 1.5707963267949      // PI / 2
#define M_PI_4 0.785398163397448    // PI / 4
#define M_1_PI 0.318309886183791    // 1 / PI
#define M_2_PI 0.636619772367582    // 2 / PI
#define M_2_SQRTPI 1.12837916709551 // 2 / sqrt(PI)
#define M_SQRT2 1.4142135623731     // sqrt(2)
#define M_SQRT1_2 0.707106781186548 // 1 / sqrt(2)
#define M_SQRT1_3 0.577350269189626 // 1 / sqrt(3)
#define M_EGAMMA 0.5772156649015328 //  euler mascheroni constant
#define M_PHI 1.61803398874989      // The golden ratio

// These are accurate up to 32 digits.
#define M_El 2.6769901293781826846423269494124L
#define M_LOG2El 1.4206118194705100048992637015555L
#define M_LOG10El 0.1524754236750955727828596599193L
#define M_LN2l 0.69314718055994530941723212145818L
#define M_LN10l 2.3025850929940456840179914546844L
#define M_PIl 3.1415926535897932384626433832795L
#define M_PI_2l 1.5707963267948966192313216916398L
#define M_PI_4l 0.78539816339744830961566084581988L
#define M_1_PIl 0.31830988618379067153776752674503L
#define M_2_PIl 0.63661977236758134307553505349006L
#define M_2_SQRTPIl 1.1283791670955125738961589031215L
#define M_SQRT2l 1.4142135623730950488016887242097L
#define M_SQRT1_2l 0.70710678118654752440084436210485L
#define M_SQRT1_3l 0.57735026918962576450914878050196L
#define M_EGAMMAl 0.57721566490153286060651209008240L
#define M_PHIl 1.6180339887498948482045868343656L

#ifdef __GNUC__
#  define INFINITY (__builtin_inff())
#  define NAN (__builtin_nanf(""))
#else
#  define INFINITY ((double)(0x7FF0000000000000))
#  define NAN (0.0 / 0.0)
#endif

#define HUGE_VAL INFINITY
#define HUGE_VALF ((float_t)INFINITY)
#define HUGE_VALL ((ldouble_t)INFINITY)

/**
 * These incorrectly return false with optimizations enabled (-ffast-math)
 * However, isnan(), isinf() and the like aren't to be used when optimizations
 * are enabled Because fast-math essentially says: "Infinities and NANs may not
 * occur, optimize as you wish."
 */
#ifndef __FAST_MATH__
#  define isnan(x) ((x) != (x))
#  define isinf(x) ((x) == (x) && (((x) + 1) == (x)))
#  define isposinf(x) ((x) == (1.0f / 0.0f))
#  define isneginf(x) ((x) == (-1.0f / 0.0f))
#elif (_RELIB_MATH_NAN_AND_INF_CHECKS_RETURN_FALSE)
#  define isnan(x) (false)
#  define isinf(x) (false)
#  define isposinf(x) (false)
#  define isneginf(x) (false)
#else
#  define isnan(x) _fast_math_is_enabled_
#  define isinf(x) _fast_math_is_enabled_
#  define isposinf(x) _fast_math_is_enabled_
#  define isneginf(x) _fast_math_is_enabled_
#endif

#define FP_INFINITE 1
#define FP_NAN 2
#define FP_NORMAL 3
#define FP_SUBNORMAL 4
#define FP_ZERO 5

#define fpclassify(x) (x == RE_TYPECAST(0.0, x) ? FP_ZERO : (isinf(x) ? FP_INFINITE : (isnan(x) ? FP_NAN : (isnormal(x) ? FP_NORMAL : FP_SUBNORMAL))))

#define isunordered(x, y) (isnan(x) || isnan(y))
#define isordered(x, y) (!isnan(x) && !isnan(y))

#define isnormalf(x) ((x) == (x) && ((x > FLT_MIN) || (x < !FLT_MAX)))
#define isnormal(x) ((x) == (x) && ((x > DBL_MIN) || (x < !DBL_MAX)))
#define isnormall(x) ((x) == (x) && ((x > LDBL_MIN) || (x < !LDBL_MAX)))

RE_HEADER_FN RE_CONST_FN int
_re_signbit(double_t x)
{
  return re_break_double(x).sign;
}

/**
 * The sign bit of a real floating type converted from a higher type should
 * most likely be the sign bit of the base type. If not, this macro is undefined.
 * However, most platforms support this behaviour, and there is no need to worry.
 */
#define signbit(x) (_re_signbit((double_t)x))

#pragma clang diagnostic push

/**
 * There are some static asserts in this header that expand to really long
 * checks. Clang sees these as VLA's but notices that their size may be deduced
 * at compile time Now, even though clang is correct here, compilers don't need
 * to compute the size and so its a GCC extension. But we need the assertations.
 */
#pragma clang diagnostic ignored "-Wgnu-folding-constant"

// RE_STATIC_ASSERT(invalid_platform_negative_zero_must_have_active_sign_bit,
// signbitl(-0.0) == 1);
// RE_STATIC_ASSERT(invalid_platform_negative_one_must_have_active_sign_bit,
// signbitl(-1.0) == 1);
// RE_STATIC_ASSERT(invalid_platform_positive_zero_must_have_inactive_sign_bit,
// signbitl(0.0) == 0);
// RE_STATIC_ASSERT(invalid_platform_positive_one_must_have_inactive_sign_bit,
// signbitl(1.0) == 0);

#pragma clang diagnostic pop

#define signf_fast(x) ((*(const unsigned int*)&(x) & 0x80000000u) ? -1.0f : 1.0f)
#define sign_fast(x) ((*(const unsigned long long*)&(x) & 0x8000000000000000ULL) ? -1.0 : 1.0)

RE_HEADER_FN RE_CONST_FN float_t
copysignf(float_t x, float_t y)
{
  return x * signf_fast(y);
}
RE_HEADER_FN RE_CONST_FN double_t
copysign(double_t x, double_t y)
{
  return x * sign_fast(y);
}
extern ldouble_t copysignl(ldouble_t x, ldouble_t y);

#define isgreater(x, y) ((x - RE_TYPECAST(y, x)) > RE_TYPECAST(0, x))
#define isgreaterequal(x, y) ((x - RE_TYPECAST(y, x)) >= RE_TYPECAST(0, x))
#define isless(x, y) ((x - RE_TYPECAST(y, x)) < RE_TYPECAST(0, x))
#define islessequal(x, y) ((x - RE_TYPECAST(y, x)) <= RE_TYPECAST(0, x))
#define islessgreater(x, y) (x != RE_TYPECAST(y, x)) // if it can only be lesser or greater, then it must not be equal

#define _RE_minmaxnancheck                                                                                                                                                                                                                               \
  if (RE_UNLIKELY(isnan(x))) return x;                                                                                                                                                                                                                   \
  else if (RE_UNLIKELY(isnan(y)))                                                                                                                                                                                                                        \
    return y;

RE_HEADER_FN RE_CONST_FN double_t
fmax(double_t x, double_t y)
{
  _RE_minmaxnancheck;
  return (x > y || x == y) ? x : y;
}
RE_HEADER_FN RE_CONST_FN float_t
fmaxf(float_t x, float_t y)
{
  _RE_minmaxnancheck;
  return (x > y || x == y) ? x : y;
}
RE_HEADER_FN RE_CONST_FN ldouble_t
fmaxl(ldouble_t x, ldouble_t y)
{
  _RE_minmaxnancheck;
  return (x > y || x == y) ? x : y;
}

RE_HEADER_FN RE_CONST_FN double_t
fmin(double_t x, double_t y)
{
  _RE_minmaxnancheck;
  return (x < y || x == y) ? x : y;
}
RE_HEADER_FN RE_CONST_FN float_t
fminf(float_t x, float_t y)
{
  _RE_minmaxnancheck;
  return (x < y || x == y) ? x : y;
}
RE_HEADER_FN RE_CONST_FN ldouble_t
fminl(ldouble_t x, ldouble_t y)
{
  _RE_minmaxnancheck;
  return (x < y || x == y) ? x : y;
}

RE_HEADER_FN RE_CONST_FN double_t
fabs(double_t x)
{
  return x * copysign(1.0, x);
}
RE_HEADER_FN RE_CONST_FN float_t
fabsf(float_t x)
{
  return x * copysignf(1.0F, x);
}
RE_HEADER_FN RE_CONST_FN ldouble_t
fabsl(ldouble_t x)
{
  return x * copysignl(1.0L, x);
}

RE_HEADER_FN RE_CONST_FN double_t
trunc(double_t x)
{
  if (isnan(x) || x == 0.0 || isinf(x)) { return x; }

  struct double_repr repr            = re_break_double(x);
  int                bits_to_discard = DBL_MANT_DIG - (repr.exponent + 1);
  repr.mantissa >>= bits_to_discard; // we shift all the mantissa bits to the right
  repr.mantissa <<= bits_to_discard; // shift back to the left, all bits after decimal point cleared.
  return re_make_double(repr);
}
RE_HEADER_FN RE_CONST_FN float_t
truncf(float_t x)
{
  if (isnan(x) || x == 0.0F || isinf(x)) { return x; }

  struct float_repr repr            = re_break_float(x);
  int               bits_to_discard = FLT_MANT_DIG - (repr.exponent + 1);
  repr.mantissa >>= bits_to_discard; // we shift all the mantissa bits to the right
  repr.mantissa <<= bits_to_discard; // shift back to the left, all bits after decimal point cleared.
  return re_make_float(repr);
}
RE_HEADER_FN RE_CONST_FN ldouble_t
truncl(ldouble_t x)
{
  if (isnan(x) || x == 0.0L || isinf(x)) { return x; }

  struct long_double_repr repr            = re_break_long_double(x);
  int                     bits_to_discard = LDBL_MANT_DIG - (repr.exponent + 1);
  repr.mantissa >>= bits_to_discard; // we shift all the mantissa bits to the right
  repr.mantissa <<= bits_to_discard; // shift back to the left, all bits after decimal point cleared.
  return re_make_long_double(repr);
}

RE_HEADER_FN RE_CONST_FN double_t
modf(double_t x, double_t* iptr)
{
  if (isnan(x))
  {
    *iptr = NAN;
    return x;
  }
  else if (isinf(x))
  {
    *iptr = copysign(INFINITY, x);
    return copysign(0.0, x);
  }

  double_t intpart = trunc(x);
  if (x < 0.0 && intpart > x) { intpart -= 1.0; }

  *iptr = intpart;
  return x - intpart;
}
RE_HEADER_FN RE_CONST_FN float_t
modff(float_t x, float_t* iptr)
{
  if (isnan(x))
  {
    *iptr = NAN;
    return x;
  }
  else if (isinf(x))
  {
    *iptr = copysignf(INFINITY, x);
    return copysignf(0.0F, x);
  }

  float_t intpart = trunc(x);
  if (x < 0.0 && intpart > x) { intpart -= 1.0; }

  *iptr = intpart;
  return x - intpart;
}
RE_HEADER_FN RE_CONST_FN ldouble_t
modfl(ldouble_t x, ldouble_t* iptr)
{
  if (isnan(x))
  {
    *iptr = NAN;
    return x;
  }
  else if (isinf(x))
  {
    *iptr = copysignl(INFINITY, x);
    return copysignl(0.0L, x);
  }

  ldouble_t intpart = trunc(x);
  if (x < 0.0 && intpart > x) { intpart -= 1.0; }

  *iptr = intpart;
  return x - intpart;
}

extern RE_CONST_FN double_t  fmod(double_t x, double_t by);
extern RE_CONST_FN float_t   fmodf(float_t x, float_t by);
extern RE_CONST_FN ldouble_t fmodl(ldouble_t x, ldouble_t by);

RE_HEADER_FN RE_CONST_FN double_t
floor(double_t x)
{
  if (isnan(x) || x == 0.0 || isinf(x)) { return x; }

  double_t intpart;
  (void)modf(x, &intpart);
  return (x < intpart) ? intpart - 1 : intpart;
}
RE_HEADER_FN RE_CONST_FN float_t
floorf(float_t x)
{
  if (isnan(x) || x == 0.0F || isinf(x)) { return x; }

  float_t intpart;
  (void)modff(x, &intpart);
  return (x < intpart) ? intpart - 1 : intpart;
}
RE_HEADER_FN RE_CONST_FN ldouble_t
floorl(ldouble_t x)
{
  if (isnan(x) || x == 0.0L || isinf(x)) { return x; }

  ldouble_t intpart;
  (void)modfl(x, &intpart);
  return (x < intpart) ? intpart - 1 : intpart;
}

RE_HEADER_FN RE_CONST_FN double_t
ceil(double_t x)
{
  if (isnan(x) || x == 0.0 || isinf(x)) { return x; }

  double_t xi = (double_t)(int64_t)x;
  return (x > xi) ? xi + 1 : xi;
}
RE_HEADER_FN RE_CONST_FN float_t
ceilf(float_t x)
{
  if (isnan(x) || x == 0.0F || isinf(x)) { return x; }

  float_t xi = (float_t)(int64_t)x;
  return (x > xi) ? xi + 1 : xi;
}
RE_HEADER_FN RE_CONST_FN ldouble_t
ceill(ldouble_t x)
{
  if (isnan(x) || x == 0.0L || isinf(x)) { return x; }

  ldouble_t xi = (ldouble_t)(int64_t)x;
  return (x > xi) ? xi + 1 : xi;
}

extern void _re_cordic(double_t xrads, double_t* RE_RESTRICT o_sin, double_t* RE_RESTRICT o_cos);
extern void _re_cordicf(float_t xrads, float_t* RE_RESTRICT o_sin, float_t* RE_RESTRICT o_cos);
extern void _re_cordicl(ldouble_t xrads, ldouble_t* RE_RESTRICT o_sin, ldouble_t* RE_RESTRICT o_cos);

extern void _re_cordic_circular(double_t x_in, double_t y_in, double_t* out_magnitude, double_t* out_angle);
extern void _re_cordic_circularf(float_t x_in, float_t y_in, float_t* out_magnitude, float_t* out_angle);
extern void _re_cordic_circularl(ldouble_t x_in, ldouble_t y_in, ldouble_t* out_magnitude, ldouble_t* out_angle);

RE_HEADER_FN RE_CONST_FN double_t
sin(double_t xrads)
{
  double_t sin_ = 0.0;
  _re_cordic(xrads, &sin_, NULL);
  return sin_;
}
RE_HEADER_FN RE_CONST_FN float_t
sinf(float_t xrads)
{
  float_t sin_ = 0.0F;
  _re_cordicf(xrads, &sin_, NULL);
  return sin_;
}
RE_HEADER_FN RE_CONST_FN ldouble_t
sinl(ldouble_t xrads)
{
  ldouble_t sin_ = 0.0L;
  _re_cordicl(xrads, &sin_, NULL);
  return sin_;
}

RE_HEADER_FN RE_CONST_FN double_t
cos(double_t xrads)
{
  double_t cos_ = 0.0;
  _re_cordic(xrads, NULL, &cos_);
  return cos_;
}
RE_HEADER_FN RE_CONST_FN float_t
cosf(float_t xrads)
{
  float_t cos_ = 0.0F;
  _re_cordicf(xrads, NULL, &cos_);
  return cos_;
}
RE_HEADER_FN RE_CONST_FN ldouble_t
cosl(ldouble_t xrads)
{
  ldouble_t cos_ = 0.0L;
  _re_cordicl(xrads, NULL, &cos_);
  return cos_;
}

extern RE_CONST_FN double_t  tan(double_t xrads);
extern RE_CONST_FN float_t   tanf(float_t xrads);
extern RE_CONST_FN ldouble_t tanl(ldouble_t xrads);

/**
 * Calculates e ^ x
 */
extern double_t  exp(double_t x);
extern float_t   expf(float_t x);
extern ldouble_t expl(ldouble_t x);

/**
 * Calculate x ^ y
 */
extern double_t  pow(double_t base, double_t exp);
extern float_t   powf(float_t base, float_t exp);
extern ldouble_t powl(ldouble_t base, ldouble_t exp);

/**
 * Calculate x * (2 ^ exp)
 * This decomposes the number into exponent and mantissa, and adds the exponent.
 */
extern double_t  ldexp(double_t x, int exp);
extern float_t   ldexpf(float_t x, int exp);
extern ldouble_t ldexpl(ldouble_t x, int exp);

/**
 * The log base e (natural logarithm)!
 * This is not the log base 10! Use log10 for that
 */
extern double_t  log(double_t x);
extern float_t   logf(float_t x);
extern ldouble_t logl(ldouble_t x);

extern double_t  log10(double_t x);
extern float_t   log10f(float_t x);
extern ldouble_t log10l(ldouble_t x);

extern double_t  log2(double_t x);
extern float_t   log2f(float_t x);
extern ldouble_t log2l(ldouble_t x);

extern double_t  frexp(double_t num, int* exp);
extern float_t   frexpf(float_t num, int* exp);
extern ldouble_t frexpl(ldouble_t num, int* exp);

#endif //_RELIBC_MATH_H
