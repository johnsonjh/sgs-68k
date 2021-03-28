/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) overlap.c: version 25.1 created on 12/2/91 at 19:23:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)overlap.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/overlap.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:overlap.c	25.1"

#include	"curses_inc.h"

/*
 * This routine writes Srcwin on Dstwin.
 * Only the overlapping region is copied.
 */

_overlap(Srcwin, Dstwin, Overlay)
register	WINDOW	*Srcwin, *Dstwin;
int			Overlay;
{
    register	int	sby, sbx, sey, sex, dby, dbx, dey, dex,
			top, bottom, left, right;

#ifdef	DEBUG
    if (outf)
	fprintf(outf, "OVERWRITE(0%o, 0%o);\n", Srcwin, Dstwin);
#endif	/* DEBUG */

    sby = Srcwin->_begy;	dby = Dstwin->_begy;
    sbx = Srcwin->_begx;	dbx = Dstwin->_begx;
    sey = sby + Srcwin->_maxy;	dey = dby + Dstwin->_maxy;
    sex = sbx + Srcwin->_maxx;	dex = dbx + Dstwin->_maxx;

    if (sey < dby || sby > dey || sex < dbx || sbx > dex)
	return (ERR);

    top = _MAX(sby, dby);	bottom = _MIN(sey, dey);
    left = _MAX(sbx, dbx);	right = _MIN(sex, dex);

    sby = top - sby;		sbx = left - sbx;
    dey = bottom - dby - 1;	dex = right - dbx - 1;
    dby = top - dby;		dbx = left - dbx;

    return (copywin(Srcwin, Dstwin, sby, sbx, dby, dbx, dey, dex, Overlay));
}
