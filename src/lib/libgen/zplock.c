/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zplock.c: version 25.1 created on 12/2/91 at 19:36:27	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zplock.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zplock.c	1.2"	*/
	
#ident	"@(#)libgen:zplock.c	25.1"


/*	plock(2) with error checking
*/

#include	<sys/lock.h>
#include	"errmsg.h"

int
zplock( severity, op )
int	 severity;
int	 op;
{

	int	err_ind;

	if( (err_ind = plock( op )) == -1 )
	    _errmsg ( "UXzplock1", severity,
		  "Cannot lock, op = %d.",
		   op);

	return err_ind;
}
