#ident	"@(#)fixmagic.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* this ascii to long deals with bases
 * base  8  ,10 and 16
 */
long myatol(s)
register char *s;
{
	register long base = 10;
	register long result = 0;
	register long neg = 0;
	register char c;
	if( *s == '-'){
		neg++; *s++;
		}
	if( *s == '+') *s++;
	if(!isdigit(*s))
		fprintf(stderr,"%s: 1.Illegal magic number format\n",utilname),exit(1);
	if( *s == '0' ){		/* base 8 preceded by 0 */
		if( *++s == 'x' ){
			base = 16;	/* base 16 preceded by 0x */
			*s++;
			}
		else base = 8;
		}
	if ( -1 == (result = radfix(s,base)))
		fprintf(stderr,"%s: 2. Illegal magic number format\n",utilname),exit(1);
	return (neg ? -result : result);
}

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

/* put the new magic number into the output file */
void
fixmagic(outp)
FILE *outp;
{
	FSEEK(outp,0,0L);
	FWRITE(&localmagic,sizeof(localmagic),1,outp);
}

