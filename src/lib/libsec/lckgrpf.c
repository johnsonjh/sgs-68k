/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) lckgrpf.c: version 25.1 created on 12/2/91 at 20:14:23	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)lckgrpf.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>

#define LOCKFILE	"/etc/.grp.lock"
#define S_WAITTIME	15

struct flock gflock =	{
			0,	/* l_type */
			0,	/* l_whence */
			0,	/* l_start */
			0,	/* l_len */
			0,	/* l_sysid */
			0	/* l_pid */
			} ;

/*	lckgrpf() returns a 0 for a successful lock within W_WAITTIME
	and -1 otherwise
*/

static int fildes = -1 ;
extern int fcntl() ;
extern void (*sigset ())() ;
extern unsigned alarm() ;

int
lckgrpf()
{
	extern void galmhdlr() ;
	int retval ;
	if ( (fildes = creat (LOCKFILE, 0600)) == -1 )
		return (-1) ;
	else
		{
		gflock.l_type = F_WRLCK ;
		(void) sigset (SIGALRM, galmhdlr) ;
		(void) alarm (S_WAITTIME) ;
		retval = fcntl (fildes, F_SETLKW, &gflock) ; 
		(void) alarm (0) ;
		(void) sigset (SIGALRM, SIG_DFL) ;
		return ( retval ) ;
		}
}

/* 	ulckgrpf() returns 0 for a successful unlock and -1 otherwise
*/
int
ulckgrpf()
{
	if ( fildes == -1 ) 
		return (-1) ;
	else	{
		gflock.l_type = F_UNLCK ;
		(void) fcntl (fildes, F_SETLK, &gflock) ;
		(void) close (fildes) ;
		fildes = -1 ;
		return (0) ;
		}
}	
	
void
galmhdlr()
{
}
