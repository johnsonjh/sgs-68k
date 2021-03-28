/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zstat.c: version 25.1 created on 12/2/91 at 19:36:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zstat.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zstat.c	1.2"	*/
	
#ident	"@(#)libgen:zstat.c	25.1"


/*	stat(2) with error checking
*/

#include "errmsg.h"
#include <sys/stat.h>

int
zstat( severity, path, buf)
int	severity;
char	*path;
struct	stat *buf;
{
	int	rc;

	if((rc = stat(path, buf)) == -1 )
		_errmsg("UXzstat1", severity,
		      "Cannot obtain information about file:  \"%s\".",
		       path);


	return  rc;
}
