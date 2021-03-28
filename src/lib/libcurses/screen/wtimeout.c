/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wtimeout.c: version 25.1 created on 12/2/91 at 19:30:42	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wtimeout.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/wtimeout.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:wtimeout.c	25.1"

#include	"curses_inc.h"

void
wtimeout(win,tm)
WINDOW	*win;
int	tm;
{
    win->_delay = tm;
}
