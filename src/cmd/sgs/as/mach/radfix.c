#ident	"@(#)radfix.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


#include <stdio.h>
#include "gendefs.h"
/* RADFIX--Convert a string of chars in array pointed to by txt--
 *		to the radix named by base.
 *		A check for out of range digits is made.
 * 		(-1) is returned if bad in badrad.
 */
short badrad;
long radfix(txt,base)
 char *txt;
 short base;
{
 	long retval = 0;		/* return value accumulator */
	short value;
	badrad=0;
	while ( *txt != '\0' ){		/* until end of string */
		value = *txt++;		/* get next character */
		retval *= base;
		if (value >= 'a'&& value <= 'f') /* convert hex-chars */
			value -= 'a' - 10 - '0';
		if (value >= 'A'&& value <= 'F')
			value -= 'A' - 10 - '0';
		if ( value - '0' >= base ) 	/* check for outta range */
			badrad++;
		retval += value - '0';
	}/* end while */
	if(badrad)
		return(-1);
	return(retval);
}/* end radfix */
