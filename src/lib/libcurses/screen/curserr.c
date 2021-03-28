/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) curserr.c: version 25.1 created on 12/2/91 at 19:18:00	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)curserr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/curserr.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:curserr.c	25.1"


#include 	"curses_inc.h"

char	*curs_err_strings[] =
{
    "I don't know how to deal with your \"%s\" terminal",
    "I need to know a more specific terminal type than \"%s\"",	/* unknown */
#ifdef	DEBUG
    "malloc returned NULL in function \"%s\"",
#else	/* DEBUG */
    "malloc returned NULL",
#endif	/* DEBUG */
};

void
curserr()
{
    fprintf(stderr, "Sorry, ");
    fprintf(stderr, curs_err_strings[curs_errno], curs_parm_err);
    fprintf(stderr, ".\r\n");
}
