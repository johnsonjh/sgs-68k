#ident	"@(#)asctime.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

 /* asctime(tvec)
 * returns a ptr to a character string
 * that has the ascii time in the form
 *	Thu Jan 01 00:00:00 1970\n\0
 *	01234567890123456789012345
 *	0	  1	    2
 *
 */

#include <time.h>
#include "_time.h"

char *asctime(const struct tm *t)
{
	register char *cp, *ncp;
	static char cbuf[26];

	cp = cbuf;
	for(ncp = "Day Mon 00 00:00:00 1900\n"; *cp++ = *ncp++; );
	ncp = &"SunMonTueWedThuFriSat"[3*t->tm_wday];
	cp = cbuf;
	*cp++ = *ncp++;
	*cp++ = *ncp++;
	*cp++ = *ncp++;
	cp++;
	ncp = &"JanFebMarAprMayJunJulAugSepOctNovDec"[(t->tm_mon)*3];
	*cp++ = *ncp++;
	*cp++ = *ncp++;
	*cp++ = *ncp++;
	cp = _ct_numb(cp, t->tm_mday);
	cp = _ct_numb(cp, t->tm_hour+100);
	cp = _ct_numb(cp, t->tm_min+100);
	cp = _ct_numb(cp, t->tm_sec+100);
	if(t->tm_year >= 100) {
		cp[1] = '2';
		cp[2] = '0';
	}
	cp += 2;
	cp = _ct_numb(cp, t->tm_year+100);
	return(cbuf);
}
