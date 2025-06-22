#include "../../include/math.h"
#include "../../include/fenv.h"

fenv_t _re_fe_curr_env = _re_def_fe_env;



int
_re_mask_fp_excepts(int excepts, int mask_or_unmask)
{
    #if defined(__STDC_IEC_559__)

    #  if defined(__x86_64__) || defined(__i386__)
    RE_ATTR_ALIGNED(16) uint16_t cw    = 0;
    RE_ATTR_ALIGNED(16) uint32_t mxcsr = 0;

    __asm__ volatile("fnstcw %0" : "=m"(cw));
    __asm__ volatile("stmxcsr %0" : "=m"(mxcsr));

    if (mask_or_unmask == FE_MASK_UNMASK) { cw &= ~(uint16_t)(excepts); }
    else { cw |= (uint16_t)(excepts); }

    uint32_t mxcsr_mask_bits = ((uint32_t)excepts) << 7;
    if (mask_or_unmask == FE_MASK_UNMASK) { mxcsr &= ~mxcsr_mask_bits; }
    else { mxcsr |= mxcsr_mask_bits; }

    __asm__ volatile("fldcw %0" : : "m"(cw));
    __asm__ volatile("ldmxcsr %0" : : "m"(mxcsr));
    #  elif defined(__aarch64__) || defined(__arm__)
    uint32_t fpscr;
    asm volatile("vmrs %0, fpscr" : "=r"(fpscr));

    if (mask_or_unmask == FE_MASK_UNMASK) { cw &= ~(uint16_t)(excepts); }
    else { cw |= (uint16_t)(excepts); }

    uint32_t mxcsr_mask_bits = ((uint32_t)excepts) << 7;
    if (mask_or_unmask == FE_MASK_UNMASK) { mxcsr &= ~mxcsr_mask_bits; }
    else { mxcsr |= mxcsr_mask_bits; }

    __asm__ volatile("vmsr fpscr, %0" : : "r"(fpscr));
    #  else
    return -1;
    #  endif

    #endif

    return 0;
}

int
feraiseexcept(int excepts)
{
    _re_fe_curr_env.except |= excepts;

    int unmasked = excepts & ~_re_fe_curr_env.masked;

    if (unmasked)
    {
        volatile double x, y, z;

        if (excepts & FE_INVALID)
        {
            x = 0.0;
            y = 0.0;
            z = x / y; // 0.0 / 0.0 = NaN
            (void)z;
        }
        if (excepts & FE_DIVBYZERO)
        {
            x = 1.0;
            y = 0.0;
            z = x / y; // division by zero
            (void)z;
        }
        if (excepts & FE_OVERFLOW)
        {
            x = FLT_MAX;
            y = FLT_MAX;
            z = x * y; // overflow
            (void)z;
        }
        if (excepts & FE_UNDERFLOW)
        {
            x = FLT_MIN;
            y = FLT_MIN;
            z = x * y; // underflow
            (void)z;
        }
        if (excepts & FE_INEXACT)
        {
            x = 1.0;
            y = 3.0;
            z = x / y; // inexact result
            (void)z;
        }
    }

    return 0;
}
