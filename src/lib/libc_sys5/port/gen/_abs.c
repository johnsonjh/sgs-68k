#ident	"@(#)_abs.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/abs.c	1.7"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#include "shlib.h"

int
abs(arg)
register int arg;
{
	return (arg >= 0 ? arg : -arg);
}
