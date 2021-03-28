/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _sprintw.c: version 25.1 created on 12/2/91 at 19:16:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_sprintw.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/V2._sprintw.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:_sprintw.c	25.1"


# include	"curses_inc.h"
# include	<varargs.h>

#ifdef _VR2_COMPAT_CODE
/*
	This is only here for compatibility with SVR2 curses.
	It will go away someday. Programs should reference
	vwprintw() instead.
 */

_sprintw(win, fmt, ap)
register WINDOW	*win;
register char * fmt;
va_list ap;
{
	return vwprintw(win, fmt, ap);
}
#endif
