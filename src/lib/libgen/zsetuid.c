/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zsetuid.c: version 25.1 created on 12/2/91 at 19:36:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zsetuid.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zsetuid.c	1.2"	*/
	
#ident	"@(#)libgen:zsetuid.c	25.1"


/*	setuid(2) with error checking
*/

#include	"errmsg.h"

int
zsetuid( severity, uid )
int	 severity;
int	 uid;
{

	int	err_ind;

	if( (err_ind = setuid( uid )) == -1 )
	    _errmsg ( "UXzsetuid1", severity,
		  "Cannot set user id to %d.",
		   uid);

	return err_ind;
}
