/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) upd_old_y.c: version 25.1 created on 12/2/91 at 19:28:14	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)upd_old_y.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/V3.upd_old_y.c	1.2"	*/
	
#ident	"@(#)lib/libcurses:upd_old_y.c	25.1"


#include	"curses_inc.h"
extern	int	_outchar();

#ifdef	_VR3_COMPAT_CODE
void
_update_old_y_area(win, nlines, ncols, start_line, start_col)
WINDOW	*win;
int	nlines, ncols, start_line, start_col;
{
    int	row, col, num_cols;

    for (row = start_line; nlines > 0; nlines--, row++)
	for (num_cols = ncols, col = start_col; num_cols > 0; num_cols--, col++)
	    win->_y16[row][col] = _TO_OCHTYPE(win->_y[row][col]);
}
#endif	/* _VR3_COMPAT_CODE */
