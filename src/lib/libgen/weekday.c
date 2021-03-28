/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) weekday.c: version 25.1 created on 12/2/91 at 19:34:38	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)weekday.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:weekday.c	1.2"	*/
	
#ident	"@(#)libgen:weekday.c	25.1"

/* weekday -- return numeric day of the week
**
** This routine returns the numeric day of the week that corresponds
** to an absolute date.  Output day 0 is a Sunday.  Input day 1 is a
** Saturday.
*/

#include	"libgen.h"

int
weekday(day)
long day;				/* absolute date */
{
    return( (day+5) % 7 );		/* day 1 is a Saturday */
}
