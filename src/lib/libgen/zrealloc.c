/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zrealloc.c: version 25.1 created on 12/2/91 at 19:36:36	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zrealloc.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zrealloc.c	1.2"	*/
	
#ident	"@(#)libgen:zrealloc.c	25.1"


/*	realloc(3C) with error checking
*/

#include	"errmsg.h"
#include	<stdio.h>

char *
zrealloc( severity, ptr, size)
int	  severity;
char	  *ptr;
unsigned  size;
{
	 char *p;

	if( ( p = (char *)realloc(ptr, size) ) == NULL )
		_errmsg("UXzrealloc1", severity,
		       "Cannot reallocate %d bytes.",
			size);
	return p;
}
