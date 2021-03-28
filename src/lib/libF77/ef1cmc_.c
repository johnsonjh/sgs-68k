/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ef1cmc_.c: version 25.1 created on 12/2/91 at 18:32:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ef1cmc_.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:ef1cmc_.c	1.3"	*/
	
#ident	"@(#)libF77:ef1cmc_.c	25.1"

/* EFL support routine to compare two character strings */

long int ef1cmc_(a, la, b, lb)
int *a, *b;
long int *la, *lb;
{
return( s_cmp( (char *)a, (char *)b, *la, *lb) );
}
