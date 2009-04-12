#ifndef __warper_h__
#define __warper_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/socket.h>

ssize_t Read(int d, char *buf, size_t count);
ssize_t Write(int d, const char *buf, size_t count);
int Close(int d);

int Pipe(int filedes[2]);
int Dup2(int oldd, int newd);
int Fork();
void Exit(int status);

int Wait(int *status);
int Waitpid(pid_t pid, int *status, int options);
int Kill(pid_t pid, int sig);

int Sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int Siginterrupt(int sif, int flag);

pid_t Getpid();
pid_t Setsid();
int Setpgid(pid_t pid, pid_t pgid);

int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addr_len);
int Socket(int domain, int type, int protocol);

void warn_out(const char *format, ...);

#endif //__warper_h__
