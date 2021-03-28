/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) stradd.c: version 25.1 created on 12/2/91 at 19:33:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)stradd.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:stradd.c 	1.2"	*/
	
#ident	"@(#)libgen:stradd.c	25.1"

/*LINTLIBRARY*/
/*
 * Add s2 to s1.  S1's space must be large enough.
 * Return the address of the last character written, i.e. the '\0' character.
 */

char *
stradd(s1, s2)
register char *s1, *s2;
{
	while(*s1++ = *s2++)
		;
	return(--s1);
}
