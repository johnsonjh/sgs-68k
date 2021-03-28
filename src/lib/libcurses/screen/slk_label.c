/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) slk_label.c: version 25.1 created on 12/2/91 at 19:25:56	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)slk_label.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/slk_label.c	1.2"	*/
	
#ident	"@(#)lib/libcurses:slk_label.c	25.1"

#include	"curses_inc.h"

/* Return the current label of key number 'n'. */

char *
slk_label(n)
int	n;
{
    register	SLK_MAP	*slk = SP->slk;

    /* strip initial blanks */
    /* for (; *lab != '\0'; ++lab)
	if(*lab != ' ')
	    break; */
    /* strip trailing blanks */
    /* for (; cp > lab; --cp)
	if (*(cp-1) != ' ')
	    break; */


    return (!slk || n < 1 || n > slk->_num) ? NULL : slk->_lval[n - 1];
}
