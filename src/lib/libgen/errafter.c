/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) errafter.c: version 25.1 created on 12/2/91 at 19:32:05	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)errafter.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:errafter.c	1.2"	*/
	
#ident	"@(#)libgen:errafter.c	25.1"


/*
	Customized routine called after error message has been printed.
	Command and library version.
	Return a value to indicate action.
*/

#include	"errmsg.h"
#include	<stdio.h>
#include	<varargs.h>

int
errafter( severity, format, print_args )
int	severity;
char	*format;
va_list print_args;
{
	switch( severity ) {
	case EHALT:
		return EABORT;
	case EERROR:
		return EEXIT;
	case EWARN:
		return ERETURN;
	case EINFO:
		return ERETURN;
	}
	return ERETURN;
}
