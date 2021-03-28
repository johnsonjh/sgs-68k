/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) restart.c: version 25.1 created on 12/2/91 at 19:24:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)restart.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/restart.c	1.9"	*/
	
#ident	"@(#)lib/libcurses:restart.c	25.1"


#include	"curses_inc.h"

/*
 * This is useful after saving/restoring memory from a file (e.g. as
 * in a rogue save game).  It assumes that the modes and windows are
 * as wanted by the user, but the terminal type and baud rate may
 * have changed.
 */

extern	char	_called_before;

restartterm(term, filenum, errret)
char	*term;
int	filenum;	/* This is a UNIX file descriptor, not a stdio ptr. */
int	*errret;
{
    int	saveecho = SP->fl_echoit;
    int	savecbreak = cur_term->_fl_rawmode;
    int	savenl;

#ifdef	SYSV
    savenl = PROGTTY.c_iflag & ONLCR;
#else	/* SYSV */
    savenl = PROGTTY.sg_flags & CRMOD;
#endif	/* SYSV */

    _called_before = 0;
    (void) setupterm(term, filenum, (int *) 0);

    /* Restore curses settable flags, leaving other stuff alone. */
    SP->fl_echoit = saveecho;

    nocbreak();
    noraw();
    if (savecbreak == 1)
	cbreak();
    else
	if (savecbreak == 2)
	    raw();

    if (savenl)
	nl();
    else
	nonl();

    reset_prog_mode();

    LINES = SP->lsize;
    COLS = columns;
    return (OK);
}
