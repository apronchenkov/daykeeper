#include "warnout.h"
#include "my_errno.h"
#include "my_string.h"
#include "my_strerror.h"
#include "unistd.h"
#include "warper.h"

ssize_t Read(int d, char *buf, size_t count) {
	ssize_t ret;
	while( -1 == (ret = read(d, buf, count)) && EINTR == errno );
	return ret;
}

ssize_t Write(int d, const char *buf, size_t count) {
	ssize_t ret;
	while( -1 == (ret = write(d, buf, count)) && EINTR == errno );
	return ret;
}

int Close(int d) {
	int ret;
	while( -1 == (ret = close(d)) && EINTR == errno );
	return ret;
}

int Poll(struct pollfd *ufds, unsigned int nfds, int timeout) {
	int ret;
	while( -1 == (ret = poll(ufds, nfds, timeout)) && EINTR == errno );
	return ret;
}

int Pipe(int filedes[2]) {
	int ret;
	while( -1 == (ret = pipe(filedes)) && EINTR == errno );
	return ret;
}

int Dup2(int oldd, int newd) {
	int ret;
	while( -1 == (ret = dup2(oldd, newd)) && EINTR == errno );
	return ret;
}

int Fork() {
	int ret;
	while( -1 == (ret = fork()) && EINTR == errno );
	return ret;
}

void Exit(int status) {
	exit(status);
}

int Wait(int *status) {
	int ret;
	while( -1 == (ret = wait(status)) && EINTR == errno );
	return ret;
}

int Waitpid(pid_t pid, int *status, int options) {
	int ret;
	while( -1 == (ret = waitpid(pid, status, options)) && EINTR == errno );
	return ret;
}

int Chdir(const char *path) {
	int ret;
	while( -1 == (ret = chdir(path)) && EINTR == errno );
	return ret;
}

int Sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
	int ret;
	while( -1 == (ret = sigaction(signum, act, oldact))  && EINTR == errno );
	return ret;
}

int Sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
	int ret;
	while( -1 == (ret = sigprocmask(how, set, oldset)) && EINTR == errno );
	return ret;
}

int Kill(pid_t pid, int sig) {
	return kill(pid, sig);
}

pid_t Getpid() {
	return getpid();
}

pid_t Setsid() {
	return setsid();
}

int Setpgid(pid_t pid, pid_t pgid) {
	return setpgid(pid, pgid);
}

int Strange_syscall_ret(int ret, const char *syscall_name) {
	if( -1 == ret ) 
		warn_out("Warning: Strange return from syscall", syscall_name, ": ", my_strerror[errno], "\n", NULL);
	return ret;
}
