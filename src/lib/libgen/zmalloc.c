/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zmalloc.c: version 25.1 created on 12/2/91 at 19:36:08	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zmalloc.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zmalloc.c	1.2"	*/
	
#ident	"@(#)libgen:zmalloc.c	25.1"


/*	malloc(3C) with error checking
*/

#include <stdio.h>
#include "errmsg.h"


char *
zmalloc( severity, n)
int		severity;
unsigned	n;
{
	char	*p;

	if( (p = (char *)malloc(n)) == NULL )
		_errmsg("UXzmalloc1", severity,
			"Cannot allocate a block of %d bytes.",
			n);

	return  p;
}
