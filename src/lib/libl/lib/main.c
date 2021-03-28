/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) main.c: version 25.1 created on 12/2/91 at 19:37:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)main.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libl:lib/main.c	1.4"	*/
	
#ident	"@(#)libl:main.c	25.1"

# include "stdio.h"
main(){
yylex();
exit(0);
}
