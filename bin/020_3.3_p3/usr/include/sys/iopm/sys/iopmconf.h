/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopmconf.h: version 24.1 created on 10/28/91 at 17:59:59	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopmconf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOP_IOPCONF_H
#define IOP_IOPCONF_H

/* IOPM system configuration */
#define NUMTCB_STRUCT   100	/* number of tcb structs, max num of tcbs */
#define NUM_START_TCB	10	/* starting tcbs, config up to NUMTCB_STRUCT */
#define NCALLOUT	200
#define NBUFFERMAP	200
#define MAXSYSMEM  0x400000	/* 4 Mbytes of sys mem mapped *on* the IOPM */

/* static BUFs configuration */
#define NUMBP   0		/* buffer headers */
#define NUMBUF  0		/* local buffers */

/* static STREAMs configuration */
#define NQUEUE  0
#define N4096   0
#define N2048   0
#define N1024   0
#define N512    0
#define N256    0
#define N128    0
#define N64     0
#define N16     0
#define N4      0

#define NSTREVENT  0

/* STREAMS configuration */
#define STRLOFRAC  80
#define STRMEDFRAC 90

#endif /* IOP_IOPCONF_H */
