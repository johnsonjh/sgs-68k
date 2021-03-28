/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iargc_.c: version 25.1 created on 12/2/91 at 18:33:52	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iargc_.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:iargc_.c	1.3"	*/
	
#ident	"@(#)libF77:iargc_.c	25.1"

long int iargc_()
{
extern int xargc;
return ( xargc - 1 );
}
