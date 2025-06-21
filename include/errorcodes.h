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

#ifndef RE_ERROR_CODES_H_INCLUDED_
#define RE_ERROR_CODES_H_INCLUDED_

#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef enum re_error
  {
    /* A shorthand alternative to NV_ERROR_SUCCESS */
    RE_SUCCESS       = 0,
    RE_ERROR_SUCCESS = RE_SUCCESS,

    /**
     * Invalid argument to function, possibly indirectly.
     */
    RE_ERROR_INVALID_ARG = 1,

    /**
     * A memory allocation has failed. May even be from a stack allocator.
     */
    RE_ERROR_MALLOC_FAILED = -1,

    /**
     * A miscellaneous IO error. These should not typically happen, and may indicate errors in something external.
     */
    RE_ERROR_IO_ERROR = 3,

    /**
     * We are sure the error isn't from our side.
     */
    RE_ERROR_EXTERNAL = 4,

    RE_ERROR_FILE_NOT_FOUND = 5,

    /**
     * The stored cache has been invalidated. This typically should induce a cache rebuild.
     */
    RE_ERROR_INVALID_CACHE = 6,

    /**
     * Our stored state is now broken. This typically is the result of a buffer overflow.
     */
    RE_ERROR_BROKEN_STATE = 7,

    /**
     * Input to the program is invalid, invalid argument by the user.
     */
    RE_ERROR_INVALID_INPUT = 8,

    /**
     * A function returned an invalid value
     */
    RE_ERROR_INVALID_RETVAL = 9,

    /**
     * An unknown error.
     * This typically is the result of something that is out of hand of the returning function.
     * You should avoid returning this, as it is really opaque as to what went wrong.
     */
    RE_ERROR_UNKNOWN = 10,

    /**
     * The operation requested (in/directly) is illegal.
     * The user asked for something they should or could not have.
     */
    RE_ERROR_INVALID_OPERATION = 11,
  } re_error;

  static inline const char*
  nv_error_str(re_error code)
  {
    switch (code)
    {
      case RE_ERROR_SUCCESS: return "Success";
      case RE_ERROR_INVALID_ARG: return "Invalid arg";
      case RE_ERROR_MALLOC_FAILED: return "malloc failed";
      case RE_ERROR_IO_ERROR: return "IO error";
      case RE_ERROR_EXTERNAL: return "External";
      case RE_ERROR_FILE_NOT_FOUND: return "File not found";
      case RE_ERROR_INVALID_CACHE: return "Invalid cache";
      case RE_ERROR_BROKEN_STATE: return "Broken state";
      case RE_ERROR_INVALID_INPUT: return "Invalid input";
      case RE_ERROR_INVALID_RETVAL: return "Invalid retval";
      case RE_ERROR_UNKNOWN: return "Unknown";
      case RE_ERROR_INVALID_OPERATION: return "Invalid operation";
      default: return "(NotAnErrorCode)";
    }
  }

  RE_STATIC_ASSERT(sizeof(re_error) == sizeof(int), sizeof_erroc_must_be_sizeof_int);

#ifdef __cplusplus
}
#endif

#endif //__RE_ERROR_CODES_H__
