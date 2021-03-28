#ident	"@(#)strstr.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1989 ARIX	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/strstr.c	1.1"	*/
	

/* strstr (s1,s2) - Returns first occurance of s2 in s1, not counting
                    the terminating character.  Written from scratch
		    for POSIX 1003.1 compliance, mr0 */

#include <sys/types.h>
#include <string.h>

char *strstr ( const register char *s1, const register char *s2 ) 
{
    register	size_t	size_diff;
    register	char	*count;

    if ( (size_diff = (strlen (s1) -  strlen (s2)) + 1) < 1 )

	return (NULL);

    for ( count = s1; count < s1 + size_diff; count++ ){

	if ( memcmp (count, s2, strlen (s2)) == 0)

	    return (count);
	}

    return (NULL);
}
