#include <string.h>
#include <errno.h>

#include "sgnl.h"
#include "warper.h"

unsigned int sgnl_from[] = {
		// типы обрабатываемых сигналов
		// sgnl_from[i] = [сигнал] означает, что i кодирует сигнал [сигнал]
	SIGHUP,
	SIGINT,
       	SIGQUIT,
       	SIGILL,
       	SIGABRT,
       	SIGFPE,
       	SIGSEGV,
       	SIGPIPE,
       	SIGALRM,
       	SIGTERM,
       	SIGUSR1,
       	SIGUSR2,
       	SIGCHLD,
       	SIGCONT,
       	SIGTSTP,
       	SIGTTIN,
       	SIGTTOU
};

unsigned int sgnl_to[32];
		// sgnl[[сигнал]] = i означает, что сигнал [сигнал] когдируется номером i

const unsigned int sgnls = sizeof(sgnl_from)/sizeof(unsigned int);;
		// число обрабатываемых сигналов
unsigned int sgnl_count[32];
		// счётчик сигналов
bool was_sgnl;
		// флаг указывающий на то, что приходил хотябы один сигнал

static sigset_t unlock_mask;
static sigset_t lock_mask;
		// переменные описывающие состояния Сигналы_разрешены и Сигналы_запрещены

void sgnl_catcher(int sig) {
		// наш замечательный обрабочик сигналов
	was_sgnl = true;
	++sgnl_count[sgnl_to[sig]];
}

void sgnl_set() {
	unsigned int i;
	
		// вычисляем одну таблицу преобразования по другой
	for(i = 0; i < sgnls; ++i) 
		sgnl_to[sgnl_from[i]] = i;

		// инициализируем маску блогировки
	sigemptyset(&lock_mask);
	for(i = 0; i < sgnls; ++i)
		sigaddset(&lock_mask, sgnl_from[i]);
		
		// вешаем наш обработчик на все сигналы указанные в таблице
		// так же делается настройка: системные вызовы будут обрываться данными сигналами
	struct sigaction act;
	act.sa_handler = sgnl_catcher;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	for(i = 0; i < sgnls; ++i) {
		if( -1 == Sigaction(sgnl_from[i], &act, NULL) ) {
			warn_out("Warning: Can't set signal handler on %s -> shell will hard halt.\n", strsignal(sgnl_from[i]));
			Exit(-1);
		}
		if( -1 == Siginterrupt(sgnl_from[i], 1) ) {
			warn_out("Warning: Can't set interrupt property on signal %s -> shell will hard halt.\n", strsignal(sgnl_from[i]));
			Exit(-1);
		}
	}

}

void sgnl_unset() {
	unsigned int i;
	
		// востанавливаем обрабочики поумолчанию
	struct sigaction act;
	act.sa_handler = SIG_DFL;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	for(i = 0; i < sgnls; ++i) {
		if( -1 == Sigaction(sgnl_from[i], &act, NULL) ) {
			warn_out("Warning: Can't set signal handler on %s -> shell will hard halt.\n", strsignal(sgnl_from[i]));
			Exit(-1);
		}
/*		На мой взгляд этот кусочек лишний
 * 		if( -1 == Siginterrupt(sgnl_from[i], 0) ) {
			warn_out("Warning: Can't set interrupt property on signal %s -> shell will hard halt.\n", strsignal(sgnl_from[i]));
			Exit(-1);
		}
*/
	}
}

void sgnl_lock() {
		// устанавливаем маску блокировки сигналов
	if( -1 == Sigprocmask(SIG_SETMASK, &lock_mask, &unlock_mask) ) {
		warn_out("Warning: can't set signal mask: %s -> shell will be hard halt.\n", strerror(errno));
		Exit(-1);
	}
}

void sgnl_unlock() {
		// устанавливаем исходную маску обработки сигналов
	if( -1 == Sigprocmask(SIG_SETMASK, &unlock_mask, NULL) ) {
		warn_out("Warning: can't set signal mask: %s -> shell will be hard halt.\n", strerror(errno));
		Exit(-1);
	}
}
