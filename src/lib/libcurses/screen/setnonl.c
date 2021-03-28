/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) setnonl.c: version 25.1 created on 12/2/91 at 19:25:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)setnonl.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/setnonl.c	1.2"	*/
	
#ident	"@(#)lib/libcurses:setnonl.c	25.1"

#include	"curses_inc.h"

_setnonl(bf)
int	bf;
{
    SP->fl_nonl = bf;
    return (OK);
}
