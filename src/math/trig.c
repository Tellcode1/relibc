#include "../../include/math.h"
#include "../../include/fenv.h"
#include "../../.cordic_atan_table.h"

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

static inline double_t
_wrap(double_t value, double_t min, double_t max)
{
    double_t range   = max - min;
    double_t wrapped = fmod(value - min, range);
    if (wrapped < 0.0) { wrapped += range; }
    return wrapped + min;
}

static inline float_t
_wrapf(float_t value, float_t min, float_t max)
{
    float_t range   = max - min;
    float_t wrapped = fmodf(value - min, range);
    if (wrapped < 0.0) { wrapped += range; }
    return wrapped + min;
}

static inline ldouble_t
_wrapl(ldouble_t value, ldouble_t min, ldouble_t max)
{
    ldouble_t range   = max - min;
    ldouble_t wrapped = fmodl(value - min, range);
    if (wrapped < 0.0) { wrapped += range; }
    return wrapped + min;
}

void
_re_cordic(double_t xrads, double_t* RE_RESTRICT o_sin, double_t* RE_RESTRICT o_cos)
{
    xrads = _wrap(xrads, -M_PI, M_PI);

    bool negate_sin = false, negate_cos = false;

    if (xrads < -M_PI_2l)
    {
        xrads += M_PI;
        negate_sin = true;
        negate_cos = true;
    }
    else if (xrads > M_PI_2l)
    {
        xrads -= M_PI;
        negate_sin = true;
        negate_cos = true;
    }

    double_t x = 1.0;
    double_t y = 0.0;
    double_t z = xrads;

    for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; i++)
    {
        double_t di     = copysign(1.0, z);
        double_t factor = 1.0 / (double_t)(1ULL << i);

        double_t x_new = x - di * y * factor;
        double_t y_new = y + di * x * factor;
        double_t z_new = z - di * (double_t)_cordic_atan_table[i];

        x = x_new;
        y = y_new;
        z = z_new;
    }

    x *= (double_t)_CORDIC_GAIN;
    y *= (double_t)_CORDIC_GAIN;

    if (negate_cos) x = -x;
    if (negate_sin) y = -y;

    if (o_cos) *o_cos = x;
    if (o_sin) *o_sin = y;
}

void
_re_cordicf(float_t xrads, float_t* RE_RESTRICT o_sin, float_t* RE_RESTRICT o_cos)
{
    xrads = _wrapf(xrads, -(float_t)M_PI, (float_t)M_PI);

    bool negate_sin = false, negate_cos = false;

    if (xrads < -M_PI_2l)
    {
        xrads += (float_t)M_PI;
        negate_sin = true;
        negate_cos = true;
    }
    else if (xrads > M_PI_2l)
    {
        xrads -= (float_t)M_PI;
        negate_sin = true;
        negate_cos = true;
    }

    float_t x = 1.0F;
    float_t y = 0.0F;
    float_t z = xrads;

    for (unsigned long i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; i++)
    {
        float_t di     = copysignf(1.0F, z);
        float_t factor = 1.0F / (float_t)(1ULL << i);

        float_t x_new = x - di * y * factor;
        float_t y_new = y + di * x * factor;
        float_t z_new = z - di * (float_t)_cordic_atan_table[i];

        x = x_new;
        y = y_new;
        z = z_new;
    }

    x *= (float_t)_CORDIC_GAIN;
    y *= (float_t)_CORDIC_GAIN;

    if (negate_cos) x = -x;
    if (negate_sin) y = -y;

    if (o_cos) *o_cos = x;
    if (o_sin) *o_sin = y;
}

