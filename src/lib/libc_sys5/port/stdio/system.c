#ident	"@(#)system.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"libc-port:stdio/system.c	1.9"	*/
/*	3.0 SID #	1.4	*/


/*LINTLIBRARY*/
#include <signal.h>

static char bin_shell[] = "/bin/sh";
static char shell[] = "sh";
static char shflg[]= "-c";

extern int fork(), execl(), wait();

int
system(s)
char	*s;
{
	int	status, pid, w;
	void (*istat)(), (*qstat)();

	if((pid = fork()) == 0) {
		(void) execl(bin_shell, shell, shflg, s, (char *)0);
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
