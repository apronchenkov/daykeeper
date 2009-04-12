#include "io.h"
#include "bool.h"

#include "pool.h"
#include "shell.h"

#include "my_errno.h"
#include "my_strerror.h"
#include "warper.h"
#include "signl.h"

#include "warnout.h"
#include "shellout.h"

#define BUF_SIZE 4096

/* ++ Вспомогательные функции ++ */
typedef enum {ERR, HUP, OK} io_ret_t;

static io_ret_t io_transfer(int in, int out) {
	// Передать блок данных из ввода на вывод.
	// Ошибки вывода игнорируются.
	
	char buf[BUF_SIZE];
	int ret;
	int i, j;
	
	ret = Read(in, buf, BUF_SIZE);
	switch( ret ) {case -1:	return ERR;
		       case 0:	return HUP;}

	i = ret;
	j = 0;
	
	do {
		if( 0 > (ret = Write(out, buf + j, i - j)) )
			break;
		j += ret;
	} while( j != i );
	
	return OK;
}

static void programm_io() {
	// Ввод вывод шела в режиме Программа.
	int nfds;
	struct 	pollfd ufds[4];

	int pool_in_revents	= 0;
	int pool_out_revents	= 0;
	int pool_err_revents	= 0;
	int stdin_revents	= 0;
	int stdout_revents	= 0;
	int stderr_revents	= 0;
	
	sigset_t set, oldset;
	
	bool fast_waiting;
		// какой тип ожидания делать в poll()
	int cnt, i;
	
	sigemptyset(&set);
	sigaddset(&set, SIGTSTP);
	Sigprocmask(SIG_BLOCK, &set, &oldset);
		// запрещаем обработку некоторых сигналов
	fast_waiting = false;
	
	while( !shll_halt && Programm == io_type ) {
		
		// построение ввода для poll
		cnt = 0;
		ufds[cnt].fd = STDIN;
		ufds[cnt].events = (stdin_revents&POLLIN)?0:POLLIN;

		cnt = 1;
		ufds[cnt].fd = STDOUT;
		ufds[cnt].events = (stdout_revents&POLLOUT)?0:POLLOUT;
		
		cnt = 2;
		ufds[cnt].fd = STDERR;
		ufds[cnt].events = (stderr_revents&POLLOUT)?0:POLLOUT;
		
		cnt = 3;
		if( pool->out >= 0 ) {
			ufds[cnt].fd = pool->out;
			ufds[cnt].events = (pool_out_revents&POLLOUT)?0:POLLOUT;
			++cnt;
		}
		
		if( pool->in >= 0 ) {
			ufds[cnt].fd = pool->in;
			ufds[cnt].events = (pool_in_revents&POLLIN)?0:POLLIN;
			++cnt;
		}

		if( pool->err >= 0 ) {
			ufds[cnt].fd = pool->err;
			ufds[cnt].events = (pool_err_revents&POLLIN)?0:POLLIN;
			++cnt;
		}

		sig_tstp = 0;
			// сбрасываем флаг сигнала
			
		Sigprocmask(SIG_SETMASK, &oldset, NULL);
			// разрешаем обработку сигналов по умолчанию
		
			// poll
		nfds = cnt;
		if( -1 == poll(ufds, nfds, (fast_waiting ? 0 : -1)) ) {
			if( EINTR == errno )
				// первичная проверка на сигналы
				break;
			else
				// Такая ситуация не должна быть возможна
				warn_out("Warning: Strange return from syscall poll(): ", my_strerror[errno], "\n", NULL);
		}

		fast_waiting = false;
		
		Sigprocmask(SIG_BLOCK, &set, &oldset);
			// запрещаем обработку некоторых сигналов
		if( sig_tstp ) 
			// вторичная проверка на сигналы
			break;
					
		// Первичная обработка poll.
		stdin_revents |= ufds[0].revents;
		stdout_revents |= ufds[1].revents;
		stderr_revents |= ufds[2].revents;
	
		i = 3;
		if( pool->out >= 0 ) {
			pool_out_revents |= ufds[i].revents;
			++i;
		}
		
		if( pool->in >= 0  ) {
			pool_in_revents |= ufds[i].revents;
			++i;
		}
		
		if( pool->err >= 0 ) {
			pool_err_revents |= ufds[i].revents;
			++i;
		}

		// Вторичная обработка результатов poll.
		if( (stdin_revents&POLLERR) || (stdout_revents&POLLERR) || (stderr_revents&POLLERR) ) {
			warn_out("Shell resive ERR -> exit(-1).\n", NULL);
			exit(-1);
		}

		if( stdout_revents&POLLHUP ) {
			// Получено событие: разорван стандартный вывод
			warn_out("Shell stdout resive HUP -> exit(-1).\n", NULL);
			exit(-1);
		}

		if( stderr_revents&POLLHUP ) {
			warn_out("Shell stderr resive HUP -> (interesting, where i will be white it?) exit(-1).\n", NULL);
			exit(-1);
		}

		if( pool_out_revents&POLLERR ) {
			pool_close_out();
			pool_out_revents = 0;
			continue;
		}

		if( pool_in_revents&POLLERR ) {
			pool_close_in();
			pool_in_revents = 0;
			continue;
		}

		if( pool_err_revents&POLLERR ) {
			pool_close_err();
			pool_err_revents = 0;
			continue;
		}

		if( pool_out_revents&POLLHUP ) {
			pool_close_out();
			pool_out_revents = 0;
			continue;
		}
				
		// Основная обработка результатов poll.
		if( (stdin_revents&POLLIN) && (pool_out_revents&POLLOUT) ) {
			pool_out_revents = 0;
			switch( io_transfer(STDIN, pool->out) ) {
			case ERR:
				stdin_revents = POLLERR;
				fast_waiting = true;
				break;
			case HUP: 
				stdin_revents = POLLHUP;
				fast_waiting = true;
				break;
			case OK:
				stdin_revents = 0;
				break;
			default:
				warn_out("Warning: developer! io_transfer has wrong interface!\n", NULL);
			}
		}
		
		if( (stdout_revents&POLLOUT) && (pool_in_revents&POLLIN) ) {
			stdout_revents = 0;
			switch( io_transfer(pool->in, STDOUT) ) {
			case ERR:
				pool_in_revents = POLLERR;
				fast_waiting = true;
				break;
			case HUP:
				pool_in_revents = POLLHUP;
				fast_waiting = true;
				break;
			case OK:
				pool_in_revents = 0;
				break;
			default:
				warn_out("Warning: developer! io_transfer has wrong interface!\n", NULL);
			}
		}

		if( (stderr_revents&POLLOUT) && (pool_err_revents&POLLIN) ) {
			stderr_revents = 0;
			switch( io_transfer(pool->err, STDERR) ) {
			case ERR:
				pool_err_revents = POLLERR;
				fast_waiting = true;
				break;
			case HUP:
				pool_err_revents = POLLHUP;
				fast_waiting = true;
				break;
			case OK:
				pool_err_revents = 0;
				break;
			default:
				warn_out("Warning: developer! io_transfer has wrong interface!\n", NULL);
			}
		}
		
		// Финальная обработка.

		if( (stdin_revents&POLLHUP) && !((stdin_revents&POLLIN) || (pool->out < 0)) ) {
			// получено событие: закрыт стандартный ввода
			// и либо буфер ввода чист, либо дискриптор ввода программы закрыт
			pool_close_out();
			stdin_revents = 0;
		}
		
		if( (pool_in_revents&POLLHUP) && !(pool_in_revents&POLLIN) ) {
			// получено событи: закрыт вывод программы
			// и в буфере вывода чисто
			pool_close_in();
			pool_in_revents = 0;
		}

		if( (pool_err_revents&POLLHUP) && !(pool_err_revents&POLLIN) ) {
			// получено событи: закрыт поток ошибок программы
			// и в буфере вывода чисто
			pool_close_err();
			pool_err_revents = 0;
		}
	}
		
	Sigprocmask(SIG_SETMASK, &oldset, NULL);
		// разрешаем обработку сигналов поумолчанию
}

static void shell_io() {
	char buf[BUF_SIZE];
	int ret;
		
	while( !shll_halt && Shell == io_type ) {

		shell_out();

		ret = Read(STDIN, buf, BUF_SIZE);
		if( 0 == ret ) {
			warn_out("Shell resive HUP -> shell halt.\n", NULL);
			shell_halt();
			continue;
		} 
		if( -1 == ret ) {
			warn_out("Shell stdin resive ERR: ", my_strerror[errno], " -> exit(-1).\n", NULL);
			exit(-1);
		}
	
		shell(buf, ret);
	}
}

void input() {
	while( !shll_halt ) {
		switch( io_type ) {
		case Programm:
			programm_io();
			break;
		case Shell:
			shell_io();
			break;
		default:
			warn_out("Warning: wrong type of io.\n", NULL);
		}
	}
}

