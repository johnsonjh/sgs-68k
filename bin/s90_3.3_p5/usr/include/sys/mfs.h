#ifndef	SYS_MFS_H
#define	SYS_MFS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mfs.h: version 24.1 created on 10/28/91 at 18:35:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mfs.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * mfs.h
 *
 *	defines for the multi-processor file system
 *
 */

#ifndef SYS_SYNCH_H
#include "sys/synch.h"
#endif

#ifndef SYS_OWN_H
#include "sys/own.h"
#endif



#define	mfs_wakeup(x)		wakeup(x)
#define	mfs_wakeup_all(x)	wakeup(x)
#define	suspend_wakeup(x)	wakeup(x)
#define	mfs_wakeup_one(x)	wakeup_one(x)

/* synchronization defines */

#ifndef M68040
#define	exit_short_cr_no_spl(x)	\
{ \
	own.o_spin_lock_cnt--; (x)->lock_psw = 0;	\
}
#define restore_spin_lock(spin_ptr, saved_psw)	\
{ \
	(spin_ptr)->lock_psw = (saved_psw); \
}
#endif	/* !M68040 */

#endif /* SYS_MFS_H */
