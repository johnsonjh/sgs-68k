/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ungetch.c: version 25.1 created on 12/2/91 at 19:28:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ungetch.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/ungetch.c	1.1.1.4"	*/
	
#ident	"@(#)lib/libcurses:ungetch.c	25.1"

#include "curses_inc.h"

/* Place a char onto the beginning of the input queue. */

ungetch(ch)
int	ch;
{
    register	int	i = cur_term->_chars_on_queue, j = i - 1;
    register	short	*inputQ = cur_term->_input_queue;

    /* Place the character at the beg of the Q */

    while (i > 0)
	inputQ[i--] = inputQ[j--];
    cur_term->_ungotten++;
    inputQ[0] = -ch - 0100;
    cur_term->_chars_on_queue++;
}
