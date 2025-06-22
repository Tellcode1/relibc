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

#ifndef RE_STRING_CONVERT_H_INCLUDED_
#define RE_STRING_CONVERT_H_INCLUDED_

#include "stdafx.h"
#include <limits.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef RE_MAX_IGNORE
#  define RE_MAX_IGNORE __SIZE_MAX__
#endif

#ifndef RE_SEPERATOR_CHAR
#  define RE_SEPERATOR_CHAR ','
#endif

  /**
   * @brief Converts an integer to ASCII.
   *
   * @param max Maximum number of characters to write.
   * @return The number of characters written (excluding null terminator).
   */
  extern size_t itoa2(intmax_t num, char* out, int base, size_t max, bool add_commas);

  /**
   * @brief Converts an unsigned integer to ASCII.
   */
  extern size_t utoa2(uintmax_t num, char* out, int base, size_t max, bool add_commas);

  /**
   * @brief Converts a double to ASCII.
   *
   * @param precision Number of digits after the decimal point.
   * @param remove_zeroes If true, trailing zeroes are removed.
   * @return number of characters written. (excluding null terminator)
   */
  extern size_t ftoa2(double num, char* out, int precision, size_t max, bool remove_zeros);

  /**
   * @brief Converts a pointer to ASCII.
   */
  extern size_t ptoa2(void* ptr, char* out, size_t max);

  /**
   * @brief Converts a byte count to ASCII.
   *
   * Supports upgrade modes (e.g. converting 1000 bytes to "1KB", etc.).
   * Up to 1 petabyte is supported. It can go farther but it is undefined behaviour
   * Writes the bytes (using utoa) and writes the suffix ( B/KB/MB/GB/PB )
   * @return The number of characters written.
   */
  extern size_t btoa2(size_t num_bytes, bool upgrade, char* out, size_t max);

  /**
   * @brief Converts a string to an integer.
   */
  extern int atoi(const char* in_string);

  extern double atof2(const char* in_string, size_t max);

  /**
   * @brief Converts a string to a double.
   */
  static inline double
  atof(const char* in_string)
  {
    return atof2(in_string, SIZE_MAX);
  }

  /**
   * @brief Converts a string to a boolean.
   */
  extern bool atobool(const char* in_string, size_t max);

  static inline char*
  itoa(intmax_t x, char* out, int base, size_t max, bool add_commas)
  {
    itoa2(x, out, base, max, add_commas);
    return out;
  }

  static inline char*
  utoa(uintmax_t x, char* out, int base, size_t max, bool add_commas)
  {
    utoa2(x, out, base, max, add_commas);
    return out;
  }

  static inline char*
  ftoa(double x, char* out, int precision, size_t max, bool remove_zeroes)
  {
    ftoa2(x, out, precision, max, remove_zeroes);
    return out;
  }

  static inline char*
  ptoa(void* p, char* buf, size_t max)
  {
    ptoa2(p, buf, max);
    return buf;
  }

  static inline char*
  btoa(size_t x, bool upgrade, char* buf, size_t max)
  {
    btoa2(x, upgrade, buf, max);
    return buf;
  }

#ifdef __cplusplus
}
#endif

#endif //__RE_STRING_CONV_H__
