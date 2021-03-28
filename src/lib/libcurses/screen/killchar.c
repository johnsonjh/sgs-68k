/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) killchar.c: version 25.1 created on 12/2/91 at 19:21:08	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)killchar.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/killchar.c	1.1.1.4"	*/
	
#ident	"@(#)lib/libcurses:killchar.c	25.1"

/*
 * Routines to deal with setting and resetting modes in the tty driver.
 * See also setupterm.c in the termlib part.
 */
#include "curses_inc.h"

char
killchar()
{
#ifdef SYSV
    return (SHELLTTY.c_cc[VKILL]);
#else
    return (SHELLTTY.sg_kill);
#endif
}
