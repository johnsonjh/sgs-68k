/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) move.c: version 25.1 created on 12/2/91 at 17:12:06	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)move.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
/*	ATT: #ident	"sccs:lib/libPW/u3b2/move.c	1.2"	*/

#ident	"@(#)sccs:move.c	25.1"

char *
move(a,b,n)
register char *a, *b;
int n;
{
	register i;
	char *rs;

	rs = b;
	for (i=0; i<n; i++)
		*b++ = *a++;
	return(rs);
}
