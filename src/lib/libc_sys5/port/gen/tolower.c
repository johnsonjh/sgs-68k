#ident	"@(#)tolower.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*
 * If arg is upper-case, return lower-case, otherwise return arg.
 * International version
 */
#include "shlib.h"
#include <ctype.h>

int
tolower(c)
int c;
{
	if (isupper(c))
		c = _tolower(c);
	return(c);
}

