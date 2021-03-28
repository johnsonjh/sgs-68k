/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zchdir.c: version 25.1 created on 12/2/91 at 19:34:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zchdir.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zchdir.c	1.2"	*/
	
#ident	"@(#)libgen:zchdir.c	25.1"


/*	chdir(2) with error checking
*/

#include	"errmsg.h"


int
zchdir( severity, path )
int     severity;
char	*path;
{
	int	rc;

	if( (rc = chdir( path )) == -1 )
	_errmsg( "UXzchdir1", severity,
		"Cannot change to directory \"%s\".", path );
	return rc;

}
