/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zaccess.c: version 25.1 created on 12/2/91 at 19:34:41	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zaccess.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zaccess.c	1.2"	*/
	
#ident	"@(#)libgen:zaccess.c	25.1"


/*	access(2) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zaccess( severity, path, amode )
int     severity;
char	*path;
int	 amode;
{

	int	err_ind;

	if( (err_ind = access(path, amode )) == -1 )
	    _errmsg("UXzaccess1", severity,
		   "Cannot access file \"%s\" with mode %d.",
		   path,amode);

	return err_ind;
}
