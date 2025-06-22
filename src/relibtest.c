#include "../include/floatfmt.h"
#include "../include/math.h"
#include "../include/string.h"
#include <stdio.h>

#define TESTFN(fn, ...) printf(#fn "(%s) = %f\n", #__VA_ARGS__, fn(__VA_ARGS__))

static inline void
decompose(double_t f)
{
  struct double_repr repr = re_break_double(f);
  uint64_t hex = 0;
  memcpy(&hex, &f, sizeof(f));
  printf("%f = -1^%i * %f * 2^%i\n", f, repr.sign, repr.mantissa / (double)(1ULL << (DBL_MANT_DIG - 1)), repr.exponent);
}

int
main(void)
{
  TESTFN(sqrt, 1);
  TESTFN(sqrt, 4);
  TESTFN(sqrt, 2);
  TESTFN(sqrt, 3);
  TESTFN(sqrt, 3 * 3);
  TESTFN(sqrt, 1.7976931348623157e+308);
  TESTFN(sqrt, 1e-45);

   TESTFN(floor, 1.1);
   TESTFN(floor, 1.0);
   TESTFN(floor, 1.9);

  TESTFN(trunc, 1.1);
  TESTFN(trunc, 1.0);
  TESTFN(trunc, 1/3.0);
  TESTFN(trunc, 324.34234);


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
  TESTFN(log, M_E);
  TESTFN(log, M_E * M_E);
  TESTFN(log, INFINITY);
  TESTFN(log2, 2.0);
  TESTFN(log2, 16384.0);
  TESTFN(log2, (double)(1 << 24));

  TESTFN(log10, 10.0);
  TESTFN(log10, 1000.0);
  TESTFN(log10, 36.0);
  TESTFN(ldexp, 1, 16384);
  TESTFN(ldexp, 1, -16384);

  TESTFN(exp, 1.0);
  TESTFN(exp, 2.0);
  TESTFN(exp, 10.0);

  decompose(ldexpl(1, 1));
  decompose(360.3);
  decompose(1.0 / 3.0);

  decompose(-3.3);

  return 0;
}
