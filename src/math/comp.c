#include "../../include/math.h"
#include "../../include/fenv.h"

double_t
ldexp(double_t x, int exp)
{
    if (RE_UNLIKELY(isnan(x) || x == 0.0 || isinf(x) || exp == 0)) { return x; }

    struct double_repr repr = re_break_double(x);
    if (RE_UNLIKELY(((int)repr.exponent + exp) >= DBL_MAX_EXP))
    {
        feraiseexcept(FE_OVERFLOW);
        return HUGE_VAL;
    }
    else if (RE_UNLIKELY(((int)repr.exponent + exp) < DBL_MIN_EXP))
    {
        feraiseexcept(FE_UNDERFLOW);
        return copysign(0.0, x);
    }
    repr.exponent += exp;
    return re_make_double(repr);
}

float_t
ldexpf(float_t x, int exp)
{
    if (RE_UNLIKELY(isnan(x) || x == 0.0F || isinf(x) || exp == 0)) { return x; }

    struct float_repr repr = re_break_float(x);
    if (RE_UNLIKELY(((int)repr.exponent + exp) >= FLT_MAX_EXP))
    {
        feraiseexcept(FE_OVERFLOW);
        return HUGE_VALF;
    }
    else if (RE_UNLIKELY(((int)repr.exponent + exp) < FLT_MIN_EXP))
    {
        feraiseexcept(FE_UNDERFLOW);
        return copysignf(0.0F, x);
    }
    repr.exponent += exp;
    return re_make_float(repr);
}

ldouble_t
ldexpl(ldouble_t x, int exp)
{
    if (RE_UNLIKELY(isnan(x) || x == 0.0L || isinf(x) || exp == 0)) { return x; }

    struct long_double_repr repr = re_break_long_double(x);
    if (RE_UNLIKELY(((int)repr.exponent + exp) >= LDBL_MAX_EXP))
    {
        feraiseexcept(FE_OVERFLOW);
        return HUGE_VALL;
    }
    else if (RE_UNLIKELY(((int)repr.exponent + exp) < LDBL_MIN_EXP))
    {
        feraiseexcept(FE_UNDERFLOW);
        return copysignl(0.0L, x);
    }
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

    repr.exponent = -1; // -1 exponent = 0.5 ≤ m < 1.0
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

    repr.exponent = -1;
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

    repr.exponent = -1;
    return re_make_long_double(repr);
}

