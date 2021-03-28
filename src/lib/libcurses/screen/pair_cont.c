/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) pair_cont.c: version 25.1 created on 12/2/91 at 19:23:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)pair_cont.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)curses:pair_cont.c	1.3"

#include "curses_inc.h"

pair_content(pair, f, b)
short pair, *f, *b;
{
    register _Color_pair *ptp;

    if (pair < 1 || pair >= COLOR_PAIRS)
	return (ERR);

    ptp = SP->_pairs_tbl + pair;

    if (!ptp->init)
        return (ERR);

    *f = ptp->foreground;
    *b = ptp->background;
    return (OK);
}
