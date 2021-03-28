#ifndef SYS_SYSTM_H
#define SYS_SYSTM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) systm.h: version 24.1 created on 10/28/91 at 18:39:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)systm.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/systm.h	10.2"		*/

/*
 * Random set of variables used by more than one routine.
 */

extern struct inode *rootdir;	/* pointer to inode of root directory */
extern time_t lbolt;		/* time in HZ since last boot */
extern time_t time;		/* time in sec from 1970 */

extern int runin;		/* scheduling flag */
extern int runout;		/* scheduling flag */
extern		maxmem;		/* max available memory (clicks) */
extern		physmem;	/* physical memory (clicks) on this CPU */
extern		maxclick;	/* Highest virtual mapped to physical  + 1. */
extern daddr_t	swplo;		/* block number of start of swap space */
extern		nswap;		/* size of swap space in blocks*/
extern dev_t	rootdev;	/* device of the root */
extern dev_t	swapdev;	/* swapping device */
extern dev_t	pipedev;	/* pipe device */
extern char	*panicstr;	/* panic string pointer */
extern		blkacty;	/* active block devices */
extern		pwr_cnt, pwr_act;

dev_t getmdev();
struct inode *iget();
struct inode *namei();
struct inode *lnamei();
struct inode *remote_call();
struct buf *getblk();
struct buf *geteblk();
struct buf *partial_getblk();
struct buf *bread();
struct buf *breada();
struct file *falloc();
int	upath();
int	spath();
struct proc *prfind();

/*
 * Structure of the system-entry table
 */

extern struct sysent {
	char	sy_narg;	/* total number of arguments */
	char	sy_setjmp;	/* if zero, systrap() should do a setjmp() */
	unchar	sy_multiproc;	/* if 0, must obtain upkern */
	int	(*sy_call)();	/* handler */
} sysent[];
extern int	num_sysent;	/* number of sysent entries */

#endif /* SYS_SYSTM_H */
