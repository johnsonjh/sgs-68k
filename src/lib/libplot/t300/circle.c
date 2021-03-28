/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) circle.c: version 25.1 created on 12/2/91 at 20:09:33	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)circle.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT/t300:#ident	"libplot:t300/circle.c	1.3"	*/
	
#ident	"@(#)libplot/t300:circle.c	25.1"

circle(x,y,r){
	arc(x,y,x+r,y,x+r,y);
}
