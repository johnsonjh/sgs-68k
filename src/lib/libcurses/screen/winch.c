/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) winch.c: version 25.1 created on 12/2/91 at 19:29:38	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)winch.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/winch.c	1.2"	*/
	
#ident	"@(#)lib/libcurses:winch.c	25.1"

#include	"curses_inc.h"

chtype
winch(win)
register	WINDOW	*win;
{
    return (win->_y[win->_cury][win->_curx]);
}
