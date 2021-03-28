/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) strend.c: version 25.1 created on 12/2/91 at 18:45:17	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)strend.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libPW:strend.c	3.3"	*/
	
#ident	"@(#)libPW:strend.c	25.1"

char *strend(p)
register char *p;
{
	while (*p++)
		;
	return(--p);
}
