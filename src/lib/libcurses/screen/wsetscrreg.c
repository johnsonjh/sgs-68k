/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wsetscrreg.c: version 25.1 created on 12/2/91 at 19:30:31	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wsetscrreg.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/wsetscrreg.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:wsetscrreg.c	25.1"

#include	"curses_inc.h"

/*
 *	Change scrolling region. Since we depend on the values
 *	of tmarg and bmarg in various ways, this can no longer
 *	be a macro.
 */

wsetscrreg(win,topy,boty)
WINDOW	*win;
int	topy, boty;
{
    if (topy < 0 || topy >= win->_maxy || boty < 0 || boty >= win->_maxy)
	return (ERR);

    win->_tmarg = topy;
    win->_bmarg = boty;
    return (OK);
}
