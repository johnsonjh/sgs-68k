#ident	"@(#)tmpnam.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
#include <stdio.h>
#include "_stdio.h"
#include <string.h>

static char str[L_tmpnam] = { '\0' }, seed[] = { 'a','a','a','\0', };
static char Xs[] = "XXXXXX";

extern char *tmpnam(char *s)
{
	register char *p, *q;

	p = (s == NULL)? str: s;
	strcpy(p, _P_tmpdir); strcat(p, seed); strcat(p, Xs);

	q = seed; while(*q == 'z') *q++ = 'a';
	if (*q != '\0') ++*q;
#ifdef _ANSI 
	_mktemp(p);
#else
	mktemp(p);
#endif /* _ANSI */
	return(p);
}
