#include "../../include/math.h"
#include "../../include/fenv.h"

static inline double_t
integral_pow(double_t base, int pow)
{
    const double_t orig_base = base;
    for (int i = 1; i < pow; i++)
    {
        base *= orig_base;
    }
    return base;
}

static inline float_t
integral_powf(float_t base, int pow)
{
    const float_t orig_base = base;
    for (int i = 1; i < pow; i++)
    {
        base *= orig_base;
    }
    return base;
}

static inline ldouble_t
integral_powl(ldouble_t base, int pow)
{
    const ldouble_t orig_base = base;
    for (int i = 1; i < pow; i++)
    {
        base *= orig_base;
    }
    return base;
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
    float_t fractional_mantissa = frexpf(x, &exponent);
    fractional_mantissa *= 2.0F;
    exponent--;

    float_t z         = (fractional_mantissa - 1) / (fractional_mantissa + 1);
    float_t z_squared = z * z;

    const int iterations = 4;

    float_t result = 1.0 / (float_t)(2 * iterations + 1);
    for (int i = iterations - 1; i >= 0; i--)
    {
        result = (1.0F / (float_t)((2 * i) + 1)) + z_squared * result;
    }

    float_t ln_fractional_mantissa = 2.0F * z * result;
    return ln_fractional_mantissa + (float_t)exponent * M_LN2;
}

ldouble_t
logl(ldouble_t x)
{
    if (RE_UNLIKELY(x < 0.0F)) { return NAN; }
    else if (RE_UNLIKELY(x == 0.0F)) { return -INFINITY; }
    else if (RE_UNLIKELY(isinf(x))) { return copysignl(INFINITY, x); }

    int       exponent            = 0;
    ldouble_t fractional_mantissa = frexpl(x, &exponent);
    fractional_mantissa *= 2.0F;
    exponent--;

    ldouble_t z         = (fractional_mantissa - 1) / (fractional_mantissa + 1);
    ldouble_t z_squared = z * z;

    const int iterations = 16;

    ldouble_t result = 1.0 / (ldouble_t)(2 * iterations + 1);
    for (int i = iterations - 1; i >= 0; i--)
    {
        result = (1.0F / (ldouble_t)((2 * i) + 1)) + z_squared * result;
    }

    ldouble_t ln_fractional_mantissa = 2.0F * z * result;
    return ln_fractional_mantissa + (ldouble_t)exponent * M_LN2l;
}

double_t
log10(double_t x)
{
    if (RE_UNLIKELY(x <= 0.0))
    {
        feraiseexcept(FE_DIVBYZERO);
        return copysign(NAN, x);
    }

    int    exponent        = 0;
    double range_reduced_x = frexp(x, &exponent);

    const int iterations = 8;

    // gazillions of times faster, gazillions of times less accurate
    // such is the way of life.
    // double pade_approx = (y * (6.0 + y)) / (6.0 + 4.0 * y);
    // ln_x = pade_approx;

    double x_minus_one = range_reduced_x - 1.0;
    double ln_m        = 0.0;
    for (int i = 1; i <= iterations; i++)
    {
        double sign = ((i % 2) == 0) ? -1.0 : 1.0;
        ln_m += sign * (integral_pow(x_minus_one, i) / (double)i);
    }

    double result = (ln_m / M_LN10) + (exponent * M_LOG102);
    return result;
}

float_t
log10f(float_t x)
{
    if (RE_UNLIKELY(x <= 0.0F))
    {
        feraiseexcept(FE_DIVBYZERO);
        return copysignf(NAN, x);
    }

    int   exponent        = 0;
    float range_reduced_x = frexpf(x, &exponent);

    const int iterations = 4;

    float x_minus_one = range_reduced_x - 1.0F;
    float ln_m        = 0.0F;
    for (int i = 1; i <= iterations; i++)
    {
        float sign = ((i % 2) == 0) ? -1.0F : 1.0F;
        ln_m += sign * (integral_powf(x_minus_one, i) / (float)i);
    }

    float result = (ln_m / M_LN10) + ((float)exponent * M_LOG102);
    return result;
}

ldouble_t
log10l(ldouble_t x)
{
    if (RE_UNLIKELY(x <= 0.0L))
    {
        feraiseexcept(FE_DIVBYZERO);
        return copysign(NAN, x);
    }

    int         exponent        = 0;
    long double range_reduced_x = frexpl(x, &exponent);

    const int iterations = 16;

    long double x_minus_one = range_reduced_x - 1.0L;
    long double ln_m        = 0.0L;
    for (int i = 1; i <= iterations; i++)
    {
        long double sign = ((i % 2) == 0) ? -1.0L : 1.0L;
        ln_m += sign * (integral_powl(x_minus_one, i) / (long double)i);
    }

    long double result = (ln_m / M_LN10l) + (exponent * M_LOG102l);
    return result;
}

double_t
log2(double_t x)
{
    if (RE_UNLIKELY(x <= 0.0))
    {
        feraiseexcept(FE_DIVBYZERO);
        return copysign(NAN, x);
    }

    int    exponent        = 0;
    double range_reduced_x = frexp(x, &exponent);
    range_reduced_x *= 2.0; // bring frexp into the range [1.0, 2.0]
    // the taylor series suffers in accuracy when y (x-1) is far from 1
    // which x-1 nearly would always would be raw
    exponent--; // we don't want to modify the source float, just our representation

    const int iterations = 8;

    double x_minus_one = range_reduced_x - 1.0;
    double ln_m        = 0.0;
    for (int i = 1; i <= iterations; i++)
    {
        double sign = ((i % 2) == 0) ? -1.0 : 1.0;
        ln_m += sign * (integral_pow(x_minus_one, i) / (double)i);
    }

    double result = (ln_m / M_LN2) + (exponent);
    return result;
}

float_t
log2f(float_t x)
{
    if (RE_UNLIKELY(x <= 0.0F))
    {
        feraiseexcept(FE_DIVBYZERO);
        return copysignf(NAN, x);
    }

    int   exponent        = 0;
    float range_reduced_x = frexpf(x, &exponent);
    range_reduced_x *= 2.0F;
    exponent--;

    const int iterations = 4;

    float x_minus_one = range_reduced_x - 1.0F;
    float ln_m        = 0.0F;
    for (int i = 1; i <= iterations; i++)
    {
        float sign = ((i % 2) == 0) ? -1.0F : 1.0F;
        ln_m += sign * (integral_powf(x_minus_one, i) / (float)i);
    }

    float result = (ln_m / M_LN2) + (float)exponent;
    return result;
}

ldouble_t
log2l(ldouble_t x)
{
    if (RE_UNLIKELY(x <= 0.0L))
    {
        feraiseexcept(FE_DIVBYZERO);
        return copysign(NAN, x);
    }

    int         exponent        = 0;
    long double range_reduced_x = frexpl(x, &exponent);
    range_reduced_x *= 2.0L;
    exponent--;

    const int iterations = 16;

    long double x_minus_one = range_reduced_x - 1.0L;
    long double ln_m        = 0.0L;
    for (int i = 1; i <= iterations; i++)
    {
        long double sign = ((i % 2) == 0) ? -1.0L : 1.0L;
        ln_m += sign * (integral_powl(x_minus_one, i) / (long double)i);
    }

    long double result = (ln_m / M_LN2l) + exponent;
    return result;
}

