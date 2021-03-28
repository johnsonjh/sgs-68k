/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wechochar.c: version 25.1 created on 12/2/91 at 19:29:24	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wechochar.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/wechochar.c	1.6"	*/
	
#ident	"@(#)lib/libcurses:wechochar.c	25.1"

#include	"curses_inc.h"

/*
 *  These routines short-circuit much of the innards of curses in order to get
 *  a single character output to the screen quickly! It is used by getch()
 *  and getstr().
 *
 *  wechochar(WINDOW *win, chtype ch) is functionally equivalent to
 *  waddch(WINDOW *win, chtype ch), wrefresh(WINDOW *win)
 */

wechochar (win, ch)
register WINDOW *win;
chtype ch;
{
    int	saveimm = win->_immed, rv;

    immedok(win,TRUE);
    rv = waddch(win,ch);
    win->_immed = saveimm;
    return (rv);
}
