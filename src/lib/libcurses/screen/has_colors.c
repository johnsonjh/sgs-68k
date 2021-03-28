/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) has_colors.c: version 25.1 created on 12/2/91 at 19:20:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)has_colors.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)curses:has_colors.c	1.1"

#include "curses_inc.h"

bool has_colors()
{
    return ((max_pairs == -1) ? FALSE : TRUE);
}
