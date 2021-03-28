/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zexecvp.c: version 25.1 created on 12/2/91 at 19:35:21	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zexecvp.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zexecvp.c	1.2"	*/
	
#ident	"@(#)libgen:zexecvp.c	25.1"


/*	execvp(2) with error checking
*/

#include	"errmsg.h"


zexecvp( severity, file, argv )
int	severity;
char	*file;
char	*argv[];
{
	int  rc;

	if(  (rc = execvp( file, argv )) == -1 )
		_errmsg( "UXzexecvp1", severity,
			"Cannot execute command \"%s\".",
			 file );
	return rc;
}
