#include "pool.h"

#include "my_errno.h"
#include "warnout.h"
#include "shellout.h"

#include "my_string.h"

#include "warper.h"

/* ++ Реализация объектов объявленых в заголовке ++ */
struct io_t *pool = NULL;
io_type_t io_type;
/* -- Реализация объектов объявленых в заголовке -- */

/* ++ Работа с памятью ++ */
#define POOL_MEMORY_SIZE 256
					// Размер буфера памяти для базы пулов ввода вывода
static struct io_t tmp[POOL_MEMORY_SIZE];
static struct io_t *pool_memory_top;

static void init_pool_memory() {
	// Инициализация буффера памяти.
	int i;
	for(i = 0; i < POOL_MEMORY_SIZE - 1; ++i)
		tmp[i].next = &tmp[i + 1];
	tmp[POOL_MEMORY_SIZE - 1].next = NULL;

	pool_memory_top = tmp;
}

static struct io_t * malloc_pool() {
	// Выделение новой ячейки.
	struct io_t *ptr;
	
	if( NULL == pool_memory_top ) {
		errno = ENOMEM;
		return NULL;
	}
	
	ptr = pool_memory_top;
	
	pool_memory_top = pool_memory_top->next;
		
	return ptr;
}

static void free_pool(struct io_t *ptr) {
	// Освобождение одной ячейки.
	ptr->next = pool_memory_top;
	pool_memory_top = ptr;
}
/* -- Работа с памятью -- */

/* ++ Реализация функций объявленных в заголовочном файле ++ */
void init_pool() {
	// Инициализировать модуль poll.
	init_pool_memory();
	io_type = Shell;
}

bool pool_switch(unsigned int no) {
	// Переключить текущий пул ввода вывода.
	// Если no == 0 переключаемся в режим шела.
	int i;
	struct io_t *k, *l;
	
	if( 0 == no ) {
		io_type = Shell;

	} else {	
		k = NULL;
		l = pool;
		for(i = 0; i < no - 1 && l != NULL; ++i, k = l, l = l->next);

		if( l == NULL )
			return false;
			
		if( k != NULL ) {
			k->next = l->next;
			l->next = pool;
			pool = l;
		}
		io_type = Programm;
	}
	return true;
}

bool pool_close_in() {
	// Закрыть ввод вывод пула.
	// Если закрыты все каналы текущего пула, то пул удаляется происходит переход в режим шела.
	struct io_t *ptr;
	
	if( Programm != io_type || pool->in < 0 )
		return false;

	Close(pool->in);
	pool->in = -1;
	
	if( pool->out < 0 && pool->err < 0 ) {
		ptr = pool;
		pool = ptr->next;
		free_pool(ptr);
		io_type = Shell;
	}
	
	return true;
}

bool pool_close_out() {
	// Закрыть поток ввода пула.
	// Если закрыты все каналы текущего пула, то пул удаляется происходит переход в режим шела.
	struct io_t *ptr;
	
	if( Programm != io_type || pool->out < 0 )
		return false;
		
	Close(pool->out);
	pool->out = -1;
	
	if( pool->in < 0 && pool->err < 0 ) {	
		ptr = pool;
		pool = pool->next;	
		free_pool(ptr);
		io_type = Shell;
	}
	
	return true;
}

bool pool_close_err() {
	// Закрыть поток ошибок пула.
	// Если закрыты все каналы текущего пула, то пул удаляется и происходит переход в режим шела.
	struct io_t *ptr;
	
	if( Programm != io_type || pool->err < 0 )
		return false;
		
	Close(pool->err);
	pool->err = -1;
	
	if( pool->in < 0 && pool->out < 0 ) {	
		ptr = pool;
		pool = pool->next;	
		free_pool(ptr);
		io_type = Shell;
	}
	
	return true;
}

bool pool_new(int out, int in, int err, pid_t gid) {
	// Создать новый пул ввода вывода.
	
	struct io_t *ptr;
	
	if( in < 0 && out < 0 && err < 0 )
		return false;

	if( NULL == (ptr = malloc_pool()) )
		return false;

	ptr->out = out;		
	ptr->in = in;
	ptr->err = err;
	ptr->gid = gid;

	ptr->next = pool;
	pool = ptr;	
	
	io_type = Programm;
	
	return true;
}
/* -- Реализация функций объявленных в заголовочном файле -- */
