#ifndef SYS_DEBUGLOG_H
#define SYS_DEBUGLOG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) debuglog.h: version 24.1 created on 10/28/91 at 18:32:00	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)debuglog.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifdef TTYDEBUG
#define BUGLOG0(s,ptr,lvl)           if (LOG_DEBUG) LOG_RTN(s,ptr,lvl)
#define BUGLOG1(s,ptr,lvl,a)         if (LOG_DEBUG) LOG_RTN(s,ptr,lvl,a)
#define BUGLOG2(s,ptr,lvl,a,b)       if (LOG_DEBUG) LOG_RTN(s,ptr,lvl,a,b)
#define BUGLOG3(s,ptr,lvl,a,b,c)     if (LOG_DEBUG) LOG_RTN(s,ptr,lvl,a,b,c)
#define BUGLOG4(s,ptr,lvl,a,b,c,d)   if (LOG_DEBUG) LOG_RTN(s,ptr,lvl,a,b,c,d)
#define BUGLOG5(s,ptr,lvl,a,b,c,d,e) if (LOG_DEBUG) LOG_RTN(s,ptr,lvl,a,b,c,d,e)

#define BUGCIR_LOG(a,b,c,d,e) if (CIRLOG_DEBUG != 0) CIRLOG_RTN(a,b,c,d,e); else

#else

#define BUGLOG0(s,ptr,lvl)
#define BUGLOG1(s,ptr,lvl,a)
#define BUGLOG2(s,ptr,lvl,a,b)
#define BUGLOG3(s,ptr,lvl,a,b,c)
#define BUGLOG4(s,ptr,lvl,a,b,c,d)
#define BUGLOG5(s,ptr,lvl,a,b,c,d,e)

#define BUGCIR_LOG(a,b,c,d,e)

#endif /* TTYDEBUG */

#define ARGS	a0,a1,a2,a3,a4,a5

#define BUG_LERR	0
#define BUG_LFSH	4
#define BUG_LSIO	5
#define BUG_LIOC	6
#define BUG_LPUT	8
#define BUG_LSRV	9
#define BUG_LEIA	10
#define BUG_LOPEN	12
#define BUG_LCLOSE	12
#define BUG_LGET	14
#define BUG_LGEN	15

#endif /* SYS_DEBUGLOG_H */
