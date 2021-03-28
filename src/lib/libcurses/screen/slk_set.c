/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) slk_set.c: version 25.1 created on 12/2/91 at 19:25:59	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)slk_set.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/slk_set.c	1.4"	*/
	
#ident	"@(#)lib/libcurses:slk_set.c	25.1"

#include	"curses_inc.h"

/*
 * Set a soft label.
 *
 * n:	label number
 * lab:	the string
 * f:	0, 1, 2 for left, center, right-justification
 */

slk_set(n, lab, f)
int	n;
char	*lab;
int	f;
{
    register	SLK_MAP	*slk = SP->slk;
    register	int	len, slklen = slk->_len, left;
    char		*cp, nlab[LABLEN + 1];

    if ((slk == NULL) || f < 0 || f > 2 || n < 1 || n > slk->_num)
	return (ERR);

    /* 0-indexing internally */
    n--;

    if (lab == NULL)
	lab = "";

    /* chop lengthy label */
    if ((len = strlen(lab)) > slklen)
	lab[len = slklen] = '\0';

    /* make the new display label */
    for (cp = nlab + slklen - 1; cp >= nlab; cp--)
	*cp = ' ';
    nlab[slklen] = '\0';
    if (f == 0)
	left = 0;
    else
	left = (slklen - len) / ((f == 1) ? 2 : 1);

    (void) memcpy(nlab + left, lab, len);

    if (strcmp(slk->_ldis[n], nlab) != 0)
    {
	(void) memcpy(slk->_lval[n], lab, len + 1);
	(void) memcpy(slk->_ldis[n], nlab, slklen + 1);
	slk->_changed = slk->_lch[n] = TRUE;
    }

    return (OK);
}
