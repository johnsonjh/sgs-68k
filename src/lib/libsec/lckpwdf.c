/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) lckpwdf.c: version 25.1 created on 12/2/91 at 20:14:26	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)lckpwdf.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>

#define LOCKFILE	"/etc/.pwd.lock"
#define S_WAITTIME	15

struct flock pflock =	{
			0,	/* l_type */
			0,	/* l_whence */
			0,	/* l_start */
			0,	/* l_len */
			0,	/* l_sysid */
			0	/* l_pid */
			} ;

/*	lckpwdf() returns a 0 for a successful lock within W_WAITTIME
	and -1 otherwise
*/

static int fildes = -1 ;
extern int fcntl() ;
extern void (*sigset ())() ;
extern unsigned alarm() ;

int
lckpwdf()
{
	extern void palmhdlr() ;
	int retval ;
	if ( (fildes = creat (LOCKFILE, 0600)) == -1 )
		return (-1) ;
	else
		{
		pflock.l_type = F_WRLCK ;
		(void) sigset (SIGALRM, palmhdlr) ;
		(void) alarm (S_WAITTIME) ;
		retval = fcntl (fildes, F_SETLKW, &pflock) ; 
		(void) alarm (0) ;
		(void) sigset (SIGALRM, SIG_DFL) ;
		return ( retval ) ;
		}
}

/* 	ulckpwdf() returns 0 for a successful unlock and -1 otherwise
*/
int
ulckpwdf()
{
	if ( fildes == -1 ) 
		return (-1) ;
	else	{
		pflock.l_type = F_UNLCK ;
		(void) fcntl (fildes, F_SETLK, &pflock) ;
		(void) close (fildes) ;
		fildes = -1 ;
		return (0) ;
		}
}	
	
void
palmhdlr()
{
}
