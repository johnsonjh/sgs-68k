/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zfdopen.c: version 25.1 created on 12/2/91 at 19:35:31	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zfdopen.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zfdopen.c	1.2"	*/
	
#ident	"@(#)libgen:zfdopen.c	25.1"


/*	fdopen(3S) with error checking
*/

#include	"errmsg.h"
#include	<stdio.h>


FILE *
zfdopen( severity, fildes, type )
int	severity;
int	fildes;
char	*type;
{
	register FILE	*fp;	/* file pointer */

	if( (fp = fdopen( fildes, type )) == NULL ) {
		_errmsg( "UXzfdopen1", severity,
			"Cannot fdopen() file descriptor %d for \"%s\".",
			fildes, type );
	}
	return  fp;
}
