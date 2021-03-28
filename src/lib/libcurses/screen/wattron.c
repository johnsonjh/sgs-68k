/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wattron.c: version 25.1 created on 12/2/91 at 19:28:52	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wattron.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)curses:screen/wattron.c	1.10"
#include	"curses_inc.h"

wattron(win,a)
WINDOW	*win;
chtype	a;
{
    /* if 'a' contains color information, then if we are on color terminal */
    /* erase color information from window attribute, otherwise erase      */
    /* color information from 'a'					   */

    if (a & A_COLOR)
        if (SP->_pairs_tbl)
            win->_attrs &= ~A_COLOR;
	else
	    a &= ~A_COLOR;

    win->_attrs |= (a & A_ATTRIBUTES);
    return (1);
}
