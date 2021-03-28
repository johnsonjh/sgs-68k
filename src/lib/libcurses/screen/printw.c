/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) printw.c: version 25.1 created on 12/2/91 at 19:23:44	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)printw.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/printw.c	1.7"	*/
	
#ident	"@(#)lib/libcurses:printw.c	25.1"

/*
 * printw and friends
 *
 */

# include	"curses_inc.h"
# include	<varargs.h>

/*
 *	This routine implements a printf on the standard screen.
 */
/*VARARGS1*/
printw(va_alist)
va_dcl
{
	register char * fmt;
	va_list ap;

	va_start(ap);
	fmt = va_arg(ap, char *);
	return vwprintw(stdscr, fmt, ap);
}
