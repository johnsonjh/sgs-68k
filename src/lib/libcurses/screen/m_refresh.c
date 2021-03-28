/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) m_refresh.c: version 25.1 created on 12/2/91 at 19:21:33	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)m_refresh.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/V3.m_refresh.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:m_refresh.c	25.1"


#include	"curses_inc.h"
extern	int	_outchar();

#ifdef	_VR3_COMPAT_CODE
m_refresh()
{
    return (wrefresh(stdscr));
}
#endif	/* _VR3_COMPAT_CODE */
