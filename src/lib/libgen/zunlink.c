/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zunlink.c: version 25.1 created on 12/2/91 at 19:36:58	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zunlink.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zunlink.c	1.2"	*/
	
#ident	"@(#)libgen:zunlink.c	25.1"


/*	unlink(2) with error checking
*/


#include <stdio.h>
#include "errmsg.h"

int
zunlink( severity, path)
int	severity;
char	*path;
{
	int	rc;

	if( (rc = unlink(path)) == -1 )
		_errmsg("UXzunlink1", severity,
		       "Cannot remove directory entry \"%s\".",
			path);

	return  rc;
}
