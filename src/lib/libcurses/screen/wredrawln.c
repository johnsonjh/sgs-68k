/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wredrawln.c: version 25.1 created on 12/2/91 at 19:30:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wredrawln.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/wredrawln.c	1.2"	*/
	
#ident	"@(#)lib/libcurses:wredrawln.c	25.1"


/*
 * This routine indicates to curses that a screen line is garbaged and
 * should be thrown away before having anything written over the top of it.
 * It could be used for programs such as editors which want a command to
 * redraw just a single line. Such a command could be used in cases where
 * there is a noisy line and redrawing the entire screen would be subject
 * to even more noise. Just redrawing the single line gives some semblance
 * of hope that it would show up unblemished.
 *
 * This is a more refined version of clearok
 */

#include	"curses_inc.h"

wredrawln(win, begline, numlines)
register	WINDOW		*win;
int		begline;
register	int	numlines;
{
    register	short	*firstch, *efirstch;

    if (numlines <= 0)
	return (ERR);
    if (begline < 0)
	begline = 0;
    if (begline + numlines > win->_maxy)
	numlines = win->_maxy - begline;

    firstch = win->_firstch + begline;
    efirstch = firstch + numlines;
    while (firstch < efirstch)
	*firstch++ = _REDRAW;

    return (win->_immed ? wrefresh(win) : OK);
}
