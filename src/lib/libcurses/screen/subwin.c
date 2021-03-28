/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) subwin.c: version 25.1 created on 12/2/91 at 19:26:14	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)subwin.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/subwin.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:subwin.c	25.1"

#include	"curses_inc.h"

WINDOW	*
subwin(win,l,nc,by,bx)
WINDOW	*win;
int	l,nc,by,bx;
{
    return (derwin(win,l,nc,by - win->_begy,bx - win->_begx));
}
