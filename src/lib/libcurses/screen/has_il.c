/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) has_il.c: version 25.1 created on 12/2/91 at 19:20:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)has_il.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/has_il.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:has_il.c	25.1"


#include "curses_inc.h"

/* Query: does the terminal have insert/delete line? */

has_il()
{
    return (((insert_line || parm_insert_line) && 
	(delete_line || parm_delete_line)) || change_scroll_region);
}
