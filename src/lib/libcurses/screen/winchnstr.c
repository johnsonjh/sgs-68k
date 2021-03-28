/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) winchnstr.c: version 25.1 created on 12/2/91 at 19:29:41	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)winchnstr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/winchnstr.c	1.2"	*/
	
#ident	"@(#)lib/libcurses:winchnstr.c	25.1"

#include	"curses_inc.h"

/*
 * Read in ncols worth of data from window win and assign the
 * chars to string. NULL terminate string upon completion.
 * Return the number of chtypes copied.
 */

winchnstr(win,string,ncols)
register	WINDOW	*win;
chtype		*string;
int		ncols;
{
    register	chtype	*ptr = &(win->_y[win->_cury][win->_curx]);
    register	int	counter;

    /*
     * Adjust ncols if it will exceed the maximum for the
     * window being referenced
     */

    if ((win->_curx + ncols) > win->_maxx)
	ncols = win->_maxx - win->_curx;

    for (counter = 0; counter < ncols; counter++)
	*string++ = *ptr++;

    *string = (chtype) 0;
    return (counter);
}
