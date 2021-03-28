/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rand_.c: version 25.1 created on 12/2/91 at 18:36:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rand_.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:rand_.c	1.5"	*/
	
#ident	"@(#)libF77:rand_.c	25.1"

/*
Uniform random number generator. 
Linear congruential generator, suitable for 32 bit machines;
multiplication is mod 2**31
*/

static	long	randx = 1;

srand_(x)	/* subroutine to set seed */
long *x;
{
randx = *x;
}

int
irand_()
{
	return(((randx = randx * 1103515245L + 12345)>>16) & 0x7fff);
}

double rand_()
{
	return(irand_()/32768.0);
}
