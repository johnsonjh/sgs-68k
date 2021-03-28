/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) r_lg10.c: version 25.1 created on 12/2/91 at 18:35:41	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)r_lg10.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:r_lg10.c	1.3"	*/
	
#ident	"@(#)libF77:r_lg10.c	25.1"

#define log10e 0.43429448190325182765

double r_lg10(x)
float *x;
{
double log();

return( log10e * log(*x) );
}
