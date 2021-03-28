/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tgoto.c: version 25.1 created on 12/2/91 at 19:26:42	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tgoto.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/tgoto.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:tgoto.c	25.1"

/*
 * tgoto: function included only for upward compatibility with old termcap
 * library.  Assumes exactly two parameters in the wrong order.
 */
extern	char	*tparm();

char	*
tgoto(cap, col, row)
char	*cap;
int	col, row;
{
    char	*cp;

    cp = tparm(cap, row, col);
    return (cp);
}
