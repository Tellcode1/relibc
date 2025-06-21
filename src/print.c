#include "../include/print.h"
#include "../include/chrclass.h"
#include "../include/stdafx.h"
#include "../include/strconv.h"

#if (false)

// Moral of the story? FU@# SIZE_MAX
// I spent an HOUR trying to figure out what's going wrong
// and I didn't even bat an eye towards it

int
printf(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  // size_t chars_written = vnprintf(RE_WBUF_SIZE, args, fmt);
  size_t chars_written = _vsfnprintf(args, stdout, 1, INT_MAX, fmt);

  va_end(args);

  return (int)chars_written;
}

int
fprintf(FILE* f, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  size_t chars_written = _vsfnprintf(args, f, 1, INT_MAX, fmt);

  va_end(args);

  return (int)chars_written;
}

int
sprintf(char* dst, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  size_t chars_written = _vsfnprintf(args, dst, 0, INT_MAX, fmt);

  va_end(args);

  return (int)chars_written;
}

int
vprintf(const char* fmt, va_list args)
{
  return _vsfnprintf(args, stdout, 1, INT_MAX, fmt);
}

int
vfprintf(FILE* f, const char* fmt, va_list args)
{
  return _vsfnprintf(args, f, 1, INT_MAX, fmt);
}

int
snprintf(char* dst, size_t max_chars, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  size_t chars_written = vsnprintf(dst, max_chars, fmt, args);

  va_end(args);

  return (int)chars_written;
}

int
nprintf(int max_chars, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  size_t chars_written = _vsfnprintf(args, stdout, 1, (int)max_chars, fmt);

  va_end(args);

  return (int)chars_written;
}

int
vnprintf(va_list args, int max_chars, const char* RE_RESTRICT fmt)
{
  return _vsfnprintf(args, stdout, 1, (int)max_chars, fmt);
}

int
vsnprintf(char* RE_RESTRICT dst, size_t max_chars, const char* RE_RESTRICT fmt, va_list args)
{
  return _vsfnprintf(args, dst, 0, (int)max_chars, fmt);
}

#  define RE_PRINTF_PEEK_FMT() ((info->itr < info->fmt_str_end) ? *info->itr : 0)
#  define RE_PRINTF_PEEK_NEXT_FMT() (((info->itr + 1) < info->fmt_str_end) ? *(info->itr + 1) : 0)
#  define RE_PRINTF_ADVANCE_FMT()                                                                                                                                                                                                                        \
    do                                                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
      if ((info->itr + 1) < info->fmt_str_end) { info->itr++; }                                                                                                                                                                                          \
      else { info->itr = info->fmt_str_end; }                                                                                                                                                                                                            \
    } while (0);
#  define RE_PRINTF_ADVANCE_NUM_CHARACTERS_FMT(num_chars)                                                                                                                                                                                                \
    do                                                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
      if ((info->itr + (num_chars)) < info->fmt_str_end) { info->itr += (num_chars); }                                                                                                                                                                   \
      else { info->itr = info->fmt_str_end; }                                                                                                                                                                                                            \
    } while (0);

typedef struct re_format_info_t
{
  va_list     args;
  void*       dst_file;
  char*       dst_string;
  char*       tmp_writebuffer;
  const char* fmt_str_end;
  const char* itr;
  size_t      chars_written;
  size_t      written;
  size_t      max_chars;
  int         padding;
  int         padding_w;
  int         precision;
  bool        pad_zero;
  bool        left_align;
  bool        file;
  bool        wbuffer_used;
  bool        precision_specified;
  char*       pad_buf;
} re_format_info_t;

static inline void
_printf_write(re_format_info_t* info, const char* write_buffer, size_t written)
{
  if (info->chars_written >= info->max_chars) { return; }

  size_t remaining = info->max_chars - info->chars_written;
  size_t to_write  = (written > remaining) ? remaining : written;
  info->chars_written += to_write;

  /* As we need to return the number of characters printf would have returned, we can't exit before here. */
  if (!write_buffer) { return; }

  if (info->file)
  {
    FILE* file = (FILE*)info->dst_file;
    fwrite(write_buffer, 1, to_write, file);
  }
  else if (info->dst_string)
  {
    if (to_write > 0)
    {
      memcpy(info->dst_string, write_buffer, to_write);
      info->dst_string += to_write;
    }
  }
}

