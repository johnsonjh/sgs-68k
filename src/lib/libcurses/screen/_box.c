/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _box.c: version 25.1 created on 12/2/91 at 19:11:55	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_box.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)curses:screen/_box.c	1.2"

#define		NOMACROS
#include	"curses_inc.h"

box(win, v, h)
WINDOW	*win;
chtype	v, h;
{
    return (wborder(win, v, v, h, h,
			 (chtype) 0, (chtype) 0, (chtype) 0, (chtype) 0));
}
