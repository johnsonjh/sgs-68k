#ifndef SYS_ELOG_H
#define SYS_ELOG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) elog.h: version 24.1 created on 10/28/91 at 18:32:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)elog.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * IO statistics are kept for each physical unit of each
 * block device (within the driver). Primary purpose is
 * to establish a guesstimate of error rates during
 * error logging.
 */

struct iostat {
	long	io_ops;		/* number of read/writes */
	long	io_misc;	/* number of "other" operations */
	long	io_qcnt;	/* number of jobs assigned to drive */
	ushort io_unlog;	/* number of unlogged errors */
};

/*
 * structure for system accounting
 */
struct iotime {
	struct iostat ios;
	long	io_bcnt;	/* total blocks transferred */
	time_t	io_act;		/* total controller active time */
	time_t	io_resp;	/* total block response time */
};
#define	io_cnt	ios.io_ops
#define	io_qc	ios.io_qc

#endif /* SYS_ELOG_H */
