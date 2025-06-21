#include "../include/floatfmt.h"
#include "../include/math.h"
#include "../include/string.h"
#include <stdio.h>

#define TESTFN(fn, ...) printf(#fn "(%s) = %f\n", #__VA_ARGS__, fn(__VA_ARGS__))

static inline void
decompose(double_t f)
{
  struct double_repr repr = re_break_double(f);
  printf("%f = -1^%i * %f * 2^%i\n", f,repr.sign, repr.mantissa / (double)(1ULL << (DBL_MANT_DIG-1)), repr.exponent);
}

int
main(void)
{
  TESTFN(floor, 1.1);
  TESTFN(floor, 1.0);
  TESTFN(floor, 1.9);

  TESTFN(ceil, 1.0);
  TESTFN(ceil, 1.1);
  TESTFN(ceil, 1.5);

  TESTFN(sin, M_PI);
  TESTFN(sin, M_PI_2);
  TESTFN(sin, 0);

  TESTFN(cos, M_PI);
  TESTFN(cos, M_PI_2);
  TESTFN(cos, 0);

  TESTFN(tan, M_PI);
  TESTFN(tan, M_PI_2);
  TESTFN(tan, 0);

  TESTFN(ldexp, 2.0, 3);
  TESTFN(ldexp, 3, 2);
  TESTFN(ldexp, 1, 100);

  TESTFN(log, 0.0);
  TESTFN(log, INFINITY);
  TESTFN(log2, 2.0);
  TESTFN(log2, 16384.0);
  TESTFN(log2, (double)(1 << 24));

  TESTFN(log10, 10.0);
  TESTFN(log10, 1000.0);
  TESTFN(log10, 36.0);
  TESTFN(ldexp, 1, 16384);

  decompose(ldexpl(1, 1));
  decompose(360.3);
  decompose(1.0 / 3.0);
  decompose(INFINITY);

  decompose(-3.3);

  return 0;
}
