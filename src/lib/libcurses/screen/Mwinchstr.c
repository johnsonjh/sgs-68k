/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mwinchstr.c: version 25.1 created on 12/2/91 at 19:10:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mwinchstr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/_mvwinchstr.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:Mwinchstr.c	25.1"


#define	NOMACROS

#include	"curses_inc.h"

mvwinchstr(win, y, x, str)
WINDOW *win;
int	y, x;
chtype	*str;
{
    return (wmove(win, y, x)==ERR?ERR:winchstr(win, str));
}
