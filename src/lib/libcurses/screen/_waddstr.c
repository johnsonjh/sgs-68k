/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _waddstr.c: version 25.1 created on 12/2/91 at 19:16:38	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_waddstr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/_waddstr.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:_waddstr.c	25.1"


#define		NOMACROS
#include	"curses_inc.h"

waddstr(win, str)
WINDOW	*win;
char	*str;
{
    return (waddnstr(win, str, -1));
}
