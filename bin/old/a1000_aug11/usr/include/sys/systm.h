#ifndef SYS_SYSTM_H
#define SYS_SYSTM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) systm.h: version 2.1 created on 5/22/89 at 19:11:48	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)systm.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/systm.h	10.2"		*/

#ident	"@(#)uts/head/sys:systm.h	2.1"

/*
 * Random set of variables used by more than one routine.
 */

extern struct inode *rootdir;	/* pointer to inode of root directory */
extern time_t lbolt;		/* time in HZ since last boot */
extern time_t time;		/* time in sec from 1970 */

extern char runin;		/* scheduling flag */
extern char runout;		/* scheduling flag */
extern struct proc *runq;	/* head of linked list of running processes */

extern		maxmem;		/* max available memory (clicks) */
extern		physmem;	/* physical memory (clicks) on this CPU */
extern		maxclick;	/* Highest physical click + 1.		*/
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
struct inode *remote_call();
struct buf *getblk();
struct buf *geteblk();
struct buf *bread();
struct buf *breada();
struct file *falloc();
int	upath();
int	spath();
struct proc *prfind();

/*
 * Structure of the system-entry table
 */
extern struct sysentarg {
	char	sy_narg;		/* total number of arguments */
} sysentarg[];
extern struct sysentcall {
	int	(*sy_call)();		/* handler */
} sysentcall[];
extern struct sysentcall sl_sysentcall[];

#endif /* SYS_SYSTM_H */
