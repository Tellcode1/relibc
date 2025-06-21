#include "../include/file.h"
#include "../include/string.h"

#if (RE_FILE_API_AVAILABLE)

#  ifdef WIN32
#    include <io.h>
#    include <sys/stat.h>
#    include <windows.h>
#    define access _access
#    define FSTAT _fstat
#    define STAT stat
#    define FILENO _filen
#  else
#    include <dirent.h>
#    include <fcntl.h>
#    include <sys/stat.h>
#    define FSTAT fstat
#    define STAT stat
#    define FILENO fileno
#  endif

int
re_fs_perms_to_win_perms(re_fs_permission perms, bool for_directory)
{
  (void)for_directory;
  switch (perms)
  {
    case RE_FS_PERMISSION_EXISTS: return 0;
    case RE_FS_PERMISSION_READ_ONLY: return 4;
    case RE_FS_PERMISSION_WRITE_ONLY: return 2;
    case RE_FS_PERMISSION_READ_WRITE: return 6;
    default: return -1;
  }
}

int
re_fs_perms_to_unix_perms(re_fs_permission perms, bool for_directory)
{
  int mode = -1;

#  ifndef _WIN32
  switch (perms)
  {
    case RE_FS_PERMISSION_EXISTS: mode = S_IRUSR | S_IWUSR; break;
    case RE_FS_PERMISSION_READ_ONLY: mode = S_IRUSR | S_IRGRP | S_IROTH; break;
    case RE_FS_PERMISSION_WRITE_ONLY: mode = S_IWUSR | S_IWGRP | S_IWOTH; break;
    case RE_FS_PERMISSION_READ_WRITE: mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; break;
    default: mode = S_IRUSR | S_IWUSR; break;
  }

  if (for_directory)
  {
    if (mode & S_IRUSR) { mode |= S_IXUSR; }
    if (mode & S_IRGRP) { mode |= S_IXGRP; }
    if (mode & S_IROTH) { mode |= S_IXOTH; }
  }
#  endif

  return mode;
}

static inline int
_re_fs_perms_to_sys_perms(re_fs_permission perms, bool for_directory)
{
#  ifdef _WIN32
  return re_fs_perms_to_win_perms(perms, for_directory);
#  else
  return re_fs_perms_to_unix_perms(perms, for_directory);
#  endif
}

bool
re_fs_file_exists(const char* fpath)
{
  return (access(fpath, _re_fs_perms_to_sys_perms(RE_FS_PERMISSION_EXISTS, false)) == 0);
}

re_error
re_fs_file_size(const char* fpath, size_t* out_size)
{
  struct STAT st;
  if (STAT(fpath, &st) == 0)
  {
    *out_size = (size_t)st.st_size;
    return RE_SUCCESS;
  }
  *out_size = 0;
  return RE_ERROR_IO_ERROR;
}

re_error
re_fs_file_get_access_time(const char* fpath, size_t* out_atime)
{
  struct STAT st;
  if (STAT(fpath, &st) == 0)
  {
    *out_atime = (size_t)st.st_atime;
    return RE_SUCCESS;
  }
  *out_atime = 0;
  return RE_ERROR_IO_ERROR;
}

re_error
re_fs_file_get_modified_time(const char* fpath, size_t* out_atime)
{
  struct STAT st;
  if (STAT(fpath, &st) == 0)
  {
    *out_atime = (size_t)st.st_mtime;
    return RE_SUCCESS;
  }
  *out_atime = 0;
  return RE_ERROR_IO_ERROR;
}

re_error
re_fs_file_read_all(const char* fpath, char** buffer, size_t* buffer_size)
{
  re_error code = RE_SUCCESS;
  size_t   size = 0;

  *buffer = NULL;
  if (buffer_size) { *buffer_size = 0; }

  FILE* fp = fopen(fpath, "rb");
  if (!fp) { return RE_ERROR_IO_ERROR; }

  if ((code = re_fs_file_size(fpath, &size)) != RE_SUCCESS)
  {
    fclose(fp);
    return code;
  }

  *buffer = malloc(size + 1);
  if (!*buffer)
  {
    fclose(fp);
    return RE_ERROR_MALLOC_FAILED;
  }

  if (fread(*buffer, 1, size, fp) != size)
  {
    free(*buffer);
    *buffer = NULL;
    fclose(fp);
    return RE_ERROR_IO_ERROR;
  }

  (*buffer)[size] = '\0';

  if (buffer_size) { *buffer_size = size; }

  fclose(fp);
  return RE_SUCCESS;
}

re_error
re_fs_file_write_all(const char* fpath, const void* data, size_t data_size)
{
  FILE* fp = fopen(fpath, "wb");
  if (!fp) { return RE_ERROR_IO_ERROR; }

  if (fwrite(data, 1, data_size, fp) != data_size)
  {
    fclose(fp);
    return RE_ERROR_IO_ERROR;
  }

  fclose(fp);
  return RE_SUCCESS;
}

