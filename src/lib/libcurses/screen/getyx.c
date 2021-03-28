/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) getyx.c: version 25.1 created on 12/2/91 at 19:20:00	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)getyx.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/getyx.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:getyx.c	25.1"

#include	"curses_inc.h"

getcury(win)
WINDOW	*win;
{
    return (win->_cury);
}

getcurx(win)
WINDOW	*win;
{
    return (win->_curx);
}
