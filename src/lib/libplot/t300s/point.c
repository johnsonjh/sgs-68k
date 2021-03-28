/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) point.c: version 25.1 created on 12/2/91 at 20:10:57	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)point.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT/t300s:#ident	"libplot:t300s/point.c	1.2"	*/
	
#ident	"@(#)libplot/t300s:point.c	25.1"

#include "con.h"
point(xi,yi){
		move(xi,yi);
		label(".");
		return;
}
