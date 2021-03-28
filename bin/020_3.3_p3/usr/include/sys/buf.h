#ifndef SYS_BUF_H
#define SYS_BUF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) buf.h: version 24.1 created on 10/28/91 at 18:31:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)buf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif /* SYS_TYPE_H */

#define	paddr(X)	(paddr_t)(X->b_un.b_addr)

/* buf.h	6.1 */
/*
 * Each buffer in the pool is usually doubly linked into 2 lists:
 * the device with which it is currently associated (always)
 * and also on a list of blocks available for allocation
 * for other use (usually).
 * A buffer is on the available list, and is liable
 * to be reassigned to another disk block, if and only
 * if it is not marked BUSY.  When a buffer is busy, the
 * available-list pointers can be used for other purposes.
 * Most drivers use the forward ptr as a link in their I/O active queue.
 * A buffer header contains all the information required to perform I/O.
 * Most of the routines which manipulate these things are in bio.c.
 */
typedef struct buf {
	uint	b_flags;		/* see defines below */
	struct	buf *b_forw;		/* headed by d_tab of conf.c */
	struct	buf *b_back;		/*  "  */
	struct	buf *av_forw;		/* position on free list, */
	struct	buf *av_back;		/*     if not BUSY*/
	dev_t	b_dev;			/* major+minor device name */
	uchar	b_error;		/* returned after I/O */
	char	pad;
	unsigned b_bcount;		/* transfer count */
	union {
	    caddr_t b_addr;		/* low order core address */
	    int	*b_words;		/* words for clearing */
	    daddr_t *b_daddr;		/* indirect block */
	} b_un;
	daddr_t	b_blkno;		/* block # on device */
	unsigned int b_resid;		/* words not transferred after error */
	time_t	b_start;		/* request start time */
	int	b_want_flag;		/* buf is wanted by another process */
	struct  proc  *b_proc;		/* process doing physical or swap I/O */
	int	b_driver_flags;		/* flag for non critical region info */
	int	b_reltime;	
	uint	:32;			/* reserved */
	void	(*b_iodone)();		/* pointer to special iodone() */
	uint	b_invalid;		/* number of bytes invalid in buffer */
	uint	:32;			/* reserved */
	uint	:32;			/* reserved */
} buf_t;

typedef struct hbuf {
	int	b_flags;		/* don't change 'cuz we're faking the */
	buf_t	*b_forw;		/* first three fields of a buf_t      */
	buf_t	*b_back;
} hbuf_t;


extern struct buf bfreelist;		/* head of available list */
extern struct buf bdirtylist;		/* head of dirty list */
extern struct buf bufhdrs[];		/* buffer pool */
extern struct buf pbuf[];		
extern struct hbuf hbuf[];
struct pfree {
	int	b_flags;
	struct	buf *av_forw;
};
extern struct pfree pfreelist;		/* head of physio pool */
extern pfreecnt;			/* number of free headers */
extern char	*buffers;
extern char	*buffers4k;
extern int	s54knbuf;


/*
 * These flags are kept in b_flags for buffer io.
 */
#define	B_WRITE			0x00000	/* non-read pseudo-flag */
#define	B_READ			0x00001	/* read when I/O occurs */
#define	B_DONE			0x00002	/* transaction finished */
#define	B_ERROR			0x00004	/* transaction aborted */
#define	B_BUSY			0x00008	/* not on av_forw/back list */
#define	B_PHYS			0x00010	/* Physical IO */
#define	B_WANTED		0x00040	/* issue wakeup when BUSY goes off */
#define	B_AGE			0x00080	/* delayed write for correct aging */
#define	B_ASYNC			0x00100	/* don't wait for I/O completion */
#define	B_DELWRI		0x00200	/* don't write until block leaves
					   available list */
#define	B_OPEN			0x00400	/* open routine called */
#define	B_READAHEAD		0x00800	/* buffer marked busy, but can't wait
				   	   for it yet */
#define B_REMOTE		0x04000 /* buffer contains RFS data */
#define B_STALE			0x08000 /* device closed but pending io not
					   done or b_error on io */
#define B_S54K			0x20000	/* 4k buffer flag */

/* 
	These flags are kept for the bfreelist
*/
#define	BFREELIST_WANTED 0x1	/* process is waiting for the bfreelist */
#define BFREELIST_LOW	0x2	/* buffer algorithms get modified if the
				   bfreelist is close to being empty */

/*
 * Fast access to buffers in cache by hashing.
 */

#define	bhash(d,b)	((struct buf *)&hbuf[((int)d+(int)b)&v.v_hmask])

/*
 * Pick up the device's error number and pass it to the user;
 * if there is an error but the number is 0 set a generalized code
 */
#define geterror(bp) \
{\
\
	if (bp->b_flags&B_ERROR)\
		if ((u.u_error = bp->b_error)==0)\
			u.u_error = EIO;\
}

#endif /* SYS_BUF_H */
