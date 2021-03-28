/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) pnoutref.c: version 25.1 created on 12/2/91 at 19:23:29	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)pnoutref.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/pnoutref.c	1.11"	*/
	
#ident	"@(#)lib/libcurses:pnoutref.c	25.1"

#include	"curses_inc.h"

/* wnoutrefresh for pads. */

pnoutrefresh(pad, pby, pbx, sby, sbx, sey, sex)
WINDOW	*pad;
int	pby, pbx, sby, sbx, sey, sex;
{
    extern	int	wnoutrefresh();

    return (_prefresh(wnoutrefresh, pad, pby, pbx, sby, sbx, sey, sex));
}
