/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) bufsplit.c: version 25.1 created on 12/2/91 at 19:31:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)bufsplit.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:bufsplit.c	1.2"	*/
	
#ident	"@(#)libgen:bufsplit.c	25.1"

/*
	split buffer into fields delimited by tabs and newlines.
	Fill pointer array with pointers to fields.
	Return the number of fields assigned to the array[].
	The remainder of the array elements point to the end of the buffer.
  Note:
	The delimiters are changed to null-bytes in the buffer and array of
	pointers is only valid while the buffer is intact.
*/

#include	"libgen.h"

static char	*bsplitchar = "\t\n";	/* characters that separate fields */

unsigned
bufsplit( buf, dim, array )
register char	*buf;		/* input buffer */
unsigned	dim;		/* dimension of the array */
char	*array[];
{
	extern	char	*strrchr();
	extern	char	*strpbrk();
	register unsigned numsplit;
	register int	i;

	if( !buf )
		return 0;
	if( buf  &&  !dim  &&  !array ) {
		bsplitchar = buf;
		return 1;
	}
	numsplit = 0;
	while ( numsplit < dim ) {
		array[numsplit] = buf;
		numsplit++;
		buf = strpbrk(buf, bsplitchar);
		if (buf)
			*(buf++) = '\0';
		else
			break;
		if (*buf == '\0') {
			break;
		}
	}
	buf = strrchr( array[numsplit-1], '\0' );
	for (i=numsplit; i < dim; i++)
		array[i] = buf;
	return numsplit;
}
