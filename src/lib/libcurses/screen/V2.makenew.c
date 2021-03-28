/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) V2.makenew.c: version 25.1 created on 12/2/91 at 19:10:28	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)V2.makenew.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/V2.makenew.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:V2.makenew.c	25.1"


#include "curses_inc.h"

#ifdef _VR2_COMPAT_CODE
extern WINDOW *_makenew();

WINDOW *
makenew(num_lines, num_cols, begy, begx)
int	num_lines, num_cols, begy, begx;
{
	return _makenew(num_lines, num_cols, begy, begx);
}
#endif
