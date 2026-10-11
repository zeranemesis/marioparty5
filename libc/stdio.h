#ifndef _STDIO_H_
#define _STDIO_H_

#include "stdarg.h"

/* Plain C in the original: the DOL defines printf, puts and sprintf unmangled.
 * This header wins over the MSL one because -i libc comes first, so it is the
 * declaration the REL units actually see, and without the guard a .cpp emits
 * printf__FPCce and the REL link cannot resolve it. */
#ifdef __cplusplus
extern "C" {
#endif

int puts(const char* s);
int printf(const char*, ...);
int sprintf(char* s, const char* format, ...);
int vprintf(const char* format, va_list arg);
int vsprintf(char* s, const char* format, va_list arg);

#ifdef __cplusplus
}
#endif

#endif