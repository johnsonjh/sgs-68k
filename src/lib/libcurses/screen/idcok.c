/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) idcok.c: version 25.1 created on 12/2/91 at 19:20:14	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)idcok.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/idcok.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:idcok.c	25.1"

#include	"curses_inc.h"

void
idcok(win, bf)
WINDOW	*win;
int	bf;
{
    win->_use_idc = (bf) ? TRUE : FALSE;
}
