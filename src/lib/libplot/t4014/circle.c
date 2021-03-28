/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) circle.c: version 25.1 created on 12/2/91 at 20:11:20	*/
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

/*	ATT/t4014:#ident	"libplot:t4014/circle.c	1.2"	*/
	
#ident	"@(#)libplot/t4014:circle.c	25.1"

circle(x,y,r){
	arc(x,y,x+r,y,x+r,y);
}
