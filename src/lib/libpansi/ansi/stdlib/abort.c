#ident	"@(#)abort.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/abort.c	1.12"		*/
/*	3.0 SID #	1.4	*/

#include <signal.h>
#include <syscall.h>
/* #include <sys/types.h> included in syscall.h */
#include <_stdlib.h>

static pass = 0;		/* counts how many times abort has been called*/

/* POSIX 1003.1 compliance, now returns void instead of int, also no longer
   returns, mr0 */

extern void abort ( void )
{
	void (*sig)();

	if ((sig = signal(SIGABRT,SIG_DFL)) != SIG_DFL) 
		(void) signal(SIGABRT,sig); 
	else if (++pass == 1)
		_cleanup();

	kill(getpid(), SIGABRT);
}
