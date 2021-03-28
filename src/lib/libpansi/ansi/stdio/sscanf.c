#ident	"@(#)sscanf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <stdarg.h>
#include <string.h>

extern int sscanf(const char *str,const char *fmt, ...)
{
	va_list ap;
	FILE strbuf;

	va_start(ap,fmt);
#ifndef DYNAMIC
	strbuf._flag = _IOREAD;
#else /* DYNAMIC */
	strbuf._flag = _IOREAD | _IOSTRING;
#endif /* DYNAMIC */
	strbuf._ptr = strbuf._base = (unsigned char*) str;
	strbuf._cnt = (int) strlen(str);
#ifndef DYNAMIC
	strbuf._file = FOPEN_MAX;
	_XFLAGS(&strbuf) = _IOXTEXT; 	/* don't write to "input stream" */
#else /* DYNAMIC */
	strbuf._file = -1;
	strbuf._flag |= _IOXTEXT;
#endif /* DYNAMIC */
	return(_doscan(&strbuf, fmt, ap));
}
