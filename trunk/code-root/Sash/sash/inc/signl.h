#ifndef __signal_h__
#define __signal_h__

#include "bool.h"
#include "unistd.h"

extern bool sig_chld;
extern bool sig_tstp;
extern bool sig_int;

extern void signl_set();
extern void signl_remove();

#endif //__signal_h__
