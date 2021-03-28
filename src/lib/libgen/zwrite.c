/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zwrite.c: version 25.1 created on 12/2/91 at 19:37:05	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zwrite.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zwrite.c	1.2"	*/
	
#ident	"@(#)libgen:zwrite.c	25.1"


/*	write(2) with error checking
*/

#include	"errmsg.h"
#include	<stdio.h>


zwrite( severity, fd, buf, count )
int	severity;
int	fd;	/* file descriptor */
char	*buf;
unsigned int	count;
{
	register int	countout;

	if( (countout = write( fd, buf, count )) != count ) {
		_errmsg( "UXzwrite1", severity,
			"File descriptor %d, tried %d, wrote %d.",
			fd, count, countout );
	}
}
