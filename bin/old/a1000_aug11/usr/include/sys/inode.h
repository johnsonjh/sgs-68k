#ifndef SYS_INODE_H
#define SYS_INODE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) inode.h: version 2.1 created on 5/22/89 at 19:07:51	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)inode.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	inode.h	3.13	*/
/*   	Arete/5.0   6/30/84   /usr/src/head/sys/inode.h   */

#ident	"@(#)uts/head/sys:inode.h	2.1"

/*
 * The I node is the focus of all
 * file activity in unix. There is a unique
 * inode allocated for each active file,
 * each current directory, each mounted-on
 * file, text file, and the root. An inode is 'named'
 * by its dev/inumber pair. (iget/iget.c)
 * Data, from mode on, is read in
 * from permanent inode on volume.
 */

#define FSPTR	1

/* file locking hooks */
struct locklist {
	/* Note link must be first in struct */
	struct	locklist  *ll_link;	/* link to next lock region */
	int		  ll_flags;	/* misc flags ** sleeping */
	struct	proc      *ll_proc;	/* process which owns region */
	off_t		  ll_start;	/* starting offset */
	off_t		  ll_end;	/* ending offset, zero is eof */
};

typedef struct	inode
{
	struct inode *i_forw;	/* hash chain forw */
	struct inode *i_back;	/* hash chain back */
	struct inode *av_forw;	/* freelist chain forw */
	struct inode *av_back;	/* freelist chain back */
	int	*i_fsptr;	/* pointer to fs dependent */

	ushort	i_flag;
	ushort	i_ftype;	/* IFDIR, IFREG, etc. */

	cnt_t	i_count;	/* reference count */
	ushort	i_fstyp;	/* file system type */
	dev_t	i_dev;		/* device where inode resides */
	dev_t	i_rdev;		/* raw device number */
	long	i_number;	/* i number, 1-to-1 with device address */
	struct	mount *i_mntdev;/* ptr to mount entry that inode resides on */
	ushort	i_mode;
	short	i_fill;		

	short	i_nlink;	/* directory entries */
	ushort	i_uid;		/* owner */

	ushort	i_gid;		/* group of owner */
	ushort	i_wcnt;		/* open for write count */
	unsigned int i_vcode;	/* inode version code */
	struct  rcvd  *i_rcvd;	/* receive descriptor */

	off_t	i_size;		/* size of file */

	union	i_u  {
		struct  mount *i_mton;  /* mount table entry that is mounted
					   on this inode */
		struct  stdata *i_sp;   /* associated stream */
	} i_un;

	struct  fstypsw *i_fstypp; /* file system switch structure */

	struct locklist *i_locklist;	/* locked region list locking	      */
	long 	*i_filocks;		/* pointer to filock (structure) list */
	int	i_inode_lock;
	int	reader_count;

} inode_t;


extern struct locklist locklist[];	/* The lock table itself */

extern struct inode inode[];	/* The inode table itself */

struct	ifreelist  {
	int	pad[2];
	struct	inode	*av_forw;
	struct	inode	*av_back;
};
extern  struct ifreelist ifreelist;
#define i_sptr i_un.i_sp
#define i_mnton i_un.i_mton


/* flags */
#define	ILOCK	0x01		/* inode is locked */
#define	IUPD	0x02		/* file has been modified */
#define	IACC	0x04		/* inode access time to be updated */
#define	IMOUNT	0x8		/* inode is mounted on */
#define	IWANT	0x10		/* some process waiting on lock */
#define	ITEXT	0x20		/* inode is pure text prototype */
#define	ICHG	0x40		/* inode has been changed */
#define	ISYN	0x80		/* do synchronous write for iupdate */
#define	IADV	0x100		/* advertised */
#define	IDOTDOT	0x200		/* object of remote mount */
#define	IRMOUNT	0x800		/* remotely mounted	*/
#define	IISROOT	0x1000		/* This is a root inode of an fs */
#define	IWROTE	0x2000		/* write has happened since open */

/* file types */
/* WARNING: The following defines should NOT change!If more */
/* file types need to be added they should be added in the low */
/* bits */

#define		IFMT	0xf000	/* type of file */
#define		IFDIR	0x4000	/* directory */
#define		IFCHR	0x2000	/* character special */
#define		IFBLK	0x6000	/* block special */
#define		IFREG	0x8000	/* regular */
#define		IFMPC	0x3000	/* multiplexed char special */
#define		IFMPB	0x7000	/* multiplexed block special */
#define		IFIFO	0x1000	/* fifo special */

/* file modes */
/* the System V Rel 2 chmod system call only knows about */
/* ISUID, ISGID, ISVTX */
/* Therefore, the bit positions of ISUID, ISGID, and ISVTX */
/* should not change */
#define	ISUID	0x800		/* set user id on execution */
#define	ISGID	0x400		/* set group id on execution */
#define ISVTX	0x200		/* save swapped text even after use */

/* access requests */
/* the System V Rel 2 chmod system call only knows about */
/* IREAD, IWRITE, IEXEC */
/* Therefore, the bit positions of IREAD, IWRITE, and IEXEC */
/* should not change */
#define	IREAD		0x100	/* read permission */
#define	IWRITE		0x080	/* write permission */
#define	IEXEC		0x040	/* execute permission */
#define	ICDEXEC		0x020	/* cd permission */
#define	IOBJEXEC	0x010	/* execute as an object file */
				/* i.e., 410, 411, 413 */
#define IMNDLCK		0x001	/* mandatory locking set */

#define	MODEMSK		0xfff	/* Nine permission bits - read/write/ */
				/* execute for user/group/others and */
				/* ISUID, ISGID, and ISVTX */	
				/* This is another way of saying: */
				/* (ISUID|ISGID|ISVTX| */
				/* (IREAD|IWRITE|IEXEC)| */
				/* ((IREAD|IWRITE|IEXEC)>>3)| */
				/* ((IREAD|IWRITE|IEXEC)>>6)) */
#define	PERMMSK		0x1ff	/* Nine permission bits: */
				/* ((IREAD|IWRITE|IEXEC)| */
				/* ((IREAD|IWRITE|IEXEC)>>3)| */
				/* ((IREAD|IWRITE|IEXEC)>>6)) */

/* lock flags */
#define INODE_WANTED	00001
#define INODE_LOCKED	00002
#define READER_LOCKED	00004


#endif /* SYS_INODE_H */
