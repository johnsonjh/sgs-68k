#ident	"@(#)memmove.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1989 ARIX Corp.	*/
/*	  All Rights Reserved  	*/

/*	ATT:#ident	"string:memmove.c	1.0"	*/
	

/*LINTLIBRARY*/
/*
 *  memmove function copies n characters from the object pointed to by
 *	s2 into the object pointed to by s1,without overlapping s1 or s2.
 *
 *	To be improved and corrected in error place  !!!
 *
 */

#include <string.h>
#include <stdlib.h>

void *memmove( void *s1, const void *s2, size_t n)
{
   register char * s3;

    s3 = malloc( n ); 		/* try to reserve space in heap */
    if ( s3 == NULL ) {		/* error  !!! */
		/* here is the place to improve the standard */
		return ( s1 ); /* only temporary, this is an error */
	}

	( void ) memcpy ( s3, s2, n ); /* copies s2 in s3 */
	( void ) memcpy ( s1, s3, n ); /* copies s3=s2 in s1 */
	free ( s3 ); 					/* return s3's space to the pool */

	return ( s1 );
}
