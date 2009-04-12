#include <sys/types.h>
#include <sys/poll.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>

#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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

int Pipe(int filedes[2]) {
	return pipe(filedes);
}
int Dup2(int oldd, int newd) {
	int ret;
	while( -1 == (ret = dup2(oldd, newd)) && EINTR == errno );
	return ret;
}
int Fork() {
	return fork();
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
int Kill(pid_t pid, int sig) {
	return kill(pid, sig);
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
int Siginterrupt(int sig, int flag) {
	return siginterrupt(sig, flag);
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

int Socket(int domain, int type, int protocol) {
	return socket(domain, type, protocol);
}
int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addr_len) {
	return connect(sockfd, serv_addr, addr_len);
}

void warn_out(const char *format, ...) {
	va_list ap;
	const char *args = *(&format + 1);
	va_start(ap, args);
	if( 0 > vfprintf(stderr, format, ap) ) 
		Exit(-1);
	va_end(ap);
}
