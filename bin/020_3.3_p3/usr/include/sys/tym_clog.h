#ifndef	SYS_TYM_CLOG_H
#define SYS_TYM_CLOG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tym_clog.h: version 24.1 created on 10/28/91 at 18:40:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tym_clog.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*------------------------------------------------------------------------*/
/*
 *	tym circular log buffer defines
 */
/*------------------------------------------------------------------------*/
#ifdef TTYDEBUG
#define TYM_CIR_LOG_SIZE	2048
#else
#define TYM_CIR_LOG_SIZE	1
#endif /* TTYDEBUG */
/*------------------------------------------------------------------------*/
/*	log_type vaules */
#define TYM_CIR_OPEN		1	/* open call  - parm = rq*/
#define TYM_CIR_CLOSE		2	/* close call - parm = rq*/
#define TYM_CIR_READ		3	/* rput unknown type */
#define TYM_CIR_READ_DATA	4	/* rput M_DATA */
#define TYM_CIR_READ_FLUSH	5	/* rput M_FLUSH */
#define TYM_CIR_READ_IOCACK	6	/* rput M_IOCACK */
#define TYM_CIR_READ_BREAK	7	/* rput M_BREAK */
#define TYM_CIR_CANON_WRAP	8	/* sending canon data upstream */
#define TYM_CIR_RAW_WRAP	9	/* sending raw data upstream */
#define TYM_CIR_WRITE		10	/* wput unknown type */
#define TYM_CIR_WRITE_DATA	11	/* wput M_DATA */
#define TYM_CIR_WRITE_FLUSH	12	/* wput M_FLUSH */
#define TYM_CIR_WRITE_IOCTL	13	/* wput M_IOCTL */
#define TYM_CIR_WRITE_MREAD	14	/* wput M_READ */
#define TYM_CIR_ECHO		15	/* sending echo buf downstream */
#define TYM_CIR_RCANPUT		16	/* canput call failed in read  service*/
#define TYM_CIR_WCANPUT		17	/* canput call failed in write service*/
#define TYM_CIR_NOBUF		18	/* no buffer available */
#define TYM_CIR_OPOST		19	/* msg sent downstream opost only */
#define TYM_CIR_XCASE		20	/* msg sent downstream xcase only */
#define TYM_CIR_XPOST		21	/* msg sent downstream xcase and opost*/
#define TYM_CIR_ALLOCB_FAIL	22	/* direct allocb call failed */
#define TYM_CIR_SIG		23	/* send signal upstream */
#define TYM_CIR_HEADOPT		24	/* stream head option change */
#define TYM_CIR_RAW_CHK		25	/* check to se if we send raw data up */
#define TYM_CIR_WSRV		26	/* write service */
#define TYM_CIR_SEND_MISC	27	/* send M_FLUSH, M_START, M_STOP */
#define TYM_CIR_MISC1		28	/* for future use */
#define TYM_CIR_MISC2		29	/* for future use */
#define TYM_CIR_CC0		30	/* set if ioctl cc[0] changed */
#define TYM_CIR_CC1		31	/* set if ioctl cc[1] changed */
#define TYM_CIR_CC2		32	/* set if ioctl cc[2] changed */
#define TYM_CIR_CC3		33	/* set if ioctl cc[3] changed */
#define TYM_CIR_CC4		34	/* set if ioctl cc[4] changed */
#define TYM_CIR_CC5		35	/* set if ioctl cc[5] changed */
#define TYM_CIR_CC6		36	/* set if ioctl cc[6] changed */
#define TYM_CIR_CC7		37	/* set if ioctl cc[7] changed */
#define TYM_CIR_MISC3		38	/* for future use */
#define TYM_CIR_MISC4		39	/* for future use */
#define TYM_CIR_IFLAG_OLD	40	/* set if ioctl iflag changed */
#define TYM_CIR_IFLAG_NEW	41	/* set if ioctl iflag changed */
#define TYM_CIR_OFLAG_OLD	42	/* set if ioctl oflag changed */
#define TYM_CIR_OFLAG_NEW	43	/* set if ioctl oflag changed */
#define TYM_CIR_LFLAG_OLD	44	/* set if ioctl lflag changed */
#define TYM_CIR_LFLAG_NEW	45	/* set if ioctl lflag changed */
#define TYM_CIR_LINE		46	/* set if ioctl line changed */
#define TYM_CIR_TEST1		47	/* test log msg, useable for anything*/ 
#define TYM_CIR_TEST2		48	/* test log msg, useable for anything*/ 
#define TYM_CIR_TEST3		49	/* test log msg, useable for anything*/ 
/*------------------------------------------------------------------------*/
#endif /* SYS_TYM_CLOG_H */
