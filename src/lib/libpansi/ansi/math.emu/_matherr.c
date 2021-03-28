#ident	"@(#)_matherr.c	23.2	1/8/91 Copyright (c) 1991 by Arix Corp."
/* C program to deal with the status register of the MC68881,errno and return
   value for a math function who used MC68881.
*/

#include <math.h>
#include <values.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/fpu.h>
#include <_math.h>

extern double _matherr ( register unsigned int status )
{
	if ((status) & (EXC_OPER | FPCC_NAN)) {		/* operand error */
		errno = EDOM;
		return ( _EDOM_RET_VAL );	/* return impl. defined value */
	}

	errno = ERANGE;		/* for both underflow or overflow */

	if ((status) & (EXC_UNFL)) 		/* underflow */
		return ( 0.0 );

	if ((status) & (EXC_OVFL))		/* overflow */
		return ( HUGE_VAL );

	/* in this case, an error here : exception not treated 		*/
}
