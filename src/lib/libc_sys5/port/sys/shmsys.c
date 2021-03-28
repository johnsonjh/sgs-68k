#ident	"@(#)shmsys.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*   Arete/5.0   6/30/84   /usr/src/lib/libc/port/sys/shmsys.c   */


#ifndef	pdp11
#include	"sys/types.h"
#include	"sys/ipc.h"
#include	"sys/region.h"
#include	"sys/shm.h"


#define	SHMAT	0
#define	SHMCTL	1
#define	SHMDT	2
#define	SHMGET	3
char *
shmat(shmid, shmaddr, shmflg)
int shmid;
char *shmaddr;
int shmflg;
{
	return((char *)shmsys(SHMAT, shmid, shmaddr, shmflg));
}

shmctl(shmid, cmd, buf)
int shmid, cmd;
struct shmid_ds *buf;
{
	return(shmsys(SHMCTL, shmid, cmd, buf));
}

shmdt(shmaddr)
char *shmaddr;
{
	return(shmsys(SHMDT, shmaddr));
}

shmget(key, size, shmflg)
key_t key;
int size, shmflg;
{
	return(shmsys(SHMGET, key, size, shmflg));
}
#endif
