#ident	"@(#)system.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <signal.h>
#include <stdio.h>
#include <syscall.h>

static char bin_shell[] = "/bin/sh";
static char shell[] = "sh";
static char shflg[]= "-c";

int system( const char *s )
{
	int	status;
	void (*istat)(), (*qstat)();

#ifdef _POSIX_SOURCE
	pid_t pid, w;
#else			/* assume UNIX */
	int   pid, w;
#endif /* _POSIX_SOURCE */

	if ( s == NULL ) {		/* only query */
		register FILE	*fd;
		int intcl;

		/* if no command processor, return zero => unsuccessful */
		if ( (fd = fopen ( bin_shell, "r" )) == NULL ) return ( 0 );
		intcl = fclose ( fd );
		return ( 1 );		/* there is a command processor */
	}

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
	return((w == -1)? (int) w : status);
}
