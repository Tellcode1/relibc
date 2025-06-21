/* Utility file for cross compiler attribute support */

#ifndef RE_ATTRIBUTES_H_INCLUDED_
#define RE_ATTRIBUTES_H_INCLUDED_

#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef RE_HAS_ATTR
#  if defined(__has_attribute)
#    define RE_HAS_ATTR(attr) __has_attribute(attr)
#  else
#    define RE_HAS_ATTR(attr)
#  endif
#endif

#if (defined(__GNUC__) || defined(__clang__)) && defined(__has_attribute)
#  if (defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 11)
#    define RE_ATTR_MALLOC(free_fn, size_arg_idx) __attribute__((malloc(free_fn, size_arg_idx)))
#  elif (RE_HAS_ATTR(malloc))
// clang surprisingly does not support arguments to malloc attributes
#    define RE_ATTR_MALLOC(free_fn, size_arg_idx) __attribute__((malloc))
#  else
#    define RE_ATTR_MALLOC(free_fn, size_arg_idx)
#  endif
#  if RE_HAS_ATTR(pure)
#    define RE_ATTR_PURE __attribute__((pure))
#  endif
#  if RE_HAS_ATTR(const)
#    define RE_ATTR_CONST __attribute__((const))
#  endif
#  if RE_HAS_ATTR(noreturn)
#    define RE_ATTR_NORETURN __attribute__((noreturn))
#  endif
#  if RE_HAS_ATTR(deprecated)
#    define RE_ATTR_DEPRECATED __attribute__((deprecated))
#  endif
#  if RE_HAS_ATTR(unused)
#    define RE_ATTR_UNUSED __attribute__((unused))
#  endif
#  if RE_HAS_ATTR(aligned)
#    define RE_ATTR_ALIGNED(n) __attribute__((aligned(n)))
#  endif
#  if RE_HAS_ATTR(format)
#    define RE_ATTR_FORMAT(fmt_idx, args_idx) __attribute__((format(printf, fmt_idx, args_idx)))
#  endif
#  if RE_HAS_ATTR(hot)
#    define RE_ATTR_HOT __attribute__((hot))
#  endif
#  if RE_HAS_ATTR(cold)
#    define RE_ATTR_COLD __attribute__((cold))
#  endif
#  if RE_HAS_ATTR(constructor)
#    define RE_ATTR_CONSTRUCTOR __attribute__((constructor))
#  endif
#  if RE_HAS_ATTR(destructor)
#    define RE_ATTR_DESTRUCTOR __attribute__((destructor))
#  endif
#  if RE_HAS_ATTR(nonnull)
#    define RE_ATTR_NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#  endif
#  if RE_HAS_ATTR(returns_nonnull)
#    define RE_ATTR_RETURNS_NONNULL __attribute__((returns_nonnull))
#  endif
#  if RE_HAS_ATTR(packed)
#    define RE_ATTR_PACKED __attribute__((packed))
#  endif
  // #  if RE_HAS_ATTR(access)
  // #    define RE_ATTR_ACCESS(...) __attribute__((access(__VA_ARGS__)))
  // #  endif

#elif defined(_MSC_VER)
#  define RE_ATTR_MALLOC __declspec(restrict)
#  define RE_ATTR_PURE
#  define RE_ATTR_CONST
#  define RE_ATTR_NORETURN __declspec(noreturn)
#  define RE_ATTR_DEPRECATED __declspec(deprecated)
#  define RE_ATTR_UNUSED
#  define RE_ATTR_ALIGNED(n) __declspec(align(n))
#  define RE_ATTR_FORMAT(fmt_idx, args_idx)
#  define RE_ATTR_HOT
#  define RE_ATTR_COLD
#  define RE_ATTR_CONSTRUCTOR __declspec(dllexport)
#  define RE_ATTR_DESTRUCTOR __declspec(dllexport)
#  define RE_ATTR_NONNULL(...)
#  define RE_ATTR_RETURNS_NONNULL
#  define RE_ATTR_ACCESS(...)
#else
#  define RE_ATTR_MALLOC
#  define RE_ATTR_PURE
#  define RE_ATTR_CONST
#  define RE_ATTR_NORETURN
#  define RE_ATTR_DEPRECATED
#  define RE_ATTR_UNUSED
#  define RE_ATTR_ALIGNED(n)
#  define RE_ATTR_FORMAT(fmt_idx, args_idx)
#  define RE_ATTR_HOT
#  define RE_ATTR_COLD
#  define RE_ATTR_CONSTRUCTOR
#  define RE_ATTR_DESTRUCTOR
#  define RE_ATTR_NONNULL(...)
#  define RE_ATTR_RETURNS_NONNULL
#  define RE_ATTR_ACCESS(...)
#endif

#ifdef __cplusplus
}
#endif

#endif // RE_ATTRIBUTES_H_INCLUDED_
