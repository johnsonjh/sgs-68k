#ident	"@(#)perror.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <errno.h>
#include "_errno.h"
#include <stdio.h>
#include "_stdio.h"
#include <string.h>

#define _UNKN_DIGITS	5

extern void perror(const char *s)
{
	register char *c;
	register size_t n;
	
	if (s == (char *) NULL || s[0] == '\0') return;
	n = strlen(s);
#ifdef _ANSI 
	if (errno >= 0 && errno < _sys_nerr)
		c = _sys_errlist[errno];
#else
	if (errno >= 0 && errno < sys_nerr)
		c = sys_errlist[errno];
#endif /* _ANSI */
	else {
		int		nflag = 0;
		register int h=errno;
		static char	unkn[] = "Unknown error # 12345";
		c = &unkn[sizeof(unkn) - 2];
		if (h < 0) { h = -h; ++nflag; }
		do { *c-- = (h % 10) + '0'; } while ((h /= 10) > 0);
		if (nflag) *c-- = '-';
		do *c = ' '; while (c-- > &unkn[sizeof(unkn) - _UNKN_DIGITS - 1]);
		c = unkn;
	}
	if(n && s[0]!=NULL) {
		fwrite((void *) s,n,1,stderr);
		fwrite((void *) ": ",2,1,stderr);
	}
	fwrite((void *) c,strlen(c),1,stderr);
	fwrite((void *) "\n",1,1,stderr);
}
