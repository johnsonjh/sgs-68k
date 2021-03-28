#ident	"@(#)_setup.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <string.h>

extern unsigned char *_setup(unsigned char *fmt,char *tab)
{
	register int b, c, d, t = 0;

	if (*fmt == '^') { t++; fmt++; }
	memset((void *) tab, !t, _NCHARS);
	if ( (c = *fmt) == ']' || c == '-')  /* first char is special */
	{ tab[c] = t; fmt++; }
	while ( (c = *fmt++) != ']') 
	{
		if (c == '\0') return(NULL); /* unexpected end of format */
		if (c == '-' && (d = *fmt) != ']' && (b = fmt[-2]) < d) 
			  { memset((void *) &tab[b], t, (size_t )d - b + 1); fmt++; } 
		else tab[c] = t;
	}
	return (fmt);
}
