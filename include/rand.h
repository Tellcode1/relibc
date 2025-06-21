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

#ifndef RE_RAND_H_
#define RE_RAND_H_

#include "attributes.h"
#include "errorcodes.h"
#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Must be larger than re_rand_t
 */
#ifndef _RE_RAND_TMP_CONVERT_TYPE
#  define _RE_RAND_TMP_CONVERT_TYPE __uint128_t
#endif

  /**
   * The type that this library works with
   * Also the type of the seed
   * Note that this isn't a changeable constant.
   * Most random functions are specialized to bit width, and
   * so would require specialization which I do not have the time to do.
   */
  typedef u64                 re_rand_t;
  typedef struct re_rand_info re_rand_info_t;

  struct re_rand_info
  {
    re_rand_t state[4];
  };

  /**
   * For 1000000000 iterations, normal rand() takes 14.829721 seconds while this takes 3.230303 seconds at -O2.
   * Yeah, the results are skewed but shut up, blazingly fast.
   */
  extern re_error re_random_bulk_range(re_rand_info_t* info, re_rand_t* outbuf, size_t buf_num_elements, size_t min, size_t max) RE_ATTR_NONNULL(1, 2);

  static inline RE_ATTR_NONNULL(1) re_rand_t re_random(re_rand_info_t* info)
  {
    re_rand_t buf[1];
    re_random_bulk_range(info, buf, 1, 0, SIZE_MAX);
    return buf[0];
  }

  static inline RE_ATTR_NONNULL(1) re_rand_t re_random_range(re_rand_info_t* info, re_rand_t min, re_rand_t max)
  {
    re_rand_t buf[1];
    re_random_bulk_range(info, buf, 1, min, max);
    return buf[0];
  }

  static inline RE_ATTR_NONNULL(1, 2) re_error re_random_bulk(re_rand_info_t* info, re_rand_t* outbuf, size_t buf_num_elements)
  {
    return re_random_bulk_range(info, outbuf, buf_num_elements, 0, SIZE_MAX);
  }

  /**
   * Seed the random number generator.
   * Do not use constants, use something like the current time
   * or the process id or something.
   */
  extern void re_random_seed(re_rand_info_t* info, re_rand_t seed) RE_ATTR_NONNULL(1);

#ifdef __cplusplus
}
#endif

#endif // RE_RAND_H_INCLUDED_
