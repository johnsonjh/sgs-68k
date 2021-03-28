
/*	Copyright (c) 1989 ARIX CORP */
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX CORP. */
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ARIX: #ident	"head:tar.h	1.1"			*/
/*	3.0 SID #	1.1	*/

/* This header file was created for POSIX 1003.1 compliance */


#ifdef _POSIX_SOURCE

#ifndef TAR_H
#ident	"@(#)tar.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define TAR_H

#include <sys/types.h>

#define	NAMSIZ	100
struct	header	{
	char	name[NAMSIZ];
	char	mode[8];
	char	uid[8];
	char	gid[8];
	char	size[12];
	char	mtime[12];
	char	chksum[8];
	char	typeflag;		/* Changed from linkflag, in V.3 */
	char	linkname[NAMSIZ];
	/* POSIX Extensions begin */
	char	magic[6];
	char	version[2];
	char	uname[32];
	char	gname[32];
	char	devmajor[8];
	char	devminor[8];
	char	prefix[155];
	/* POSIX Extensions end */
	};

/* These fields are used to show this version of tar is the POSIX one,
   in the 'magic' & 'version' fields */
#define	TMAGIC		"ustar"
#define	TMAGLEN		6
#define	TVERSION	"00"
#define	TVERSLEN	2

/* These values are used in the typeflag field */
#define	REGTYPE		'0'		/* regular file */
#define	AREGTYPE	'\0'		/* regular file */
#define	LNKTYPE		'1'		/* link */
#define	SYMTYPE		'2'		/* symbolic link, not supported */
#define	CHRTYPE		'3'		/* character special */
#define	BLKTYPE		'4'		/* block special */
#define	DIRTYPE		'5'		/* directory */
#define	FIFOTYPE	'6'		/* fifo */
#define	CONTTYPE	'7'		/* reserved */
#define	ACLTYPE		'8'		/* ACL file */

/* These values are used in the mode field */
#define	TSUID	04000		/* Set uid on execution */
#define	TSGID	02000		/* Set gid on execution */
#define	TSVTX	01000		/* Reserved */
#define	TUREAD	00400		/* read by owner */
#define	TUWRITE	00200		/* write by owner */
#define	TUEXEC	00100		/* Execute / search by owner */
#define	TGREAD	00040		/* read by group */
#define	TGWRITE 00020		/* write by group */
#define	TGEXEC	00010		/* Execute / search by group */
#define	TOREAD	00004		/* read by other */
#define	TOWRITE	00002		/* write by other */
#define TOEXEC  00001		/* Execute / search by other */

/* This structure is necessary for acl support in tar, and resides in
   the data portion of the file */
struct	secure_tar {

	char		*dflt_acl;	/* Ptr to beginning of default acl */
	uint		dflt_acl_length;	/* Length of default acl */
	ino_t		dflt_acl_ino;	/* Inode # of default acl */
	dev_t		dflt_acl_dev;	/* Device of dflt acl */
	char		dflt_file [100];/* File dflt acl belongs to */
	char		*reg_acl;	/* Ptr to beginning of acl */
	uint		reg_acl_length;	/* Length of acl */
	ino_t		reg_acl_ino;	/* Inode # of acl */
	dev_t		reg_acl_dev;	/* Device of acl */
	char		reg_file [100]; /* File acl belongs to */
	char		pad [288];
};
	
#endif /* TAR_H */

#endif /* _POSIX_SOURCE */
