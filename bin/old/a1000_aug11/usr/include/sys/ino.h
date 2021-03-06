#ifndef SYS_INO_H
#define SYS_INO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ino.h: version 2.1 created on 5/22/89 at 19:07:48	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ino.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)kern-port:sys/ino.h	10.2"

/*
 *	Inode structure as it appears on a disk block.
 */

struct	dinode
{
	ushort	di_mode;	/* mode and type of file */
	short	di_nlink;    	/* number of links to file */
	ushort	di_uid;      	/* owner's user id */
	ushort	di_gid;      	/* owner's group id */
	off_t	di_size;     	/* number of bytes in file */
	char  	di_addr[39];	/* disk block addresses */
	char	di_gen;		/* file generation number */
	time_t	di_atime;   	/* time last accessed */
	time_t	di_mtime;   	/* time last modified */
	time_t	di_ctime;   	/* time created */
};
/*
 * The 40 address bytes:
 *  39 used as 13 addresses of 3 bytes each.
 *  40th byte is used as a file generation count to allow detection of
 *  the disk inode being reused.
 */
#endif /* SYS_INO_H */
