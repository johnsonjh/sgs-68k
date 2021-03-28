#ifndef SYS_FSTYP_H
#define SYS_FSTYP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) fstyp.h: version 24.1 created on 10/28/91 at 18:33:08	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)fstyp.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/fstyp.h	10.10"		*/

#define NULL_FS	0		/* Null FS type - Invalid */

#define FSTYPSZ		16	/* max size of fs identifier */

/*
 * Opcodes for the sysfs() system call.
 */
#define GETFSIND	1	/* translate fs identifier to fstype index */
#define GETFSTYP	2	/* translate fstype index to fs identifier */
#define GETNFSTYP	3	/* return the number of fstypes */

/* Bit settings for fs_flags */
#define FS_NOICACHE	0x1	/* Don't retain old inodes in in-core cache */
				/* after last reference. Should be off for */
				/* the S5 fstype */
#define FS_RECYCLE	0x2	/* always call FS_IPUT for recycled inode */
#define FS_NOTBUFFERED  0x4	/* Fstyp unsuitable for */

#define	FSBSIZE(ip)	((ip)->i_mntdev)->m_bsize

/* Passed to fs_getinode to indicate intended use of inode */
#define FSG_PIPE	1	/* Pipe */
#define FSG_CLONE	2	/* Use for clone device */

extern	short		pipefstyp;
extern struct inode	*FS_IREAD();

#endif /* SYS_FSTYP_H */
