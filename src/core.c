/*
  MIT License

  Copyright (c) 2025 Fouzan MD Ishaque (fouzanmdishaque@gmail.com)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  */

#include "../include/attributes.h"
#include "../include/rand.h"
#include "../include/stdafx.h"

#include "../include/errorcodes.h"
#include "../include/log.h"
#include "../include/math.h"
#include "../include/print.h"
#include "../include/strconv.h"
#include "../include/string.h"
#include "../include/types.h"

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _min(x, y) (((x) < (y)) ? (x) : (y))

void
_re_core_log(const char* file, size_t line, const char* fn, const char* preceder, bool err, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  re_log_va(file, line, fn, preceder, err, fmt, args);

  va_end(args);
}

void
re_log_va(const char* file, size_t line, const char* fn, const char* preceder, bool err, const char* fmt, va_list args)
{
  // FILE* out = (err) ? stderr : stdout;

  // /* Two fprintf calls, good. */

  // struct tm* time = NULL;
  // fprintf(out, "[%d:%d:%d] [%s:%zu]%s%s(): ", time->tm_hour % 12, time->tm_min, time->tm_sec, basename(file), line, preceder, fn);

  // vfprintf(out, fmt, args);
}

// printf

size_t
itoa2(intmax_t num, char out[], int base, size_t max, bool add_commas)
{
  re_assert(base >= 2 && base <= 36);
  re_assert(out != NULL);

  if (max == 0)
  {
    return 0; // this shouldn't be an error
  }
  if (max == 1)
  {
    out[0] = 0;
    return 0;
  }

  /* The second parameter should always evaluate to true, left for brevity */
  if (num == 0 && max >= 2)
  {
    out[0] = '0';
    out[1] = 0;
    return 1;
  }

  size_t i = 0;

  // we need 1 space for NULL terminator!!
  // max will be greater or equal to 1 due to past checks
  max--;
  if (i >= max)
  {
    out[i] = 0;
    return i;
  }

  /* TODO: If max is exactly two, and x is negative, the function will only output a - */
  if (num < 0 && base == 10)
  {
    out[i] = '-';
    i++;
    num = -num;
  }
  if (i >= max)
  {
    out[i] = 0;
    return i;
  }

  /* highest power of base that is <= x */
  intmax_t highest_power_of_base = 1;
  while (highest_power_of_base <= num / base)
  {
    highest_power_of_base *= base;
  }

  size_t   dig_count = 0;
  intmax_t temp      = num;
  while (temp > 0)
  {
    dig_count++;
    temp /= base;
  }

  size_t loop_digits_written = 0;

  do
  {
    if (i >= max) { break; }

    if (add_commas && '.' && dig_count > 3 && loop_digits_written > 0 && (dig_count - loop_digits_written) % 3 == 0)
    {
      if (i < max)
      {
        out[i] = '.';
        i++;
      }
      else { break; }
    }

    intmax_t dig = num / highest_power_of_base;

    if (dig < 10) { out[i] = (char)('0' + dig); }
    else { out[i] = (char)('a' + (dig - 10)); }
    i++;

    num %= highest_power_of_base;
    highest_power_of_base /= base;
    loop_digits_written++;
  } while (highest_power_of_base > 0);

  out[i] = 0;
  return i;
}

size_t
utoa2(uintmax_t num, char out[], int base, size_t max, bool add_commas)
{
  re_assert(base >= 2 && base <= 36);
  re_assert(out != NULL);

  if (max == 0)
  {
    return 0; // this shouldn't be an error
  }
  if (max == 1)
  {
    out[0] = 0;
    return 0;
  }

  /* The second parameter should always evaluate to true, left for brevity */
  if (num == 0 && max >= 2)
  {
    out[0] = '0';
    out[1] = 0;
    return 1;
  }

  size_t i = 0;

  // we need 1 space for NULL terminator!!
  // max will be greater or equal to 1 due to past checks
  max--;
  if (i >= max)
  {
    out[i] = 0;
    return i;
  }

  size_t    dig_count = 0;
  uintmax_t temp      = num;
  while (temp > 0)
  {
    dig_count++;
    temp /= base;
  }

  /* highest power of base that is <= x */
  uintmax_t highest_power_of_base = 1;
  while (highest_power_of_base <= num / (uintmax_t)base)
  {
    highest_power_of_base *= base;
  }

  size_t loop_digits_written = 0;
  do
  {
    if (i >= max) { break; }

    if (add_commas && '.' && dig_count > 3 && loop_digits_written > 0 && (dig_count - loop_digits_written) % 3 == 0)
    {
      if (i < max)
      {
        out[i] = '.';
        i++;
      }
      else { break; }
    }

    uintmax_t dig = num / highest_power_of_base;

    if (dig < 10) { out[i] = (char)('0' + dig); }
    else { out[i] = (char)('a' + (dig - 10)); }
    i++;

    num %= highest_power_of_base;
    highest_power_of_base /= base;
    loop_digits_written++;
  } while (highest_power_of_base > 0);

  out[i] = 0;
  return i;
}

