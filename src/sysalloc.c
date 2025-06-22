#include "../include/string.h"
#include "../include/sysalloc/allocator.h"
#include <stdio.h>
#include <unistd.h>

void*
malloc(size_t size)
{
  void* ret = heap + heap_bumper;
  heap_bumper += size;
  char buffer[128];
  itoa(size, buffer, 10, sizeof(buffer), false);
  strlcat(buffer, "\n", sizeof(buffer));
  write(STDOUT_FILENO, buffer, strlen(buffer));
  return ret;
}

void*
calloc(size_t groups, size_t group_size)
{
  size_t size = groups * group_size;
  void*  ret  = heap + size;
  heap_bumper += size;
  memset(ret, 0, size);
  return ret;
}

void*
realloc(void* oldp, size_t new_size)
{
  free(oldp);
  return malloc(new_size);
}

void
free(void* block)
{
  (void)block;
}
