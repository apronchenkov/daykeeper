#ifndef __shell_h__
#define __shell_h__

#include "bool.h"

extern bool shll_halt;

extern void shell(char *str, int len);
extern void shell_halt();

#endif // __shell_h__
