/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) d_mod.c: version 25.1 created on 12/2/91 at 18:31:54	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)d_mod.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:d_mod.c	1.3"	*/
	
#ident	"@(#)libF77:d_mod.c	25.1"

double d_mod(x,y)
double *x, *y;
{
double floor(), quotient;
if( (quotient = *x / *y) >= 0)
	quotient = floor(quotient);
else
	quotient = -floor(-quotient);
return(*x - (*y) * quotient );
}
