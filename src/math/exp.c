
#include "../../include/math.h"
#include "../../include/fenv.h"

double_t
exp(double_t x)
{
    if (RE_UNLIKELY(x < -12.0)) { return 0.0L; }

    // The taylor series falls off for x ~< -2
    // We repeatedly halve x to reduce the input and later undo this
    // this is called range reduction btw. It's done a lot here, we need to generalize the inputs yknow.
    int    exponent        = 0;
    double range_reduced_x = frexp(x, &exponent);

    // Execute the taylor series on reduced x
    double term = 1.0;
    double sum  = 1.0;
    for (int i = 1; i <= 32; i++)
    {
        term *= range_reduced_x / (double)i;
        sum += term;
    }

    // undo the previous range reduction
    for (int i = 0; i < exponent; i++)
    {
        sum *= sum;
    }

    return sum;
}

float_t
expf(float_t x)
{
    if (RE_UNLIKELY(x < -12.0)) { return 0.0L; }

    int    exponent        = 0;
    double range_reduced_x = frexp(x, &exponent);

    double term = 1.0;
    double sum  = 1.0;
    for (int i = 1; i <= 32; i++)
    {
        term *= range_reduced_x / (double)i;
        sum += term;
    }

    for (int i = 0; i < exponent; i++)
    {
        sum *= sum;
    }

    return sum;
}

ldouble_t
expl(ldouble_t x)
{
    if (RE_UNLIKELY(x < -12.0)) { return 0.0L; }

    int       reduced_exponent = 0;
    ldouble_t reduced_x        = x;
    while (reduced_x < -1.0L)
    {
        reduced_x *= 0.5L;
        reduced_exponent++;
    }

    ldouble_t term = 1.0L;
    ldouble_t sum  = 1.0L;
    for (int i = 1; i <= 64; i++)
    {
        term *= reduced_x / i;
        sum += term;
    }

    for (int i = 0; i < reduced_exponent; i++)
    {
        sum *= sum;
    }

    return sum;
}

double
exp2(double power)
{
    if (RE_UNLIKELY(power >= DBL_MAX_EXP))
    {
        feraiseexcept(FE_OVERFLOW);
        return HUGE_VAL;
    }
    else if (RE_UNLIKELY(power < DBL_MIN_EXP)) { return 0.0; }

    /**
     * We modf the input power into its integral and fractional constituents.
     * Then, compute the 2 ^ fractional_exponent using a taylor polynomial which does best
     * around x ~ 1.0 which is perfect for us.
     * Load the integral exponent onto the computed value
     */
    double integral_exponent   = 0;
    double fractional_exponent = modf(power, &integral_exponent);

    double y = fractional_exponent * M_LN2;

    const int iterations = 16;

    double term            = 1.0;
    double fractional_exp2 = 1.0;
    for (int i = 1; i <= iterations; i++)
    {
        term *= y / i;
        fractional_exp2 += term;
    }

    return ldexp(fractional_exp2, integral_exponent);
}

float
exp2f(float power)
{
    if (RE_UNLIKELY(power >= FLT_MAX_EXP))
    {
        feraiseexcept(FE_OVERFLOW);
        return HUGE_VALF;
    }
    else if (RE_UNLIKELY(power < FLT_MIN_EXP)) { return 0.0; }

    float integral_exponent   = 0;
    float fractional_exponent = modff(power, &integral_exponent);

    float y = fractional_exponent * M_LN2;

    const int iterations = 8;

    float term            = 1.0F;
    float fractional_exp2 = 1.0F;
    for (int i = 1; i <= iterations; i++)
    {
        term *= y / i;
        fractional_exp2 += term;
    }

    return ldexpf(fractional_exp2, integral_exponent);
}

long double
exp2l(long double power)
{
    if (RE_UNLIKELY(power >= LDBL_MAX_EXP))
    {
        feraiseexcept(FE_OVERFLOW);
        return HUGE_VALL;
    }
    else if (RE_UNLIKELY(power < LDBL_MIN_EXP)) { return 0.0; }

    long double integral_exponent   = 0.0L;
    long double fractional_exponent = modfl(power, &integral_exponent);

    long double y = fractional_exponent * M_LN2l;

    const int iterations = 32;

    long double term            = 1.0L;
    long double fractional_exp2 = 1.0L;
    for (int i = 1; i <= iterations; i++)
    {
        term *= y / i;
        fractional_exp2 += term;
    }

    return ldexpl(fractional_exp2, integral_exponent);
}
