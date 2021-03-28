/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) waitpid.c: version 25.1 created on 12/2/91 at 19:34:35	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)waitpid.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:waitpid.c	1.2"	*/
	
#ident	"@(#)libgen:waitpid.c	25.1"

/*	Issue a wait(2) until the expected process id (pid) comes back.
*/

#include	<errno.h>
#include	"libgen.h"


int
waitpid( pid, statusp )
int	pid;
int	*statusp;
{
	int	rc;

	while( (rc = wait( statusp )) != pid  &&  rc != -1 )
		;
	return  rc;
}
