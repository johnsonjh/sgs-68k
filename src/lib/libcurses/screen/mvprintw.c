/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mvprintw.c: version 25.1 created on 12/2/91 at 19:22:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mvprintw.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/mvprintw.c	1.3.1.4"	*/
	
#ident	"@(#)lib/libcurses:mvprintw.c	25.1"

# include	"curses_inc.h"
# include	<varargs.h>

/*
 * implement the mvprintw commands.  Due to the variable number of
 * arguments, they cannot be macros.  Sigh....
 *
 */

/*VARARGS*/
mvprintw(va_alist)
va_dcl
{
	register int	y, x;
	register char * fmt;
	va_list ap;

	va_start(ap);
	y = va_arg(ap, int);
	x = va_arg(ap, int);
	fmt = va_arg(ap, char *);
	return move(y, x) == OK ? vwprintw(stdscr, fmt, ap) : ERR;
}
