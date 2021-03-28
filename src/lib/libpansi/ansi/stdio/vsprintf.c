#ident	"@(#)vsprintf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <stdarg.h>
#include <limits.h>

extern int vsprintf(char *string,const char *format,va_list ap)
{
	register int count;
	FILE siop;

	siop._cnt = INT_MAX;
	siop._base = siop._ptr = (unsigned char *)string;
#ifndef DYNAMIC
	siop._flag = _IOWRT;
	siop._file = FOPEN_MAX;
#else /* DYNAMIC */
	siop._flag = _IOWRT | _IOSTRING;
#endif /* DYNAMIC */
	count = _doprnt(format, ap, &siop);
	*siop._ptr = '\0'; /* plant terminating null character */
	return(count);
}
