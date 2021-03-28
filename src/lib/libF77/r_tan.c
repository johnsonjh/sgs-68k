/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) r_tan.c: version 25.1 created on 12/2/91 at 18:36:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)r_tan.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:r_tan.c	1.3"	*/
	
#ident	"@(#)libF77:r_tan.c	25.1"

double r_tan(x)
float *x;
{
double tan();
return( tan(*x) );
}
