#ident	"@(#)mktime.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include <math.h>
#include <limits.h>
#include "_time.h"
#include <stdlib.h>

static div_t _div(int a,int b)
{
	div_t ret;

	if ( a == INT_MIN ) {
	  /* Now we cannot do the trick because -INT_MAX = INT_MIN+1 */
	  ret.rem = -1;
	  return ret;
	}
	if ( a < 0 ) { a=-a; b=-b; }
	/* Now a>=0 */
	ret.rem = a; ret.quot = 0;
	while ( ret.rem >= fabs(b) ) { 
		/* Invariant: ( a = ret.quot*b + ret.quot AND 0<=ret.rem) */
		ret.rem -= fabs(b); 
		if ( b>=0 ) ret.quot++; else ret.quot--;
	}
	return ret;
}

time_t mktime(struct tm *tmp)
{
	div_t secdiv,mindiv,hourdiv;
	time_t days,ret,DSTstart,DSTend;
	int r;

	secdiv=_div(tmp->tm_sec,60);
	if ( secdiv.rem == -1 ) return (time_t) -1;
	tmp->tm_sec=secdiv.rem; ret = secdiv.rem;

	mindiv=_div(tmp->tm_min + secdiv.quot,60);
	if ( mindiv.rem == -1 ) return (time_t) -1;
	tmp->tm_min=mindiv.rem; ret += mindiv.rem*60;

	hourdiv=_div(tmp->tm_hour + mindiv.quot,24);
	if ( hourdiv.rem == -1 ) return (time_t) -1;
	tmp->tm_hour=hourdiv.rem; ret += hourdiv.rem*60*60;

	/* Semantics of remainder/quotient calculus require remainder to be
	   non-negative. So no adjusts for negative values are needed. */

	days=hourdiv.quot + tmp->tm_mday-1;
	for (r=70; r<tmp->tm_year; r++) {days+=_year_size(r); };
	for (r=0 ; r<tmp->tm_mon ; r++) {days+=(_year_size(tmp->tm_year)==366 && 
#ifdef _ANSI
											 r==1 ? 1:0) + _month_size[r]; };
#else
											 r==1 ? 1:0) + month_size[r]; };
#endif /* _ANSI */
	ret += days*_SEC_IN_DAY;
	tmp->tm_wday = ((ret%_SEC_IN_DAY<0 ?-1:0)+ret/_SEC_IN_DAY+7340036L)%7;
#ifdef _ANSI
	_tzset();
	ret += _altzone; /* To get calendar time instead of local time */
#else
	tzset();
	ret += altzone; /* To get calendar time instead of local time */
#endif /* _ANSI */
	if (ret < 0) return (time_t) -1;

	for (tmp->tm_year=70; days >= _year_size(tmp->tm_year); tmp->tm_year++)
		{days-=_year_size(tmp->tm_year); };
    tmp->tm_yday=days;
#ifdef _ANSI
	for (tmp->tm_mon=0; days >= _month_size[tmp->tm_mon]; tmp->tm_mon++)
		{days-= _month_size[tmp->tm_mon] + 
#else
	for (tmp->tm_mon=0; days >= month_size[tmp->tm_mon]; tmp->tm_mon++)
		{days-= month_size[tmp->tm_mon] + 
#endif /* _ANSI */
			   (_year_size(tmp->tm_year)==366 && tmp->tm_mon==1 ? 1:0);}
	tmp->tm_mday=days+1;

	_getusa(&DSTstart,&DSTend,tmp);
	days=tmp->tm_sec + tmp->tm_min*60 + tmp->tm_hour*60*60 +
		 tmp->tm_yday*_SEC_IN_DAY;
	if (tmp->tm_isdst==0 && DSTstart<=days && days<DSTend) 
	   {tmp->tm_isdst=1; _addhour(tmp); ret+=60*60; };
	if (tmp->tm_isdst==1 && (DSTstart>days || days>=DSTend)) 
	   {tmp->tm_isdst=0; _dechour(tmp); ret-=60*60; };
	tmp->tm_isdst= (DSTstart<= days && days< DSTend ? 1:0);

	return ret;
}
