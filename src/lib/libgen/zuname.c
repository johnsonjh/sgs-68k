/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zuname.c: version 25.1 created on 12/2/91 at 19:36:55	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zuname.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zuname.c	1.2"	*/
	
#ident	"@(#)libgen:zuname.c	25.1"


/*	uname(2) with error checking
*/

#include	"errmsg.h"
#include	<sys/utsname.h>

int
zuname( severity, name)
int    severity;
struct utsname *name;
{

	int	err_ind;

	if( (err_ind = uname( name )) == -1 )
	    _errmsg("UXzuname1", severity,
		  "Cannot get name of current UNIX system.");

	return err_ind;
}
