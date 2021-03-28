/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zcreat.c: version 25.1 created on 12/2/91 at 19:35:06	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zcreat.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zcreat.c	1.2"	*/
	
#ident	"@(#)libgen:zcreat.c	25.1"


/*	creat(2) with error checking
*/

#include <stdio.h>
#include "errmsg.h"

int
zcreat( severity, path, mode )
int  severity;
char *path;
int  mode;
{
	int	rc;

	if( (rc = creat( path, mode ) ) == -1 )
		_errmsg("UXzcreat1", severity,
		       "Cannot create file \"%s\" of mode %d.",
		       path,mode);

	return  rc;
}
