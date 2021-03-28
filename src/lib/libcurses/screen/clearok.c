/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) clearok.c: version 25.1 created on 12/2/91 at 19:17:39	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)clearok.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/clearok.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:clearok.c	25.1"

#include	"curses_inc.h"

clearok(win,bf)
WINDOW	*win;
int	bf;
{
    win->_clear = bf;
    return (OK);
}
