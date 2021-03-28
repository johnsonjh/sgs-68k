/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) longname.c: version 25.1 created on 12/2/91 at 19:21:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)longname.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/longname.c	1.4.1.4"	*/
	
#ident	"@(#)lib/libcurses:longname.c	25.1"


/* This routine returns the long name of the terminal. */

char *
longname()
{
    extern	char	ttytype[], *strrchr();
    register	char	*cp = strrchr(ttytype, '|');

    if (cp)
	return (++cp);
    else
	return (ttytype);
}
