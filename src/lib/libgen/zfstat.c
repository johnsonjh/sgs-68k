/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zfstat.c: version 25.1 created on 12/2/91 at 19:35:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zfstat.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zfstat.c	1.2"	*/
	
#ident	"@(#)libgen:zfstat.c	25.1"


/*	fstat(2) with error checking
*/

#include <stdio.h>
#include "errmsg.h"
#include <sys/stat.h>

int
zfstat( severity, fildes, buf)
int    severity;
int    fildes;
struct stat *buf;
{
	int	rc;

	if( (rc = fstat(fildes, buf)) == -1 )
		_errmsg( "UXzfstat1", severity,
		       "Cannot get information about file, fildes = %d.",
		       fildes);

	return  rc;
}
