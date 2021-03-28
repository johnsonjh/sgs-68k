/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) erase.c: version 25.1 created on 12/2/91 at 20:13:19	*/
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

/*	ATT/vt0:#ident	"libplot:vt0/erase.c	1.2"	*/
	
#ident	"@(#)libplot/vt0:erase.c	25.1"

extern vti;
erase(){
	int i;
	i=0401;
	write(vti,&i,2);
}
