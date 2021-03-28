/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) vwprintw.c: version 25.1 created on 12/2/91 at 19:28:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)vwprintw.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/vwprintw.c	1.4"	*/
	
#ident	"@(#)lib/libcurses:vwprintw.c	25.1"

/*
 * printw and friends
 *
 */

# include	"curses_inc.h"
# include	<varargs.h>

/*
 *	This routine actually executes the printf and adds it to the window
 *
 *	This code now uses the vsprintf routine, which portably digs
 *	into stdio.  We provide a vsprintf for older systems that don't
 *	have one.
 */

/*VARARGS2*/
vwprintw(win, fmt, ap)
register WINDOW	*win;
register char * fmt;
va_list ap;
{
	char	buf[BUFSIZ];
	register int n;

	n = vsprintf(buf, fmt, ap);
	va_end(ap);
	if (n == ERR)
		return ERR;
	return waddstr(win, buf);
}
