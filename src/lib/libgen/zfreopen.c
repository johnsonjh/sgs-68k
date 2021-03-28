/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zfreopen.c: version 25.1 created on 12/2/91 at 19:35:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zfreopen.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zfreopen.c	1.3"	*/
	
#ident	"@(#)libgen:zfreopen.c	25.1"


/*	freopen(3S) with error checking
*/

#include	"errmsg.h"
#include	<stdio.h>


FILE *
zfreopen( severity, path, type, stream )
int	severity;
char	*path;
char	*type;
FILE	*stream;
{
	register FILE	*fp;	/* file pointer */

	if( (fp = freopen( path, type, stream )) == NULL ) {
		_errmsg( "UXzfreopen1", severity,
			"Cannot open file \"%s\".",
			 path );
	}
	return  fp;
}
