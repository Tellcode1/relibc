#include "../../include/math.h"
#include "../../include/fenv.h"

double_t
hypot(double_t x, double_t y)
{
    if (isinf(x) || isinf(y)) return INFINITY;
    if (isnan(x) || isnan(y)) return NAN;

    // Negative values make this function cry. We don't need em anyway
    x = fabs(x);
    y = fabs(y);

    // Store the larger value in x and the smaller value in y
    // The reason we do this is pretty much for accuracy conservation.
    if (x < y)
    {
        double swap = x;
        x           = y;
        y           = swap;
    }

    /**
     * What we do here is like pretty simple.
     * We take the bigger value, unload its exponent into the smaller value
     * which gives us better precision for wildly ranging inputs.
     */

    // load x's exponent and normalize it
    int x_exponent = 0;
    x              = frexp(x, &x_exponent);

    // subtract y's exponent with x's exponent.
    y = ldexp(y, -x_exponent);

    // Take the square root of normalized x, y with x's exponent subtracted from it squared and multiplied
    double root = sqrt(x * x + y * y);

    // Load x's exponent back in
    // This undos the exponent subtracting from y we did earlier.
    return ldexp(root, x_exponent);
}

float_t
hypotf(float_t x, float_t y)
{
    if (isinf(x) || isinf(y)) return INFINITY;
    if (isnan(x) || isnan(y)) return NAN;

    x = fabsf(x);
    y = fabsf(y);

    if (x < y)
    {
        float swap = x;
        x          = y;
        y          = swap;
    }

    int x_exponent = 0;
    x              = frexpf(x, &x_exponent);
    y              = ldexpf(y, -x_exponent);

    float root = sqrtf(x * x + y * y);
    return ldexpf(root, x_exponent);
}

ldouble_t
hypotl(ldouble_t x, ldouble_t y)
{
    if (isinf(x) || isinf(y)) return INFINITY;
    if (isnan(x) || isnan(y)) return NAN;

    x = fabsl(x);
    y = fabsl(y);

    if (x < y)
    {
        long double swap = x;
        x                = y;
        y                = swap;
    }

    int x_exponent = 0;
    x              = frexpl(x, &x_exponent);
    y              = ldexpl(y, -x_exponent);

    long double root = sqrtl(x * x + y * y);
    return ldexpl(root, x_exponent);
}
