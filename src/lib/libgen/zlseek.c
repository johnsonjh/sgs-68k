/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zlseek.c: version 25.1 created on 12/2/91 at 19:36:05	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zlseek.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zlseek.c	1.2"	*/
	
#ident	"@(#)libgen:zlseek.c	25.1"


/*	lseek(2) with error checking
*/


#include <stdio.h>
#include "errmsg.h"

long
zlseek( severity, fildes, offset, whence)
int	severity;
int	fildes;
long	offset;
int	whence;
{
	int	rc;

	if( (rc = lseek(fildes, offset, whence)) == -1 )
		_errmsg("UXzlseek1", severity,
		       "Cannot set file pointer for fildes '%d', offset '%d', whence '%d'.",
			fildes,offset,whence);


	return  rc;
}
