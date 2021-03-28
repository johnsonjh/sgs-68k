#ident	"@(#)_getusa.c	25.2	4/1/92 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include "_time.h"

extern void _getusa(time_t *s,time_t *e,struct tm *t)
{
	static struct {
		int	yrbgn;
		int	daylb;
		int	dayle;
	} daytab[] = {
		87,	96,	303,	/* new legislation - 1st Sun in April	*/
		76,	119,	303,
		75,	58,	303,	/* 1975: Last Sun in Feb - last Sun in Oct */
		74,	5,	333,	/* 1974: Jan 6 - last Sun. in Nov */
		67,	119,	303,/* start GMT	*/
		46, 1, 1,		/* No DST from 1946 to 1967 */
		45, 1, 303,		/* Until October 1945, all year round */
		44, 1, 366,
		43, 1, 365,
		42, 31, 365,	/* Starting February 1942, all year round */
		19, 1, 1,		/* No DST from 1919 to 1942 */
		18, 89, 303,	/* 1918: Last Sun in Mar - last Sun in Oct */
		00, 1, 1,		/* No DST from 1900 to 1918 */
	};
	int i = 0;

	while (t->tm_year < daytab[i].yrbgn) /* can't be less than 70	*/
		i++;
	*s = daytab[i].daylb; /* fall through the loop when in correct interval	*/
	*e = daytab[i].dayle;

	*s = _sunday(t, *s);
	*e = _sunday(t, *e);
	*s = *s * _SEC_IN_DAY +2*60*60;
	*e = *e * _SEC_IN_DAY +60*60;
	return;
}
