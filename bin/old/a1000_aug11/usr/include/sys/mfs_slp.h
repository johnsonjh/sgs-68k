#ifndef SYS_MFS_SLP_H
#define SYS_MFS_SLP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mfs_slp.h: version 2.1 created on 5/22/89 at 19:08:45	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mfs_slp.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:mfs_slp.h	2.1"

/* mfs_sleep.h
 *
 *	macros and definitions for the multiprocessor sleep/wakeup
 */

#ifndef SYS_PROC_H
#include "sys/proc.h"
#endif /* SYS_PROC_H */

typedef struct {
	struct proc	*first;	/* head of list, proc waiting the longest */
	struct proc	*last;	/* tail of list, proc most recently added */
} MFS_SLEEP_QUE;

/*
 * *mfs_sleep_que[] (defined in mfs_sleep.c) is queue of processes waiting
 * to enter long critical regions.  they are added to the end of the queue
 * by the mfs_sleep() and mfs_sleep_with_sig_check().
 *
 * hsque[] (the sleep queue) is maintained separately by sleep for uniprocessor
 * use only. sleep should not be called by any multiprocessor kernel routines.
 */

extern MFS_SLEEP_QUE	mfs_sleep_que[];
extern unsigned int	mfs_sleep_sem;

#define	LDQUE_LEN	128	/* must be power of 2 */
#define	ldqhash(X)	(&mfs_sleep_que[((int)X >> 3) & (LDQUE_LEN-1)])

#endif /* SYS_MFS_SLP_H */
