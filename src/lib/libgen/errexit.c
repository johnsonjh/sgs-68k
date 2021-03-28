/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) errexit.c: version 25.1 created on 12/2/91 at 19:32:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)errexit.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:errexit.c	1.1"	*/
	
#ident	"@(#)libgen:errexit.c	25.1"


#include  "errmsg.h"

/*	This routine sets the exit(2) value for exit actions.
	It returns the previous value.
*/

int
errexit( e )
int	e;
{
	int	oe;

	oe = Err.exit;
	Err.exit = e;
	return  oe;
}
