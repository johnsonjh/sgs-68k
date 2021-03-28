/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zulimit.c: version 25.1 created on 12/2/91 at 19:36:49	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zulimit.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zulimit.c	1.2"	*/
	
#ident	"@(#)libgen:zulimit.c	25.1"


/*	ulimit(2) with error checking
*/

#include	"errmsg.h"

daddr_t
zulimit( severity, cmd, newlimit )
int	 severity;
int	 cmd;
daddr_t	 newlimit;
{
	daddr_t	err_ind;

	if( (err_ind = ulimit(cmd, newlimit )) ==  - 1 )
	    _errmsg("UXzulimit1", severity,
		  "Cannot get or set user limits, cmd %d ,newlimit is %d.",
		   cmd, newlimit);

	return err_ind;
}
