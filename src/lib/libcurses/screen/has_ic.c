/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) has_ic.c: version 25.1 created on 12/2/91 at 19:20:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)has_ic.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/has_ic.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:has_ic.c	25.1"


#include "curses_inc.h"

/* Query: Does it have insert/delete char? */

has_ic()
{
    return ((insert_character || enter_insert_mode || parm_ich) &&
		(delete_character || parm_dch));
}
