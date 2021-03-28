#ifndef SYS_TIME_H
#define SYS_TIME_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) time.h: version 24.1 created on 10/28/91 at 18:39:35	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)time.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * Unit conversions between user's and kernel's view of interval timers.
 */
#define	milstoticks(mils)	(((mils) + ((1000 / HZ) - 1)) / (1000 / HZ))
#define	tickstomils(ticks)		((ticks) * (1000 / HZ))

#define	secstomils(sec)			((sec) * 1000)
#define	milstosecs(mils)		((mils) / 1000)

#define	secstousecs(sec)		((sec) * 1000000)
#define	usecstosecs(usec)		((usec) / 1000000)

#define	secstoticks(sec)		(milstoticks(secstomils(sec)))
#define	tickstosecs(ticks)		(milstosecs(tickstomils(ticks)))

#define	usecstomils(usec)		(((usec) + 999) / 1000)
#define	milstousecs(mils)		((mils) * 1000)

#define	usecstoticks(usec)		(milstoticks(usecstomils(usec)))
#define	tickstousecs(ticks)		(milstousecs(tickstomils(ticks)))

#define	secsusecstomils(sec,usec)	(secstomils(sec) + usecstomils(usec))
#define	secsusecstoticks(sec,usec)	(secstoticks(sec) + usecstoticks(usec))

/*
 * Operations on timevals.
 *
 * NB: timercmp does not work for >= or <=.
 */
#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timercmp(tvp, uvp, cmp)	\
		(((tvp)->tv_sec cmp (uvp)->tv_sec) || \
		(((tvp)->tv_sec == (uvp)->tv_sec)  && \
		((tvp)->tv_usec cmp (uvp)->tv_usec)))
#define	timerclear(tvp)		(tvp)->tv_sec = (tvp)->tv_usec = 0

/*
 * Names of the interval timers, and structure defining a timer setting.
 */
#define	ITIMER_REAL	0	/* real time timer			*/
#define	ITIMER_VIRTUAL	1	/* user mode only timer			*/
#define	ITIMER_PROF	2	/* user+sys modes timer			*/

/*
 * User's view of a time interval.
 */
typedef	struct	timeval
	{
	long		tv_sec;		/* seconds	*/
	long		tv_usec;	/* microseconds	*/
	} timeval_t;

/*
 * This is the user's view of interval timers.
 */
typedef	struct	itimerval
	{
	timeval_t	it_interval;	/* timer interval (reload value) */
	timeval_t	it_value;	/* current value (0 = timer is off) */
	} itimerval_t;

/*
 * This is the kernel's view of interval timers.
 * We convert from seconds/micro-seconds that user passes down to
 * milli-seconds that kernel needs.
 */
typedef	struct	ktimerval
	{
	long		it_interval;	/* timer interval (reload value) */
	long		it_value;	/* current value (0 = timer is off) */
	} ktimerval_t;

#endif /* SYS_TIME_H */
