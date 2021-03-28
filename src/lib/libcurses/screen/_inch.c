/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _inch.c: version 25.1 created on 12/2/91 at 19:12:49	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_inch.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/_inch.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:_inch.c	25.1"


#define		NOMACROS
#include	"curses_inc.h"

chtype
inch()
{
    return (winch(stdscr));
}
