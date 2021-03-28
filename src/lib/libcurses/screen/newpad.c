/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) newpad.c: version 25.1 created on 12/2/91 at 19:22:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)newpad.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/newpad.c	1.1.1.4"	*/
	
#ident	"@(#)lib/libcurses:newpad.c	25.1"

#include	"curses_inc.h"

WINDOW	*
newpad(l,nc)
int	l,nc;
{
    WINDOW	*pad;

    pad = newwin(l,nc,0,0);
    if (pad != (WINDOW *) NULL)
	pad->_flags |= _ISPAD;
    return (pad);
}