#define RE_FTOA_HANDLE_CASE(fn, n, str)                                                                                                                                                                                                                  \
  if (fn(n))                                                                                                                                                                                                                                             \
  {                                                                                                                                                                                                                                                      \
    if (signbit(n) == 0) return strncpy2(out, str, max);                                                                                                                                                                                                 \
    else                                                                                                                                                                                                                                                 \
      return strncpy2(out, "-" str, max);                                                                                                                                                                                                                \
  }

// WARNING::: I didn't write most of this, stole it from stack overflow.
// if it explodes your computer its your fault!!!
size_t
ftoa2(double num, char out[], int precision, size_t max, bool remove_zeros)
{
  if (max == 0) { return 0; }
  if (max == 1)
  {
    out[0] = 0;
    return 0;
  }

  RE_FTOA_HANDLE_CASE(isnan, num, "nan");
  RE_FTOA_HANDLE_CASE(isinf, num, "inf");
  RE_FTOA_HANDLE_CASE(0.0 ==, num, "0.0");

  char* itr = out;

  const int neg = (num < 0);
  if (neg)
  {
    num  = -num;
    *itr = '-';
    itr++;
  }

  int exponent        = (num == 0.0) ? 0 : (int)log10(num);
  int to_use_exponent = (exponent >= 14 || (neg && exponent >= 9) || exponent <= -9);
  if (to_use_exponent) { num /= pow(10.0, exponent); }

  double rounding = pow(10.0, -precision) * 0.5;
  num += rounding;

  /* n has been absoluted before so we can expect that it won't be negative */
  uintmax_t int_part = (uintmax_t)num;

  // int part is now floored
  double frac_part = num - (double)int_part;

  itr += utoa2(int_part, itr, 10, max - 1, false);

  if (precision > 0 && (size_t)(itr - out) < max - 2)
  {
    *itr = '.';
    itr++;

    for (int i = 0; i < precision && (size_t)(itr - out) < max - 1; i++)
    {
      frac_part *= 10;
      int digit = (int)frac_part;
      *itr      = (char)('0' + digit);
      itr++;
      frac_part -= digit;
    }
  }

  if (remove_zeros && precision > 0)
  {
    while (*(itr - 1) == '0')
    {
      itr--;
    }
    if (*(itr - 1) == '.') { itr--; }
  }

  if (to_use_exponent && (size_t)(itr - out) < max - 4)
  {
    *itr = 'e';
    itr++;
    *itr = (exponent >= 0) ? '+' : '-';
    itr++;

    exponent = (exponent >= 0) ? exponent : -exponent;

    if (exponent >= 100)
    {
      *itr = (char)('0' + (exponent / 100U));
      itr++;
    }
    if (exponent >= 10)
    {
      *itr = (char)('0' + ((exponent / 10U) % 10U));
      itr++;
    }
    *itr = (char)('0' + (exponent % 10U));
    itr++;
  }

  *itr = 0;
  return itr - out;
}

#define RE_SKIP_WHITSPACE(s) strtrim_c(s, &s, NULL);

intmax_t
re_atoi(const char in_string[], size_t max)
{
  if (!in_string) { return __INTMAX_MAX__; }

  const char* c   = in_string;
  intmax_t    ret = 0;
  size_t      i   = 0;

  RE_SKIP_WHITSPACE(c);

  bool neg = 0;
  if (i < max && *c == '-')
  {
    neg = 1;
    c++;
    i++;
  }
  else if (i < max && *c == '+')
  {
    c++;
    i++;
  }

  while (i < max && *c)
  {
    if (!isdigit(*c)) { break; }

    int digit = *c - '0';
    ret       = ret * 10 + digit;

    c++;
    i++;
  }

  if (neg) { ret *= -1; }

  return ret;
}

double
atof2(const char in_string[], size_t max)
{
  if (!in_string) { return 0.0; }

  double      result = 0.0, fraction = 0.0;
  int         divisor = 1;
  bool        neg     = 0;
  const char* c       = in_string;
  size_t      i       = 0;

  RE_SKIP_WHITSPACE(c);

  if (i < max && *c == '-')
  {
    neg = 1;
    c++;
    i++;
  }
  else if (i < max && *c == '+')
  {
    c++;
    i++;
  }

  while (i < max && isdigit(*c))
  {
    result = result * 10 + (*c - '0');
    c++;
  }

  if (*c == '.')
  {
    c++;
    while (isdigit(*c))
    {
      fraction = fraction * 10 + (*c - '0');
      divisor *= 10;
      c++;
      i++;
    }
    result += fraction / divisor;
  }

  if (i < max && (*c == 'e' || *c == 'E'))
  {
    c++;
    i++;
    int exp_sign = 1;
    int exponent = 0;

    if (*c == '-')
    {
      exp_sign = -1;
      c++;
      i++;
    }
    else if (*in_string == '+')
    {
      c++;
      i++;
    }

    while (i < max && isdigit(*c))
    {
      exponent = exponent * 10 + (*c - '0');
      c++;
      i++;
    }

    result = ldexp(result, exp_sign * exponent);
  }

  if (neg) { result *= -1.0; }

  return result;
}

