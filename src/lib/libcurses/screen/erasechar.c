/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) erasechar.c: version 25.1 created on 12/2/91 at 19:19:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)erasechar.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/erasechar.c	1.1.1.4"	*/
	
#ident	"@(#)lib/libcurses:erasechar.c	25.1"

/*
 * Routines to deal with setting and resetting modes in the tty driver.
 * See also setupterm.c in the termlib part.
 */
#include "curses_inc.h"

char
erasechar()
{
#ifdef SYSV
    return (SHELLTTY.c_cc[VERASE]);
#else
    return (SHELLTTY.sg_erase);
#endif
}
