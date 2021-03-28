/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zcalloc.c: version 25.1 created on 12/2/91 at 19:34:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zcalloc.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zcalloc.c	1.2"	*/
	
#ident	"@(#)libgen:zcalloc.c	25.1"


/*	calloc(3C) with error checking
*/

#include	"errmsg.h"
#include	<stdio.h>

char *
zcalloc( severity, nelem, elsize)
int	  severity;
unsigned  nelem, elsize;
{
	char  *p;

	if( ( p = (char *) calloc(nelem, elsize) ) == NULL )
		_errmsg( "UXzcalloc1", severity,
		  "Cannot allocate space for array, %d elements, size %d.",
			nelem, elsize);
	return p;
}
