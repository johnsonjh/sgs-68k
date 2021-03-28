/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wprintw.c: version 25.1 created on 12/2/91 at 19:30:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wprintw.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/wprintw.c	1.7"	*/
	
#ident	"@(#)lib/libcurses:wprintw.c	25.1"

/*
 * printw and friends
 *
 */

# include	"curses_inc.h"
# include	<varargs.h>

/*
 *	This routine implements a printf on the given window.
 */
/*VARARGS*/
wprintw(va_alist)
va_dcl
{
	va_list ap;
	register WINDOW	*win;
	register char * fmt;

	va_start(ap);
	win = va_arg(ap, WINDOW *);
	fmt = va_arg(ap, char *);
	return vwprintw(win, fmt, ap);
}
