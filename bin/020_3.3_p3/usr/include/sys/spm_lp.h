#ifndef	SYS_SPM_LP_H
#define SYS_SPM_LP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) spm_lp.h: version 24.1 created on 10/28/91 at 18:38:14	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)spm_lp.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	spm_lp.h - header file for streams lp driver on the SPM
 *
 *	gil hunt - May 30, 1989
 */
#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

typedef struct spm_pm_lp {
	uchar	*base;		/* base address of PM-SPM printer buffer */
	uint	count;		/* no. of data bytes in buffer */
	uchar	command;	/* non-zero = flush output */
	uchar	busy;		/* copy of port status as seen by SPM */
	uchar	status;		/* copy of port status as seen by SPM (debug)*/
	uchar	pad1;
} spm_pm_lp_t;

#define MS_LP_FLUSH	1	/* command to flush output */
#endif /* SYS_SPM_LP_H */
