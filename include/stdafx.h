#ifndef RE_STDAFX_H_
#define RE_STDAFX_H_

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"

#if !defined(RE_RESTRICT)
#  if defined(_MSC_VER)
#    define RE_RESTRICT __restrict
#  elif (defined(__GNUC__) || defined(__clang__)) && __STDC_VERSION__ >= 199901L
#    define RE_RESTRICT __restrict__
#  else
#    define RE_RESTRICT
#  endif
#endif

#ifndef RE_TYPEOF
#  if defined(__GNUC__) || defined(__clang__)
#    define RE_TYPEOF(x) __typeof__(x)
#  elif defined(_MSC_VER)
#    define RE_TYPEOF(x) decltype(x)
#  else
#    pragma message("RE_TYPEOF not available")
#    define RE_TYPEOF(x)
#  endif
#endif

#if defined(__GNUC__) && defined(__has_builtin) && __has_builtin(__builtin_expect)
#  define RE_LIKELY(expr) (__builtin_expect(!!(expr), 1))
#  define RE_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
/* Note that equals must be a long (or convertible to a long) */
#  define RE_EXPECT_EQUALS(expr, equals) (__builtin_expect(!!(expr), equals))
#else
#  define RE_LIKELY(expr) (expr)
#  define RE_UNLIKELY(expr) (expr)
#  define RE_EXPECT_EQUALS(expr, equals) (expr)
#endif

#define RE_CONCAT(x, y) x##y

#ifndef RE_STATIC_ASSERT
#  define RE_STATIC_ASSERT(expr, errmsg) static volatile char static_assert_failed__##errmsg[!(expr) ? -1 : 1]
#endif

/*
 * GNUC and builtin have protection from accidentally passing in pointers instead of stack arrays
 */
#if defined(__GNUC__) && (__STDC_VERSION__ >= 201112L) && defined(RE_TYPEOF)
#  define re_arrlen(arr) _Generic(&(arr), RE_TYPEOF(*(arr))(*): 0, default: (sizeof(arr) / sizeof((arr)[0])))
#elif defined(__has_builtin) && __has_builtin(__builtin_choose_expr) && __has_builtin(__builtin_types_compatible_p) && defined(RE_TYPEOF)
#  define re_arrlen(arr) __builtin_choose_expr(__builtin_types_compatible_p(RE_TYPEOF(arr), RE_TYPEOF(&(arr)[0])), 0, (sizeof(arr) / sizeof((arr)[0])))
#else
#  define re_arrlen(arr) ((size_t)(sizeof(arr) / sizeof((arr)[0])))
#endif

#ifndef re_zero_init
#  ifndef __cplusplus
#    define re_zero_init(TYPE) (TYPE){ 0 }
#  else
#    define re_zero_init(TYPE)                                                                                                                                                                                                                           \
      (TYPE) {}
#  endif
#endif

/**
 * https://stackoverflow.com/a/11172679
 * Stupid fix to , ##__VA_ARGS__ being a GNU extension
 * Only supports up to 10 arguments! However, increasing the limit is easy
 * Go to __GNUC_HELP_ME_PLEASE_SELECT_10TH and just add more variables and set the define to the last one
 */
#define RE_COMMA_ARGS_FIRST(...) __GNUC_HELP_ME_PLEASE_FIRST_HELPER(__VA_ARGS__, throwaway)

/*
 * if there's only one argument, expands to nothing.  if there is more
 * than one argument, expands to a comma followed by everything but
 * the first argument.  only supports up to 10 arguments but can be
 * trivially expanded.
 */
#define RE_COMMA_ARGS_REST(...) __GNUC_HELP_ME_PLEASE_REST_HELPER(__GNUC_HELP_ME_PLEASE_NUM(__VA_ARGS__), __VA_ARGS__)

#define __GNUC_HELP_ME_PLEASE_FIRST_HELPER(first, ...) first
#define __GNUC_HELP_ME_PLEASE_REST_HELPER(qty, ...) __GNUC_HELP_ME_PLEASE_REST_HELPER2(qty, __VA_ARGS__)
#define __GNUC_HELP_ME_PLEASE_REST_HELPER2(qty, ...) __GNUC_HELP_ME_PLEASE_REST_HELPER_##qty(__VA_ARGS__)
#define __GNUC_HELP_ME_PLEASE_REST_HELPER_ONE(first)
#define __GNUC_HELP_ME_PLEASE_REST_HELPER_TWOORMORE(first, ...) , __VA_ARGS__
#define __GNUC_HELP_ME_PLEASE_NUM(...) __GNUC_HELP_ME_PLEASE_SELECT_10TH(__VA_ARGS__, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, ONE, throwaway)
#define __GNUC_HELP_ME_PLEASE_SELECT_10TH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10

#endif
