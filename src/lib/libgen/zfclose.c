/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zfclose.c: version 25.1 created on 12/2/91 at 19:35:24	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zfclose.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zfclose.c	1.2"	*/
	
#ident	"@(#)libgen:zfclose.c	25.1"


/*	fclose(3S) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zfclose( severity, stream )
int	severity;
FILE	*stream;
{

	int	err_ind;

	if( (err_ind = fclose( stream )) == EOF )
	    _errmsg( "UXzfclose1", severity,
		   "Cannot close file.");

	return err_ind;
}
