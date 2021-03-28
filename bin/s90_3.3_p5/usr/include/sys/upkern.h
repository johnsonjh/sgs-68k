#ifndef SYS_UPKERN_H
#define SYS_UPKERN_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) upkern.h: version 24.1 created on 10/28/91 at 18:40:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)upkern.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* UPKERN support */

typedef struct {
	ushort	up_cnt;		/* reference count of lock */
	ushort	up_pm_id;	/* pm_id of processor that has lock */
	uint	up_lock_cnt;	/* number of successful upkern_locks */
	uint	up_qswtch_cnt;	/* number of upkern_locks that had to qswtch */
} upkern_t;

/* the upper bit of up_cnt is used as the upkern_waiting_bit  */
#define UPKERN_WAITING_BIT_SHORT	0x8000
#define UPKERN_WAITING_BIT_LONG		0x80000000

#define UPKERN_CNT_LONG			0x00010000

#endif /* SYS_UPKERN_H */
