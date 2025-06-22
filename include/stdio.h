#ifndef _RE_STDIO_H
#define _RE_STDIO_H

#include <stddef.h>
#include "stdafx.h"
#include "errorcodes.h"

typedef long    ssize_t;
typedef ssize_t fpos_t;
typedef ssize_t off_t;

typedef struct _re_stream FILE;

FILE* stdin  = NULL;
FILE* stderr = NULL;
FILE* stdout = NULL;

// POSIX requires these be macros for some reason.
#define stdin stdin
#define stderr stderr
#define stdout stdout

typedef re_error (*re_stream_read_fn)(FILE* stream, void* buffer, size_t read_bytes);
typedef re_error (*re_stream_write_fn)(FILE* stream, const void* buffer, size_t write_bytes);

struct _re_stream
{
  void*  buffer;
  size_t buffer_size;
  off_t  buffer_offset;

  fpos_t pos;

  re_stream_read_fn  read;
  re_stream_write_fn write;
};

#define _IOFBF 1
#define _IOLBF 0 // default
#define _IONBF 2

#define SEEK_CUR -3
#define SEEK_END -1
#define SEEK_SET -2

#define FILENAME_MAX 4096
#define FOPEN_MAX 128

#define EOF -1

#define P_tmpdir "/tmp"

extern FILE* fmemopen(void* RE_RESTRICT buffer, size_t size, const char* mode);

extern void fclose(FILE* fp);

#endif //_RE_STDIO_H
