/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) error.c: version 25.1 created on 12/2/91 at 17:09:27	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)error.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
/*	ATT: #ident	"sccs:lib/cassi/error.c	6.2"		*/

#ident	"@(#)sccs:error.c	25.1"

#include <stdio.h>
error(dummy)	
	char *dummy;
	{
	 printf("%s\n",dummy);
	}
