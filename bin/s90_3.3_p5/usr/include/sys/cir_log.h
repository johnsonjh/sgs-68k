#ifndef	SYS_CIR_LOG_H
#define	SYS_CIR_LOG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cir_log.h: version 24.1 created on 10/28/91 at 18:31:29	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cir_log.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*------------------------------------------------------------------------*/
/*
 *	circular log buffer structure
 */
/*------------------------------------------------------------------------*/
struct cir_log
{
	uint	rel_time;	/* upper byte is dev, lower 3 bytes are time*/
	ushort parm_1;		/* message-dependent parameter */
	ushort parm_2;		/* message-dependent parameter */
	ushort parm_3;		/* message-dependent parameter */
	unchar parm_4;		/* usually message type from stream.h */
	unchar log_type;	/* log type */
};
#endif	/* SYS_CIR_LOG_H */
