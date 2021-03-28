#ifndef SYS_GCP_SIZES_H
#define SYS_GCP_SIZES_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sizes.h: version 2.1 created on 5/22/89 at 19:06:53	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sizes.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/gcp:sizes.h	2.1"

/*
	S  I  Z  E  S
 */

#define ICBILEN		100		/* icb command input buffer */
#define ICBOLEN		100		/* icb command output buffer */
#define NTEB		100		/* number timer event blocks */

#if defined SNA || defined BSC || defined DOW

#define OUTQ_LEN	0x0b00		/* output queue length */
#define CANQ_LEN	0x0200		/* canon queue length */
#define LPOQ_LEN	0x0800		/* parallel port output queue length */

#ifdef SNA
#define STKLEN		0x180		/* stack length in long words */
#define RAWQ_LEN	0x1000		/* raw queue length */
#define RBUF_LEN	0x1000		/* receiver buffer length */
#define TBUF_LEN	0x0400		/* transmitter buffer length */
#define LBUF_LEN	0x1000		/* parallel port int buffer length */
#endif

#ifdef BSC
#define STKLEN		0x100		/* stack length in long words */
#define RAWQ_LEN	0x1000		/* raw queue length */
#define RBUF_LEN	0x1000		/* receiver buffer length */
#define TBUF_LEN	0x0400		/* transmitter buffer length */
#define LBUF_LEN	0x1000		/* parallel port int buffer length */
#endif

#ifdef DOW
#define STKLEN		0x100		/* stack length in long words */
#define RAWQ_LEN	0x0800		/* raw queue length */
#define RBUF_LEN	0x1000		/* receiver buffer length */
#define TBUF_LEN	0x0400		/* transmitter buffer length */
#define LBUF_LEN	0x0800		/* parallel port int buffer length */
#endif

#else

#define OUTQ_LEN	0x1400		/* output queue length */
#define CANQ_LEN	0x0800		/* canon queue length */
#define LPOQ_LEN	0x1000		/* parallel port output queue length */

#define STKLEN		0x100		/* stack length in long words */
#define RAWQ_LEN	0x1000		/* raw queue length */
#define RBUF_LEN	0x2000		/* receiver buffer length */
#define TBUF_LEN	0x1000		/* transmitter buffer length */
#define LBUF_LEN	0x1000		/* parallel port int buffer length */

#endif

#define ICMD_LEN	0x0080		/* input command buffer length */

#define OUTQ_HOG_WATER	OUTQ_LEN	/* output queue hog water mark */
#define OUTQ_HI_WATER	OUTQ_LEN	/* output queue hi water mark */
#define OUTQ_LOW_WATER	OUTQ_LEN/16	/* output queue low water mark */

#define LPOQ_HOG_WATER	LPOQ_LEN-10	/* lp output queue hog water mark */
#define LPOQ_HI_WATER	LPOQ_LEN-10	/* lp output queue hi water mark */
#define LPOQ_LOW_WATER	LPOQ_LEN/16	/* lp output queue low water mark */

#define CANQ_HOG_WATER	CANQ_LEN	/* canon queue hog water mark */
#define CANQ_HI_WATER	CANQ_LEN	/* canon queue hi water mark */
#define CANQ_LOW_WATER	CANQ_LEN/2	/* canon queue low water mark */

#define RAWQ_HOG_WATER	RAWQ_LEN-50	/* raw queue hog water mark */
#define RAWQ_HI_WATER	RAWQ_LEN/2	/* raw queue hi water mark */
#define RAWQ_LOW_WATER	RAWQ_LEN/16	/* raw queue low water mark */

#define TBUF_HOG_WATER	TBUF_LEN	/* transmitter buffer hog water mark */
#define TBUF_HI_WATER	TBUF_LEN	/* transmitter buffer hi water mark */
#define TBUF_LOW_WATER	TBUF_LEN/16	/* transmitter buffer low water mark */

#define RBUF_HOG_WATER	RBUF_LEN-100	/* receiver buffer hog water mark */
#define RBUF_HI_WATER	RBUF_LEN/2	/* receiver buffer hi water mark */
#define RBUF_LOW_WATER	RBUF_LEN/16	/* receiver buffer low water mark */

#define LBUF_HOG_WATER	LBUF_LEN	/* lp buffer hog water mark */
#define LBUF_HI_WATER	LBUF_LEN	/* lp buffer hi water mark */
#define LBUF_LOW_WATER	LBUF_LEN/16	/* lp buffer low water mark */

#endif /* SYS_GCP_SIZES_H */
