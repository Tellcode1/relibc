#include "math.h"
#include <stdio.h>

#define TESTFN(fn, ...) printf(#fn "(%s) = %f\n", #__VA_ARGS__, fn(__VA_ARGS__))

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

  return 0;
}