static inline void
_printf_get_padding_and_precision_if_given(re_format_info_t* info)
{
  if (!RE_PRINTF_PEEK_FMT()) { return; }

  if (RE_PRINTF_PEEK_FMT() == '-')
  {
    info->left_align = true;
    RE_PRINTF_ADVANCE_FMT();
  }
  if (RE_PRINTF_PEEK_FMT() == '0')
  {
    info->pad_zero = true;
    RE_PRINTF_ADVANCE_FMT();
  }

  if (RE_PRINTF_PEEK_FMT() == '*')
  {
    info->padding_w = va_arg(info->args, int);
    if (info->padding_w < 0)
    {
      // negative width means left align
      info->left_align = true;
      info->padding_w  = -info->padding_w;
    }
    else
    {
      /* Positive padding width means right padded */
      info->left_align = false;
    }
    RE_PRINTF_ADVANCE_FMT();
  }
  else
  {
    while (isdigit(RE_PRINTF_PEEK_FMT()))
    {
      info->padding_w = info->padding_w * 10 + (RE_PRINTF_PEEK_FMT() - '0');
      RE_PRINTF_ADVANCE_FMT();
    }
  }

  if (RE_PRINTF_PEEK_FMT() == '.')
  {
    RE_PRINTF_ADVANCE_FMT();
    info->precision_specified = 1;
    if (RE_PRINTF_PEEK_FMT() == '*')
    {
      info->precision = va_arg(info->args, int);
      if (info->precision < 0) { info->precision = 6; }
      RE_PRINTF_ADVANCE_FMT();
    }
    else
    {
      info->precision = 0;
      while (isdigit(RE_PRINTF_PEEK_FMT()))
      {
        info->precision = info->precision * 10 + (RE_PRINTF_PEEK_FMT() - '0');
        RE_PRINTF_ADVANCE_FMT();
      }
    }
  }
}

static inline void
_printf_format_process_char(re_format_info_t* info)
{
  if (info->chars_written >= info->max_chars - 1) { return; }

  // if user is asking for literal % sign, *iter will be the percent sign!!
  int chr = (char)(tolower(RE_PRINTF_PEEK_FMT()) == 'c') ? va_arg(info->args, int) : RE_PRINTF_PEEK_FMT();
  if (info->file) { fputc(chr, (FILE*)info->dst_file); }
  else if (info->dst_string)
  {
    *info->dst_string = (char)chr;
    info->dst_string++;
  }
  info->wbuffer_used = false;
  info->chars_written++;
}

static inline void
_printf_handle_long_type(re_format_info_t* info)
{
  if (!RE_PRINTF_PEEK_NEXT_FMT() || RE_PRINTF_PEEK_FMT() != 'l') { return; }

  const bool type_is_long_long_integer = tolower(RE_PRINTF_PEEK_NEXT_FMT()) == 'l';
  if (type_is_long_long_integer)
  {
    /* Move past the extra l */
    RE_PRINTF_ADVANCE_FMT();
    RE_PRINTF_ADVANCE_FMT();

    /* long long integers */
    switch (tolower(RE_PRINTF_PEEK_FMT()))
    {
      case 'd':
      case 'i': info->written = itoa2(va_arg(info->args, long long), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); break;
      case 'u': info->written = utoa2(va_arg(info->args, unsigned long long), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); break;
    }
  }
  else
  {
    RE_PRINTF_ADVANCE_FMT();
    /* standard long integers */
    /* %lF\f is non standard behaviour. Use Lf or LF instead. */
    switch (tolower(RE_PRINTF_PEEK_FMT()))
    {
      case 'd':
      case 'i': info->written = itoa2(va_arg(info->args, long), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); break;
      case 'u': info->written = utoa2(va_arg(info->args, unsigned long), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); break;
    }
  }
}

static inline void
_printf_handle_short_type(re_format_info_t* info)
{
  if (!RE_PRINTF_PEEK_NEXT_FMT() || RE_PRINTF_PEEK_FMT() != 'h') { return; }

  const bool type_is_short_short_integer = tolower(RE_PRINTF_PEEK_NEXT_FMT()) == 'h';
  if (type_is_short_short_integer)
  {
    /* Move past the extra h */
    RE_PRINTF_ADVANCE_FMT();
    RE_PRINTF_ADVANCE_FMT();

    switch (tolower(RE_PRINTF_PEEK_FMT()))
    {
      case 'd':
      case 'i': info->written = itoa2((char)va_arg(info->args, int), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); break;
      case 'u': info->written = utoa2((unsigned char)va_arg(info->args, unsigned int), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); break;
    }
  }
  else
  {
    RE_PRINTF_ADVANCE_FMT();
    switch (tolower(RE_PRINTF_PEEK_FMT()))
    {
      case 'd':
      case 'i': info->written = itoa2((short)va_arg(info->args, long), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); break;
      case 'u': info->written = utoa2((unsigned short)va_arg(info->args, unsigned long), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); break;
    }
  }
}