bool
atobool(const char in_string[], size_t max)
{
  size_t i = 0;
  RE_SKIP_WHITSPACE(in_string);
  if (i > max) { return true; }
  if (strcasencmp(in_string, "false", max - i) == 0 || strncmp(in_string, "0", max - i) == 0) { return false; }
  return true;
}

size_t
ptoa2(void* ptr, char out[], size_t max)
{
  if (ptr == NULL) { return strncpy2(out, "NULL", max); }

  u64        addr   = (u64)ptr;
  const char digs[] = "0123456789abcdef";

  size_t w = 0;

  w += strncpy2(out, "0x", max);

  // stolen from stack overflow
  /* forgot where I stole it from, god. */
  for (int i = (sizeof(addr) * 2) - 1; i >= 0 && w < max - 1; i--)
  {
    int dig = (int)((addr >> (i * 4)) & 0xF);
    out[w]  = digs[dig];
    w++;
  }
  out[w] = 0;
  return w;
}

/* Written by yours truly. */
size_t
btoa2(size_t num_bytes, bool upgrade, char out[], size_t max)
{
  size_t written = 0;
  if (upgrade)
  {
    const char* stages[] = { " B", " KB", " MB", " GB", " TB", " PB", " Comically large number of bytes" };
    double      b        = (double)num_bytes;
    u32         stagei   = 0;

    const size_t num_stages = re_arrlen(stages) - 1;
    while (b >= 1000.0 && stagei < num_stages)
    {
      stagei++;
      b /= 1000.0;
    }

    written = ftoa2(b, out, 3, max, 1);
    strcat_max(out, stages[stagei], max);
    written += strlen(stages[stagei]);
    written = _min(written, max);
  }
  else { written = utoa2(num_bytes, out, 10, max, true); }
  return written;
}

// rewritten memcpy
void*
memset(void* dst, int to, size_t sz)
{
  re_assert(dst != NULL);
  re_assert(sz != 0);

  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(memset, dst, to, sz);

  uchar* byte_write = (uchar*)dst;
  while ((sz--) > 0)
  {
    *byte_write = to;
    byte_write++;
  }

  return dst;
}

void*
memmove(void* dst, const void* src, size_t sz)
{
  re_assert_else_return(sz > 0, NULL);

  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(memmove, dst, src, sz);

  uchar*       d = (uchar*)dst;
  const uchar* s = (const uchar*)src;

  if (d > s && d < s + sz)
  {
    d += sz;
    s += sz;
    while ((sz--) > 0)
    {
      d--;
      s--;
      *d = *s;
    }
  }
  else
  {
    while ((sz--) > 0)
    {
      *d = *s;
      d++;
      s++;
    }
  }

  return dst;
}

static inline size_t
_align_up(size_t sz, size_t align)
{
  return (sz + (align - 1)) & ~(align - 1);
}

void*
aligned_alloc(size_t size, size_t alignment)
{
  re_assert_else_return((alignment & (alignment - 1)) == 0, NULL);
  re_assert_else_return(size > 0, NULL);

  const size_t total_size = _align_up(size + sizeof(void*) + sizeof(size_t), alignment);

  void* const orig = malloc(total_size);
  if (!orig) { return NULL; }

  /**
   * We need to store the original pointer and the previous size just behind the
   * aligned block of memory.
   */
  uchar* ptr     = (uchar*)orig + sizeof(void*) + sizeof(size_t);
  uchar* aligned = (uchar*)((uintptr_t)(ptr + alignment - 1) & ~(alignment - 1));

  size_t* stosize = (size_t*)(aligned - sizeof(void*) - sizeof(size_t));
  *stosize        = size;

  void** store_ptr = (void**)(aligned - sizeof(void*));
  *store_ptr       = orig;

  re_assert_else_return(((uintptr_t)aligned % alignment) == 0, NULL);

  return aligned;
}

