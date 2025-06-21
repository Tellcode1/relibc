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

/* Utilities for handling files that should have been part of the standard library. */

#ifndef RE_FILE_H_INCLUDED_
#define RE_FILE_H_INCLUDED_

#include "errorcodes.h"
#include "stdafx.h"

#if !defined(_WIN32)
#  include <unistd.h>
#  ifndef _POSIX_VERSION
#    warning "No POSIX implementation was found, and the OS is not windows, can not use the file API on this platform"
#    define RE_FILE_API_AVAILABLE false
#  endif
#else
#  define RE_FILE_API_AVAILABLE true
#endif

#ifndef RE_FILE_API_AVAILABLE
#  define RE_FILE_API_AVAILABLE true
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  typedef enum re_fs_permission
  {
    RE_FS_PERMISSION_EXISTS     = 0,
    RE_FS_PERMISSION_READ_ONLY  = 4,
    RE_FS_PERMISSION_WRITE_ONLY = 2,
    RE_FS_PERMISSION_READ_WRITE = 6,
  } re_fs_permission;

  extern bool re_fs_file_exists(const char* fpath);

  /**
   * Returns -1 on invalid input.
   * Windows permissions are available even on unix
   */
  extern int re_fs_perms_to_win_perms(re_fs_permission perms, bool for_directory);

  /**
   * Returns -1 on invalid input or
   * if the platform is windows.
   */
  extern int re_fs_perms_to_unix_perms(re_fs_permission perms, bool for_directory);

  /**
   * If file does not exist, out_size is set to 0.
   */
  extern re_error re_fs_file_size(const char* fpath, size_t* out_size);

  /**
   * If file does not exist, out_atime is set to 0
   */
  extern re_error re_fs_file_get_access_time(const char* fpath, size_t* out_atime);

  /**
   * Get the time that the file was last modified in.
   */
  extern re_error re_fs_file_get_modified_time(const char* fpath, size_t* out_mtime);

  /**
   * Read all of the data of a file into a buffer, allocated with 'alloc'
   * Note that the buffer is NULL terminated and the terminator is not included in buffer_size
   */
  extern re_error re_fs_file_read_all(const char* fpath, char** buffer, size_t* buffer_size);

  /**
   * If the user has passed in a directory that does not exist, this function will fail
   * and return RE_ERROR_INVALID_OPERATION. You need to create all parent directories before writing.
   */
  extern re_error re_fs_file_write_all(const char* fpath, const void* data, size_t data_size);

  /**
   * Create an empty file.
   * You will need to create all parent directories before calling this function.
   */
  extern re_error re_fs_file_create(const char* fpath);

  /**
   * If the file does not exist, returns RE_ERROR_FILE_NOT_FOUND, though technically not an error.
   * It is invalid to attempt to remove non-empty directories through this function.
   */
  extern re_error re_fs_file_delete(const char* fpath);

  /**
   * Must create parent directories before calling this function.
   */
  extern re_error re_fs_dir_create(const char* dpath, re_fs_permission perms);

  /**
   * Recursively create all parent directories of the directory path specified.
   * It is invalid to call this with a 'dpath' that references a file, not a directory as this function
   * will create a directory with the name of the file (and overwrite it).
   */
  extern re_error re_fs_dir_create_recursive(const char* dpath, re_fs_permission perms);

  /**
   * Create the directories for a file, but not the file itself.
   */
  extern re_error re_fs_dir_create_recursive_for_file(const char* fpath, re_fs_permission perms);

  /**
   * Though confusing, returns RE_ERROR_FILE_NOT_FOUND if the directory does not
   * exist.
   * WARNING: Removes everything from the directory. (Of which we have permission to delete)
   * If a file exists in the directory which we do not have permissions to delete, the file will
   * not be deleted and RE_ERROR_INVALID_OPERATION is returned.
   */
  extern re_error re_fs_dir_delete_recursive(const char* dpath);

#ifdef __cplusplus
}
#endif

#endif //__RE_FILE_H__
