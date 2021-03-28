#ident	"@(#)gmtime.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include "_time.h"

struct tm *gmtime(const time_t *tim)
{
	register int		d0, d1;
	long				hms, day;
	static struct tm	xtime;

	/* Break initial number into days */
	hms = *tim % _SEC_IN_DAY;
	day = *tim / _SEC_IN_DAY;
	if(hms < 0) { hms += _SEC_IN_DAY; day -= 1; }

	/* Generate hours:minutes:seconds */
	xtime.tm_sec = hms % 60; d1 = hms / 60;
	xtime.tm_min = d1 % 60; d1 /= 60;
	xtime.tm_hour = d1;

	/* Generate day of the week. The addend is 4 mod 7
	   (1/1/1970 was Thursday) */

	xtime.tm_wday = (day + 7340036L) % 7;

	/* Generate year number */
	if(day >= 0) 
		for(d1=70; day >= _year_size(d1); d1++) day -= _year_size(d1);
	else
		for(d1=70; day < 0; d1--) day += _year_size(d1-1);
	xtime.tm_year = d1;
	xtime.tm_yday = d0 = day;

	/* Generate month */
#ifdef _ANSI
	if(_year_size(d1) == 366) _month_size[1] = 29; /* This is February */
	for(d1 = 0; d0 >= _month_size[d1]; d1++) d0 -= _month_size[d1];
	_month_size[1] = 28;
#else
	if(_year_size(d1) == 366) month_size[1] = 29; /* This is February */
	for(d1 = 0; d0 >= month_size[d1]; d1++) d0 -= month_size[d1];
	month_size[1] = 28;
#endif /* _ANSI */
	xtime.tm_mday = d0 + 1;
	xtime.tm_mon = d1;
	xtime.tm_isdst = 0;
	return(&xtime);
}
