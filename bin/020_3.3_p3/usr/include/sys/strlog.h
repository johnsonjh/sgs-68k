#ifndef SYS_STRLOG_H
#define SYS_STRLOG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) strlog.h: version 24.1 created on 10/28/91 at 18:38:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)strlog.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/strlog.h	10.1"		*/

/*
 * Streams Log Driver Interface Definitions
 */

/*
 * structure of control portion of log message 
 */
struct log_ctl {
	short	mid;
	short	sid;
	char 	level;		/* level of message for tracing */
	short	flags;		/* message disposition */
	long	ltime;		/* time in machine ticks since boot */
	long	ttime;		/* time in seconds since 1970 */
	long	seq_no;		/* sequence number */
};
	
/* Flags for log messages */

#define SL_FATAL	01	/* indicates fatal error */
#define SL_NOTIFY	02	/* logger must notify administrator */
#define SL_ERROR	04	/* include on the error log */
#define SL_TRACE	010	/* include on the trace log */


/*
 * Structure defining ids and levels desired by the tracer (I_TRCLOG).
 */
struct trace_ids {
	short ti_mid;
	short ti_sid;
	char  ti_level;
};


/*
 * Log Driver I_STR ioctl commands
 */

#define I_TRCLOG	1	/* process is tracer */
#define I_ERRLOG	2	/* process is error logger */

/*
 * Parameter definitions for logger messages 
 */
#define LOGMSGSZ	128
#define NLOGARGS	3


#endif /* SYS_STRLOG_H */
