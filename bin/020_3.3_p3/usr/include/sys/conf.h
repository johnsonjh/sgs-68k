#ifndef SYS_CONF_H
#define SYS_CONF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) conf.h: version 24.1 created on 10/28/91 at 18:31:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)conf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/conf.h	10.4"		*/


#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#define UNIPROC		0	/* defines for devsw tables */
#define MULTIPROC	1

/*
 * Declaration of block device switch. Each entry (row) is
 * the only link between the main unix code and the driver.
 * The initialization of the device switches is in the file conf.c.
 */
struct bdevsw {
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_strategy)();
	int	(*d_print)();
	int	multiproc;	/* set for multiprocessor drivers */
};
extern struct bdevsw bdevsw[];

/*
 * Character device switch.
 */
struct cdevsw {
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_read)();
	int	(*d_write)();
	int	(*d_ioctl)();
	struct tty *d_ttys;
	struct streamtab *d_str;
	int	multiproc;	/* set for multiprocessor drivers */
};
extern struct cdevsw cdevsw[];

#define	FMNAMESZ	8

struct fmodsw {
						/* Round to long for aSPARC */
 	char	f_name[(FMNAMESZ + sizeof(long)) & ~(sizeof(long) - 1)];
	struct  streamtab *f_str;
};
extern struct fmodsw fmodsw[];

extern int	bdevcnt;
extern int	cdevcnt;
extern int	fmodcnt;

/*
 * Line discipline switch.
 */
struct linesw {
	int	(*l_open)();
	int	(*l_close)();
	int	(*l_read)();
	int	(*l_write)();
	int	(*l_ioctl)();
	int	(*l_input)();
	int	(*l_output)();
	int	(*l_mdmint)();
};
extern struct linesw linesw[];

extern int	linecnt;
/*
 * Terminal switch
 */
struct termsw {
	int	(*t_input)();
	int	(*t_output)();
	int	(*t_ioctl)();
};
extern struct termsw termsw[];

extern int	termcnt;

/*file system switch structure */
struct fstypsw {
/* 0*/	int		(*fs_init)();
	uint		init_flag;
/* 1*/	int		(*fs_iput)();
	uint		iput_flag;
/* 2*/	struct inode	*(*fs_iread)();
	uint		iread_flag;
/* 3*/	int		(*fs_filler)();
	uint		filler_flag;
/* 4*/	int		(*fs_iupdat)();
	uint		iupdat_flag;
/* 5*/	int		(*fs_readi)();
	uint		readi_flag;
/* 6*/	int		(*fs_writei)();
	uint		writei_flag;
/* 7*/	int		(*fs_itrunc)();
	uint		itrunc_flag;
/* 8*/	int		(*fs_statf)();
	uint		statf_flag;
/* 9*/	int		(*fs_namei)();
	uint		namei_flag;
/*10*/	int		(*fs_mount)();
	uint		mount_flag;
/*11*/	int		(*fs_umount)();
	uint		umount_flag;
/*12*/	struct inode	*(*fs_getinode)();
	uint		getinode_flag;
/*13*/	int		(*fs_openi)();		/* open inode */
	uint		openi_flag;
/*14*/	int		(*fs_closei)();		/* close inode */
	uint		closei_flag;
/*15*/	int		(*fs_update)();		/* update */
	uint		update_flag;
/*16*/	int		(*fs_statfs)();		/* statfs and ustat */
	uint		statfs_flag;
/*17*/	int		(*fs_access)();
	uint		access_flag;
/*18*/	int		(*fs_getdents)();
	uint		getdents_flag;
/*19*/	int		(*fs_allocmap)();	/* Let the fs decide if */
						/* if can build a map so */
						/* this fs can be used for */
						/* paging */
	uint		allocmap_flag;
/*20*/	int		(*fs_freemap)();	/* free block list */
	uint		freemap_flag;
/*21*/	int		(*fs_readmap)();	/* read a page from the fs */
						/* using the block list */
	uint		readmap_flag;
/*22*/	int		(*fs_setattr)();	/* set attributes */
	uint		setattr_flag;
/*23*/	int		(*fs_notify)();		/* notify fs of action */
	uint		notify_flag;
/*24*/	int		(*fs_fcntl)();		/* fcntl */
	uint		fcntl_flag;
/*25*/	int		(*fs_fsinfo)();		/* additional info */
	uint		fsinfo_flag;
/*26*/	int		(*fs_ioctl)();		/* ioctl */
	uint		ioctl_flag;
/*27*/	int		(*fs_fill[5])();
};
extern struct fstypsw fstypsw[];
extern short nfstyp;

/* fstypsw flag defines */
#define FS_MULTIPROC	1			/* multi-processor function */


/* FS specific data */
struct fsinfo {
	long		fs_flags;	/* flags - see below */
	struct mount	*fs_pipe;	/* The mount point to be used */
					/* as the pipe device for */
					/* this fstyp */
	char		*fs_name; 	/* Pointer to fstyp name */
					/* See above */
	long		fs_notify;	/* Flags for fs_notify */
					/* e.g., NO_CHDIR, NO_CHROOT */
					/* see nami.h */
};
extern struct fsinfo fsinfo[];

#endif /* SYS_CONF_H */
