#ifndef SYS_FS_S5PARAM_H
#define SYS_FS_S5PARAM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) s5param.h: version 24.1 created on 10/28/91 at 18:33:01	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)s5param.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	kern-port:sys/fs/s5param.h	10.4"	*/

/*
/*
 * filesystem parameters
 */

#define	SUPERB	((daddr_t)1)	/* block number of the super block */
#define	DIRSIZ	14		/* max characters per directory */
#define	NICINOD	100		/* number of superblock inodes */
#define	NICFREE	50		/* number of superblock free blocks */
#define	S5ROOTINO	2	/* i number of all roots */

#ifndef FsTYPE
#define FsTYPE	2
#endif


#if FsTYPE==2
/* New 1024 byte file system */
#define	BSIZE		1024		/* size of secondary block (bytes) */
#define SBUFSIZE	1024		/* system buffer size */
#define	BSHIFT		10		/* log2(BSIZE) */
#define	NINDIR		(BSIZE/sizeof(daddr_t))	/* BSIZE/sizeof(daddr_t) */
#define	BMASK		01777		/* BSIZE-1 */

#define INOPB 		16
#define INOSHIFT	4

/* SECURITY*/
#define SECINOPB(isize) 	(BSIZE/isize)
#define SECINOSHIFT(isize)	((isize==64) ? 4 : 3)

#define	NMASK		0377		/* NINDIR-1 */
#define	NSHIFT		8		/* log2(NINDIR) */
#define NDPC		4		/* number of blocks per click */
#define Fs2BLK		0x8000		/* large block flag in bsize */
#endif


#if FsTYPE==3
/* New 2048 byte file system */
#define	BSIZE		2048		/* size of secondary block (bytes) */
#define SBUFSIZE	2048		/* system buffer size */
#define	BSHIFT		11		/* log2(BSIZE) */
#define	NINDIR		(BSIZE/sizeof(daddr_t))	/* BSIZE/sizeof(daddr_t) */
#define	BMASK		03777		/* BSIZE-1 */
#define INOPB		32		/* BSIZE/sizeof(struct dinode) */
#define INOSHIFT	5		/* log2(INOPB) */
#define	NMASK		0777		/* NINDIR-1 */
#define	NSHIFT		9		/* log2(NINDIR) */
#define NDPC		2		/* number of blocks per click */
#define Fs2BLK		0x8000		/* large block flag in bsize */
#endif


#if FsTYPE==4
/* New 4096 byte file system */
#define BSIZE           4096            /* size of secondary block (bytes) */
#define SBUFSIZE        4096            /* system buffer size */
#define BSHIFT          12              /* log2(BSIZE) */
#define NINDIR          (BSIZE/sizeof(daddr_t)) /* BSIZE/sizeof(daddr_t) */
#define BMASK           07777           /* BSIZE-1 */
#define INOPB           64              /* BSIZE/sizeof(struct dinode) */
#define INOSHIFT        6               /* log2(INOPB) */
/* SECURITY*/
#define SECINOPB(isize) 	(BSIZE/isize)
#define SECINOSHIFT(isize)	((isize==64) ? 6 : 5)
#define NMASK           01777           /* NINDIR-1 */
#define NSHIFT          10              /* log2(NINDIR) */
#define NDPC            1               /* number of blocks per click */
#define Fs2BLK          0x8000          /* large block flag in bsize */
#endif

#define SUPERBOFF	512	/* superblock offset */

#endif /* SYS_FS_S5PARAM_H */