re_error
re_fs_file_create(const char* fpath)
{
#  ifndef _WIN32
  int fd = open(fpath, O_RDWR | O_CREAT | O_EXCL, 0755);
  if (fd < 0 && errno != EEXIST) { return RE_ERROR_IO_ERROR; }
  close(fd);

#  else
  HANDLE bimbows_file_handle = CreateFileA(fpath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

  if (bimbows_file_handle == INVALID_HANDLE_VALUE)
  {
    DWORD error = GetLastError();
    if (error != ERROR_FILE_EXISTS && error != ERROR_ALREADY_EXISTS) { return RE_ERROR_IO_ERROR; }
  }
  else { CloseHandle(bimbows_file_handle); }

#  endif

  return RE_SUCCESS;
}

re_error
re_fs_file_delete(const char* fpath)
{
  if (remove(fpath) != 0) { return RE_ERROR_IO_ERROR; }
  return RE_SUCCESS;
}

re_error
re_fs_dir_create(const char* dpath, re_fs_permission perms)
{
#  ifndef _WIN32
  if (mkdir(dpath, _re_fs_perms_to_sys_perms(perms, true)) != 0 && errno != EEXIST) { return RE_ERROR_IO_ERROR; }
#  else
  (void)perms;

  if (!CreateDirectoryA(dpath, NULL))
  {
    DWORD err = GetLastError();
    if (err != ERROR_ALREADY_EXISTS) { return RE_ERROR_IO_ERROR; }
  }
#  endif
  return RE_SUCCESS;
}

re_error
re_fs_dir_create_recursive(const char* dpath, re_fs_permission perms)
{
  const size_t dirlen    = strlen(dpath);
  char*        path_copy = calloc(1, dirlen + 1);
  char*        buffer    = calloc(1, dirlen + 1);

  strlcpy(path_copy, dpath, dirlen + 1);

  char* last_separator = strrchr(path_copy, '/');
  if (last_separator != NULL)
  {
    *last_separator = '\0';

    strlcpy(buffer, path_copy, dirlen + 1);
    size_t len = strlen(buffer);

    if (buffer[len - 1] == '/') { buffer[len - 1] = 0; }

    for (char* p = buffer + 1; *p; p++)
    {
      if (*p == '/')
      {
        *p = 0;
        re_fs_dir_create(buffer, perms);
        *p = '/';
      }
    }

    re_fs_dir_create(buffer, perms);
  }

  re_fs_dir_create(buffer, perms);
  free(path_copy);
  free(buffer);

  return RE_SUCCESS;
}

re_error
re_fs_dir_create_recursive_for_file(const char* fpath, re_fs_permission perms)
{
  const size_t pathlen   = strlen(fpath);
  char*        path_copy = calloc(1, pathlen + 1);
  char*        buffer    = calloc(1, pathlen + 1);

  strlcpy(path_copy, fpath, pathlen + 1);

  char* last_separator = strrchr(path_copy, '/');
  if (last_separator != NULL)
  {
    *last_separator = '\0';

    strlcpy(buffer, path_copy, pathlen + 1);
    size_t len = strlen(buffer);

    if (buffer[len - 1] == '/') { buffer[len - 1] = 0; }

    char* start = buffer;
    if (buffer[0] == '/') { start = buffer + 1; }

    for (char* p = start; *p; p++)
    {
      if (*p == '/')
      {
        *p = 0;

        if (re_fs_dir_create(buffer, perms) != RE_SUCCESS)
        {
          free(path_copy);
          free(buffer);
          return RE_ERROR_IO_ERROR;
        }

        *p = '/';
      }
    }

    if (re_fs_dir_create(buffer, perms) != RE_SUCCESS)
    {
      free(path_copy);
      free(buffer);
      return RE_ERROR_IO_ERROR;
    }
  }

  free(path_copy);
  free(buffer);

  return RE_SUCCESS;
}

#  define _max(x, y) ((x) > (y) ? (x) : (y))

re_error
re_fs_dir_delete_recursive(const char* dpath)
{
#  ifndef _WIN32
  DIR* dir = opendir(dpath);
  if (!dir) { return RE_ERROR_IO_ERROR; }

  char*    fullpath = calloc(1, _max(PATH_MAX, strlen(dpath)));
  re_error code     = RE_SUCCESS;

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }

    snprintf(fullpath, sizeof(fullpath), "%s/%s", dpath, entry->d_name);

    struct stat st;
    if (stat(fullpath, &st) == 0)
    {
      if (S_ISDIR(st.st_mode)) { code = re_fs_dir_delete_recursive(fullpath); }
      else { code = re_fs_file_delete(fullpath); }

      if (RE_UNLIKELY(code != RE_SUCCESS))
      {
        closedir(dir);
        free(fullpath);
        return code;
      }
    }
  }
  closedir(dir);
  remove(dpath);

  free(fullpath);

#  else
  char pattern[MAX_PATH];
  snprintf(pattern, MAX_PATH, "%s\\*", dpath);

  WIN32_FIND_DATAA fd;
  HANDLE           bimbows_find_file = FindFirstFileA(pattern, &fd);
  if (bimbows_find_file == INVALID_HANDLE_VALUE) { return RE_ERROR_IO_ERROR; }

  re_error code = RE_SUCCESS;

  do
  {
    if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) { continue; }

    char fullpath[MAX_PATH];
    snprintf(fullpath, MAX_PATH, "%s\\%s", dpath, fd.cFileName);

    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { code = re_fs_dir_delete_recursive(fullpath); }
    else { DeleteFileA(fullpath); }

    if (code != RE_SUCCESS)
    {
      FindClose(bimbows_find_file);
      return code;
    }
  } while (FindNextFileA(bimbows_find_file, &fd));
  FindClose(bimbows_find_file);

  RemoveDirectoryA(dpath);

#  endif

  return RE_SUCCESS;
}

#endif