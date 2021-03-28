/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) keypad.c: version 25.1 created on 12/2/91 at 19:21:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)keypad.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/keypad.c	1.6"	*/
	
#ident	"@(#)lib/libcurses:keypad.c	25.1"


#include	"curses_inc.h"

/* TRUE => special keys should be passed as a single value by getch. */

keypad(win, bf)
WINDOW	*win;
int	bf;
{
    extern	int	_outch();
    /*
     * Since _use_keypad is a bit and not a bool, we must check
     * bf, in case someone uses an odd number instead of 1 for TRUE
     */

    win->_use_keypad = (bf) ? TRUE : FALSE;
    if (bf && (!SP->kp_state))
    {
	tputs(keypad_xmit, 1, _outch);
	(void) fflush(SP->term_file);
	SP->kp_state = TRUE;
	return (setkeymap());
    }
    return (OK);
}
