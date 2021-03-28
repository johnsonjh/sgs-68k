/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) smemcpy.c: version 25.1 created on 12/2/91 at 19:33:44	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)smemcpy.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:smemcpy.c	1.2"	*/
	
#ident	"@(#)libgen:smemcpy.c	25.1"

/*	Safe memory copy.
	Fast if the to and from strings do not overlap,
	slower but safe if they do.
*/

#include	<memory.h>
#include	"libgen.h"


char *
smemcpy( to, from, count )
register char		*to, *from;
register unsigned	count;
{
	char	*savedto;

	if( &to[ count ] <= from  ||  &from[ count ] <= to )
		return  memcpy( to, from, count );

	if( to == from )
		return  to;

	savedto = to;
	if( to < from )
		while( count-- )
			*(to++) = *(from++);
	else {
		to += count;
		from += count;
		while( count-- )
			*(--to) = *(--from);
	}

	return  savedto;
}
