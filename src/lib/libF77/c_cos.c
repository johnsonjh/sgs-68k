/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) c_cos.c: version 25.1 created on 12/2/91 at 18:30:44	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)c_cos.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:c_cos.c	1.3"	*/
	
#ident	"@(#)libF77:c_cos.c	25.1"

#include "complex"

c_cos(r, z)
complex *r, *z;
{
double sin(), cos(), sinh(), cosh();

r->real = cos(z->real) * cosh(z->imag);
r->imag = - sin(z->real) * sinh(z->imag);
}
