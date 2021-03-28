#ifndef	SYS_TYD_CLOG_H
#define SYS_TYD_CLOG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tyd_clog.h: version 24.1 created on 10/28/91 at 18:40:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tyd_clog.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*------------------------------------------------------------------------*/
/*
 *	tyd circular log buffer defines
 */
/*------------------------------------------------------------------------*/
#ifdef TTYDEBUG
#define TYD_CIR_LOG_SIZE	2048
#else
#define TYD_CIR_LOG_SIZE	1
#endif /* TTYDEBUG */
/*------------------------------------------------------------------------*/
/*	log_type vaules */
#define TYD_CIR_OPEN		1	/* open call  - parm = rq*/
#define TYD_CIR_CLOSE		2	/* close call - parm = rq*/
#define TYD_CIR_ALLOCB_FAIL	3	/* direct call to allocb failed */
#define TYD_CIR_NOBUF		4	/* no buffer available */
#define TYD_CIR_TYDALLOC	5	/* call to tyd_alloc failed */
#define TYD_CIR_RPUT		6	/* rput - should never occur */
#define TYD_CIR_RSRV		7	/* rsrv unknown type */
#define TYD_CIR_RCANPUT		8	/* rsrv M_DATA - pass upstream */
#define TYD_CIR_RPUTBQ		9	/* rsrv M_DATA - put back on queue */
#define TYD_CIR_WSRV		10	/* wsrv unknown type */
#define TYD_CIR_WRITE_DATA	11	/* wsrv M_DATA */
#define TYD_CIR_WRITE_FLUSH	12	/* wput M_FLUSH */
#define TYD_CIR_WRITE_IOCTL	13	/* wsrv M_IOCTL */
#define TYD_CIR_WRITE_IOCDATA	14	/* wsrv M_IOCDATA */
#define TYD_CIR_GCTIO_DCD	15	/* GCTIO_SET dcd timer change */
#define TYD_CIR_GCTIO_DCD_SIG	16	/* GCTIO_SET dcd timer signal change */
#define TYD_CIR_GCTIO_ACT	17	/* GCTIO_SET active timer change */
#define TYD_CIR_GCTIO_ACT_SIG	18	/* GCTIO_SET active timer signal chg */
#define TYD_CIR_WCOPY		19	/* wsrv M_DATA bcopy */
#define TYD_CIR_EV_EIA		20	/* microseq. event - eia change */
#define TYD_CIR_EV_OUTPUT_STOP	21	/* microseq. event - output stopped */
#define TYD_CIR_EV_OUTPUT_START	22	/* microseq. event - output started */
#define TYD_CIR_EV_OVERRUN	23	/* microseq. event - overrun */
#define TYD_CIR_EV_INPUT_STOP	24	/* microseq. event - input stopped */
#define TYD_CIR_EV_INPUT_START	25	/* microseq. event - input started */
#define TYD_CIR_EV_BREAK	26	/* microseq. event - break */
#define TYD_CIR_EV_INPUT	27	/* microseq. event - data received */
#define TYD_CIR_WPUT		28	/* wput - not flush nor ioctl */
#define TYD_CIR_XON		30	/* xon sent to terminal */ 
#define TYD_CIR_XOF		31	/* xoff sent to terminal */ 
#define TYD_CIR_OUT_STUCK	32	/* output is stuck */
#define TYD_CIR_TIMER		33	/* tyd_timer called qenable */
#define TYD_CIR_BRK		34	/* start break */
#define TYD_CIR_ENDBRK		35	/* end break */
#define TYD_CIR_NO_HARDWARE	36	/* can't talk to uart for this port */
#define TYD_CIR_WSRV_ERR	37	/* tp == NULL in tyd_wsrv */
#define TYD_CIR_SAK_SET		38	/* sak ioctl set request */
#define TYD_CIR_IFLAG_OLD	40	/* set if ioctl iflag changed, old */
#define TYD_CIR_IFLAG_NEW	41	/* set if ioctl iflag changed, new */
#define TYD_CIR_CFLAG_OLD	42	/* set if ioctl cflag changed, old */
#define TYD_CIR_CFLAG_NEW	43	/* set if ioctl cflag changed, new */
#define TYD_CIR_RPUTBQ1		44	/* tyd_read canput fail */
#define TYD_CIR_RPUTBQ2		45	/* tyd_read endi of canput fail */
#define TYD_CIR_TEST1		46	/* test log msg, useable for anything*/ 
#define TYD_CIR_TEST2		47	/* test log msg, useable for anything*/ 
#define TYD_CIR_RPUTBQ3		48	/* tyd_wsrv putbq */
#define TYD_CIR_TEST3		49	/* count no. queues on qhead */ 
/*------------------------------------------------------------------------*/
#endif /* SYS_TYD_CLOG_H */
