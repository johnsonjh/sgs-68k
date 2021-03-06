/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sesystem.c: version 25.1 created on 12/2/91 at 19:33:38	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sesystem.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:sesystem.c	1.2"	*/
	
#ident	"@(#)libgen:sesystem.c	25.1"

/*	system(3S) with standard out redirected to standard error
*/

#include	<signal.h>
#include	"libgen.h"


int
sesystem(s)
char	*s;
{
	extern int fork(), execl(), wait();
	register int	pid, w;
	int	status;
	register int (*istat)(), (*qstat)();

	if((pid = fork()) == 0) {
		close(1);	/* redirect standard out to standard err */
		dup(2);
		(void) execl("/bin/sh", "sh", "-c", s, 0);
		_exit(127);
	}
	istat = signal(SIGINT, SIG_IGN);
	qstat = signal(SIGQUIT, SIG_IGN);
	while((w = wait(&status)) != pid && w != -1)
		;
	(void) signal(SIGINT, istat);
	(void) signal(SIGQUIT, qstat);
	return((w == -1)? w: status);
}
