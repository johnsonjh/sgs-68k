/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zexecve.c: version 25.1 created on 12/2/91 at 19:35:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zexecve.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zexecve.c	1.2"	*/
	
#ident	"@(#)libgen:zexecve.c	25.1"


/*	execve(2) with error checking
*/

#include	"errmsg.h"


zexecve( severity, file, argv, envp )
int	severity;
char	*file;
char	*argv[];
char	*envp[];
{
	int	rc;

	if( (rc = execve( file, argv, envp )) == -1 )
		_errmsg( "UXzexecve1", severity,
			"Cannot execute command \"%s\".",
			 file );
	return rc;
}
