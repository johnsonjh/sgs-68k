#ident	"@(#)_string.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>

extern int _string(int stow,int type,int len,char *tab,FILE *iop,va_list *listp)
{
	register int ch;
	register char *ptr;
	char *start;

	start = ptr = stow ? va_arg(*listp, char *) : NULL;
	if (type == 'c' && len == INT_MAX) len = 1;
	while ( (ch = _locgetc()) != EOF &&
	    !(type == 's' && isspace(ch) || type == '[' && tab[ch])) 
        { if (stow) *ptr = ch; ptr++; if (--len <= 0) break; }
	if (ch == EOF ) { _flag_eof = 1; _chcount-=1; }
    else if (len > 0 && _locungetc(ch) == EOF) _flag_eof = 1;
	if (ptr == start) return(0); /* no match */
	if (stow && type != 'c') *ptr = '\0';
	return (1); /* successful match */
}
