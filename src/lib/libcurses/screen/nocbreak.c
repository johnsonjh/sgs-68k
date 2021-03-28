/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) nocbreak.c: version 25.1 created on 12/2/91 at 19:22:42	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)nocbreak.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/nocbreak.c	1.3.1.5"	*/
	
#ident	"@(#)lib/libcurses:nocbreak.c	25.1"


#include	"curses_inc.h"

nocbreak()
{
#ifdef	SYSV
    /* See comment in cbreak.c about ICRNL. */
    PROGTTY.c_iflag |= ICRNL;
    PROGTTY.c_lflag |= ICANON;
    PROGTTY.c_cc[VEOF] = _CTRL('D');
    PROGTTY.c_cc[VEOL] = 0;
#else	/* SYSV */
    PROGTTY.sg_flags &= ~(CBREAK | CRMOD);
#endif	/* SYSV */

#ifdef	DEBUG
#ifdef	SYSV
    if (outf)
	fprintf(outf, "nocbreak(), file %x, flags %x\n",
	    cur_term->Filedes, PROGTTY.c_lflag);
#else	/* SYSV */
    if (outf)
	fprintf(outf, "nocbreak(), file %x, flags %x\n",
	    cur_term->Filedes, PROGTTY.sg_flags);
#endif	/* SYSV */
#endif	/* DEBUG */

    cur_term->_fl_rawmode = FALSE;
    cur_term->_delay = -1;
    reset_prog_mode();
#ifdef	FIONREAD
    cur_term->timeout = 0;
#endif	/* FIONREAD */
    return (OK);
}
