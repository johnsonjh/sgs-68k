/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) leaveok.c: version 25.1 created on 12/2/91 at 19:21:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)leaveok.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/leaveok.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:leaveok.c	25.1"

#include	"curses_inc.h"

leaveok(win,bf)
WINDOW	*win;
int	bf;
{
    win->_leave = bf;
    return (OK);
}
