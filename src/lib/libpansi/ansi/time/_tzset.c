#ident	"@(#)_tzset.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include "_time.h"
#include <stdlib.h>

#ifdef _ANSI 
void _tzset(void)
#else
void tzset(void)
#endif /* _ANSI */
{
  char *ptr;
  
	if ( (ptr = getenv("TZ")) == NULL) return;
	
	/* Get main time zone name and difference from GMT */
#ifdef _ANSI 
	if ( ((ptr = _getzname(ptr,_tzname[0])) == 0) || 
		 ((ptr = _gettime(ptr,&_timezone,1)) == 0)) return;
#else
	if ( ((ptr = _getzname(ptr,tzname[0])) == 0) || 
		 ((ptr = _gettime(ptr,&timezone,1)) == 0)) return;
#endif /* _ANSI */

#ifdef _ANSI 
	_altzone = _timezone - 60 * 60; _start_dst = _end_dst = 0; 
	_daylight = 0;
#else
	altzone = timezone - 60 * 60; start_dst = end_dst = 0; daylight = 0;
#endif /* _ANSI */

	/* Get alternate time zone name */
#ifdef _ANSI 
	if ( (ptr = _getzname(ptr,_tzname[1])) == 0) return; _daylight = 1;
#else
	if ( (ptr = _getzname(ptr,tzname[1])) == 0) return; daylight = 1;
#endif /*  _ANSI */

	/* If the difference between alternate time zone and
	 * GMT is not given, use one hour as default.
	 */
	if (*ptr == '\0')
		return;
	if (*ptr != ';')
#ifdef _ANSI 
		if ( (ptr = _gettime(ptr,&_altzone,1)) == 0 || *ptr != ';') return;
	_getdst(ptr + 1,&_start_dst, &_end_dst);
#else
		if ( (ptr = _gettime(ptr,&altzone,1)) == 0 || *ptr != ';') return;
	_getdst(ptr + 1,&start_dst, &end_dst);
#endif /* _ANSI */
}
