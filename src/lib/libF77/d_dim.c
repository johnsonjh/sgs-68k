/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) d_dim.c: version 25.1 created on 12/2/91 at 18:31:35	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)d_dim.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:d_dim.c	1.3"	*/
	
#ident	"@(#)libF77:d_dim.c	25.1"

double d_dim(a,b)
double *a, *b;
{
return( *a > *b ? *a - *b : 0);
}
