/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tinputfd.c: version 25.1 created on 12/2/91 at 19:27:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tinputfd.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/tinputfd.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:tinputfd.c	25.1"

#include	"curses_inc.h"

/* Set the input channel for the current terminal. */

void
tinputfd(fd)
int	fd;
{
    cur_term->_inputfd = fd;
    cur_term->_delay = -1;

    /* so that tgetch will reset it to be _inputd */
    /* cur_term->_check_fd = -2; */
}
