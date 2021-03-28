/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wbkgdset.c: version 25.1 created on 12/2/91 at 19:29:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wbkgdset.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/wbkgdset.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:wbkgdset.c	25.1"

#include	"curses_inc.h"

void
wbkgdset(win,c)
WINDOW	*win;
chtype	c;
{
    win->_attrs = (win->_attrs & ~(win->_bkgd & A_ATTRIBUTES)) | (c & A_ATTRIBUTES);
    win->_bkgd = c;
}
