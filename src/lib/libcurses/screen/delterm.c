/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) delterm.c: version 25.1 created on 12/2/91 at 19:18:41	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)delterm.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/delterm.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:delterm.c	25.1"

#include "curses_inc.h"

/*
 * Relinquish the storage associated with "terminal".
 */
extern	TERMINAL	_first_term;
extern	char		_called_before;
extern	char		_frst_tblstr[];

delterm(terminal)
register	TERMINAL	*terminal;
{
    if (!terminal)
	return (ERR);
    delkeymap(terminal);
    if (terminal->_check_fd >= 0)
	close(terminal->_check_fd);
    if (terminal == &_first_term)
    {
	/* next setupterm can re-use static areas */
	_called_before = FALSE;
	if (terminal->_strtab != _frst_tblstr)
	    free((char *)terminal->_strtab);
    }
    else
    {
	free((char *)terminal->_bools);
	free((char *)terminal->_nums);
	free((char *)terminal->_strs);
	free((char *)terminal->_strtab);
	free((char *)terminal);
    }
    return (OK);
}
