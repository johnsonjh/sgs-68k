/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) patoi.c: version 25.1 created on 12/2/91 at 17:12:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)patoi.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	AT&T: #ident	"sccs:lib/mpwlib/patoi.c	6.2"	*/

#ident	"@(#)sccs:patoi.c	25.1"

/*
	Function to convert ascii string to integer.  Converts
	positive numbers only.  Returns -1 if non-numeric
	character encountered.
*/

patoi(s)
register char *s;
{
	register int i;

	i = 0;
	while(*s >= '0' && *s <= '9') i = 10 * i + *s++ - '0';

	if(*s) return(-1);
	return(i);
}
