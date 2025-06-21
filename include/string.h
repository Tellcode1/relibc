#ifndef RE_STRING_H_
#define RE_STRING_H_

// implementation: core.c

#include "attributes.h"
#include "chrclass.h"
#include "stdafx.h"
#include "strconv.h"
#include "sysalloc/allocator.h"
#include <stddef.h>
#include <stdint.h>

#define _external extern
#ifdef __cplusplus
#  define _re_except(status) noexcept(status)
#else
#  define _re_except(status)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// Whether to use the __builtin functions provided by GNU C
// They are generally faster, so no reason not to?
// Can only work if the program is compiling with GNU C
#ifndef RE_STRING_USE_BUILTIN
#  define RE_STRING_USE_BUILTIN false
#endif

#define alloc_struct(struc) (calloc(sizeof(struc)))
#define zero_struct(struc) (memset(&struc, 0, sizeof(struc)))
#define zero_structp(struc) (memset(struc, 0, sizeof(*struc)))

#if RE_STRING_USE_BUILTIN && defined(__GNUC__) && defined(__has_builtin)
#  define RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(fn, ...)                                                                                                                                                                                            \
    do                                                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
      if (__has_builtin(__builtin_##fn)) { return __builtin_##fn(__VA_ARGS__); }                                                                                                                                                                         \
    } while (0);
#else
#  define RE_STRING_RETURN_WITH_BUILTIN_IF_AVAILABLE(fn, ...)
#endif

  /**
   *  @brief sets 'sz' bytes of 'dst' to 'to'
   *  @return null on error and dst for success
   */
  _external void* memset(void* dst, int to, size_t dst_size);

#define bzero(dst, sz) memset((dst), 0, (sz))

  /**
   *  @brief copy memory from src to dst
   */
  _external void* memmove(void* dst, const void* src, size_t sz);

/**
 * memcpy is marginally faster than memmove, but also has issues on overlapping regions of memory
 * So, we ditch memcpy for memmove for the sake of safety.
 */
#define memcpy memmove

  /**
   *  @return a pointer to the first occurance of chr in p
   *  searches at most psize bytes of p
   */
#ifdef __cplusplus
  _external const void* memchr(const void* ptr, int chr, size_t psize) _re_except(true);
#else
_external const void* memchr(const void* ptr, int chr, size_t psize);
#endif

  /**
   *  @return non zero if p1 is not equal to p2
   */
  _external int memcmp(const void* ptr1, const void* ptr2, size_t max);

  /**
   *  get the size of the string
   *  the size is determined by the position of the null terminator.
   */
  _external size_t strlen(const char* s);

  /**
   * @brief Get the length of the string, reading no more than max characters
   * @return size_t The length of the string
   */
  _external size_t strnlen(const char* s, size_t max);

  /**
   * Copy from src to dst ensuring NULL termination using the length of the dst buffer.
   *  Will copy not more than dst_size - 1 characters from source.
   */
  _external size_t strlcpy(char* dst, const char* src, size_t dst_size);

  /**
   *  copy from src to dst, stopping once it hits the null terminator in src
   */
  _external char* strcpy(char* dst, const char* src);

  /**
   *  copies min(strlen(dst), min(strlen(src), max)) chars.
   *  ie. the least of the lengths and the max chars
   */
  _external char* strncpy(char* dst, const char* src, size_t max);

/* https://manpages.debian.org/testing/linux-manual-4.8/strscpy.9.en.html */
#define strscpy strncpy

  /**
   *  concatenate src to dst
   */
  _external char* strcat(char* dst, const char* src);

  /**
   *  concatenate src to dst while copying no more than max characters.
   */
  _external char* strncat(char* dst, const char* src, size_t max);

  /**
   *  copy from src to dst while ensuring that there are no more than dest_size characters in dst
   *  if strlen(dst) > dest_size, this function will return and do nothing.
   *  @return the length of the string it TRIED to create.
   */
  _external size_t strcat_max(char* dst, const char* src, size_t dest_size);

/* strcat_max is functionally equivalent to strlcat */
#define strlcat strcat_max

  /**
   *  @return the number of characters copied.
   */
  _external size_t strncpy2(char* dst, const char* src, size_t max);

  /**
   * @brief Copy characters from src to dst
   * @return The pointer to the NULL terminator of dst
   */
  _external char* stpcpy(char* dst, const char* src);

  /**
   * @brief Remove leading and trailing whitspaces from string
   * @return s if success, NULL if not.
   */
  _external char* strtrim(char* s);

  /**
   * @brief A constant version of strtrim. begin will contain the first non whitespace character and end will contain the last non space char.
   * @param begin may be NULL
   * @param end may be NULL
   * @return s if success, NULL if not.
   */
  _external const char* strtrim_c(const char* s, const char** begin, const char** end);

  /**
   *  @brief compare two strings, stopping at either s1 or s2's null terminator or at max.
   *  it will stop when it reaches the null terminator, no segv
   */
  _external int strncmp(const char* s1, const char* s2, size_t max);

  /**
   *  @brief compare two strings case insensitively
   *  does not care whether s1 or s2 has 'a' or 'A', they're the same thing
   */
  _external int strcasencmp(const char* s1, const char* s2, size_t max);

  /**
   *  @brief case insensitively compare no more than max chars
   */
  _external int strcasecmp(const char* s1, const char* s2);

  /**
   *  @brief find the first occurence of a character in a string
   *  @return NULL if chr is not in s or there is no n-th occurence of chr.
   */
  _external const char* strchr(const char* s, int chr) _re_except(true);

  /**
   *  @brief Find the n-th occurence of a character in a string.
   *  @return NULL if chr is not in s or there is no n-th occurence of chr.
   */
  _external char* strchr_n(const char* s, int chr, int n);

  /**
   *  @brief find the last occurence of a character in a string
   *  @sa use strstr if you want to find earliest occurence of a string in a string
   */
  _external char* strrchr(const char* s, int chr) _re_except(true);

  /**
   *  strchr() but string
   *  @brief find the earlier occurence of a (sub)string in a string.
   *  eg. for s baller and sub ll
   *  @return a pointer to the first l
   */
  _external const char* strstr(const char* s, const char* sub) _re_except(true);

  /**
   *  @brief copy two strings, ensuring null termination
   *  WARNING: use strlcpy, noob.
   */
  _external size_t strcpy2(char* dst, const char* src);

  /**
   *  @return 0 when they are equal,
   *  positive number if the lc (last character) of s1 is greater than lc of s2
   *  negative number if the lc (last character) of s1 is less than lc of s2
   */
  _external int strcmp(const char* s1, const char* s2);

  /**
   *  @brief return the number of characters after which 's' contains a character in 'reject'
   *  for example,
   *  s is balling, reject is hello
   *  so, strcspn will return 2 because after b and a,
   *  l is in both s and reject.
   */
  _external size_t strcspn(const char* s, const char* reject);

  /**
   *  @brief return the number of characters after which s does not contain a character in accept
   *  ie. the number of similar characters they have.
   *  for example,
   *  s is balling and accept is ball
   *  strspn will return 4 because ball is found in both s and accept and it is of 4 characters.
   */
  _external size_t strspn(const char* s, const char* accept);

  /**
   * @brief return a pointer ot the first character in s1 that is also in s2.
   *
   */
  _external const char* strpbrk(const char* s1, const char* s2) _re_except(true);

  /**
   *  warning: modifies s directly.
   *  @brief splits 's' by a delimiter
   *  so if you have obama-is-good-gamer,
   *  it'll first return to you 'obama', then 'is' them 'good' then 'gamer'
   *  you should pass null instead of the string for chaining calls
   *  like:
   *  char buf[] = "obama-care-gaming";
   *  char *ptr = strtok(buf, '-'); * this will return 'obama'
   *  while (ptr != null) {
   *    ptr = strtok(null, '-'); * this will first return care, and in second iteration, return gaming
   *  }
   *  context must be declared typically on the stack that the string is declared, as a char *
   *  You must pass the address of the char * to this function.
   *  INFO: This is more in line with strtok_r. We use it because it comes at literally no cost to the user.
   */
  _external char* _re_strtokctx;
  _external char* strtok(char* s, const char* delim);

  _external char* strtok_r(char* s, const char* delim, char** context);

/* As strtok is functionally equivalent to strtok_r, we just define to allow for idiots who don't want to open this header to use it. */
#define strtok_r(...) (strtok(__VA_ARGS__))

  /**
   * @brief Replace every occurence of to_replace in string with replace_with
   * @return Pointer to NULL terminator of s
   */
  _external char* strreplace(char* s, char to_replace, char replace_with);

  /**
   *  @brief @return the name of the file
   *  basically, ../../pdf/nuclearlaunchcodes.pdf would give you nuclearlaunchcodes.pdf in return.
   */
  _external const char* basename(const char* path) _re_except(true);

  /**
   *  @brief duplicate a string (using malloc)
   *  and return it
   */
  _external char* strdup(const char* s);

  /**
   *  @brief duplicate a string (using malloc) with extra space
   *  Note that the NULL terminator is excluded from 'size', i.e. one extra byte will be allocated at the end for the terminator.
   */
  _external char* strexdup(const char* s, size_t size);

  /**
   *  @brief make a substring of the string s
   *  the returned string is malloc'd and must be freed by the caller.
   */
  _external char* substr(const char* s, size_t start, size_t len);

  /**
   * @brief Reverse a string, in place, no copying to a buffer or whatever.
   */
  _external char* strrev(char* str);

  /**
   * @brief Reverse a string, in place, copying no more than max chars.
   *  Note that the NULL terminator is not touched, only the characters before it.
   */
  _external char* strnrev(char* str, size_t max);

  _external char* strerror(int errcode);

#ifdef __cplusplus
}
#endif

#endif //__RE_STRING_H__
