/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zchmod.c: version 25.1 created on 12/2/91 at 19:34:54	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zchmod.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zchmod.c	1.2"	*/
	
#ident	"@(#)libgen:zchmod.c	25.1"


/*	chmod(2) with error checking
*/

#include	"errmsg.h"

int
zchmod( severity, path, mode )
int	 severity;
char	*path;
int	 mode;
{

	int	err_ind;

	if( (err_ind = chmod(path, mode )) == -1 )
	    _errmsg ( "UXzchmod1", severity,
		  "Cannot change the mode of file \"%s\" to %d.",
		   path, mode);

	return err_ind;
}
