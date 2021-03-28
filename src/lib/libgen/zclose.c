/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zclose.c: version 25.1 created on 12/2/91 at 19:35:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zclose.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zclose.c	1.2"	*/
	
#ident	"@(#)libgen:zclose.c	25.1"


/*	close(2) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zclose( severity, fildes )
int	 severity;
int	 fildes;
{

	int	err_ind;

	if( (err_ind = close( fildes )) == -1 )
	    _errmsg("UXzclose1", severity,
		  "Cannot close file descriptor %d.",
		   fildes);

	return err_ind;
}
