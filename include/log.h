#ifndef _RE_LOG_H
#define _RE_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdafx.h"

#ifndef NDEBUG
#  define re_assert_else_return(expr, retval)                                                                                                                                                                                                            \
    do                                                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
      if (RE_UNLIKELY(!((bool)(expr))))                                                                                                                                                                                                                  \
      {                                                                                                                                                                                                                                                  \
        re_log_error("Assertion failed -> %s\n", #expr);                                                                                                                                                                                                 \
        return retval;                                                                                                                                                                                                                                   \
      }                                                                                                                                                                                                                                                  \
    } while (0);
#  define re_assert_and_exec(expr, code)                                                                                                                                                                                                                 \
    if (RE_UNLIKELY(!((bool)(expr))))                                                                                                                                                                                                                    \
    {                                                                                                                                                                                                                                                    \
      re_log_error("Assertion failed -> %s\n", #expr);                                                                                                                                                                                                   \
      code                                                                                                                                                                                                                                               \
    }
#  define re_assert(expr)                                                                                                                                                                                                                                \
    do                                                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
      if (RE_UNLIKELY(!((bool)(expr)))) { re_log_error("Assertion failed -> %s\n", #expr); }                                                                                                                                                             \
    } while (0);
#else
// These are typecasted to void because they give warnings because result (its
// like expr != NULL) is not used
#  define re_assert_else_return(expr, retval) (void)(expr)
#  define re_assert_and_exec(expr, code) (void)(expr)
#  define re_assert(expr) (void)(expr)
#  pragma message("Assertions disabled")
#endif

#define _RE_LOG_EXPAND_PARAMETERS(preceder, is_error, ...) _re_core_log(__FILE__, __LINE__, __func__, preceder, is_error, RE_COMMA_ARGS_FIRST(__VA_ARGS__) RE_COMMA_ARGS_REST(__VA_ARGS__))

#define re_log_error(...) _RE_LOG_EXPAND_PARAMETERS(" err: ", true, RE_COMMA_ARGS_FIRST(__VA_ARGS__) RE_COMMA_ARGS_REST(__VA_ARGS__))
#define re_log_and_abort(...)                                                                                                                                                                                                                            \
  {                                                                                                                                                                                                                                                      \
    _RE_LOG_EXPAND_PARAMETERS(" _fatal_: ", true, RE_COMMA_ARGS_FIRST(__VA_ARGS__) RE_COMMA_ARGS_REST(__VA_ARGS__));                                                                                                                                     \
    abort();                                                                                                                                                                                                                                             \
  }
#define re_log_warning(...) _RE_LOG_EXPAND_PARAMETERS(" warning: ", false, RE_COMMA_ARGS_FIRST(__VA_ARGS__) RE_COMMA_ARGS_REST(__VA_ARGS__))
#define re_log_info(...) _RE_LOG_EXPAND_PARAMETERS(" info: ", false, RE_COMMA_ARGS_FIRST(__VA_ARGS__) RE_COMMA_ARGS_REST(__VA_ARGS__))
#define re_log_debug(...) _RE_LOG_EXPAND_PARAMETERS(" debug: ", false, RE_COMMA_ARGS_FIRST(__VA_ARGS__) RE_COMMA_ARGS_REST(__VA_ARGS__))
#define re_log_verbose(...) _RE_LOG_EXPAND_PARAMETERS(" verbose: ", false, RE_COMMA_ARGS_FIRST(__VA_ARGS__) RE_COMMA_ARGS_REST(__VA_ARGS__))
#define re_log_custom(...) _RE_LOG_EXPAND_PARAMETERS(preceder, false, RE_COMMA_ARGS_FIRST(__VA_ARGS__) RE_COMMA_ARGS_REST(__VA_ARGS__))

  extern void _re_core_log(const char* file, size_t line, const char* fn, const char* preceder, bool err, const char* fmt, ...);
  extern void re_log_va(const char* file, size_t line, const char* fn, const char* preceder, bool err, const char* fmt, va_list args);

#ifdef __cplusplus
}
#endif

#endif
