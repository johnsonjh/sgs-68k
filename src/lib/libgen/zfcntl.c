/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zfcntl.c: version 25.1 created on 12/2/91 at 19:35:28	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zfcntl.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zfcntl.c	1.1"	*/
	
#ident	"@(#)libgen:zfcntl.c	25.1"


/*	fcntl(2) with error checking
*/

#include	"errmsg.h"

int
zfcntl( severity, fildes, cmd, arg )
int	 severity;
int	 fildes;
int	 cmd;
int	 arg;
{

	int	err_ind;

	if( (err_ind = fcntl(fildes, cmd, arg )) == -1 )
	    _errmsg ( "UXzfcntl1", severity,
		  "Cannot provide file control for fildes %d cmd %d and arg %d.",
		   fildes, cmd, arg);

	return err_ind;
}
