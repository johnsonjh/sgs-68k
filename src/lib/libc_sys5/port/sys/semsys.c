#ident	"@(#)semsys.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*   Arete/5.0   6/30/84   /usr/src/lib/libc/port/sys/semsys.c   */


#include	"sys/types.h"
#include	"sys/ipc.h"
#include	"sys/sem.h"


#define	SEMCTL	0
#define	SEMGET	1
#define	SEMOP	2

semctl(semid, semnum, cmd, arg)
int semid, cmd;
int semnum;
union semun {
	int val;
	struct semid_ds *buf;
	ushort array[1];
} arg;
{
	return(semsys(SEMCTL, semid, semnum, cmd, arg));
}

semget(key, nsems, semflg)
key_t key;
int nsems, semflg;
{
	return(semsys(SEMGET, key, nsems, semflg));
}

semop(semid, sops, nsops)
int semid;
struct sembuf (*sops)[];
int nsops;
{
	return(semsys(SEMOP, semid, sops, nsops));
}
