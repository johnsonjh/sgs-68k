/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) resetty.c: version 25.1 created on 12/2/91 at 19:24:08	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)resetty.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/resetty.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:resetty.c	25.1"


#include	"curses_inc.h"

resetty()
{
    if ((_BR(SP->save_tty_buf)) != 0)
    {
	PROGTTY = SP->save_tty_buf;
#ifdef	DEBUG
	if (outf)
#ifdef	SYSV
	    fprintf(outf, "resetty(), file %x, SP %x, flags %x, %x, %x, %x\n",
		cur_term->Filedes, SP, PROGTTY.c_iflag, PROGTTY.c_oflag,
		PROGTTY.c_cflag, PROGTTY.c_lflag);
#else	/* SYSV */
	    fprintf(outf, "resetty(), file %x, SP %x, flags %x\n",
		cur_term->Filedes, SP, PROGTTY.sg_flags);
#endif	/* SYSV */
#endif	/* DEBUG */
	reset_prog_mode();
    }
    return (OK);
}
