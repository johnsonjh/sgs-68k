#ifndef SYS_DEBUG_H
#define SYS_DEBUG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) debug.h: version 24.1 created on 10/28/91 at 18:31:57	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)debug.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/debug.h	10.4"		*/


#define	YES 1
#define	NO  0

#if ASDEBUG == YES
#ifdef __STDC__
#define ASSERT(EX) if (!(EX))assfail(#EX, __FILE__, __LINE__)
#else
#define ASSERT(EX) if (!(EX))assfail("EX", __FILE__, __LINE__)
#endif /* __STDC__ */
#else
#define ASSERT(x)
#endif /* ASDEBUG */

#ifdef MONITOR
#define MONITOR(id, w1, w2, w3, w4) monitor(id, w1, w2, w3, w4)
#else
#define MONITOR(id, w1, w2, w3, w4)
#endif

#define	Trace_BEGIN	printf("At %u in %s:", __LINE__, __FILE__)
#ifdef __STDC__
#define	Trace_MID(fmt, exp)	printf(" %%" #fmt "," #exp " is ")
#define	Trace_END(fmt, exp)	printf("%" #fmt "\n", exp)
#else
#define	Trace_MID(fmt, exp)	printf(" %%fmt,exp is ")
#define	Trace_END(fmt, exp)	printf("%fmt\n", exp)
#endif
#define	Trace	{Trace_BEGIN; printf("\n");}
#define	Tracef(fmt, exp)	{Trace_BEGIN; Trace_MID(fmt, exp); Trace_END(fmt, exp);}
#define	Tracem(mesg)	{Trace_BEGIN; printf(" %s\n", mesg); }
#define	Tracex(exp)	Tracef(x, exp)
#define	Traceu(exp)	Tracef(u, exp)
#define	Traced(exp)	Tracef(d, exp)
#define	Traces(exp)	Tracef(s, exp)
#define	Tracec(exp)	Tracef(c, exp)

#endif /* SYS_DEBUG_H */
