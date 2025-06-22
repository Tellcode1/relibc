#include "../../include/math.h"
#include "../../include/fenv.h"

 RE_ATTR_CONST double_t
fmod(double_t x, double_t by)
{
    if (RE_UNLIKELY(isnan(x))) { return x; }
    if (RE_UNLIKELY(isnan(by))) { return by; }
    if (RE_UNLIKELY(by == 0.0))
    {
        feraiseexcept(FE_DIVBYZERO);
        return NAN;
    }

    return x - trunc(x / by) * by;
}

 RE_ATTR_CONST float_t
fmodf(float_t x, float_t by)
{
    if (RE_UNLIKELY(isnan(x))) { return x; }
    if (RE_UNLIKELY(isnan(by))) { return by; }
    if (RE_UNLIKELY(by == 0.0))
    {
        feraiseexcept(FE_DIVBYZERO);
        return NAN;
    }

    return x - truncf(x / by) * by;
}

 RE_ATTR_CONST ldouble_t
fmodl(ldouble_t x, ldouble_t by)
{
    if (RE_UNLIKELY(isnan(x))) { return x; }
    if (RE_UNLIKELY(isnan(by))) { return by; }
    if (RE_UNLIKELY(by == 0.0))
    {
        feraiseexcept(FE_DIVBYZERO);
        return NAN;
    }

    return x - truncl(x / by) * by;
}
