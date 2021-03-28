/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zread.c: version 25.1 created on 12/2/91 at 19:36:33	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zread.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zread.c	1.2"	*/
	
#ident	"@(#)libgen:zread.c	25.1"


/*	read(2) with error checking
	End-Of-File and incomplete reads are considered errors!
*/

#include	"errmsg.h"
#include	<stdio.h>

zread(  severity, fd, buf, count )
int	severity;
int	fd;	/* file descriptor */
char	*buf;
unsigned int	count;
{
	register int	countin;

	if( (countin = read( fd, buf, count )) != count ) {
		_errmsg( "UXzread1", severity,
			"File descriptor %d, asked for %d, got %d.",
			 fd, count, countin );
	}
}
