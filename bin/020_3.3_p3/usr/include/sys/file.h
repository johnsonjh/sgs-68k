#ifndef SYS_FILE_H
#define SYS_FILE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) file.h: version 24.1 created on 10/28/91 at 18:32:31	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)file.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif /* SYS_TYPES_H */
/*
 * One file structure is allocated for each open/creat/pipe call.
 * Main use is to hold the read/write pointer associated with
 * each open file.
 */
struct	file  {
	char	f_flag;
	char	pad[3];
	cnt_t	f_count;		/* reference count */
	char	pad1[2];
	union {
		struct inode *f_uinode;	/* pointer to inode structure */
		struct file  *f_unext;	/* next entry in freelist */
	} f_up;
	off_t	f_offset;		/* read/write character pointer */
};

#define	f_inode		f_up.f_uinode
#define	f_next		f_up.f_unext

extern struct file file[];	/* The file table itself */
extern struct file *ffreelist;	/* Head of freelist pool */

/* flags */
#define	FOPEN	(-1)
#define	FREAD	0x01
#define	FWRITE	0x02
#define	FNDELAY	0x04
#define	FAPPEND	0x08
#define	FSYNC	0x10
#define	FRCACH	0x20
#define	FMASK	0x7f

/* open only modes */
#define	FCREAT	0x0100
#define	FTRUNC	0x0200
#define	FEXCL	0x0400
#ifdef _POSIX_SOURCE
#define FNOCTTY 0x0800
#endif

#endif /* SYS_FILE_H */
