/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scr_dump.c: version 25.1 created on 12/2/91 at 19:24:55	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scr_dump.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/scr_dump.c	1.7"	*/
	
#ident	"@(#)lib/libcurses:scr_dump.c	25.1"

#include	"curses_inc.h"

/*
 * Dump a screen image to a file. This routine and scr_reset
 * can be used to communicate the screen image across processes.
 */

scr_dump(file)
char	*file;
{
    int		rv;
    FILE	*filep;

    if ((filep = fopen(file,"w")) == NULL)
    {
#ifdef	DEBUG
	if (outf)
	    (void) fprintf (outf, "scr_dump: cannot open \"%s\".\n", file);
#endif	/* DEBUG */
	return (ERR);
    }
    rv = scr_ll_dump(filep);
    fclose(filep);
    return (rv);
}
