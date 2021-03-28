/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ttimeout.c: version 25.1 created on 12/2/91 at 19:27:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ttimeout.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/ttimeout.c	1.2"	*/
	
#ident	"@(#)lib/libcurses:ttimeout.c	25.1"

#include	"curses_inc.h"
#include	<fcntl.h>

/*
 * Set the current time-out period for getting input.
 *
 * delay:	< 0 for infinite delay (blocking read)
 * 		= 0 for no delay read
 * 		> 0 to specify a period in millisecs to wait
 * 		    for input, then return '\0' if none comes
 */

void	_setblock(), _settimeout();

ttimeout(delay)
int	delay;
{
    if (cur_term->_inputfd < 0)
	return (ERR);

    if (delay < 0)
	delay = -1;

    if (cur_term->_delay != delay)
    {
	_settimeout(delay);
	cur_term->_delay = delay;
    }
    return (OK);
}

/*
 * set the terminal to timeout in t milliseconds,
 * rounded up to the nearest 10th of a second.
 */
static	void
_settimeout(num)
int	num;
{
    if (num >= 0)
    {
	PROGTTY.c_lflag &= ~ICANON;
	PROGTTY.c_cc[VMIN] = 0;
	PROGTTY.c_cc[VTIME] = (num > 25500) ? 255 : (num + 99) / 100;
	if ( num )
		cur_term->_fl_rawmode = 3;
    }
    else
    {
	PROGTTY.c_lflag |= ICANON;
	PROGTTY.c_cc[VMIN] = 1;
	PROGTTY.c_cc[VTIME] = 0;
	cur_term->_fl_rawmode = 1;
    }
    reset_prog_mode();
}
