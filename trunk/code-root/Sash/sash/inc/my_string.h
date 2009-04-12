#ifndef __my_string_h__
#define __my_string_h__

#include <sys/types.h>
#include "bool.h"

extern size_t my_strlen(const char *s);
extern void my_strcpy(char *dest, const char *src);
extern void my_strncpy(char *dest, const char *src, size_t n);
extern char * my_strcat(char *dest, const char *src);
extern char * my_strncat(char *dest, const char *src, size_t n);
extern int my_strcmp(const char *s1, const char *s2);
extern int my_strncmp(const char *s1, const char *s2, size_t n);

extern char * my_l2str(char *dest, long s);
extern bool my_str2l(const char *src, long *l);

#endif //__my_string_h__
