/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) reset_sh.c: version 25.1 created on 12/2/91 at 19:24:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)reset_sh.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/reset_sh.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:reset_sh.c	25.1"

#include	"curses_inc.h"

reset_shell_mode()
{
#ifdef	DIOCSETT
    /*
     * Restore any virtual terminal setting.  This must be done
     * before the TIOCSETN because DIOCSETT will clobber flags like xtabs.
     */
    cur_term -> old.st_flgs |= TM_SET;
    (void) ioctl(cur_term->Filedes, DIOCSETT, &cur_term -> old);
#endif	/* DIOCSETT */
    if (_BR(SHELLTTY))
    {
	(void) ioctl(cur_term -> Filedes,
#ifdef	SYSV
	    TCSETAW,
#else	/* SYSV */
	    TIOCSETN,
#endif	/* SYSV */
		    &SHELLTTY);
#ifdef	LTILDE
	if (cur_term -> newlmode != cur_term -> oldlmode)
	    (void) ioctl(cur_term -> Filedes, TIOCLSET, &cur_term -> oldlmode);
#endif	/* LTILDE */
    }
    return (OK);
}
