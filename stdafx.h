#ifndef _RELIB_STDAFX_H
#define _RELIB_STDAFX_H

#define re_volatile_asm __asm__ volatile

#ifdef __has_attribute
#define attr_aligned(N) __attribute__((aligned(N)))
#endif

#endif//_RELIB_STDAFX_H
