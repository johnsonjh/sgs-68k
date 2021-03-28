/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zchown.c: version 25.1 created on 12/2/91 at 19:34:57	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zchown.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zchown.c	1.2"	*/
	
#ident	"@(#)libgen:zchown.c	25.1"


/*	chown(2) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zchown( severity, path, owner, group )
int	severity;
char	*path;
int	 owner;
int	 group;
{

	int	err_ind;

	if( (err_ind = chown( path, owner, group )) == -1 )
	    _errmsg("UXzchown1", severity,
		  "Cannot change owner to %d and group to %d for file \"%s\".",
		   owner,group,path);

	return err_ind;
}
