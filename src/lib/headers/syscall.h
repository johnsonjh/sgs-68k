
#ifndef _SYSCALL_H
#ident	"@(#)syscall.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _SYSCALL_H


#include "sys/types.h"	/* for stat prototype, struct stat typedef needed*/
#include "stat.h"
#include "unistd.h"

int kill(pid_t,int);
pid_t getpid(void);
off_t lseek(int,off_t,int);
pid_t fork(void);
pid_t wait(int *);
int fcntl(int,int,...);
int link(char *,char *);
int unlink(char *);
int access(char *,int);
int read(int,char *,unsigned int);
int write(int,char *,unsigned int);
int isatty(int);
int open ( char*, int,...);
int close(int);
int brk ( char * );
char *sbrk ( int );
int stat (char *, struct stat *);
int fstat ( int, struct stat *);
int execl(char *,...);
void _exit(int);

#if !defined __STRICT_STDC__

#include "pwd.h"

uid_t getuid(void);
struct passwd *getpwuid(uid_t);
int creat(char *, mode_t);
int pause(void );
int execv(char *, char *[]);
char *getlogin(void);
void endpwent(void);
int ioctl(int,int,signed int);
int pipe(int[2]);
void profil ( char *, int, int, int );
unsigned alarm ( unsigned );

#endif /* ! __STRICT_STDC__ */

#endif /* _SYSCALL_H */
