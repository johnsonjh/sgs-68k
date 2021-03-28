/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) r_exp.c: version 25.1 created on 12/2/91 at 18:35:31	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)r_exp.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:r_exp.c	1.3"	*/
	
#ident	"@(#)libF77:r_exp.c	25.1"

double r_exp(x)
float *x;
{
double exp();
return( exp(*x) );
}
