/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) initscr.c: version 25.1 created on 12/2/91 at 19:20:51	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)initscr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/initscr.c	1.3.1.7"	*/
	
#ident	"@(#)lib/libcurses:initscr.c	25.1"

#include	"curses_inc.h"
#include	<signal.h>

/*
 * This routine initializes the current and standard screen,
 * and sets up the terminal.  In case of error, initscr aborts.
 * If you want an error status returned, call
 *	scp = newscreen(getenv("TERM"), 0, 0, 0, stdout, stdin);
 */

WINDOW	*
initscr()
{
#ifdef	SIGPOLL
    void	(*savsignal)();
#else	/* SIGPOLL */
    int		(*savsignal)();
#endif	/* SIGPOLL */

    extern	int	_ccleanup();
#ifdef	SIGTSTP
    extern	int	_tstp();
#endif	/* SIGTSTP */

    static	char	i_called_before = FALSE;

/* Free structures we are about to throw away so we can reuse the memory. */

    if (i_called_before && SP)
    {
	delscreen(SP);
	SP = NULL;
    }
    if (newscreen(NULL, 0, 0, 0, stdout, stdin) == NULL)
    {
	reset_shell_mode();
	if (term_errno != -1)
	    termerr();
	else
	    curserr();
	exit(1);
    }

    i_called_before = TRUE;

#ifdef	SIGTSTP
    if ((savsignal = signal(SIGTSTP, SIG_IGN)) == SIG_DFL)
	(void) signal(SIGTSTP, _tstp);
    else
	(void) signal(SIGTSTP, savsignal);
#endif	/* SIGTSTP */
    if ((savsignal = signal(SIGINT, SIG_IGN)) == SIG_DFL)
	(void) signal(SIGINT, _ccleanup);
    else
	(void) signal(SIGINT, savsignal);

    if ((savsignal = signal(SIGQUIT, SIG_IGN)) == SIG_DFL)
	(void) signal(SIGQUIT, _ccleanup);
    else
	(void) signal(SIGQUIT, savsignal);

    return (stdscr);
}
