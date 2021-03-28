/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Maddchnstr.c: version 25.1 created on 12/2/91 at 19:09:54	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Maddchnstr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/_mvaddchnstr.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:Maddchnstr.c	25.1"


#define	NOMACROS

#include	"curses_inc.h"

mvaddchnstr(y, x, s, n)
int	y, x, n;
chtype	*s;
{
    return (wmove(stdscr, y, x)==ERR?ERR:waddchnstr(stdscr, s, n));
}
