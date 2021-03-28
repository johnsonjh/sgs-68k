/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) redrawwin.c: version 25.1 created on 12/2/91 at 19:24:01	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)redrawwin.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/redrawwin.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:redrawwin.c	25.1"

#include	"curses_inc.h"

redrawwin(win)
WINDOW	*win;
{
    return (wredrawln(win,0,win->_maxy));
}
