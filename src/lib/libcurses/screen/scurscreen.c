/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scurscreen.c: version 25.1 created on 12/2/91 at 19:25:06	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scurscreen.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/setcurscreen.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:scurscreen.c	25.1"

#include "curses_inc.h"

SCREEN	*
setcurscreen(new)
SCREEN	*new;
{
    register	SCREEN	*rv = SP;

    if (new != SP)
    {

#ifdef	DEBUG
	if (outf)
	    fprintf(outf, "setterm: old %x, new %x\n", rv, new);
#endif	/* DEBUG */

	SP = new;
	if (new) {
		(void) setcurterm(SP->tcap);
		LINES = SP->lsize;
		COLS = SP->csize;
		TABSIZE = SP->tsize;
		stdscr = SP->std_scr;
		curscr = SP->cur_scr;
		_virtscr = SP->virt_scr;
	}
    }
    return (rv);
}
