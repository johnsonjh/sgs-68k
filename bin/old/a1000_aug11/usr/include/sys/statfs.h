#ifndef SYS_STATFS_H
#define SYS_STATFS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) statfs.h: version 2.1 created on 5/22/89 at 19:11:05	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)statfs.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/statfs.h	10.2"		*/

#ident	"@(#)uts/head/sys:statfs.h	2.1"

/*
 * Structure returned by the statfs() system call.
 */

struct	statfs {
	short	f_fstyp;	/* File system type */
	long	f_bsize;	/* Block size */
	long	f_frsize;	/* Fragment size (if supported) */
	long	f_blocks;	/* Total number of blocks on file system */
	long	f_bfree;	/* Total number of free blocks */
	long	f_files;	/* Total number of file nodes (inodes) */
	long	f_ffree;	/* Total number of free file nodes */
	char	f_fname[6];	/* Volume name */
	char	f_fpack[6];	/* Pack name */
};

#endif /* SYS_STATFS_H */
