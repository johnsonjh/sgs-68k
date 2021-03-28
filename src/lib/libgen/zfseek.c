/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zfseek.c: version 25.1 created on 12/2/91 at 19:35:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zfseek.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zfseek.c	1.2"	*/
	
#ident	"@(#)libgen:zfseek.c	25.1"


/*	fseek(3S) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zfseek( severity, stream, offset, ptrname )
int	 severity;
FILE	*stream;
long	 offset;
int	 ptrname;
{

	int	err_ind;

	if( (err_ind = fseek(stream, offset, ptrname )) < 0 )
	    _errmsg ( "UXzfseek1", severity,
		  "Cannot reposition file pointer, offset %d and ptrname %d.",
		   offset, ptrname );

	return err_ind;
}
