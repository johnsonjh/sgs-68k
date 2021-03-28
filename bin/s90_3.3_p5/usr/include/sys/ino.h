#ifndef SYS_INO_H
#define SYS_INO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ino.h: version 24.1 created on 10/28/91 at 18:33:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ino.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/ino.h	10.1"		*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#if defined SECON && !defined SYS_MLS_H
#include "sys/mls.h"
#endif 

/*
 *	Inode structure as it appears on a disk block.
 */

struct	dinode {
	ushort	di_mode;	/* mode and type of file */
	short	di_nlink;    	/* number of links to file,used as a reference
				   count in an ACL inode */
	ushort	di_uid;      	/* owner's user id */
	ushort	di_gid;      	/* owner's group id */
	off_t	di_size;     	/* number of bytes in file(or in the ACL 
				   in the case of an ACL inode) */
	char  	di_addr[39];	/* disk block addresses */
	uchar	di_gen;		/* file generation number */
	time_t	di_atime;   	/* time last accessed */
	time_t	di_mtime;   	/* time last modified */
	time_t	di_ctime;   	/* time created */
	uint di_priv;		/* inherited priviledge */
#ifdef SECON
	slabel_t di_label;	/* MAC label */
#else
	uchar	di_lpad[18];	 
#endif
	uint di_acl_inode_type;    /* private,shared or named*/
	ino_t di_acl_inode;	/*special ACL inode */
	ino_t di_default_acl_inode;
	/*
	 * The di_quantum	should be declared as SIGNED   SHORT.
	 * The di_slice_clamp	should be declared as SIGNED   SHORT.
	 * The di_pri_clamp	should be declared as SIGNED   SHORT.
	 * The di_usrpri_clamp	should be declared as SIGNED   SHORT.
	 * The di_cpu_clamp	should be declared as SIGNED   SHORT.
	 * The di_pri_adj	should be declared as SIGNED   SHORT.
	 * The di_perf_flags	should be declared as UNSIGNED INT.
	 */
#ifdef	PERF
	short di_quantum;	/* this binary's private quantum size */
	short di_slice_clamp;	/* this binary's private time slice limit */
	short di_pri_clamp;	/* this binary's private pri limit */
	short di_usrpri_clamp;	/* this binary's private usrpri limit */
	short di_cpu_clamp;	/* this binary's private cpu limit */
#else	/* PERF */
	short di_pad1;
	short di_pad2;
	short di_pad3;
	short di_pad4;
	short di_pad5;
#endif	/* PERF */
	short di_pri_adj;	/* this binary's private pri adj value */
	uint  di_perf_flags;	/* this binary's private performance flags */
	uchar pad[18];	
};
/*
 * The 39 address bytes: 13 addresses of 3 bytes each.
 *
 * The 40'th byte is used as generation count to allow detection of
 * the disk inode being reused.
 */

#endif /* SYS_INO_H */
