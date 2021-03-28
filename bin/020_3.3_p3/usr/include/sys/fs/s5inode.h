#ifndef SYS_FS_S5INODE_H
#define SYS_FS_S5INODE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) s5inode.h: version 24.1 created on 10/28/91 at 18:32:54	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)s5inode.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/fs/s5inode.h	10.2"	*/

#if defined SECON && !defined  SYS_MLS_H
#include "sys/mls.h"
#endif 

#define	NADDR	13
#define	NSADDR	(NADDR*sizeof(daddr_t)/sizeof(short))
#define NMACBLK	4	/* number of MAC blocks */

struct s5inode {
	ushort	s5i_flags;	/* see below */	
	ushort	s5i_mode;	/* file mode and type */
	union s5i_u1 {
		struct	s5inode	*s5i_nxt; /* free list pntr */
		daddr_t s5i_a[NADDR];	/* if normal file/directory */
		short	s5i_f[NSADDR];	/* counts for fifo's */
	} s5i_u1;
	union s5i_u2 {
		int	*s5i_mp;	/* pointer to blk # map for paging */
		off_t	s5i_doff;	/* directory offset - */
					/* for directory caching */
	} s5i_u2;
	daddr_t	s5i_l;		/* last logical block read (for read-ahead) */
	char s5i_gen;		/* generation number */
#ifdef SECON
	slabel_t s5i_label;			/* for security label */
#else 
	uchar	s5i_lpad[18];			/* for security labels */
#endif
	uint	s5i_acl_inode_type;		/* private, shared, or named */
	ino_t 	s5i_acl_inode;			/* ACL inode */
	ino_t	s5i_default_acl_inode;
	/*
	 * The s5i_quantum	should be declared as SIGNED   SHORT.
	 * The s5i_slice_clamp	should be declared as SIGNED   SHORT.
	 * The s5i_pri_clamp	should be declared as SIGNED   SHORT.
	 * The s5i_usrpri_clamp	should be declared as SIGNED   SHORT.
	 * The s5i_cpu_clamp	should be declared as SIGNED   SHORT.
	 * The s5i_pri_adj	should be declared as SIGNED   SHORT.
	 * The s5i_perf_flags	should be declared as UNSIGNED INT.
	 */
#ifdef	PERF
	short	s5i_quantum;		/* this binary's quantum size	   */
	short	s5i_slice_clamp;	/* this binary's time slice limit  */
	short	s5i_pri_clamp;		/* this binary's pri limit	   */
	short	s5i_usrpri_clamp;	/* this binary's usrpri limit	   */
	short	s5i_cpu_clamp;		/* this binary's cpu limit	   */
#else	/* PERF */
	short	s5i_pad1;
	short	s5i_pad2;
	short	s5i_pad3;
	short	s5i_pad4;
	short	s5i_pad5;
#endif	/* PERF */
	short	s5i_pri_adj;		/* this binary's pri adj value	   */
	uint	s5i_perf_flags;		/* this binary's performance flags */
};

/* Flags */
#define	S5IFREE		0x0	/* Free cell */

#define	s5i_next	s5i_u1.s5i_nxt
#define	s5i_addr	s5i_u1.s5i_a
#define	s5i_lastr	s5i_l
#define	s5i_rdev	s5i_u1.s5i_a[0]

#define	s5i_map		s5i_u2.s5i_mp
#define	s5i_diroff	s5i_u2.s5i_doff

#define	s5i_faddr	s5i_u1.s5i_a
#define	NFADDR	10
#define	PIPSIZ	NFADDR*BSIZE
#define	s5i_frptr	s5i_u1.s5i_f[NSADDR-5]
#define	s5i_fwptr	s5i_u1.s5i_f[NSADDR-4]
#define	s5i_frcnt	s5i_u1.s5i_f[NSADDR-3]
#define	s5i_fwcnt	s5i_u1.s5i_f[NSADDR-2]
#define	s5i_waite	s5i_u1.s5i_f[NSADDR-3]
#define	s5i_waitf	s5i_u1.s5i_f[NSADDR-2]
#define	s5i_fflag	s5i_u1.s5i_f[NSADDR-1]
#define	IFIR	01
#define	IFIW	02

extern struct s5inode s5inode[];

#endif /* SYS_FS_S5INODE_H */