void*
aligned_realloc(void* orig, size_t size, size_t alignment)
{
  re_assert_else_return(size != 0, NULL);
  re_assert_else_return(alignment != 0, NULL);
  re_assert_else_return((alignment & (alignment - 1)) == 0, NULL);

  if (orig == NULL) { return aligned_alloc(size, alignment); }

  void* absolute = aligned_get_absolute_ptr(orig);
  if (RE_LIKELY(absolute))
  {
    size_t prev_size = *(size_t*)((uchar*)orig - sizeof(void*) - sizeof(size_t));

    if (prev_size == size) { return orig; }

    void* new_block = aligned_alloc(size, alignment);
    memmove(new_block, orig, _min(prev_size, size));
    aligned_free(orig);

    return new_block;
  }
  else { re_log_error("double free %p\n", orig); }

  return NULL;
}

void*
aligned_get_absolute_ptr(void* aligned_ptr)
{
  void** orig_location = (void**)((uchar*)aligned_ptr - sizeof(void*));
  return *orig_location;
}

size_t
aligned_ptr_get_size(void* aligned_ptr)
{
  size_t prev_size = *(size_t*)((uchar*)aligned_ptr - sizeof(void*) - sizeof(size_t));
  return prev_size;
}

void*
re_realloc(void* prevblock, size_t new_sz)
{
  re_assert_else_return(new_sz > 0, NULL);

  void* ptr = realloc(prevblock, new_sz);

  return ptr;
}

void
aligned_free(void* aligned_block)
{
  void** orig_location = (void**)((uchar*)aligned_block - sizeof(void*));

  void* orig = *orig_location;
  // if (orig)
  // {
  // *orig_location = NULL;
  free(orig);
  // }
  // else
  // {
  //   re_log_error("double free %p\n", aligned_block);

  //   /**
  //    * TODO: should we?
  //    */
  //   abort();
  // }
}

const void*
memchr(const void* p, int chr, size_t psize)
{
  re_assert_else_return(psize > 0, NULL);

  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(memchr, p, chr, psize);

  const uchar* read = (const uchar*)p;
  const uchar  chk  = chr;
  for (size_t i = 0; i < psize; i++)
  {
    if (read[i] == chk) { return (const void*)(read + i); }
  }
  return NULL;
}

int
memcmp(const void* _p1, const void* _p2, size_t max)
{
  re_assert_else_return(max > 0, -1);

  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(memcmp, _p1, _p2, max);

  const uchar* p1 = (const uchar*)_p1;
  const uchar* p2 = (const uchar*)_p2;

  while ((max--) != 0)
  {
    if (*p1 != *p2) { return *p1 - *p2; }
    p1++;
    p2++;
  }

  return 0;
}

size_t
strncpy2(char* dst, const char* src, size_t max)
{
  re_assert_else_return(max > 0, 0);

  size_t slen         = strlen(src);
  size_t original_max = max;

  if (!dst) { return _min(slen, max); }

#if STRING_USE_BUILTIN && defined(__GNUC__) && defined(__has_builtin) && __has_builtin(__builtin_strncpy)
  __builtin_strncpy(dst, src, max);
  return _min(slen, max);
#endif

  while (*src && max > 0)
  {
    *dst = *src;
    dst++;
    src++;
    max--;
  }

  *dst = 0;

  return _min(slen, original_max);
}

char*
strcpy(char* dst, const char* src)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strcpy, dst, src); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)

  char* const dst_orig = dst;

  while (*src)
  {
    *dst = *src;
    dst++;
    src++;
  }

  *dst = 0;

  return dst_orig;
}

char*
stpcpy(char* dst, const char* src)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(stpcpy, dst, src);

  while (*src)
  {
    *dst = *src;
    dst++;
    src++;
  }

  *dst = 0;
  return dst;
}

char*
strncpy(char* dst, const char* src, size_t max)
{
  re_assert_else_return(max > 0, NULL);

  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strncpy, dst, src, max);

  (void)strncpy2(dst, src, max);
  return dst;
}

char*
strcat(char* dst, const char* src)
{
  char* end = dst + strlen(dst);

  while (*src)
  {
    *end = *src;
    end++;
    src++;
  }
  *end = 0;

  return dst;
}

char*
strncat(char* dst, const char* src, size_t max)
{
  re_assert_else_return(max > 0, NULL);

  char* original_dest = dst;

  dst = (char*)strchr(dst, '\0');
  if (!dst)
  {
    re_assert(0);
    return NULL;
  }

  size_t i = 0;
  while (*src && i < max)
  {
    *dst = *src;
    i++;
    src++;
    dst++;
  }
  *dst = 0;
  return original_dest;
}

size_t
strcat_max(char* dst, const char* src, size_t dest_size)
{
  /* Optionally, memset the remaining part of dst to 0? */

  if (dest_size == 0) { return strlen(src); }

  size_t dst_len = 0;
  while (dst_len < dest_size && dst[dst_len])
  {
    dst_len++;
  }

  if (dst_len == dest_size) { return dst_len + strlen(src); }

  size_t copy_len = dest_size - dst_len - 1;
  size_t i        = 0;
  while (i < copy_len && src[i])
  {
    dst[dst_len + i] = src[i];
    i++;
  }

  dst[dst_len + i] = 0;

  return dst_len + strlen(src);
}

