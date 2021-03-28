#ident	"@(#)calloc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/calloc.c	1.7"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
/*	calloc - allocate and clear memory block
*/

#include "shlib.h"
#include <stdlib.h>
#include <string.h>

void *calloc(size_t num, size_t size)
{
	register char *mp;

	if((mp = malloc(num *= size)) != NULL)
		(void)memset(mp, 0, num);
	return(mp);
}
