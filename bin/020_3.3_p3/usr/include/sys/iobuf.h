#ifndef SYS_IOBUF_H
#define SYS_IOBUF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iobuf.h: version 24.1 created on 10/28/91 at 18:33:53	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iobuf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * Each block device has a iobuf, which contains private state stuff
 * and 2 list heads: the b_forw/b_back list, which is doubly linked
 * and has all the buffers currently associated with that major
 * device; and the d_actf/d_actl list, which is private to the
 * device but in fact is always used for the head and tail
 * of the I/O queue for the device.
 * Various routines in bio.c look at b_forw/b_back
 * (notice they are the same as in the buf structure)
 * but the rest is private to each device driver.
 */
struct iobuf {
	int	b_flags;		/* see buf.h */
	struct	buf *b_forw;		/* first buffer for this dev */
	struct	buf *b_back;		/* last buffer for this dev */
	struct	buf *b_actf;		/* head of I/O queue */
	struct 	buf *b_actl;		/* tail of I/O queue */
	dev_t	b_dev;			/* major+minor device name */
	char	b_active;		/* busy flag */
	char	b_errcnt;		/* error count (for recovery) */
	struct eblock	*io_erec;	/* error record */
	int	io_nreg;		/* num of registers to log on errors */
	struct	iostat	*io_stp;	/* unit I/O statistics */
	time_t	io_start;
	int	io_s1;			/* space for drivers to leave things */
	int	io_s2;			/* space for drivers to leave things */
	int 	b_driver_flags;		/* more space */
};


#define tabinit(dv,stat)	{0,0,0,0,0,makedev(dv,0),0,0,0,0,0,0,stat,0,0}
#define NDEVREG	(sizeof(struct device)/sizeof(int))

#define	B_ONCE	01	/* flag for once only driver operations */
#define	B_TIME	04	/* for timeout use */

#endif /* SYS_IOBUF_H */
