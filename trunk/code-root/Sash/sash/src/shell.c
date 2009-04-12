#include "my_string.h"
#include "my_errno.h"
#include "my_strerror.h"
#include "my_exec.h"

#include "shell.h"
#include "shellout.h"
#include "warnout.h"
#include "warper.h"

#include "pool.h"
#include "pool_list.h"

extern char **Envp;

bool shll_halt;

void shell_exec_low(char *const Argv[]) {
	int in[2];
	int out[2];
	int err[2];
	
	pid_t pid;
	
	int rec;
	int t;
	
	enum{INIT, PIPE0, PIPE1, PIPE2, FORK, CLOSE, DUP, EXEC, SETPGID, CLOSE2, NEWPOOL} state = INIT;
	bool ok = true;
	bool ok1, ok2, ok3;
	
	if( ok ) {
		state = PIPE0;
		ok = (0 == Pipe(out));
	}
	
	if( ok ) {
		state = PIPE1;
		ok = (0 == Pipe(in));
	}
	
	if( ok ) {
		state = PIPE2;
		ok = (0 == Pipe(err));
	}

	if( ok ) {
		state = FORK;
		pid = Fork();
		ok = (-1 != pid);
	}
	
	if( pid == 0 ) {
		if( ok ) {
			state = SETPGID;
			pid = Getpid();
			ok = (0 == Setpgid(pid, pid));
		}
		
		if( ok ) {
			state = CLOSE;
			ok = 	(0 == Close(out[1])) &&
				(0 == Close(in[0])) &&
				(0 == Close(err[0]));
		}

		if( ok ) {
			state = DUP;
			ok = 	(0 <= Dup2(out[0], 0)) &&
				(0 <= Dup2(in[1], 1)) &&
				(0 <= Dup2(err[1], 2));
		}

		if( ok ) {
			state = EXEC;
			rec = my_exec(Argv[0], Argv, Envp);
			ok = false;
		}
	} else {
		if( ok ) {
			state = CLOSE2;
			ok1 = (0 == Close(out[0]));
			ok2 = (0 == Close(in[1]));
			ok3 = (0 == Close(err[1]));
			ok = ok1 && ok2 && ok3;
		}
		if( ok ) {
			state = NEWPOOL;
			ok = pool_new(out[1], in[0], err[0], pid);
		}
	}
	
	if( ok )
		return;
	
	t = errno;
	
	switch( state ) {
		case NEWPOOL:
		case CLOSE2:
			Kill(pid, SIGKILL);	// !!!!!!!!!!!!!!
		case FORK:
			Close(err[0]);
			Close(err[1]);
		case PIPE2:
			Close(in[0]);
			Close(in[1]);
		case PIPE1:
			Close(out[0]);
			Close(out[1]);
		case PIPE0:
			break;

		case SETPGID:
		case CLOSE:
		case DUP:
			warn_out("Warning: ", my_strerror[t], "\n", NULL);
			Exit(-1);
		case EXEC:
			warn_out("exec: ", my_strerror[t], "\n", NULL);
			Exit(-1);

		case INIT: ;
	}
}

void shell_exec(char *const Argv[]) {
	long t;
	int i;
	
	if( 0 == my_strcmp(Argv[0], "exit") ) {
		shell_halt();

	} else if( 0 == my_strcmp(Argv[0], "list" ) ) {
		pool_list();

	} else if( 0 == my_strcmp(Argv[0], "switch") ) {
		if( NULL == Argv[1] ) 
			shell_add_message("switch [num]\n", NULL);
		else if( !my_str2l(Argv[1], &t) || !pool_switch(t) )
			shell_add_message("Can't switch to this.\n", NULL);

	} else if( 0 == my_strcmp(Argv[0], "cd") ) {
		if( NULL == Argv[1] )
			shell_add_message("cd [path]\n", NULL);
		else if( -1 == chdir(Argv[1]) )
			shell_add_message("cd: ", my_strerror[errno], "\n", NULL);

	} else if( 0 == my_strcmp(Argv[0], "env") ) {
		for(i = 0; Envp[i]; ++i)
		    shell_add_message(Envp[i], "\n", NULL);

	} else
		shell_exec_low(Argv);
}

void shell_parse(const char *str, int len) {
	
	static char buf[65536];
	static int i = 0;
	static char *argv[32768] = {buf, };
	static int j = 0;
	
	int k;
	
	for(k = 0; k < len; ++k) {
		if( ' ' == str[k] || '\t' == str[k] ) {
			if( argv[j] != buf + i ) {
				buf[i] = '\0';
				argv[++j] = buf + (++i);
			}
		} else if( '\n' == str[k] || ';' == str[k] ) {
			if( argv[j] != buf + i ) {
				buf[i] = '\0';
				++j;
			}
			if( 0 == j ) {
				i = 0;
				continue;
			}
			argv[j] = NULL;
			shell_exec(argv);
			i = 0;
			j = 0;
			argv[0] = buf;
		} else if( ('0' <= str[k] && '9' >= str[k]) ||
			('A' <= str[k] && 'Z' >= str[k]) ||
			('a' <= str[k] && 'z' >= str[k]) ||
			('_' == str[k] || '/' == str[k] || 
			 '.' == str[k] || '-' == str[k] ||
			 '*' == str[k]) ) {
			
			buf[i] = str[k];
			++i;
		} else 
			warn_out("Warning: left characters in line.\n", NULL);
	}
}


void shell(char *str, int len) {
	shell_parse(str, len);
}

void shell_halt() {
	shll_halt = true;
}
