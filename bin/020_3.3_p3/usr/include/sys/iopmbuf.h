#ifndef SYS_IOPMBUF_H
#define SYS_IOPMBUF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopmbuf.h: version 24.1 created on 10/28/91 at 18:34:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopmbuf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif /* SYS_TYPE_H */

/*	iopm command flags kept in the b_driver_flag */
#define IOPM_DRIVER_FLAGS	0xff000000
#define I_OPEN			0x01000000	/* open */
#define I_CLOSE			0x02000000	/* close */
#define I_STRAT			0x04000000	/* strategy */
#define I_IOCTL			0x08000000	/* ioctl */
#define I_DATA			0x10000000	/* iopm<->kernel data */
#define I_ERROR			0x20000000	/* error on copyin/copyout */
#define I_PRINT			0x40000000	/* d_print routine */

/* control struct passed to iopm for opens, closes & ioctls */
struct iopmbcb {			/* iopm buf control block */
	short    ib_pid;
	ushort   ib_uid;
	ushort   ib_gid;
	ushort   ib_ruid;
	ushort   ib_rgid;
	short    filler1;	/* pad to long boundary */
	union {
	    long  flag;		/* long vs int for proccessor independence */
	    long  cmd;
	} ib_arg1;
	union {
	    long  otyp;
	    long  arg;
	} ib_arg2;
	union {
	    long  mode;
	} ib_arg3;
	int      ib_rval;
	short	ib_pgrp;	/* FIX MSS, should these move up */
	short	ib_session_id;	/* FIX MSS, should these move up */
};

#endif /* SYS_IOPMBUF_H */
