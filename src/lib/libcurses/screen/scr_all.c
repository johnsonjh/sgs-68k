/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scr_all.c: version 25.1 created on 12/2/91 at 19:24:52	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scr_all.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/scr_all.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:scr_all.c	25.1"

#include	"curses_inc.h"

/* Set <screen> idea of the screen image to that stored in "file". */

_scr_all(file,which)
char	*file;
int	which;
{
    int		rv;
    FILE	*filep;

    if ((filep = fopen(file,"r")) == NULL)
	return (ERR);
    rv = scr_reset(filep,which);
    fclose(filep);
    return (rv);
}
