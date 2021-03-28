#ident	"@(#)cfree.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/calloc.c	1.7"	*/
	
#define NULL 0
#include "shlib.h"
#include <sys/types.h>

extern void free();

/* POSIX 1003.1 compliance, size, num type changed from unsigned to size_t, 
   calloc changed to return void *, mr0 */

void
cfree(p, num, size)
char *p;
unsigned num, size;
{
	free(p);
}
