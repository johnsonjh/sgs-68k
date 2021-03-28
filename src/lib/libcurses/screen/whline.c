/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) whline.c: version 25.1 created on 12/2/91 at 19:29:34	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)whline.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/whline.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:whline.c	25.1"

#include	"curses_inc.h"

whline(win, horch, num_chars)
register	WINDOW	*win;
chtype		horch;
int		num_chars;
{
    int     cury = win->_cury, curx = win->_curx;
    chtype  a, *fp = &(win->_y[cury][curx]);

    if (num_chars <= 0)
	 return (ERR);

    if (num_chars > win->_maxx - curx + 1)
	num_chars = win->_maxx - curx + 1;
    if (horch == 0)
	horch = ACS_HLINE;
    a = _ATTR(horch);
    horch = _WCHAR(win, horch) | a;
    memSset (fp, horch | win->_attrs, num_chars);
    if (curx < win->_firstch[cury])
	win->_firstch[cury] = curx;
    if ((curx += (num_chars - 1)) > win->_lastch[cury])
	win->_lastch[cury] = curx;
    win->_flags |= _WINCHANGED;

    if (win->_sync)
	wsyncup(win);

    return (win->_immed ? wrefresh(win) : OK);
}
