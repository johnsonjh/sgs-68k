/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zferror.c: version 25.1 created on 12/2/91 at 19:35:34	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zferror.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zferror.c	1.2"	*/
	
#ident	"@(#)libgen:zferror.c	25.1"


/*	ferror(3S) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zferror(severity, stream )
int	 severity;
FILE	*stream;
{

	int	err_ind;

	if( (err_ind = ferror( stream )) )
	    _errmsg("UXzferror1", severity,
		  " I/O error has occurred reading/writing ");

	return err_ind;
}
