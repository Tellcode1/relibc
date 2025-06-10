#include "math.h"
#include ".cordic_atan_table.h"

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

static inline double_t
wrap_radians_from_neg_pi_to_pos_pi(double_t xrads)
{
  if (xrads < -3.14159265) { xrads += 6.28318531; }
  else if (xrads > 3.14159265) { xrads -= 6.28318531; }
  return xrads;
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

void
_re_cordic(double_t xrads, double_t* RE_RESTRICT o_sin, double_t* RE_RESTRICT o_cos)
{
}

double_t
sin(double_t xrads)
{
  const double_t orig_xrads        = xrads;
  const double_t taylor_iterations = 16.0; // keep the iteration count in a floating point register
  for (double_t i = 0.0; i < taylor_iterations; i += 1.0) {}
}

float_t
sinf(float_t xrads)
{
}

ldouble_t
sinl(ldouble_t xrads)
{
}
