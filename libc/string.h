#ifndef _STRING_H_
#define _STRING_H_

typedef unsigned long size_t;

/* Plain C in the original: the DOL defines memcpy, strcmp and friends
 * unmangled, and this header wins over the MSL one because -i libc comes
 * first, so without the guard a .cpp emits memcpy__FPvPvii and the REL link
 * cannot resolve it. */
#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dst, const void* src, size_t n);
void* memset(void* dst, int val, size_t n);

char* strrchr(const char* str, int c);
char* strchr(const char* str, int c);
int strncmp(const char* str1, const char* str2, size_t n);
int strcmp(const char* str1, const char* str2);
char* strcat(char* dst, const char* src, size_t n);
char* strncpy(char* dst, const char* src, size_t n);
char* strcpy(char* dst, const char* src);
size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif

#endif
