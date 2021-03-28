/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) raw.c: version 25.1 created on 12/2/91 at 19:23:57	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)raw.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/raw.c	1.12"	*/
	
#ident	"@(#)lib/libcurses:raw.c	25.1"

/*
 * Routines to deal with setting and resetting modes in the tty driver.
 * See also setupterm.c in the termlib part.
 */
#include "curses_inc.h"

raw()
{
#ifdef SYSV
    /* Disable interrupt characters */
    PROGTTY.c_lflag &= ~(ISIG|ICANON);
    PROGTTY.c_cc[VMIN] = 1;
    PROGTTY.c_cc[VTIME] = 0;
    PROGTTY.c_iflag &= ~IXON;
#else
    PROGTTY.sg_flags &= ~CBREAK;
    PROGTTY.sg_flags |= RAW;
#endif

#ifdef DEBUG
# ifdef SYSV
    if (outf)
	fprintf(outf, "raw(), file %x, iflag %x, cflag %x\n",
	    cur_term->Filedes, PROGTTY.c_iflag, PROGTTY.c_cflag);
# else
    if (outf)
	fprintf(outf, "raw(), file %x, flags %x\n",
	    cur_term->Filedes, PROGTTY.sg_flags);
# endif /* SYSV */
#endif

    if (!needs_xon_xoff)
	xon_xoff = 0;	/* Cannot use xon/xoff in raw mode */
    cur_term->_fl_rawmode = 2;
    cur_term->_delay = -1;
    reset_prog_mode();
#ifdef FIONREAD
    cur_term->timeout = 0;
#endif /* FIONREAD */
    return (OK);
}
