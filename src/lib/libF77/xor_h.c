/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) xor_h.c: version 25.1 created on 12/2/91 at 18:37:05	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)xor_h.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:xor_h.c	1.2"	*/
	
#ident	"@(#)libF77:xor_h.c	25.1"

short xor_h(m,n)
short *m, *n;
{
	return(*m ^ *n);
}
