#ifndef SYS_LOG_H
#define SYS_LOG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) log.h: version 24.1 created on 10/28/91 at 18:35:08	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)log.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/log.h	10.1"		*/

#ifndef	SYS_STREAM_H
#include <sys/stream.h>
#endif	/* SYS_STREAM_H */
/*
 * Header file for the Streams Log Driver
 */

struct log {
	unsigned log_state;
	queue_t *log_rdq;
	int 	log_bcnt;
};

/*
 * Driver state values.
 */
#define LOGOPEN 	01

/* 
 * Module information structure fields
 */
#define LOG_MID		44
#define LOG_NAME	"LOG"
#define LOG_MINPS	0
#define LOG_MAXPS	512
#define LOG_HIWAT	512
#define LOG_LOWAT	256

extern strlog();

extern struct log log_log[];		/* sad device state table */
extern int log_cnt;			/* number of configured minor devices */
extern int log_bsz;			/* size of internal buffer of log messages */

/*
 * STRLOG(mid,sid,level,flags,fmt,args) should be used for those trace
 * calls that are only to be made during debugging.
 */
#ifdef DEBUG
#define STRLOG	strlog
#else
#define STRLOG
#endif


/*
 * Utility macros for strlog.
 */

/*
 * logadjust - move a character pointer up to the next int boundary
 * after its current value.  Assumes sizeof(int) is 2**n bytes for some integer n. 
 */
#define logadjust(wp) (char *)(((unsigned)wp + sizeof(int)) & ~(sizeof(int)-1))

/*
 * logstrcpy(dp, sp) copies string sp to dp.
 */
#define logstrcpy(dp, sp)  for (; *dp = *sp; dp++, sp++)
	
#endif /* SYS_LOG_H */
