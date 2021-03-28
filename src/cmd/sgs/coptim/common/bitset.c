#ident	"@(#)bitset.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	bitset.c	7.1		*/
/*	Copyright 1985 Motorola Inc.			*/
#include "bitset.h"


/*F*********************** FUNCTION HEADER *******************************

FUNCTION NAME:

	CREATESET

ACTION:

	Allocates space for a bit set capable of supporting at least
	"num_els" number of elements. The set is initialized to
	be empty.

PARAMETERS:

	Receives:

		num_els: minimum number of elements in set

	Returns:

		pointer to the created set

EXTERNAL DATA REFERENCES:

	none

COMPLEX LOCALS:

	none

FUNCTIONS CALLED:

	calloc

CALLED BY:

	findnaturalloop

METHOD:

	self explanatory

NOTES:

	none

*FE********************************************************************/

SET *
CREATESET(num_els)
int num_els;

{
SET *retval;

retval = (SET *) calloc ((unsigned) 12+num_els/8,1); /* alloc and clear space*/
	 /* The constant 12 allows for 4 bytes for "setlen", 4 bytes for the
	 ** address of "set", and 4 bytes slop at the end of the bit array.
	 */

if (!retval) /* out of space */
    cerror("Out of Memory - in CREATESET");
else
    {
    retval->setlen = num_els;		 /* assign number of elements */
    retval->set    = (char *)&retval[1]; /* assign array to first int after 
					 ** the pointer to the start of it */
    return(retval);
    }
}


/* bk: need to pretend that return type is not void, so that
 * bitrangeerr() can be used in conditional operator expression.
 */

int
bitrangeerr()
{
    cerror("Bit set index out of range");
}
