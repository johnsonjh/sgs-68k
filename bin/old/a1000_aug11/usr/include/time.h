/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	AT&T: #ident	"head:time.h	1.5"			*/
/*	3.0 SID #	1.2	*/

#ident	"%Z%uts/head:%M%	%I%"

struct	tm {	/* see ctime(3) */
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};
extern struct tm *gmtime(), *localtime();
extern char *ctime(), *asctime();
int  cftime(),  ascftime();
extern void tzset();
extern long timezone, altzone;
extern int daylight;
extern char *tzname[];
