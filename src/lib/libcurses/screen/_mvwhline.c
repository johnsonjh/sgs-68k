/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _mvwhline.c: version 25.1 created on 12/2/91 at 19:14:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_mvwhline.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/_mvwhline.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:_mvwhline.c	25.1"


#define		NOMACROS
#include	"curses_inc.h"

mvwhline(win, y, x, c, n)
WINDOW	*win;
int	y, x, n;
chtype	c;
{
    return (wmove(win, y, x)==ERR?ERR:whline(win, c, n));
}
