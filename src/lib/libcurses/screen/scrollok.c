/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scrollok.c: version 25.1 created on 12/2/91 at 19:25:02	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scrollok.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/scrollok.c	1.7"	*/
	
#ident	"@(#)lib/libcurses:scrollok.c	25.1"

#include	"curses_inc.h"

scrollok(win,bf)
WINDOW	*win;
int	bf;
{
    win->_scroll = (bf) ? TRUE : FALSE;
    return (OK);
}
