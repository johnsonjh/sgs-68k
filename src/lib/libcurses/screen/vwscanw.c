/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) vwscanw.c: version 25.1 created on 12/2/91 at 19:28:35	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)vwscanw.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/vwscanw.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:vwscanw.c	25.1"

/*
 * scanw and friends
 *
 */

# include	"curses_inc.h"
# include	<varargs.h>

/*
 *	This routine actually executes the scanf from the window.
 *
 *	This code calls vsscanf, which is like sscanf except
 * 	that it takes a va_list as an argument pointer instead
 *	of the argument list itself.  We provide one until
 *	such a routine becomes available.
 */

/*VARARGS2*/
vwscanw(win, fmt, ap)
WINDOW	*win;
char *fmt;
va_list	ap;
{
	char	buf[256];
	register int n;

	if (wgetstr(win, buf) == ERR)
		n = ERR;
	else
		n = vsscanf(buf, fmt, ap);
	va_end(ap);
	return n;
}
