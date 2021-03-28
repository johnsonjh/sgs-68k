/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) t_error.c: version 25.1 created on 12/2/91 at 20:17:42	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)t_error.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libnsl:nsl/t_error.c	1.2"	*/
	
#ident	"@(#)nsl:t_error.c	25.1"

#include "sys/stropts.h"
#include "sys/tiuser.h"
#include "sys/errno.h"
#include "_import.h"

extern int t_errno, strlen(), write();
extern  char *t_errlist[];
extern t_nerr;
extern void perror();

void
t_error(s)
char	*s;
{
	register char *c;
	register int n;

	c = "Unknown error";
	if(t_errno <= t_nerr)
		c = t_errlist[t_errno];
	n = strlen(s);
	if(n) {
		(void) write(2, s, (unsigned)n);
		(void) write(2, ": ", 2);
	}
	(void) write(2, c, (unsigned)strlen(c));
	if (t_errno == TSYSERR) {
		(void) write(2, ": ", 2);
		perror("");
	} else
		(void) write(2, "\n", 1);
}
