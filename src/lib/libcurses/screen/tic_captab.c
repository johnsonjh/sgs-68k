/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tic_captab.c: version 25.1 created on 12/2/91 at 19:26:45	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tic_captab.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/tic_captab.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:tic_captab.c	25.1"

/*
 *	comp_captab.c -- The names of the capabilities in a form ready for
 *		         the making of a hash table for the compiler.
 *
 */


#include "curses_inc.h"
#include "compiler.h"


struct name_table_entry	cap_table[512];

struct name_table_entry *cap_hash_table[360];

int	Hashtabsize = 360;
int	Captabsize = 0;
int	BoolCount = 0;
int	NumCount = 0;
int	StrCount = 0;
