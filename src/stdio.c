#include "../include/stdio.h"

FILE*
fmemopen(void* RE_RESTRICT buffer, size_t size, const char* mode)
{
  FILE* fp = (FILE*)malloc(sizeof(FILE));
  return fp;
}

void
fclose(FILE* fp)
{
}