static inline void
_printf_handle_size_type(re_format_info_t* info)
{
  if (!RE_PRINTF_PEEK_NEXT_FMT() || RE_PRINTF_PEEK_FMT() != 'z') { return; }

  if ((tolower(RE_PRINTF_PEEK_NEXT_FMT()) == 'i' || tolower(RE_PRINTF_PEEK_NEXT_FMT()) == 'd' || tolower(RE_PRINTF_PEEK_NEXT_FMT()) == 'u')) { RE_PRINTF_ADVANCE_FMT(); }

  if (tolower(RE_PRINTF_PEEK_FMT()) == 'i' || tolower(RE_PRINTF_PEEK_FMT()) == 'd') { info->written = re_itoa2(va_arg(info->args, ssize_t), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); }
  else { info->written = utoa2(va_arg(info->args, size_t), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); }
}

static inline void
_printf_handle_ptrdiff_type(re_format_info_t* info)
{
  if (!RE_PRINTF_PEEK_NEXT_FMT() || RE_PRINTF_PEEK_FMT() != 't') { return; }

  if (tolower(RE_PRINTF_PEEK_NEXT_FMT()) == 'i' || tolower(RE_PRINTF_PEEK_NEXT_FMT()) == 'd') { RE_PRINTF_ADVANCE_FMT(); }

  if (tolower(RE_PRINTF_PEEK_FMT()) == 'i' || tolower(RE_PRINTF_PEEK_FMT()) == 'd') { info->written = re_itoa2(va_arg(info->args, ptrdiff_t), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); }
  else { info->written = utoa2(va_arg(info->args, ptrdiff_t), info->tmp_writebuffer, 10, info->max_chars - info->chars_written, RE_PRINTF_ADD_COMMAS); }
}

static inline void
_printf_handle_hash_prefix_and_children(re_format_info_t* info)
{
  if (!RE_PRINTF_PEEK_NEXT_FMT() || RE_PRINTF_PEEK_FMT() != '#') { return; }

  info->tmp_writebuffer[0] = '0';

  /* If the character is X, then the prefix needs to have X. If it is x, then teh prefix needs to have x */
  info->tmp_writebuffer[1] = RE_PRINTF_PEEK_NEXT_FMT();

  /* Move past x\X */
  RE_PRINTF_ADVANCE_FMT();

  /* The 0x\0X prefix */
  info->written = 2;
  info->written += re_utoa2(va_arg(info->args, unsigned), info->tmp_writebuffer + 2, 16, info->max_chars - info->chars_written, false);
}

static inline void
_printf_format_parse_string(re_format_info_t* info)
{
  if (!RE_PRINTF_PEEK_FMT() || RE_PRINTF_PEEK_FMT() != 's') { return; }

  const char* string = va_arg(info->args, const char*);
  /* strlen(NULL) is not ok */
  if (!string) { string = "(null)"; }

  size_t string_length = 0;
  if (info->precision_specified) { string_length = info->precision; }
  else { string_length = strlen(string); }

  _printf_write(info, string, string_length);
  info->wbuffer_used = false;
}

static inline void
_printf_format_parse_format(re_format_info_t* info)
{
  if (!RE_PRINTF_PEEK_FMT()) { return; }

  size_t remaining = info->max_chars - info->chars_written;

  switch (tolower(RE_PRINTF_PEEK_FMT()))
  {
    /* By default, ftoa should not trim trailing zeroes. */
    case 'f': info->written = re_ftoa2(va_arg(info->args, double), info->tmp_writebuffer, info->precision, remaining, false); break;
    case 'l': _printf_handle_long_type(info); break;
    case 'd':
    case 'i': info->written = itoa2(va_arg(info->args, int), info->tmp_writebuffer, 10, remaining, RE_PRINTF_ADD_COMMAS); break;
    case 'u': info->written = utoa2(va_arg(info->args, unsigned), info->tmp_writebuffer, 10, remaining, RE_PRINTF_ADD_COMMAS); break;

    /* size_t based formats. This is standard. */
    case 'z': _printf_handle_size_type(info); break;

    /* hash tells us that we need to have the 0x prefix (or the 0X prefix) */
    case '#': _printf_handle_hash_prefix_and_children(info); break;

    /* hex integer */
    case 'x': info->written = re_utoa2(va_arg(info->args, unsigned), info->tmp_writebuffer, 16, info->max_chars - info->chars_written, false); break;

    /* pointer */
    case 'p': info->written = re_ptoa2(va_arg(info->args, void*), info->tmp_writebuffer, remaining); break;

    /* bytes, custom */
    case 'b': info->written = re_btoa2(va_arg(info->args, size_t), 1, info->tmp_writebuffer, remaining); break;

    case 's': _printf_format_parse_string(info); break;

    case 'c':
    /* If there are two % symbols in a row */
    case '%':
    default: _printf_format_process_char(info); break;
  }
}

