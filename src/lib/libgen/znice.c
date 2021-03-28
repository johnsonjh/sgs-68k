/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) znice.c: version 25.1 created on 12/2/91 at 19:36:17	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)znice.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:znice.c	1.2"	*/
	
#ident	"@(#)libgen:znice.c	25.1"


/*	nice(2) with error checking
*/

#include	"errmsg.h"

int
znice( severity, incr )
int	 severity;
int	 incr;
{

	int	err_ind;

	if( (err_ind = nice( incr )) == -1 )
	    _errmsg ( "UXznice1", severity,
		  "Cannot add %d to the nice value of the calling process.",
		   incr);

	return err_ind;
}
