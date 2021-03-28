/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zlink.c: version 25.1 created on 12/2/91 at 19:36:02	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zlink.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zlink.c	1.2"	*/
	
#ident	"@(#)libgen:zlink.c	25.1"


/*	link(2) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zlink( severity, path1, path2 )
int	severity;
char	*path1;
char	*path2;
{

	int	err_ind;

	if( (err_ind = link(path1, path2 )) == -1 )
	    _errmsg("UXzlink1", severity,
		  "Cannot create link for \"%s\" to file \"%s\".",
		   path2 , path1);

	return err_ind;
}
