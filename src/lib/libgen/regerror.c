/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) regerror.c: version 25.1 created on 12/2/91 at 19:33:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)regerror.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:regerror.c	1.2"	*/
	
#ident	"@(#)libgen:regerror.c	25.1"

/*
	return the a message explaining a regexp(5) error code
*/

#include "libgen.h"

char *
regerror(code)
int	code;
{
	switch( code ) {
	case 11:
		return "Range endpoint too large.";
	case 16:
		return "Bad number.";
	case 25:
		return "'\\digit' out of range.";
	case 36:
		return "Illegal or missing delimiter.";
	case 41:
		return "No remembered search string.";
	case 42:
		return "'\\( \\)' imbalance.";
	case 43:
		return "Too many '\\('.";
	case 44:
		return "More than 2 numbers given in \\{ \\}.";
	case 45:
		return "} expected after \\.";
	case 46:
		return "First number exceeds second in \\{ \\}.";
	case 49:
		return "[] imbalance.";
	case 50:
		return "Regular expression too long or too complex.";
	}
	return "Unknown code!";
}
