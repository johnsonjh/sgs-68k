/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mvderwin.c: version 25.1 created on 12/2/91 at 19:22:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mvderwin.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/mvderwin.c	1.2"	*/
	
#ident	"@(#)lib/libcurses:mvderwin.c	25.1"

#include	"curses_inc.h"

/*
 * Move a derived window inside its parent window.
 * This routine does not change the screen relative
 * parameters begx and begy. Thus, it can be used to
 * display different parts of the parent window at
 * the same screen coordinate.
 */

mvderwin(win, pary, parx)
WINDOW	*win;
int	pary, parx;
{
    register	int	y, maxy, maxx;
    register	WINDOW	*par;
    chtype		obkgd, **wc, **pc;
    short		*begch, *endch;

    if ((par = win->_parent) == NULL)
	goto bad;
    if (pary == win->_pary && parx == win->_parx)
	return (OK);

    maxy = win->_maxy-1;
    maxx = win->_maxx-1;
    if ((parx + maxx) >= par->_maxx || (pary + maxy) >= par->_maxy)
bad:
	return (ERR);

    /* save all old changes */
    wsyncup(win);

    /* rearrange pointers */
    win->_parx = parx;
    win->_pary = pary;
    wc = win->_y;
    pc = par->_y + pary;
    begch = win->_firstch;
    endch = win->_lastch;
    for (y = 0; y <= maxy; ++y, ++wc, ++pc, ++begch, ++endch)
    {
	*wc = *pc + parx;
	*begch = 0;
	*endch = maxx;
    }

    /* change background to our own */
    obkgd = win->_bkgd;
    win->_bkgd = par->_bkgd;
    return (wbkgd(win, obkgd));
}
