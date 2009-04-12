#ifndef __warper_h__
#define __warper_h__

#include "unistd.h"

extern ssize_t Read(int d, char *buf, size_t count);
extern ssize_t Write(int d, const char *buf, size_t count);
extern int Close(int d);
extern int Poll(struct pollfd *ufds, unsigned int nfds, int timeout);
extern int Pipe(int filedes[2]);
extern int Dup2(int oldd, int newd);
extern int Fork();
extern void Exit(int status);
extern int Wait(int *status);
extern int Waitpid(pid_t pid, int *status, int options);
extern int Chdir(const char *path);

extern int Sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
extern int Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
extern int Kill(pid_t pid, int sig);

extern pid_t Getpid();
extern pid_t Setsid();
extern int Setpgid(pid_t pid, pid_t pgid);

int Strange_syscall_ret(int ret, const char *syscall_name);

#endif //__warper_h__
