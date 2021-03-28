/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zkill.c: version 25.1 created on 12/2/91 at 19:35:59	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zkill.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zkill.c	1.2"	*/
	
#ident	"@(#)libgen:zkill.c	25.1"


/*	kill(2) with error checking
*/


#include <stdio.h>
#include "errmsg.h"

int
zkill( severity, pid, sig)
int 	severity;
int 	pid;
int	sig;
{
	int	rc;

	if( (rc = kill(pid, sig)) == -1 )
		_errmsg("UXzkill1", severity,
		       "Cannot send signal %d to process %d.",
			sig,pid);

	return  rc;
}
