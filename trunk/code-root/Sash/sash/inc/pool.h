#ifndef __pool_h__
#define __pool_h__

#include "bool.h"
#include "warper.h"

struct io_t {
	int out, in, err;
	pid_t gid;
	struct io_t *next;
};
extern struct io_t *pool;
					// Пул ввода вывода

typedef enum {Shell, Programm} io_type_t;
extern io_type_t io_type;
					// Тип ввода вывода

extern void init_pool();
extern bool pool_switch(unsigned int i);
extern bool pool_new(int out, int in, int err, pid_t gid);
extern bool pool_close_in();
extern bool pool_close_out();
extern bool pool_close_err();

#endif //__pool_h__
