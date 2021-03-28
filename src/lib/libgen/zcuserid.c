/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zcuserid.c: version 25.1 created on 12/2/91 at 19:35:09	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zcuserid.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zcuserid.c	1.2"	*/
	
#ident	"@(#)libgen:zcuserid.c	25.1"


/*	cuserid(3S) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

char  *
zcuserid( severity, s )
int	severity;
char	*s;
{

	char	*err_ind;

	if( (err_ind = cuserid( s )) == NULL )
	    _errmsg("UXzcuserid1", severity,
		  "Login name cannot be found.");

	return err_ind;
}
