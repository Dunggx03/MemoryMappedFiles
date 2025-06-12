#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "lib/user/syscall.h"

void syscall_init(void);

extern struct lock filesys_lock;
void validate_user_vaddr(const void *vaddr);
void halt(void);
void exit(int status);
pid_t exec(const char *command);
int wait(pid_t pid);
bool create(const char *file, unsigned initial_size);
bool remove(const char *file);
int open(const char *file);
int filesize(int fd);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
void seek(int fd, unsigned position);
unsigned tell(int fd);
void close(int fd);
void sched_yield(void);
void sigaction(int signum, void (*handler)(void));
void sendsig(pid_t pid, int signum);

#endif /* userprog/syscall.h */
