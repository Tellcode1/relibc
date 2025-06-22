#include "../../include/math.h"
#include "../../include/fenv.h"

double_t
sqrt(double_t x)
{
  if (RE_UNLIKELY(x < 0.0))
  {
    feraiseexcept(FE_INVALID);
    return x;
  }
  else if (RE_UNLIKELY(x == 0.0)) { return x; }
  /**
   * Exploit the property that sqrt(a * x) = sqrt a * sqrt x
   * Chop x in half until it is in a suitable range, and then perform
   * newton-raphson on it. Then, undo the range reduction by multiplying
   * repeatedly by the log base 10 of 2.
   */
  int    exponents_reduced = 0;
  double range_reduced_x   = frexp(x, &exponents_reduced) * 2.0;
  exponents_reduced--;

  double y = range_reduced_x;
  for (int i = 0; i < 16; i++)
  {
    y = 0.5 * (y + (range_reduced_x / y));
  }

  y *= exp2(exponents_reduced * 0.5);

  return y;
}

float_t
sqrtf(float_t x)
{
  if (RE_UNLIKELY(x < 0.0F))
  {
    feraiseexcept(FE_INVALID);
    return x;
  }
  else if (RE_UNLIKELY(x == 0.0F)) { return x; }

  int   exponents_reduced = 0;
  float range_reduced_x   = frexpf(x, &exponents_reduced) * 2.0;
  exponents_reduced--;

  float y = range_reduced_x;
  for (int i = 0; i < 8; i++)
  {
    y = 0.5F * (y + (range_reduced_x / y));
  }

  y *= exp2f(exponents_reduced * 0.5F);

  return y;
}

ldouble_t
sqrtl(ldouble_t x)
{
  if (RE_UNLIKELY(x < 0.0L))
  {
    feraiseexcept(FE_INVALID);
    return x;
  }
  else if (RE_UNLIKELY(x == 0.0L)) { return x; }

  int         exponents_reduced = 0;
  long double range_reduced_x   = frexpl(x, &exponents_reduced) * 2.0;
  exponents_reduced--;

  long double y = range_reduced_x;
  for (int i = 0; i < 32; i++)
  {
    y = 0.5L * (y + (range_reduced_x / y));
  }

  y *= exp2l(exponents_reduced * 0.5L);

  return y;
}
