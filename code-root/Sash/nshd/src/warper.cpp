#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>

#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <fcntl.h>

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
int Shutdown(int s, int how) {
	return shutdown(s, how);
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

pid_t Getpid() {
	return getpid();
}
pid_t Setsid() {
	return setsid();
}
int Setpgid(pid_t pid, pid_t pgid) {
	return setpgid(pid, pgid);
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

int Socket(int domain, int type, int protocol) {
	return socket(domain, type, protocol);
}
int Setsockopt(int s, int level, int optname, void *optval, socklen_t optlen) {
	return setsockopt(s, level, optname, optval, optlen);
}
int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen) {
	return bind(sockfd, my_addr, addrlen);
}
int Listen(int s, int backlog) {
	return listen(s, backlog);
}
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) {
	int ret;
	while( -1 == (ret = accept(s, addr, addrlen)) && EINTR == errno );
	return ret;
}
int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addr_len) {
	return connect(sockfd, serv_addr, addr_len);
}

int Fd_close_on_exec(int fd) {
	int ret;
	while( -1 == (ret = fcntl(fd,  F_SETFD, FD_CLOEXEC)) && EINTR == errno );
	return ret;
}

//void Fd_not_close_on_exec(int fd) {
//	int ret;
//	while( -1 == (ret = fcntl(fd,  F_SETFD, 0)) && EINTR == errno );
//	return ret;
//}


void warn_out(const char *format, ...) {
	va_list ap;
	const char *args = *(&format + 1);
	va_start(ap, args);
	if( 0 > vfprintf(stderr, format, ap) ) 
		Exit(-1);
	va_end(ap);
}
