#ifndef SYS_FS_S5FILSYS_H
#define SYS_FS_S5FILSYS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) s5filsys.h: version 24.1 created on 10/28/91 at 18:32:51	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)s5filsys.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *	Structure of the super-block.
 */

struct	filsys
{
	ushort	s_isize;	/* size in blocks of i-list */
	daddr_t	s_fsize;	/* size in blocks of entire volume */
	short	s_nfree;	/* number of addresses in s_free */
	daddr_t	s_free[NICFREE];/* free block list */
	short	s_ninode;	/* number of i-nodes in s_inode */
	ushort	s_inode[NICINOD];/* free i-node list */
	char	s_flock;	/* lock during free list manipulation */
	char	s_ilock;	/* lock during i-list manipulation */
	char  	s_fmod; 	/* super block modified flag */
	char	s_ronly;	/* mounted read-only flag */
	time_t	s_time; 	/* last super block update */
	short	s_dinfo[4];	/* device information */
	daddr_t	s_tfree;	/* total free blocks*/
	ushort	s_tinode;	/* total free inodes */
	char	s_fname[6];	/* file system name */
	char	s_fpack[6];	/* file system pack name */
	long	s_dinode_size;	/* size of a disk inode on this file system */
	long	s_fill[11];	/* adjust to make sizeof filsys */
	long	s_state;	/* file system state */
	long	s_magic;	/* magic number to indicate new file system */
	long	s_type;		/* type of new file system */
	char	s_fill2[8];	/* adjust to really make sizeof filsys */
};

#define FsMAGIC	0xfd187e20	/* s_magic */

#define Fs1b	1	/* 512 byte block */
#define Fs2b	2	/* 1024 byte block */
#define Fs4b	3	/* 2048 byte block */
#define Fs8b	4	/* 4096 byte block */
#define FsSEC1   6	/* Secure file system with 128 byte inodes(1k) */
#define FsSEC4   7	/* Secure file system with 128 byte inodes(4k) */

#define	FsOKAY		0x7c269d38	/* s_state: clean */
#define	FsACTIVE	0x5e72d81a	/* s_state: active */
#define	FsBAD		0xcb096f43	/* s_state: bad root */
#define FsBADBLK	0xbadbc14b	/* s_state: bad block corrupted it */

#define FsINODESIZE	64	/* standard file system inode size */
#define SEC_INODESIZE	128	/* secure file system inode size */

#define getfs(mp)	(struct filsys *)((struct buf *)mp->m_bufp)->b_un.b_addr


#endif /* SYS_FS_S5FILSYS_H */
