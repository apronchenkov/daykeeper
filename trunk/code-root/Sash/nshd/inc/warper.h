#ifndef __warper_h__
#define __warper_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <signal.h>

ssize_t Read(int d, char *buf, size_t count);
ssize_t Write(int d, const char *buf, size_t count);
int Close(int d);
int Shutdown(int s, int how);

int Pipe(int filedes[2]);
int Dup2(int oldd, int newd);
int Fork();
void Exit(int status);

pid_t Getpid();
pid_t Setsid();
int Setpgid(pid_t pid, pid_t pgid);

int Wait(int *status);
int Waitpid(pid_t pid, int *status, int options);
int Kill(pid_t pid, int sig);

int Sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int Siginterrupt(int sif, int flag);

int Socket(int domain, int type, int protocol);
int Setsockopt(int s, int level, int optname, void *optval, socklen_t optlen);
int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
int Listen(int s, int backlog);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addr_len);

int Fd_close_on_exec(int fd);

void warn_out(const char *format, ...);

#endif //__warper_h__
