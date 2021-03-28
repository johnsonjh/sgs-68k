/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Minchnstr.c: version 25.1 created on 12/2/91 at 19:10:01	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Minchnstr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/_mvinchnstr.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:Minchnstr.c	25.1"


#define	NOMACROS

#include	"curses_inc.h"

mvinchnstr(y, x, s, n)
int	y, x, n;
chtype	*s;
{
    return (mvwinchnstr(stdscr, y, x, s, n));
}
