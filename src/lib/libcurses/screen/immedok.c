/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) immedok.c: version 25.1 created on 12/2/91 at 19:20:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)immedok.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/immedok.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:immedok.c	25.1"

#include	"curses_inc.h"

extern	int	_quick_echo();

void
immedok(win, bf)
WINDOW	*win;
int	bf;
{
    if (bf)
    {
	win->_immed = TRUE;
	_quick_ptr = _quick_echo;
    }
    else
	win->_immed = FALSE;
}
