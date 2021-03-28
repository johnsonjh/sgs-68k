/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zchroot.c: version 25.1 created on 12/2/91 at 19:35:00	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zchroot.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zchroot.c	1.2"	*/
	
#ident	"@(#)libgen:zchroot.c	25.1"


/*	chroot(2) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zchroot(  severity, path )
int	severity;
char	*path;
{

	int	err_ind;

	if( (err_ind = chroot( path )) == -1 )
	    _errmsg("UXzchroot1", severity,
		  "Cannot change root directory to \"%s\".",
		   path);

	return err_ind;
}
