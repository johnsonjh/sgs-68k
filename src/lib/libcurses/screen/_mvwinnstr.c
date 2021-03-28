/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _mvwinnstr.c: version 25.1 created on 12/2/91 at 19:14:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_mvwinnstr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/_mvwinnstr.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:_mvwinnstr.c	25.1"


#define		NOMACROS
#include	"curses_inc.h"

mvwinnstr(win, y, x, s, n)
WINDOW *win;
int	y, x, n;
char	*s;
{
    return (wmove(win, y, x)==ERR?ERR:winnstr(win, s, n));
}
