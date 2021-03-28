/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) nodelay.c: version 25.1 created on 12/2/91 at 19:22:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)nodelay.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/nodelay.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:nodelay.c	25.1"

/*
 * Routines to deal with setting and resetting modes in the tty driver.
 * See also setupterm.c in the termlib part.
 */
#include "curses_inc.h"

/*
 * TRUE => don't wait for input, but return -1 instead.
 */

nodelay(win,bf)
WINDOW *win; int bf;
{
    win->_delay = (bf) ? 0 : -1;
    return (OK);
}
