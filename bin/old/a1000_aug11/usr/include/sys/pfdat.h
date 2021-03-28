#ifndef SYS_PFDAT_H
#define SYS_PFDAT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) pfdat.h: version 2.1 created on 5/22/89 at 19:09:25	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)pfdat.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/pfdat.h	10.2"		*/

#ident	"@(#)uts/head/sys:pfdat.h	2.1"


typedef struct pfdat {
	unsigned	pf_blkno : 24,	/* Disk block nummber.	*/
			pf_flags : 8;	/* page flags		*/
	cnt_t		pf_use;		/* share use count	*/
	dev_t		pf_dev;		/* Disk device code.	*/
	long		pf_inumber;	/* inode number of 	*/
					/* matching file	*/
	char		pf_swpi;	/* Index into swaptab.	*/
	unsigned char	pf_rawcnt;	/* Cnt of processes	*/
					/* doing raw I/O to 	*/
					/* page.		*/
	short		pf_waitcnt;	/* Number of processes	*/
					/* waiting for PG_DONE	*/
	struct pfdat	*pf_next;	/* Next free pfdat.	*/
	struct pfdat	*pf_prev;	/* Previous free pfdat.	*/
	struct pfdat	*pf_hchain;	/* Hash chain link.	*/
} pfd_t;

#define	P_QUEUE		0x01	/* Page on free queue		*/
#define	P_BAD		0x02	/* Bad page (parity error, etc.)*/
#define	P_HASH		0x04	/* Page on hash queue		*/
#define P_DONE		0x08	/* I/O to read page is done	*/
#define	P_SWAP		0x10	/* Page on swap (not file).	*/

extern struct pfdat phead;
extern struct pfdat pbad;
extern struct pfdat *pfdat;
extern struct pfdat **phash;
extern struct pfdat ptfree;
extern int phashmask;
extern struct pfdat	*pfind();


#define BLKNULL		0	/* pf_blkno null value		*/

extern int	mem_lock;
extern int	memlock();
extern int	memunlock();
extern int	memlocked();

#endif /* SYS_PFDAT_H */
