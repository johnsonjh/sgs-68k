#ifndef	SYS_LPM_CLOG_H
#define SYS_LPM_CLOG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) lpm_clog.h: version 24.1 created on 10/28/91 at 18:35:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)lpm_clog.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*------------------------------------------------------------------------*/
/*
 *	lpm circular log buffer defines
 */
/*------------------------------------------------------------------------*/
#ifdef TTYDEBUG
#define LPM_CIR_LOG_SIZE	100
#else
#define LPM_CIR_LOG_SIZE	1
#endif /* TTYDEBUG */
/*------------------------------------------------------------------------*/
/*	log_type vaules */
#define LPM_CIR_OPEN		1	/* open call  - parm = rq*/
#define LPM_CIR_CLOSE		2	/* close call - parm = rq*/
#define LPM_CIR_READ		3	/* rput unknown type */
#define LPM_CIR_READ_DATA	4	/* rput M_DATA */
#define LPM_CIR_READ_FLUSH	5	/* rput M_FLUSH */
#define LPM_CIR_WRITE		10	/* wput unknown type */
#define LPM_CIR_WRITE_DATA	11	/* wput M_DATA */
#define LPM_CIR_WRITE_FLUSH	12	/* wput M_FLUSH */
#define LPM_CIR_WRITE_IOCTL	13	/* wput M_IOCTL */
#define LPM_CIR_WCANPUT		17	/* canput call failed in write service*/
#define LPM_CIR_NOBUF		18	/* no buffer available */
#define LPM_CIR_OPOST		19	/* msg sent downstream opost only */
#define LPM_CIR_XPOST		21	/* msg sent downstream xcase and opost*/
#define LPM_CIR_ALLOCB_FAIL	22	/* direct allocb call failed */
#define LPM_CIR_WRITE_IOCDATA	25	/* wsrv M_IOCDATA */
#define LPM_CIR_TEST		39	/* test - general purpose */
#define LPM_CIR_IND		40	/* set if ioctl ind changed */
#define LPM_CIR_COL		41	/* set if ioctl col changed */
#define LPM_CIR_FLAG		42	/* set if ioctl flag changed */
#define LPM_CIR_LINE		43	/* set if ioctl line changed */
/*------------------------------------------------------------------------*/
#endif /* SYS_LPM_CLOG_H */
