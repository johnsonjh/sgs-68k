/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) winstr.c: version 25.1 created on 12/2/91 at 19:30:02	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)winstr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/winstr.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:winstr.c	25.1"

#include	"curses_inc.h"

winstr(win,str)
WINDOW	*win;
char	*str;
{
    return (winnstr(win,str,win->_maxx - win->_curx));
}
