/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) is_wintou.c: version 25.1 created on 12/2/91 at 19:20:58	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)is_wintou.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/is_wintou.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:is_wintou.c	25.1"

#include	"curses_inc.h"

is_wintouched(win)
WINDOW	*win;
{
    return (win->_flags & _WINCHANGED);
}
