#ident	"@(#)toupper.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*
 * If arg is lower-case, return upper-case, otherwise return arg.
 * International version
 */
#include "shlib.h"
#include <ctype.h>

int
toupper(c)
int c;
{
	if (islower(c))
		c = _toupper(c); 
	return(c);
}
