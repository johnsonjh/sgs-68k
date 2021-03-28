#ident	"@(#)_getdigit.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <ctype.h>

extern char *_getdigit(char *ptr,int *d)
{
	if (!isdigit(*ptr)) return(0);
 	*d = 0;
	do { *d *= 10; *d += *ptr - '0'; } while( (isdigit(*++ptr)));
	return(ptr);
}
