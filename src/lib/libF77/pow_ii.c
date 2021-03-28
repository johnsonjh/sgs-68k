/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) pow_ii.c: version 25.1 created on 12/2/91 at 18:34:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)pow_ii.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:pow_ii.c	1.4"	*/
	
#ident	"@(#)libF77:pow_ii.c	25.1"


long int pow_ii(ap, bp)
long int *ap, *bp;
{
long int pow, x, n;
if(n = *bp) { if (n<0) return(0);
	else {
	pow=1;
	x = *ap;
	for( ; ; )
		{
		if(n & 01)
			pow *= x;
		if(n >>= 1)
			x *= x;
		else
			return(pow);
		}
	}
     }
else return(1);
}