char*
strtrim(char* s)
{
  char *begin, *end;
  if (strtrim_c(s, (const char**)&begin, (const char**)&end) == NULL) { return NULL; }

  /* end *may* be a pointer to the NULL terminator but yeah, still works */
  *end = 0;

  return begin;
}

const char*
strtrim_c(const char* s, const char** begin, const char** end)
{
  while (*s && isspace((uchar)*s))
  {
    s++;
  }

  if (begin) { *begin = (const char*)s; }

  const char* begin_copy = s;

  s += strlen(s);

  while (s > begin_copy && isspace((uchar) * (s - 1)))
  {
    s--;
  }

  if (end) { *end = (char*)s; }
  return begin_copy;
}

int
strcmp(const char* s1, const char* s2)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strcmp, s1, s2);

  while (*s1 && *s2 && (*s1 == *s2))
  {
    s1++;
    s2++;
  }

  return (uchar)*s1 - (uchar)*s2;
}

const char*
strchr(const char* s, int chr)
{
  uchar c = (uchar)chr;

  while (*s)
  {
    if (*s == c) { return s; }
    s++;
  }

  return (c == 0) ? s : NULL;
}

char*
strn(const char* s, int chr, int n)
{
  while (*s)
  {
    if (*s == chr)
    {
      n--;
      if (n <= 0) { return (char*)s; }
    }
  }
  return NULL;
}

char*
strrchr(const char* s, int chr)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strrchr, s, chr);

  const char* beg = s;
  char*       end = (char*)s + strlen(s) - 1;

  if (chr == 0) { return (char*)s + 1; }

  while (end > beg)
  {
    end--;
    if (*end == (char)chr) { return (char*)end; }
  }
  return NULL;
}

int
strncmp(const char* s1, const char* s2, size_t max)
{
  re_assert_else_return(max > 0, -1);

  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strncmp, s1, s2, max);
  size_t i = 0;
  while (*s1 && *s2 && (*s1 == *s2) && i < max)
  {
    s1++;
    s2++;
    i++;
  }
  return (i == max) ? 0 : (*(const uchar*)s1 - *(const uchar*)s2);
}

int
strcasencmp(const char* s1, const char* s2, size_t max)
{
  re_assert_else_return(max > 0, -1);

  size_t i = 0;
  while (*s1 && *s2 && i < max)
  {
    uchar c1 = tolower(*(uchar*)s1);
    uchar c2 = tolower(*(uchar*)s2);
    if (c1 != c2) { return c1 - c2; }

    s1++;
    s2++;
    i++;
  }
  if (i == max) { return 0; }
  return tolower(*(const uchar*)s1) - tolower(*(const uchar*)s2);
}

int
strcasecmp(const char* s1, const char* s2)
{
  while (*s1 && *s2)
  {
    uchar c1 = tolower(*(uchar*)s1);
    uchar c2 = tolower(*(uchar*)s2);
    if (c1 != c2) { return c1 - c2; }
    s1++;
    s2++;
  }
  return tolower(*(uchar*)s1) - tolower(*(uchar*)s2);
}

size_t
strlen(const char* s)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strlen, s);

  const char* start = s;
  while (*s)
  {
    s++;
  }

  return s - start;
}

size_t
strnlen(const char* s, size_t max)
{
  re_assert_else_return(max > 0, 0);

  const char* s_orig = s;
  while (*s && max > 0)
  {
    s++;
    max--;
  }

  return s - s_orig;
}

const char*
strstr(const char* s, const char* sub)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strstr, s, sub);

  for (; *s; s++)
  {
    const char* str_it = s;
    const char* sub_it = sub;

    while (*str_it && *sub_it && *str_it == *sub_it)
    {
      str_it++;
      sub_it++;
    }

    /* If we reach the NULL terminator of the substring, it exists in str. */
    if (!*sub_it) { return (char*)s; }
  }

  return NULL;
}

size_t
strlcpy(char* dst, const char* src, size_t dst_size)
{
  if (dst_size == 0) { return 0; }

  char* dst_orig = dst;

  size_t i = 0;
  while (*src && i < dst_size - 1)
  {
    *dst = *src;
    dst++;
    src++;
    i++;
  }

  *dst = 0;

  return dst - dst_orig;
}

size_t
strcpy2(char* dst, const char* src)
{
  size_t slen = strlen(src);
  if (!dst) { return slen; }

#if STRING_USE_BUILTIN && defined(__GNUC__) && defined(__has_builtin) && __has_builtin(__builtin_strcpy)
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strlen, __builtin_strcpy(dst, src)); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
#endif

  const char* original_dest = dst;
  while (*src)
  {
    *dst = *src;
    src++;
    dst++;
  }
  *dst = 0;
  return dst - original_dest;
}

