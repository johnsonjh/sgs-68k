/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) V3.initscr.c: version 25.1 created on 12/2/91 at 19:10:35	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)V3.initscr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/V3.initscr.c	1.1"	*/
	
#ident	"@(#)lib/libcurses:V3.initscr.c	25.1"


#include	"curses_inc.h"
extern	int	_outchar();

#ifdef	_VR3_COMPAT_CODE
extern	void	_update_old_y_area();

#undef	initscr
WINDOW	*
initscr()
{
    _y16update = _update_old_y_area;
    return (initscr32());
}
#endif /* _VR3_COMPAT_CODE */
