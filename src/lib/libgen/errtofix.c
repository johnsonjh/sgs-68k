/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) errtofix.c: version 25.1 created on 12/2/91 at 19:32:33	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)errtofix.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:errtofix.c	1.1"	*/
	
#ident	"@(#)libgen:errtofix.c	25.1"


/*	Set tofix string.
*/

#include	"errmsg.h"


void
errtofix( str )
char	*str;
{
	Err.tofix = str;
}
