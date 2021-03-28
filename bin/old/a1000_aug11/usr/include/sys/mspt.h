
/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mspt.h: version 2.1 created on 5/22/89 at 19:08:54	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mspt.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
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

