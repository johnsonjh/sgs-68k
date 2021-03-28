/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) logname.c: version 25.1 created on 12/2/91 at 17:11:58	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)logname.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	AT&T: #ident	"sccs:lib/mpwlib/logname.c	6.2"	*/

#ident	"@(#)sccs:logname.c	25.1"

char *
logname()
{
	return((char *)getenv("LOGNAME"));
}
