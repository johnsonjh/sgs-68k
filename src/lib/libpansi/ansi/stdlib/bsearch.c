#ident	"@(#)bsearch.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/bsearch.c	1.7"	*/
	

/*LINTLIBRARY*/
/*
 * Binary search algorithm, generalized from Knuth (6.2.1) Algorithm B.
 *
 */

/* POSIX 1003.1 compliance, size_t added where required, mr0 */

#include <sys/types.h>

typedef char * _POINTER;

void *bsearch(const void *key, const void *base, size_t nel, size_t width,       int(*compar)(const void *,const void *))

/*    old definition and explications 
POINTER bsearch (key, base, nel, width, compar)
POINTER	key;		 Key to be located 
POINTER	base;		 Beginning of table 
size_t nel;			 Number of elements in the table 
size_t width;			 Width of an element (bytes) 
int	(*compar)();		 Comparison function */
{
	size_t two_width = width + width;
	_POINTER last = (char *) base + width * (nel - 1); /* Last element in table */

	while (last >= base) {

		register _POINTER p = (char *) base + width * ((last - (char *) base)/two_width);
		register int res = (*compar)(key, p);

		if (res == 0)
			return (p);	/* Key found */
		if (res < 0)
			last = p - width;
		else
			base = p + width;
	}
	return ((_POINTER) 0);		/* Key not found */
}