static inline void
_printf_write_padding(re_format_info_t* info)
{
  info->padding = info->padding_w - (int)info->written;
  char pad_char = info->pad_zero ? '0' : ' ';
  if (info->left_align && info->padding <= 0) { return; }

  /* No, we cannot run out of padding buffer space, because we write it in chunks. */

  memset(info->pad_buf, pad_char, 64);
  while (info->padding > 0)
  {
    int chunk = info->padding > 64 ? 64 : info->padding;
    _printf_write(info, info->pad_buf, chunk);
    info->padding -= chunk;
  }
}

static inline void
_printf_format_upload_to_destination(re_format_info_t* info)
{
  if (!info->wbuffer_used) { return; }

  _printf_write_padding(info);

  _printf_write(info, info->tmp_writebuffer, info->written);

  _printf_write_padding(info);
}

static inline void
_printf_format_parse_format_specifier(re_format_info_t* info)
{
  info->wbuffer_used        = true;
  info->padding_w           = 0;
  info->precision           = 6;
  info->precision_specified = 0;

  _printf_get_padding_and_precision_if_given(info);

  _printf_format_parse_format(info);

  _printf_format_upload_to_destination(info);
}

static inline void
_printf_write_iterated_char(re_format_info_t* info)
{
  if (info->chars_written >= info->max_chars - 1) { return; }

  if (RE_PRINTF_PEEK_FMT() == '\b')
  {
    if (info->chars_written == 0) { return; }

    if (info->file) { fputc('\b', (FILE*)info->dst_file); }
    else if (info->dst_string)
    {
      info->dst_string--;
      info->chars_written--;
    }
  }
  else if (info->file)
  {
    if (fputc(RE_PRINTF_PEEK_FMT(), (FILE*)info->dst_file) != RE_PRINTF_PEEK_FMT()) { /* we can't use printf here to avoid recursion */ }
  }
  else if (info->dst_string)
  {
    *info->dst_string = RE_PRINTF_PEEK_FMT();
    info->dst_string++;
  }
  info->chars_written++;
}

static inline void
_printf_loop(re_format_info_t* info)
{
  for (; RE_PRINTF_PEEK_FMT() && info->chars_written < info->max_chars; info->itr++)
  {
    if (RE_PRINTF_PEEK_FMT() == '%')
    {
      RE_PRINTF_ADVANCE_FMT();
      _printf_format_parse_format_specifier(info);
    }
    else { _printf_write_iterated_char(info); }
  }
}

int
_vsfnprintf(va_list args, void* RE_RESTRICT dst, bool is_file, int max_chars, const char* RE_RESTRICT fmt)
{
  if (max_chars == 0) { return 0; }

  re_format_info_t info = re_zero_init(re_format_info_t);

  /* Aligned to 64 bytes for fast writing and reading access */
  RE_ATTR_ALIGNED(64) char pad_buf[64];

  char wbuf[RE_WBUF_SIZE];

  info.dst_file        = is_file ? (FILE*)dst : NULL;
  info.dst_string      = is_file ? NULL : (char*)dst;
  info.max_chars       = max_chars;
  info.precision       = 6;
  info.fmt_str_end     = fmt + strlen(fmt);
  info.itr             = fmt;
  info.file            = is_file;
  info.pad_buf         = pad_buf;
  info.tmp_writebuffer = wbuf;

  va_copy(info.args, args);

  _printf_loop(&info);

  if (!is_file && info.dst_string && max_chars > 0)
  {
    size_t width        = (info.chars_written < max_chars) ? info.chars_written : max_chars - 1;
    ((char*)dst)[width] = 0;
  }

  va_end(info.args);

  return (int)info.chars_written;
}

#endif