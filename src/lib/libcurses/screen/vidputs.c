/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) vidputs.c: version 25.1 created on 12/2/91 at 19:28:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)vidputs.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/vidputs.c	1.9"	*/
	
#ident	"@(#)lib/libcurses:vidputs.c	25.1"

#include	"curses_inc.h"

vidputs(a,b)
chtype	a;
int	(*b)();
{
    vidupdate(a,cur_term->sgr_mode,b);
    return (OK);
}
