#ident	"@(#)strerror.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1989 ARIX Corp.	*/
/*	  All Rights Reserved  	*/

/*	ATT:#ident	"string:strerror.c	1.0"	*/
	

/*LINTLIBRARY*/
/*
 * strerror returns a pointer to the error message corresponding to the
 * argument errnum.
 *
 * The STANDARD missed again to state what happens if errnum is not valid
 *
 */

#include "_errno.h"

#define _UNKN_DIGITS	5

char *strerror ( register int errnum )
{
	register char *c;
	
#ifdef __STRICT_STDC__ 
	if (errnum >= 0 && errnum < _sys_nerr)
		c = _sys_errlist[errnum];
#else
	if (errnum >= 0 && errnum < sys_nerr)
		c = sys_errlist[errnum];
#endif /* __STRICT_STDC__ */
	else {
		int		nflag = 0;
		register int h=errnum;
		static char	unkn[] = "Unknown error # 12345";
		c = &unkn[sizeof(unkn) - 2];
		if (h < 0) { h = -h; ++nflag; }
		do { *c-- = (h % 10) + '0'; } while ((h /= 10) > 0);
		if (nflag) *c-- = '-';
		do *c = ' '; while (c-- > &unkn[sizeof(unkn) - _UNKN_DIGITS - 1]);
		c = unkn;
	}

	return ( c );
}
