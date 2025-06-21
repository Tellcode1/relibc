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

/* Utilities to classify characters in strings. */
/* An implementation to ctype.h, Notably missing isgraph and ishex(or whatever its called) */

#ifndef __RE_CHRCLASS_H__
#define __RE_CHRCLASS_H__

#include "attributes.h"
#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define nothing

  /**
   * @brief Alphabet?
   */
  extern int isalpha(int chr) nothing;

  /**
   * @brief Digit of a number?
   */
  extern int isdigit(int chr) nothing;

  /**
   * @brief Alphabetical/Numerical?
   */
  extern int isalnum(int chr) nothing;

  /**
   * @brief Space/Tab?
   */
  extern int isblank(int chr) nothing;

  /**
   * @brief Backslash control sequence?
   */
  extern int iscntrl(int chr) nothing;

  /**
   * @brief Lowercase alphabet?
   */
  extern int islower(int chr) nothing;

  /**
   * @brief Uppercase alphabet?
   */
  extern int isupper(int chr) nothing;

  /**
   * @brief Space, newline or tab?
   */
  extern int isspace(int chr) nothing;

  /**
   * @brief Punctuation?
   */
  extern int ispunct(int chr) nothing;

  /**
   * @brief Convert a character to lower. Will return the character if it does not have a lower representation.
   */
  extern int tolower(int chr) nothing;

  /**
   * @brief Convert a character to upper. Will return the character if it does not have a lower representation.
   */
  extern int toupper(int chr) nothing;

#ifdef __cplusplus
}
#endif

#endif //__RE_CHRCLASS_H__
