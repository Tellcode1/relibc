#ifndef _RE_FENV_H
#define _RE_FENV_H

#include "math.h"
#include <stdint.h>

typedef int32_t fexcept_t;

#define FE_MASK_MASK 0
#define FE_MASK_UNMASK 1

typedef struct fenv
{
  fexcept_t except;
  fexcept_t masked;
} fenv_t;

static const fenv_t _re_def_fe_env = { .except = 0, .masked = INT32_MAX };
extern fenv_t       _re_fe_curr_env;

#define FE_DFL_ENV ((const fenv_t*)&_re_def_fe_env)

#define FE_DIVBYZERO (1 << 0)
#define FE_DENORMAL (1 << 1)
#define FE_INEXACT (1 << 2)
#define FE_INVALID (1 << 3)
#define FE_OVERFLOW (1 << 4)
#define FE_UNDERFLOW (1 << 5)

#define FE_ALL_EXCEPT (FE_DIVBYZERO | FE_DENORMAL | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)

#define FE_DOWNWARD 0
#define FE_TONEAREST 1
#define FE_TOWARDZERO 2
#define FE_UPWARD 3

RE_HEADER_FN int
feclearexcept(int excepts)
{
  _re_fe_curr_env.except &= ~excepts;
  return 0;
}

RE_HEADER_FN int
fetestexcept(int excepts)
{
  return (int)(_re_fe_curr_env.except & excepts);
}

RE_HEADER_FN int
fegetexceptflag(fexcept_t* o_excepts, int excepts_to_check)
{
  *o_excepts = 0;
  for (int i = 0; i < (sizeof(fexcept_t) * 8); i++)
  {
    if ((_re_fe_curr_env.except & (1 << i)) != 0 && (excepts_to_check & (1 << i)) != 0) { *o_excepts |= (1 << i); }
  }
  return 0;
}

RE_HEADER_FN int
fesetexceptflag(const fexcept_t* excepts, int excepts_to_set)
{
  for (int i = 0; i < (sizeof(fexcept_t) * 8); i++)
  {
    if ((*excepts & (1 << i)) != 0 && (excepts_to_set & (1 << i)) != 0) { _re_fe_curr_env.except |= (1 << i); }
  }
  return 0;
}

/**
 * "Raise" the floating point exceptions. This does not actually cause a hardware trap if those exceptions are masked
 * Which is all of them unless unmasked by the user.
 */
extern int feraiseexcept(int excepts);

RE_HEADER_FN int
fegetenv(fenv_t* env)
{
  *env = _re_fe_curr_env;
  return 0;
}

RE_HEADER_FN int
fesetenv(const fenv_t* env)
{
  _re_fe_curr_env = *env;
  return 0;
}

RE_HEADER_FN int
feupdateenv(const fenv_t* env)
{
  fenv_t tmp_copy = _re_fe_curr_env;
  _re_fe_curr_env = *env;
  return feraiseexcept(tmp_copy.except);
}

RE_HEADER_FN int
feholdexcept(fenv_t* env)
{
  if (env == NULL) return -1;

  *env = _re_fe_curr_env;

  _re_fe_curr_env.except = 0;
  _re_fe_curr_env.masked = FE_ALL_EXCEPT;

  return 0;
}

extern int _re_mask_fp_excepts(int excepts, int mask_or_unmask);
#define feenableexcept(excepts) _re_mask_fp_excepts(excepts, FE_MASK_UNMASK)
#define fedisableexcept(excepts) _re_mask_fp_excepts(excepts, FE_MASK_MASK)

#endif //_RE_FENV_H
