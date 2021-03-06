#ident	"@(#)_strncat.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/strncat.c	1.7"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
/*
 * Concatenate s2 on the end of s1.  S1's space must be large enough.
 * At most n characters are moved.
 * Return s1.
 */
#include <sys/types.h>

char *
strncat(s1, s2, n)
register char *s1, *s2;
register size_t n;
{
	register char *os1;

	n++;
	os1 = s1;
	while(*s1++)
		;
	--s1;
	while(*s1++ = *s2++)
		if(--n == 0) {
			*--s1 = '\0';
			break;
		}
	return(os1);
}
