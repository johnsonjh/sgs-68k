/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) buf.h: version 24.1 created on 10/28/91 at 17:59:39	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)buf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_BUF_H
#define IOPM_BUF_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef IOPM_OS
#define buf    i_buf
#define buf_t  i_buf_t
#endif

typedef struct i_buf
{
	int           b_flags;		/* see defines below */
	struct i_buf  *av_forw;		/* position on free list, */
	struct i_buf  *av_back;		/*     if not BUSY*/
	dev_t         b_dev;		/* major+minor device name */
	char          b_error;		/* returned after I/O */
	uchar         b_iflags;		/* IOPM os flags */
	unsigned      b_bcount;		/* transfer count */
	union {
	    caddr_t       b_addr;	/* low order core address */
	    int           *b_words;	/* words for clearing */
	    daddr_t       *b_daddr;	/* indirect block */
	} b_un;
	daddr_t       b_blkno;		/* block # on device */
	unsigned      b_resid;		/* words not transferred after error */
	time_t        b_start;		/* request start time */
	int           b_reltime;	
	void          *b_proc;
	void          *b_kbp;
} i_buf_t;

#ifndef IOPM_OS
/*
 * These flags are kept in b_flags for buffer io.
 */
#define	B_WRITE		0x00000	/* non-read pseudo-flag */
#define	B_READ		0x00001	/* read when I/O occurs */
#define	B_DONE		0x00002	/* transaction finished */
#define	B_ERROR		0x00004	/* transaction aborted */
#define	B_BUSY		0x00008	/* not on av_forw/back list */
#define	B_PHYS		0x00010	/* Physical IO */
#define	B_WANTED	0x00040	/* issue wakeup when BUSY goes off */
#define	B_AGE		0x00080	/* delayed write for correct aging */
#define	B_ASYNC		0x00100	/* don't wait for I/O completion */
#define	B_DELWRI	0x00200	/* delayed write */
#define	B_OPEN		0x00400	/* open routine called */
#define	B_READAHEAD	0x00800	/* buffer marked busy, but don't wait */
#define B_REMOTE	0x04000 /* buffer contains RFS data */
#define B_STALE		0x08000 /* dev closed, io not done or error */
#define B_VIRTUAL	0x10000 /* this is a virtual bp */
#endif /* IOPM_OS */

#define BI_BUF		0x01	/* IOPM buffer for this bp */

#endif /* IOPM_BUF_H */
