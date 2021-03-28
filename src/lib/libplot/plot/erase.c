/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) erase.c: version 25.1 created on 12/2/91 at 20:08:52	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)erase.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT/plot:#ident	"libplot:plot/erase.c	1.2"	*/
	
#ident	"@(#)libplot/plot:erase.c	25.1"

#include <stdio.h>
erase(){
	putc('e',stdout);
}
