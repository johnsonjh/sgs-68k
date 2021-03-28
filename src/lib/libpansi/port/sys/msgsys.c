#ident	"@(#)msgsys.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*   Arete/5.0   6/30/84   /usr/src/lib/libc/port/sys/msgsys.c   */


#include	"sys/types.h"
#include	"sys/ipc.h"
#include	"sys/msg.h"


#define	MSGGET	0
#define	MSGCTL	1
#define	MSGRCV	2
#define	MSGSND	3

msgget(key, msgflg)
key_t key;
int msgflg;
{
	return(msgsys(MSGGET, key, msgflg));
}

msgctl(msqid, cmd, buf)
int msqid, cmd;
struct msqid_ds *buf;
{
	return(msgsys(MSGCTL, msqid, cmd, buf));
}

msgrcv(msqid, msgp, msgsz, msgtyp, msgflg)
int msqid;
struct msgbuf *msgp;
int msgsz;
long msgtyp;
int msgflg;
{
	return(msgsys(MSGRCV, msqid, msgp, msgsz, msgtyp, msgflg));
}

msgsnd(msqid, msgp, msgsz, msgflg)
int msqid;
struct msgbuf *msgp;
int msgsz, msgflg;
{
	return(msgsys(MSGSND, msqid, msgp, msgsz, msgflg));
}
