#include "signl.h"

#include "pool.h"

#include "my_errno.h"
#include "my_strerror.h"

#include "my_string.h"
#include "shellout.h"
#include "warnout.h"

#include "warper.h"

#include "bool.h"

bool sig_chld;
bool sig_tstp;
bool sig_int;

void signl_child(int sig) {
	int status;
	pid_t pid;

	char tmp_pid[16];
	char tmp_status[16];
	
	sig_chld = true;

	while( 0 < (pid = Waitpid(-1, &status, WNOHANG|WUNTRACED)) ) {
		my_l2str(tmp_pid, pid);
		if( WIFEXITED(status) ) 
			shell_add_message("Child process ", tmp_pid, " is terminate normaly [status = ", my_l2str(tmp_status, WEXITSTATUS(status)), "].\n", NULL);
		else if( WIFSIGNALED(status) ) 
			shell_add_message("Child process ", tmp_pid, " is terminate by signal [", my_strsignal[WTERMSIG(status)], "].\n", NULL);
		else if( WIFSTOPPED(status) ) 
			shell_add_message("Child process ", tmp_pid, " is stopped by signal [", my_strsignal[WSTOPSIG(status)], "].\n", NULL);
		else 
			warn_out("Warining: Child process ", tmp_pid, " has strange status.\n", NULL);
	}
}

void signl_termstop(int sig) {	
	sig_tstp = true;
	pool_switch(0);
}

void signl_int(int sig) {
	char tmp_gid[16];
	
	sig_int = true;
	
	if( Programm == io_type ) {
		if( 0 > Kill(-pool->gid, SIGINT) ) 
			warn_out("Kill(", my_l2str(tmp_gid, -pool->gid), ", SIGINT): ", my_strerror[errno], "\n", NULL);
	}
}

void signl_set() {
	struct sigaction act;

	act.sa_handler = signl_child;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	Sigaction(SIGCHLD, &act, NULL);
	
	act.sa_handler = signl_termstop;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	Sigaction(SIGTSTP, &act, NULL);
	
	act.sa_handler = signl_int;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	Sigaction(SIGINT, &act, NULL);
}

void signl_remove() {
	struct sigaction act;

	act.sa_handler = SIG_DFL;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	Sigaction(SIGCHLD, &act, NULL);
	Sigaction(SIGTSTP, &act, NULL);
	Sigaction(SIGINT, &act, NULL);
}
