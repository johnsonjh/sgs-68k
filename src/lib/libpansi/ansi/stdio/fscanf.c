#ident	"@(#)fscanf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <stdarg.h>

extern int fscanf(FILE *iop,const char *fmt, ...)
{
	va_list ap;

	va_start(ap,fmt);
	return(_doscan(iop, fmt, ap));
}
