/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) setcurterm.c: version 25.1 created on 12/2/91 at 19:25:09	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)setcurterm.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/setcurterm.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:setcurterm.c	25.1"

#include "curses_inc.h"

/*
 * Establish the terminal that the #defines in term.h refer to.
 */

TERMINAL *
setcurterm(newterminal)
register TERMINAL *newterminal;
{
    register	TERMINAL	*oldterminal = cur_term;

    if (newterminal)
    {
#ifdef	_VR3_COMPAT_CODE
	acs_map = cur_term->_acs32map;
#else	/* _VR3_COMPAT_CODE */
	acs_map = cur_term->_acsmap;
#endif	/* _VR3_COMPAT_CODE */
	cur_bools = newterminal->_bools;
	cur_nums = newterminal->_nums;
	cur_strs = newterminal->_strs;
	cur_term = newterminal;
    }
    return (oldterminal);
}
