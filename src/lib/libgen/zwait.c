/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zwait.c: version 25.1 created on 12/2/91 at 19:37:02	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zwait.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zwait.c	1.2"	*/
	
#ident	"@(#)libgen:zwait.c	25.1"


/*	wait(2) with error checking
*/


#include <stdio.h>
#include "errmsg.h"

int
zwait( severity, stat_loc)
int	severity;
int	*stat_loc;
{
	int	rc;

	if( (rc = wait(stat_loc)) == -1 )
		_errmsg("UXzwait1", severity,
		       "Cannot wait for child process to stop/terminate.");

	return  rc;
}
