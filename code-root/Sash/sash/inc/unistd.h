#ifndef __unistd_h__
#define __unistd_h__

#include <sys/types.h>
#include <asm/poll.h>
#include <sys/wait.h>
#include <signal.h>

#ifndef NULL
#define NULL 0
#endif //NULL

#define STDIN		0
#define STDOUT		1
#define STDERR		2

extern ssize_t read(int d, char *buf, size_t count);
extern ssize_t write(int d, const char *buf, size_t count);
extern int close(int d);

extern int poll(struct pollfd *ufds, unsigned int nfds, int timeout);

extern int pipe(int filedes[2]);
extern int dup2(int oldd, int newd);
extern int fork();

extern void exit(int status);

extern int wait(int *status);
extern int waitpid(pid_t pid, int *status, int options);

//extern int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
//extern int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
extern int kill(pid_t pid, int sig);

extern int execve(const char *filename, char *const avgv[], char *const envp[]);
extern int chdir(const char *path);

extern pid_t getpid();
extern pid_t setsid();
extern int setpgid(pid_t pid, pid_t pgid);


#endif //__unistd_h__
