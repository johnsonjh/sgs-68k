/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zacct.c: version 25.1 created on 12/2/91 at 19:34:44	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zacct.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zacct.c	1.2"	*/
	
#ident	"@(#)libgen:zacct.c	25.1"


/*	acct(2) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zacct( severity, path )
int     severity;
char	*path;
{

	int	err_ind;

	if( (err_ind = acct( path )) == -1 )
	    _errmsg("UXzacct1", severity,
		  "Cannot enable/disable sys. processing routine path \"%s\".",
		   path);

	return err_ind;
}
