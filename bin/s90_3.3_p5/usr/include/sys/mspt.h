#ifndef	SYS_MSPT_H
#define SYS_MSPT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mspt.h: version 24.1 created on 10/28/91 at 18:35:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mspt.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * Structures and definitions supporting the pseudo terminal
 * drivers.
 */
struct mspt_ttys {
	unsigned pt_state;	/* state of master/slave pair */
	queue_t *mpt_wrq; 	/* master's write queue pointer */
	queue_t *spt_wrq; 	/* slave's write queue pointer */
	mblk_t *pt_bufp;        /* ptr. to zero byte msg. blk. */
	short tty;	        /* controlling tty pid */
};

/*
 * pt_state values
 */
#define PTLOCK	01	/* master/slave pair is locked */
#define PTMOPEN 02  	/* master side is open */
#define PTSOPEN 04	/* slave side is open */

/*
 * ioctl commands
 */
#define ISPTM	(('P'<<8)|1)	/* query for master */
#define UNLKPT	(('P'<<8)|2)	/* unlock master/slave pair */

#endif /*  SYS_MSPT_H */