void
_re_cordicl(ldouble_t xrads, ldouble_t* RE_RESTRICT o_sin, ldouble_t* RE_RESTRICT o_cos)
{
    xrads = _wrapl(xrads, -M_PI_2l, M_PI_2l);

    bool negate_sin = false, negate_cos = false;

    if (xrads < -M_PI_2l)
    {
        xrads += M_PIl;
        negate_sin = true;
        negate_cos = true;
    }
    else if (xrads > M_PI_2l)
    {
        xrads -= M_PIl;
        negate_sin = true;
        negate_cos = true;
    }

    ldouble_t x = 1.0L;
    ldouble_t y = 0.0L;
    ldouble_t z = xrads;

    for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; i++)
    {
        ldouble_t di     = copysignl(1.0L, z);
        ldouble_t factor = 1.0L / (1ULL << i);

        ldouble_t x_new = x - di * y * factor;
        ldouble_t y_new = y + di * x * factor;
        ldouble_t z_new = z - di * _cordic_atan_table[i];

        x = x_new;
        y = y_new;
        z = z_new;
    }

    x *= _CORDIC_GAIN;
    y *= _CORDIC_GAIN;

    if (negate_cos) x = -x;
    if (negate_sin) y = -y;

    if (o_cos) *o_cos = x;
    if (o_sin) *o_sin = y;
}

void
_re_cordic_circular(double_t x_in, double_t y_in, double_t* out_magnitude, double_t* out_angle)
{
    double_t x = x_in;
    double_t y = y_in;
    double_t z = 0.0;

    for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; ++i)
    {
        double_t di = copysign(1.0, y);

        double_t x_new = x + di * y * (1.0 / (double_t)(1 << i));
        double_t y_new = y - di * x * (1.0 / (double_t)(1 << i));
        double_t z_new = z + di * (double_t)_cordic_atan_table[i];

        x = x_new;
        y = y_new;
        z = z_new;
    }

    *out_magnitude = x * (double_t)_CORDIC_GAIN; // multiply final x by gain to undo CORDIC scaling
    if (out_angle) *out_angle = z;
}

void
_re_cordic_circularf(float_t x_in, float_t y_in, float_t* out_magnitude, float_t* out_angle)
{
    float_t x = x_in;
    float_t y = y_in;
    float_t z = 0.0F;

    for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; ++i)
    {
        float_t di = copysignf(1.0F, y);

        float_t x_new = x + di * y * (1.0F / (float_t)(1 << i));
        float_t y_new = y - di * x * (1.0F / (float_t)(1 << i));
        float_t z_new = z + di * (float_t)_cordic_atan_table[i];

        x = x_new;
        y = y_new;
        z = z_new;
    }

    *out_magnitude = x * (float_t)_CORDIC_GAIN; // multiply final x by gain to undo CORDIC scaling
    if (out_angle) *out_angle = z;
}

void
_re_cordic_circularl(ldouble_t x_in, ldouble_t y_in, ldouble_t* out_magnitude, ldouble_t* out_angle)
{
    ldouble_t x = x_in;
    ldouble_t y = y_in;
    ldouble_t z = 0.0;

    for (int i = 0; i < _CORDIC_ATAN_TABLE_ENTRIES; ++i)
    {
        ldouble_t di = copysignl(1.0L, y);

        ldouble_t x_new = x + di * y * (1.0L / (1 << i));
        ldouble_t y_new = y - di * x * (1.0L / (1 << i));
        ldouble_t z_new = z + di * _cordic_atan_table[i];

        x = x_new;
        y = y_new;
        z = z_new;
    }

    *out_magnitude = x * _CORDIC_GAIN; // multiply final x by gain to undo CORDIC scaling
    if (out_angle) { *out_angle = z; }
}

#define COS_LOW_THRESHOLD 1e-3

double_t
tan(double_t xrads)
{
    double_t sin_ = 0.0, cos_ = 0.0;
    _re_cordic(xrads, &sin_, &cos_);

    if (fabs(cos_) <= COS_LOW_THRESHOLD)
    {
        feraiseexcept(FE_INVALID);
        return INFINITY;
    }

    return sin_ / cos_;
}

float_t
tanf(float_t xrads)
{
    float_t sin_ = 0.0F, cos_ = 0.0F;
    _re_cordicf(xrads, &sin_, &cos_);

    if (fabsf(cos_) < (float_t)COS_LOW_THRESHOLD)
    {
        feraiseexcept(FE_INVALID);
        return INFINITY;
    }

    return sin_ / cos_;
}

ldouble_t
tanl(ldouble_t xrads)
{
    ldouble_t sin_ = 0.0L, cos_ = 0.0L;
    _re_cordicl(xrads, &sin_, &cos_);

    if (fabsl(cos_) < COS_LOW_THRESHOLD)
    {
        feraiseexcept(FE_INVALID);
        return INFINITY;
    }

    return sin_ / cos_;
}
