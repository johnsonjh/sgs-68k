/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sputs.c: version 25.1 created on 12/2/91 at 19:33:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sputs.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:sputs.c	1.1"	*/
	
#ident	"@(#)libgen:sputs.c	25.1"

/*
	Provide machine independent transfer of shorts, ala sputl(3X).
*/

/*
 * The intent here is to provide a means to make the value of
 * bytes in an io-stream correspond to the value of the short
 * in the memory while doing the io a `short' at a time.
 * Files written and read in this way are machine-independent.
 *
 */
#include <values.h>

void
sputs(w, buffer)
register short w;
register char *buffer;
{
	register int i = BITSPERBYTE * sizeof(short);

	while ((i -= BITSPERBYTE) >= 0)
		*buffer++ = (char) (w >> i);
}
