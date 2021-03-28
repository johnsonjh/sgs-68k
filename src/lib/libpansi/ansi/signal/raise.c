#ident	"@(#)raise.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <syscall.h>
#include <signal.h>
#include <errno.h>

int raise(int sig)
{
  int pid;

  pid = getpid();
  if (sig == SIGKILL && pid == 1) { errno = EINVAL; return -1;}
  return kill(pid,sig);
}
