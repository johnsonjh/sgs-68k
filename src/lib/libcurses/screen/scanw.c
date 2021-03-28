/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scanw.c: version 25.1 created on 12/2/91 at 19:24:49	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scanw.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/scanw.c	1.7"	*/
	
#ident	"@(#)lib/libcurses:scanw.c	25.1"

/*
 * scanw and friends
 *
 */

# include	"curses_inc.h"
# include	<varargs.h>

/*
 *	This routine implements a scanf on the standard screen.
 */
/*VARARGS1*/
scanw(va_alist)
va_dcl
{
	register char	*fmt;
	va_list	ap;

	va_start(ap);
	fmt = va_arg(ap, char *);
	return vwscanw(stdscr, fmt, ap);
}
