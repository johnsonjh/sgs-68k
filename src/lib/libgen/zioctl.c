/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zioctl.c: version 25.1 created on 12/2/91 at 19:35:56	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zioctl.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zioctl.c	1.2"	*/
	
#ident	"@(#)libgen:zioctl.c	25.1"


/*	ioctl(2) with error checking
*/

#include <stdio.h>
#include "errmsg.h"

int
zioctl( severity, fildes, request, arg)
int	severity;
int 	fildes;
int	request;
{
	int	rc;

	if( (rc = ioctl(fildes, request, arg)) == -1 )
		_errmsg("UXzioctl1", severity,
		       "Cannot control device; fildes = %d request = %d arg = %d.",
			fildes, request, arg);

	return  rc;
}