size_t
strspn(const char* s, const char* accept)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strspn, s, accept);
  size_t i = 0;
  while (*s && *accept && *s == *accept)
  {
    i++;
    s++;
    accept++;
  }
  return i;
}

size_t
strcspn(const char* s, const char* reject)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strcspn, s, reject);

  const char* base = reject;
  size_t      i    = 0;

  while (*s)
  {
    const char* j = base;
    while (*j && *j != *s)
    {
      j++;
    }
    if (*j) { break; }
    i++;
    s++;
  }
  return i;
}

const char*
strpbrk(const char* s1, const char* s2)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strpbrk, s1, s2);

  while (*s1)
  {
    const char* j = s2;
    while (*j)
    {
      if (*j == *s1) { return (char*)s1; }
      j++;
    }
    s1++;
  }
  return NULL;
}

char* _re_strtokctx = NULL;
char*
strtok(char* s, const char* delim)
{
  if (!s) { s = _re_strtokctx; }
  char* p;

  s += strspn(s, delim);
  if (!s || *s == 0)
  {
    _re_strtokctx = s;
    return NULL;
  }

  p = s;
  s = (char*)strpbrk(s, delim);

  if (!s)
  {
    _re_strtokctx = p + strlen(p); // get pointer to last char
    return p;
  }
  *s            = 0;
  _re_strtokctx = s + 1;
  return p;
}

char*
strreplace(char* s, char to_replace, char replace_with)
{
  while (*s)
  {
    if (*s == to_replace) { *s = replace_with; }
  }
  return s;
}

// const char*
// basename(const char* path)
// {
//   char* p         = (char*)path; // shut up C compiler
//   char* backslash = strrchr(path, '/');
//   if (backslash != NULL) { return backslash + 1; }
//   return p;
// }

char*
strdup(const char* s)
{
  RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(strdup, s);

  size_t slen = strlen(s);

  char* new_s = malloc(slen + 1);

  strlcpy(new_s, s, slen + 1);

  return new_s;
}

char*
strexdup(const char* s, size_t size)
{
  char* new_s = malloc(size + 1);

  strlcpy(new_s, s, size + 1);

  return new_s;
}

char*
substr(const char* s, size_t start, size_t len)
{
  re_assert_else_return(len > 0, NULL);

  size_t slen = strlen(s);
  if (start + len > slen) { return NULL; }

  char* sub = malloc(len + 1);
  strncpy(sub, s + start, len);
  sub[len] = 0;
  return sub;
}

char*
strrev(char* str)
{
  size_t len = strlen(str);
  for (size_t i = 0; i < len / 2; i++)
  {
    char temp        = str[i];
    str[i]           = str[len - i - 1];
    str[len - i - 1] = temp;
  }
  return str;
}

char*
strnrev(char* str, size_t max)
{
  re_assert_else_return(max != 0, NULL);

  size_t len = strnlen(str, max);
  if (len == 0) { return str; }

  char* fwrd = str;
  char* back = str + len - 1;

  while (fwrd < back)
  {
    char temp = *fwrd;
    *fwrd     = *back;
    *back     = temp;

    fwrd++;
    back--;
  }

  return str;
}

