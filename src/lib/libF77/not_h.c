/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) not_h.c: version 25.1 created on 12/2/91 at 18:34:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)not_h.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:not_h.c	1.2"	*/
	
#ident	"@(#)libF77:not_h.c	25.1"

short not_h(m)
short *m;
{
	return(~*m);
}
