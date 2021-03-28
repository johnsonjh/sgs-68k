/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) erraction.c: version 25.1 created on 12/2/91 at 19:32:02	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)erraction.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:erraction.c	1.2"	*/
	
#ident	"@(#)libgen:erraction.c	25.1"


/*
	Routine called after error message has been printed.
	Dependent upon the return code of errafter.
	Command and library version.
*/

#include	"errmsg.h"
#include	<stdio.h>

void
erraction( action )
int      action;
{


	switch( action ){
	case EABORT:
	     abort();
	     break;
	case EEXIT:
	     exit( Err.exit );
	     break;
	case ERETURN:
	     break;
	}
	return;
}