int
isalpha(int chr)
{
  return (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z');
}

int
isdigit(int chr)
{
  return (chr >= '0' && chr <= '9');
}

int
isalnum(int chr)
{
  return isalpha(chr) || isdigit(chr);
}

int
isblank(int chr)
{
  return (chr == ' ') || (chr == '\t');
}

/* https://en.wikipedia.org/wiki/Control_character */
int
iscntrl(int chr)
{
  switch (chr)
  {
    /* \e also exists. non standard. yep. */
    case '\0':
    case '\a':
    case '\b':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r': return true;
    default: return false;
  }
  return false;
}

int
islower(int chr)
{
  return (chr >= 'a' && chr <= 'z');
}

int
isupper(int chr)
{
  return (chr >= 'A' && chr <= 'Z');
}

int
isspace(int chr)
{
  return (chr == ' ' || chr == '\n' || chr == '\t');
}

int
ispunct(int chr)
{
  /* Generated with
    for (i = 0; i < 256; i++)
    {
      if (ispunct(i))
      {
        printf("%c ", i);
      }
    }
  */
  switch (chr)
  {
    case '!':
    case '\"':
    case '#':
    case '$':
    case '%':
    case '&':
    case '\'':
    case '(':
    case ')':
    case '*':
    case '+':
    case ',':
    case '-':
    case '.':
    case '/':
    case ':':
    case ';':
    case '?':
    case '@':
    case '[':
    case '\\':
    case ']':
    case '^':
    case '_':
    case '`':
    case '{':
    case '|':
    case '}':
    case '~': return true;
    default: return false;
  }
}

int
tolower(int chr)
{
  if (isupper(chr)) { return chr + 32; }
  return chr;
}

int
toupper(int chr)
{
  if (islower(chr)) { return chr - 32; /* chr - 32 */ }
  return chr;
}

static inline re_rand_t
rotl(re_rand_t x, int k)
{
#if defined(__GNUC__) && defined(__has_builtin) && __has_builtin(__builtin_rotateleft64) && (__SIZEOF_LONG_LONG__ == 8)
  return __builtin_rotateleft64(x, k);
#else
  return (x << k) | (x >> ((sizeof(re_rand_t) * 8 - k) & ((sizeof(re_rand_t) * 8) - 1)));
#endif
}

re_error
re_random_bulk_range(re_rand_info_t* info, re_rand_t* outbuf, size_t outbuf_size, size_t min, size_t max)
{
  re_assert_else_return(outbuf_size != 0, RE_ERROR_INVALID_ARG);

  if (min >= max) { return min; }

  for (size_t i = 0; i < outbuf_size; i++)
  {
    /**
     * xoshiro 256 random number generator
     * https://prng.di.unimi.it/xoshiro256plusplus.c
     */
    const _RE_RAND_TMP_CONVERT_TYPE bound = max - min + 1;

    const re_rand_t result = rotl(info->state[0] + info->state[3], 23) + info->state[0];

    _RE_RAND_TMP_CONVERT_TYPE tmp = ((_RE_RAND_TMP_CONVERT_TYPE)result * bound);
    outbuf[i]                     = min + (tmp >> (sizeof(re_rand_t) * 8));

    const re_rand_t t = info->state[1] << 17;

    info->state[2] ^= info->state[0];
    info->state[3] ^= info->state[1];
    info->state[1] ^= info->state[2];
    info->state[0] ^= info->state[3];

    info->state[2] ^= t;
    info->state[3] = rotl(info->state[3], 45);
  }

  return RE_SUCCESS;
}

static inline re_rand_t
_splitmix(re_rand_t* state)
{
  re_rand_t tmp = (*state += 0x9E3779B97f4A7C15);
  tmp           = (tmp ^ (tmp >> 30)) * 0xBF58476D1CE4E5B9;
  tmp           = (tmp ^ (tmp >> 27)) * 0x94D049BB133111EB;
  return tmp ^ (tmp >> 31);
}

void
re_random_seed(re_rand_info_t* info, re_rand_t seed)
{
  re_rand_t splitmixstate = seed;
  for (size_t i = 0; i < 4; i++)
  {
    info->state[i] = _splitmix(&splitmixstate);
  }
}

// static struct sa_page* root = null;

// static inline re_error
// re_alloc_and_link_page(struct sa_page** dstptr, size_t min_size)
// {
//   min_size += SYSALLOC_PAGE_HEADER_SIZE;
//   const size_t map_size = RE_MAX(min_size, SYSALLOC_PAGE_HEADER_SIZE + SYSALLOC_PAGE_CAPACITY);

//   void* ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
//   if (ptr == MAP_FAILED)
//   {
//     return RE_ERROR_EXTERNAL;
//   }

//   *dstptr = (struct sa_page*)ptr;

//   struct sa_freeblock* new_node = malloc(sizeof(struct sa_freeblock));
//   re_zero_structp(new_node);

//   new_node->offset = SYSALLOC_PAGE_HEADER_SIZE;
//   new_node->size   = map_size - SYSALLOC_PAGE_HEADER_SIZE;
//   new_node->next   = NULL;

//   (*dstptr)->root_free_block = new_node;
//   (*dstptr)->page_size       = map_size;

//   if (root != null)
//   {
//     struct sa_page* last_page = root;
//     while (last_page->next)
//     {
//       if (last_page->next == last_page)
//       {
//         re_log_and_abort("Circular reference detected: page->next == page\n");
//       }
//       last_page = last_page->next;
//     }

//     if (last_page == *dstptr)
//     {
//       re_log_and_abort("BUG: Attempt to link page to itself!\n");
//     }

//     last_page->next = *dstptr;
//   }
//   else
//   {
//     root = (*dstptr);
//   }

//   return RE_SUCCESS;
// }

// static inline void
// re_free_page(struct sa_page* page)
// {
//   if (!page)
//   {
//     return;
//   }

//   struct sa_freeblock* node = page->root_free_block;
//   while (node)
//   {
//     struct sa_freeblock* next = node->next;
//     free(next);
//     node = next;
//   }

//   // dislodge the page from the linked list
//   struct sa_page* page_it = root;
//   while (page_it)
//   {
//     if (page_it->next == page)
//     {
//       page_it->next = page->next;
//       break;
//     }
//     page_it = page_it->next;
//   }

//   if (munmap((void*)page, page->page_size) != 0)
//   {
//     re_log_and_abort("munmap: %s\n", strerror(errno));
//   }
// }

// static inline void
// _trim_free_block(struct sa_page* page, struct sa_freeblock* block, size_t trim_size_trailing)
// {
//   if (block->size <= trim_size_trailing)
//   {
//     return;
//   }

//   re_assert_else_return(trim_size_trailing > 0, );

//   struct sa_freeblock* new_node = malloc(sizeof(struct sa_freeblock));
//   re_zero_structp(new_node);

//   new_node->size   = trim_size_trailing;
//   new_node->offset = block->offset + (block->size - trim_size_trailing);
//   new_node->next   = NULL;

//   // append a node
//   new_node->next = page->root_free_block;
//   if (page->root_free_block)
//   {
//     page->root_free_block = new_node;
//   }

//   block->size -= trim_size_trailing;
// }

// static inline size_t
// _get_block_size(void* block)
// {
//   struct sa_block* header = (struct sa_block*)((uchar*)block - sizeof(struct sa_block));
//   return header->size;
// }

// void*
// sa_alloc(size_t size)
// {
//   if (!root)
//   {
//     struct sa_page* tmp = NULL;
//     re_alloc_and_link_page(&tmp, size);
//   }

//   const size_t total_size = size + sizeof(struct sa_block);

//   struct sa_page* page = root;
//   while (page)
//   {
//     struct sa_freeblock* block = page->root_free_block;
//     while (block)
//     {
//       if (block->size >= total_size)
//       {
//         if (block->size > total_size)
//         {
//           _trim_free_block(page, block, block->size - total_size);
//         }

//         struct sa_block* header = (struct sa_block*)((uchar*)page->capacity + block->offset);
//         void*               ptr    = (void*)(header + 1);

//         // insert bookkeeping info
//         header->page               = page;
//         header->size               = size;
//         header->offset             = block->offset;
//         header->alignment_exponent = 0; // 2 ^ 0 = 1

//         return ptr;
//       }
//       block = block->next;
//     }

//     page = page->next;
//   }

//   if (!page)
//   {
//     struct sa_page* new_page = NULL;
//     re_alloc_and_link_page(&new_page, size);
//     return sa_alloc(size);
//   }
//   return null;
// }

// void
// _sort_and_merge_free_blocks(struct sa_page* page)
// {
//   // struct sa_freeblock* sorted = NULL;
//   // struct sa_freeblock* curr   = page->root_free_block;

//   // while (curr)
//   // {
//   //   struct sa_freeblock* next = curr->next;

//   //   if (!sorted || curr->offset < sorted->offset)
//   //   {
//   //     curr->next = sorted;
//   //     sorted     = curr;
//   //   }
//   //   else
//   //   {
//   //     struct sa_freeblock* s = sorted;
//   //     while (s->next && s->next->offset < curr->offset)
//   //     {
//   //       s = s->next;
//   //     }
//   //     curr->next = s->next;
//   //     s->next    = curr;
//   //   }

//   //   curr = next;
//   // }

//   // page->root_free_block = sorted;

//   // struct sa_freeblock* block = sorted;
//   // while (block && block->next)
//   // {
//   //   if (block->offset + block->size == block->next->offset)
//   //   {
//   //     block->size += block->next->size;

//   //     struct sa_freeblock* to_free = block->next;
//   //     block->next                     = to_free->next;

//   //     free(to_free);
//   //   }
//   //   else
//   //   {
//   //     block = block->next;
//   //   }
//   // }
// }

// void*
// sa_realloc(void* old_ptr, size_t new_size)
// {
//   void* new_ptr = sa_alloc(new_size);
//   memmove(new_ptr, old_ptr, _get_block_size(old_ptr));
//   sa_free(old_ptr);
//   return new_ptr;
// }

// void
// sa_free(void* ptr)
// {
//   struct sa_block* header = (struct sa_block*)((uchar*)ptr - sizeof(struct sa_block));

//   struct sa_freeblock* last_block = header->page->root_free_block;
//   while (last_block->next)
//   {
//     last_block = last_block->next;
//   }

//   struct sa_freeblock* new_node = malloc(sizeof(struct sa_freeblock));
//   re_zero_structp(new_node);

//   new_node->size   = header->size + sizeof(struct sa_freeblock);
//   new_node->offset = header->offset;
//   new_node->next   = NULL;

//   new_node->next                = header->page->root_free_block;
//   header->page->root_free_block = new_node;

//   _sort_and_merge_free_blocks(header->page);

//   if (header->page->root_free_block == NULL)
//   {
//     re_free_page(header->page);
//   }
// }

char*
strerror(int errcode)
{
  (void)errcode;
  return (char*)"";
}
