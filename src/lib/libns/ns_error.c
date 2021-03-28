/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ns_error.c: version 25.1 created on 12/2/91 at 19:42:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ns_error.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:ns_error.c	1.2"	*/
	
#ident	"@(#)libns:ns_error.c	25.1"

/*	nserror.c	*/

extern int ns_errno, ns_err, strlen(), write();
extern char *ns_errlist[];

void
nserror(s)
char	*s;
{
	register char *c;
	register int n;

	c = "Unknown error";
	if(ns_errno < ns_err)
		c = ns_errlist[ns_errno];
	n = strlen(s);
	if(n) {
		(void) write(2, s, (unsigned)n);
		(void) write(2, ": ", 2);
	}
	(void) write(2, c, (unsigned)strlen(c));
	(void) write(2, "\n", 1);
}
