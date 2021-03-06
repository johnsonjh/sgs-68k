/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) flushinp.c: version 25.1 created on 12/2/91 at 19:19:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)flushinp.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/flushinp.c	1.4.1.5"	*/
	
#ident	"@(#)lib/libcurses:flushinp.c	25.1"


#include	"curses_inc.h"

flushinp()
{
#ifdef	DEBUG
    if (outf)
	fprintf(outf, "flushinp(), file %x, SP %x\n", cur_term->Filedes, SP);
#endif	/* DEBUG */

#ifdef	SYSV
    (void) ioctl(cur_term -> Filedes, TCFLSH, 0);
#else	/* SYSV */
    /* for insurance against someone using their own buffer: */
    (void) ioctl(cur_term -> Filedes, TIOCGETP, &(PROGTTY));

    /*
     * SETP waits on output and flushes input as side effect.
     * Really want an ioctl like TCFLSH but Berkeley doesn't have one.
     */
    (void) ioctl(cur_term -> Filedes, TIOCSETP, &(PROGTTY));
#endif	/* SYSV */

    /*
     * Get rid of any typeahead which was read().
     * Leave characters which were ungetch()'d.
     */
    cur_term->_chars_on_queue = cur_term->_ungotten;

    /*
     * Have to doupdate() because, if we have stopped output due to
     * typeahead, now that typeahead is gone, so we had better catch up.
     */
    if (_INPUTPENDING)
	(void) doupdate();
    return (OK);
}
