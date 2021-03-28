#ident	"@(#)iswhat.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1989 ARIX	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ARIX:#ident	"libc-port:gen/iswhat.c	1.1"	*/
	

#ifdef	_POSIX_SOURCE		/* These functions are only included in POSIX libc's */

/* iswhat.c - iswhat contains function calls for the following MACROS:

   isalnum, isalpha, iscntrl, isdigit, isgraph, islower, isprint, ispunct, 
   isspace, isupper, & isxdigit.  All of these are required to be func-
   tions by POSIX 1003.1 & x/OPEN.  mr */

#include <ctype.h>


/* Tests if testvalue is ascii */
int
isascii (testvalue)
int	testvalue;
{
	return ((int)(!((testvalue) & ~0177)));
}

#endif /* _POSIX_SOURCE */
