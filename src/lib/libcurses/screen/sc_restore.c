/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sc_restore.c: version 25.1 created on 12/2/91 at 19:24:45	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sc_restore.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)curses:screen/_scr_restore.c	1.1"

#define	NOMACROS

#include	"curses_inc.h"

scr_restore(file)
char	*file;
{
    return (_scr_all(file, 1));
}
