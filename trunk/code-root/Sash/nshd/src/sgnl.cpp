#include <errno.h>
#include <string.h>
#include "sgnl.h"
#include "warper.h"
#include "main.h"

static unsigned int sgnl_from[] = {
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

static const unsigned int sgnls = sizeof(sgnl_from)/sizeof(unsigned int);

static void sgnl_child(int sig) {
		// обработчик прерывания
	while( 0 < Waitpid(-1, NULL, WNOHANG|WUNTRACED) ); 
}

void sgnl_set() {
		// установить наш обработчик прерывания
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_handler = sgnl_child;
	act.sa_flags = 0;
	if( -1 == Sigaction(SIGCHLD, &act, NULL) ) {
		warn_out("%s: sigaction: %s\n", argv_0, strerror(errno));
		Exit(-1);
	}
}

void signl_unset() {
		// снять наш обработчик прерывания
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
		// учитывания, что данная фукнция вызывается в конце программы
		// считаю что ошибка уже не будет являться критичной
	if( -1 == Sigaction(SIGCHLD, &act, NULL) ) {
		warn_out("%s: sigaction: %s\n", argv_0, strerror(errno));
//		Exit(-1);
	}
}

bool sgnl_send(pid_t pid, unsigned int sgnl) {
		// послать сигнал приложению
	if( sgnl >= sgnls ) {
		warn_out("Warning: Net package contain wrong signal data -> break connection.\n");
		return false;
	}
	if( -1 == Kill(pid, sgnl_from[sgnl]) ) 
		warn_out("%s: kill: %s\n", argv_0, strerror(errno));
	return true;
}
