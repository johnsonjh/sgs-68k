/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) lcksec.c: version 25.1 created on 12/2/91 at 20:14:29	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)lcksec.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>

#define S_WAITTIME	15

struct file_lock {
	char file_name[25];
	char lock_name[30];
};

struct file_lock file_locks[] = {"/etc/passwd","/etc/pwd.lock",
		  "/etc/security/passwd","/etc/security/shadow.lock",
		  "/etc/security/user_auth","/etc/security/user.lock",
		  "/etc/security/tty_auth","/etc/security/tty.lock",
		  "/etc/security/group","/etc/security/sgroup.lock",
		  "/etc/group","/etc/group.lock",
		  "","",
};

struct flock flock =	{
			0,	/* l_type */
			0,	/* l_whence */
			0,	/* l_start */
			0,	/* l_len */
			0,	/* l_sysid */
			0	/* l_pid */
			} ;

/*	lcksec() returns a valid file descriptor for a successful lock 
	within W_WAITTIME and -1 otherwise
*/

extern int fcntl() ;
extern void (*signal ())() ;
extern unsigned alarm() ;



int
lcksec(filename)
char *filename;
{
	extern void secahdlr() ;
	int retval ;
	char *LOCKFILE, *getptr();
	int fildes = -1 ;

	if((LOCKFILE = getptr(filename)) == NULL)
		return(-1);

	if ( (fildes = creat (LOCKFILE, 0600)) == -1 )
		return (-1) ;
	else
		{
		flock.l_type = F_WRLCK ;
		(void) signal (SIGALRM, secahdlr) ;
		(void) alarm (S_WAITTIME) ;
		retval = fcntl (fildes, F_SETLKW, &flock) ; 
		(void) alarm (0) ;
		(void) signal (SIGALRM, SIG_DFL) ;
		return ( (retval >= 0) ? fildes : retval ) ;
		}
}

/* 	ulcksec() returns 0 for a successful unlock and -1 otherwise
*/
int
ulcksec(fildes)
int fildes;
{
	if ( fildes == -1 ) 
		return (-1) ;
	else	{
		flock.l_type = F_UNLCK ;
		(void) fcntl (fildes, F_SETLK, &flock) ;
		(void) close (fildes) ;
		return (0) ;
		}
}	
	
void
secahdlr()
{
}

char *
getptr(filename)
char *filename;
{
	register struct file_lock *flp;

	flp = file_locks;

	while( *(flp->file_name) != NULL ) {

	 	if( strcmp(flp->file_name,filename ) == 0 )
			return( flp->lock_name );
		flp++;

	}

	return(NULL);

}
