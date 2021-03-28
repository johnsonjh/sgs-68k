/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ilp_clog.h: version 24.1 created on 10/28/91 at 17:39:02	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ilp_clog.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)ilp_clog.h   24.1"
/*------------------------------------------------------------------------*/
/*
 *	ilp circular log buffer defines
 */
/*------------------------------------------------------------------------*/
#ifdef TTYDEBUG
#define ILP_CIR_LOG_SIZE	100
#else
#define ILP_CIR_LOG_SIZE	1
#endif /* TTYDEBUG */
/*------------------------------------------------------------------------*/
/*	log_type vaules */
#define ILP_CIR_OPEN		1	/* open call  - parm = rq*/
#define ILP_CIR_CLOSE		2	/* close call - parm = rq*/
#define ILP_CIR_ALLOCB_FAIL	3	/* direct call to allocb failed */
#define ILP_CIR_NOBUF		4	/* no buffer available */
#define ILP_CIR_CLOSE_END	5
#define ILP_CIR_RPUT		6	/* rput - should never occur */
#define ILP_CIR_CANCEL		7
#define ILP_CIR_WOUT		8
#define ILP_CIR_WPB		9	/* wsrv - put back on queue */
#define ILP_CIR_WSRV		10	/* wsrv unknown type */
#define ILP_CIR_WRITE_DATA	11	/* wsrv M_DATA */
#define ILP_CIR_WRITE_FLUSH	12	/* wput M_FLUSH */
#define ILP_CIR_WRITE_IOCTL	13	/* wsrv M_IOCTL */
#define ILP_CIR_MOVE		14	/* wsrv MOVE data to useq ram */
#define ILP_CIR_DONE		15	/* output complete */
#define ILP_CIR_STATUS		16	/* change in printer status */
/*------------------------------------------------------------------------*/
