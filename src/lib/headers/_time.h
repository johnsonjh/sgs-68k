#ifndef __TIME_H
#ident	"@(#)_time.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#include <time.h>

#define _SEC_IN_DAY	(24 * 60 * 60)
#define	_year_size(A)	(((A) % 4) || (!((A) % 100) && ((A) % 400)) ? 365 : 366)
#define _MAXTZNAME 3

/* Every leap year is divisible by 4, but not by 100 except for those 
   divisible by 400. (like 2000) */

#ifdef __STRICT_STDC__
extern time_t _timezone;
extern time_t _altzone;
extern time_t _start_dst ;  /* Start date of alternate time zone */
extern time_t _end_dst;     /* End date of alternate time zone */
extern int _daylight;
extern char	*_tzname[2];
extern short _month_size[12];
#else
extern time_t timezone;
extern time_t altzone;
extern time_t start_dst ;  /* Start date of alternate time zone */
extern time_t end_dst;     /* End date of alternate time zone */
extern int daylight;
extern char	*tzname[2];
extern short month_size[12];
#endif /* __STRICT_STDC__ */

extern char *_time[];

struct _tms {
	time_t	tms_utime;		/* user time */
	time_t	tms_stime;		/* system time */
	time_t	tms_cutime;		/* user time, children */
	time_t	tms_cstime;		/* system time, children */
};

extern enum {
	   aJan, aFeb, aMar, aApr, aMay, aJun, aJul, aAug, aSep, aOct, aNov, aDec,
	   Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec,
	   aSun, aMon, aTue, aWed, aThu, aFri, aSat,
	   Sun, Mon, Tue, Wed, Thu, Fri, Sat,
	   Local_time, Local_date, DFL_FMT,
	   AM, PM,
	   LAST
};

void _addhour(struct tm *);
char *_ct_numb(char *,int);
void _dechour(struct tm *);
char *_getdigit(char *,int *);
int _getdst(char *,time_t *,time_t *);
char *_gettime(char *,time_t *,int);
void _getusa(time_t *,time_t *,struct tm *);
char *_getzname(char *,char *);
char *_itoa(int,char *,int);
int _sunday(struct tm *,int);
void _tzset(void);

#define __TIME_H

#endif /* __TIME_H */
