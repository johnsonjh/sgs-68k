/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zpipe.c: version 25.1 created on 12/2/91 at 19:36:24	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zpipe.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zpipe.c	1.2"	*/
	
#ident	"@(#)libgen:zpipe.c	25.1"


/*	pipe(2) with error checking
*/

#include	"errmsg.h"

int
zpipe( severity, fildes )
int	severity;
int	 fildes[2];
{

	int	err_ind;

	if( (err_ind = pipe( fildes )) == -1 )
	    _errmsg("UXzpipe1", severity,
		  "Cannot pipe");

	return err_ind;
}
