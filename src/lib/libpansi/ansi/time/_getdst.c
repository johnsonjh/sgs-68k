#ident	"@(#)_getdst.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include "_time.h"

extern int _getdst(char *p,time_t *s,time_t *e)
{
	int lsd,led;
	time_t st,et;

	st = et = 0; 		/* Default for start and end time is 00:00:00 */
	if ( (p = _getdigit(p,&lsd)) == 0 )
		return(0);
	lsd -= 1; 	/* keep julian count in sync with date  1-366 */
	if ( (*p == '/') &&  ((p = _gettime(p+1,&st,0)) == 0) )
			return(0);
	if (*p == ',')
	{
		if ( (p = _getdigit(p+1,&led)) == 0 )
			return(0);
		led -= 1; 	/* keep julian count in sync with date  1-366 */
		if ((*p == '/') &&  ((p = _gettime(p+1,&et,0)) == 0) )
				return(0);
	}
	/* Convert the time into seconds */
	*s = (long)(lsd * _SEC_IN_DAY + st);
#ifdef _ANSI 
	*e = (long)(led * _SEC_IN_DAY + et - (_timezone - _altzone));
#else
	*e = (long)(led * _SEC_IN_DAY + et - (timezone - altzone));
#endif /* _ANSI */
	return(1);
}
