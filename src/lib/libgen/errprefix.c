/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) errprefix.c: version 25.1 created on 12/2/91 at 19:32:17	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)errprefix.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:errprefix.c	1.1"	*/
	
#ident	"@(#)libgen:errprefix.c	25.1"


/*	Set prefix string.
*/

#include	"errmsg.h"


void
errprefix( str )
char	*str;
{
	Err.prefix = str;
}
