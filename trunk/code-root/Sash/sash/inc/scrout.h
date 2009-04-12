#ifndef __src_out_h__
#define __scr_out_h__ 1

#include "bool.h"

#define In	0
#define Out	1
#define Err	2

extern bool scrout(int fd, const char *str);

#endif // __scr_out_h__
