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

#ifndef RE_PRINT_H_INCLUDED_
#define RE_PRINT_H_INCLUDED_

#include "attributes.h"
#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * default size of the write buffer
 */
#ifndef RE_WBUF_SIZE
#  define RE_WBUF_SIZE 1024
#endif

/* Add commas to integers when printing. */
#ifndef RE_PRINTF_ADD_COMMAS
#  define RE_PRINTF_ADD_COMMAS false
#endif

  /**
   * set the write buffer for printf
   *
   * if buf is NULL and size != 0, it allocates a buffer of 'size' bytes
   * if buf is NULL and size is 0, it allocates a buffer of RE_WBUF_SIZE bytes
   *
   * @param buf ptr to buffer or NULL
   * @param size size of the buffer
   */
  extern void setwbuf(char* buf, int size);

  /**
   * Get the write buffer used by printf.
   * This function MAY return NULL, ensure proper checking!
   * Values in the write buffer may be overriden at any time!
   * Ensure that you sequentially write to the buffer, read it, and then perform any operations!
   * DO NOT WRITE A STRING TO THIS AND USE IT IN FORMATTING YOU MORON!!!
   */
  extern char* getwbuf(void);

  /**
   * set the output stream for printf
   *
   * calls to printf go to this stream. no checks are done.
   *
   * @param stream a valid FILE ptr, no sanity checks are done on stream
   */
  extern void setstdout(FILE* stream);

  /**
   * prints formatted output to the g_stdstream
   * @return number of characters written
   */
  extern int printf(const char* fmt, ...) RE_ATTR_FORMAT(1, 2) RE_ATTR_NONNULL(1);

  /**
   * prints formatted output to a file
   */
  extern int fprintf(FILE* RE_RESTRICT outfile, const char* RE_RESTRICT fmt, ...) RE_ATTR_FORMAT(2, 3) RE_ATTR_NONNULL(1, 2);

  /**
   * prints no more than max_chars to g_stdstream
   */
  extern int nprintf(int max_chars, const char* fmt, ...) RE_ATTR_FORMAT(2, 3) RE_ATTR_NONNULL(2);

  /**
   * prints formatted output to a string
   *
   * note: not recommended. use snprintf instead.
   */
  extern int sprintf(char* RE_RESTRICT dst, const char* RE_RESTRICT fmt, ...) RE_ATTR_FORMAT(2, 3) RE_ATTR_NONNULL(2);

  /**
   * prints formatted output using a va_list
   */
  extern int vprintf(const char* RE_RESTRICT fmt, va_list args) RE_ATTR_NONNULL(2);

  /**
   * prints formatted output using a va_list to a file
   */
  extern int vfprintf(FILE* RE_RESTRICT outfile, const char* RE_RESTRICT fmt, va_list args) RE_ATTR_NONNULL(1, 3);

  /**
   * prints formatted output to a string, writing no more than max_chars
   */
  extern int snprintf(char* RE_RESTRICT dst, size_t max_chars, const char* RE_RESTRICT fmt, ...) RE_ATTR_FORMAT(3, 4) RE_ATTR_NONNULL(3);

  /**
   * prints no more than max_chars to g_stdstream using a va_list
   */
  extern int vnprintf(va_list args, int max_chars, const char* RE_RESTRICT fmt) RE_ATTR_NONNULL(1, 3);

  /**
   * prints no more than max_chars to a string using a va_list
   */
  extern int vsnprintf(char* RE_RESTRICT dst, size_t max_chars, const char* RE_RESTRICT fmt, va_list args) RE_ATTR_NONNULL(3, 4);

  /**
   * the core print function
   *
   * all printf* funcs call this in the end.
   * stops formatting when max_chars is hit.
   *
   * @param dst destination buffer or FILE ptr. It is safe to pass NULL.
   * @return The number of characters that WOULD be written, even if dst is NULL, but truncated to max_chars.
   */
  extern int _vsfnprintf(va_list args, void* RE_RESTRICT dst, bool is_file, int max_chars, const char* RE_RESTRICT fmt) RE_ATTR_NONNULL(1, 5);

#ifdef __cplusplus
}
#endif

#endif // __RE_PRINT_H__
