#ident	"@(#)localtime.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include "_time.h"

struct tm *localtime(const time_t *tim)
{
	register struct tm *ct;
	time_t daybegin, dayend, copyt, curr;

#ifdef _ANSI
	_tzset();
	copyt = *tim - _timezone;
#else
	tzset();
	copyt = *tim - timezone;
#endif /* _ANSI */
	ct = gmtime(&copyt);
#ifdef _ANSI
	if (!_daylight) return(ct);
#else
	if (!daylight) return(ct);
#endif /* _ANSI */
	curr = ct->tm_yday*_SEC_IN_DAY + ct->tm_hour*60*60 + 
		   ct->tm_min*60 + ct->tm_sec;

#ifdef _ANSI
	if ( _start_dst == 0 && _end_dst == 0)
		_getusa(&daybegin, &dayend, ct);
	else
	{
		daybegin = _start_dst;
		dayend  = _end_dst;
#else
	if ( start_dst == 0 && end_dst == 0)
		_getusa(&daybegin, &dayend, ct);
	else
	{
		daybegin = start_dst;
		dayend  = end_dst;
#endif /* _ANSI */
	}
	if (curr >= daybegin && curr < dayend)
	{
#ifdef _ANSI
		copyt = *tim - _altzone;
#else
		copyt = *tim - altzone;
#endif /* _ANSI */
		ct = gmtime(&copyt);
		ct->tm_isdst = 1;
	}
	return(ct);
}
