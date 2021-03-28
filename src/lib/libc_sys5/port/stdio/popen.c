#ident	"@(#)popen.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:stdio/popen.c	1.13"	*/
/*	kd0 - 20 has been changed to _NFILE for number of open  */
/*		files (64)			 		*/

/*LINTLIBRARY*/
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#define	tst(a,b) (*mode == 'r'? (b) : (a))
#define	RDR	0
#define	WTR	1
extern int errno;
extern FILE *fdopen();
extern int execl(), fork(), pipe(), close(), fcntl();
#ifndef DYNAMIC
static int popen_pid[_NFILE];	/*kd0*/
#else /* DYNAMIC */
typedef struct pop_t {
	struct pop_t *next;
	struct pop_t *prev;
	int file;	/* file number */
	int popid;	/* process id of pipe */
} popen_t;

static popen_t *first_pipe = NULL;
#endif /* DYNAMIC */

static char bin_shell[] = "/bin/sh" ;
static char shell[] = "sh";
static char shflg[] = "-c";

FILE *
popen(cmd, mode)
char	*cmd, *mode;
{
	int	p[2];
#ifndef DYNAMIC
	register int *poptr;
#else /* DYNAMIC */
	register popen_t *poptr;
#endif /* DYNAMIC */
	register int myside, yourside, pid;

	if(pipe(p) < 0)
		return(NULL);
	myside = tst(p[WTR], p[RDR]);
	yourside = tst(p[RDR], p[WTR]);
	if((pid = fork()) == 0) {
		/* myside and yourside reverse roles in child */
		int	stdio;

		/* close all pipes from other popen's */
		/*kd0*/
#ifndef DYNAMIC
		for (poptr = popen_pid; poptr < popen_pid+_NFILE; poptr++) {
			if(*poptr)
				close(poptr - popen_pid);
		}
#else /* DYNAMIC */
		for ( ;first_pipe != NULL; first_pipe = first_pipe->next ) {
			close(first_pipe->file);
		}
#endif /* DYNAMIC */
		stdio = tst(0, 1);
		(void) close(myside);
		(void) close(stdio);
		(void) fcntl(yourside, F_DUPFD, stdio);
		(void) close(yourside);
		(void) execl(bin_shell, shell, shflg, cmd, (char *)0);
		_exit(1);
	}
	if(pid == -1)
		return(NULL);
#ifndef DYNAMIC
	popen_pid[myside] = pid;
#else /* DYNAMIC */
	if (( poptr = malloc(sizeof(popen_t))) < 0) {
		return(NULL);
	}
	poptr->popid = pid;
	poptr->file = myside;
	poptr->prev = NULL;
	if (first_pipe == NULL) {
		first_pipe = poptr;
		first_pipe->next = NULL;
	} else {
		poptr->next = first_pipe;
		first_pipe->prev = poptr;
		first_pipe = poptr;
	}
#endif /* DYNAMIC */
	(void) close(yourside);
	return(fdopen(myside, mode));
}

int
pclose(ptr)
FILE	*ptr;
{
	register int f, r;
	int status;
	void (*hstat)(), (*istat)(), (*qstat)();
#ifdef DYNAMIC
	popen_t	*poptr;
#endif /* DYNAMIC */

	f = fileno(ptr);
#ifdef DYNAMIC 
	for ( poptr = first_pipe; 
		poptr && poptr->file != f; poptr = poptr->next )
		;

	if ( poptr == NULL )
		return( NULL );

#endif /* DYNAMIC */
	(void) fclose(ptr);
	istat = signal(SIGINT, SIG_IGN);
	qstat = signal(SIGQUIT, SIG_IGN);
	hstat = signal(SIGHUP, SIG_IGN);

	/* while the child is not done and no error has *
	 * occured wait in the loop			*/

#ifndef DYNAMIC
	while((r = wait(&status)) != popen_pid[f] &&
#else /* DYNAMIC */
	while(((r = wait(&status)) != poptr->popid) &&
#endif /* DYNAMIC */
	((r != -1) || (errno == EINTR)))
		;
	if(r == -1)
		status = -1;
	(void) signal(SIGINT, istat);
	(void) signal(SIGQUIT, qstat);
	(void) signal(SIGHUP, hstat);
	/* mark this pipe closed */
#ifndef DYNAMIC
	popen_pid[f] = 0;
#else /* DYNAMIC */
	if (poptr == first_pipe) {
		if ( ( first_pipe = first_pipe->next ) != NULL ) {
			first_pipe->prev = NULL;
		}
	} else {
		if( ( poptr->prev->next = poptr->next ) != NULL ) {
			poptr->next->prev = poptr->prev;
		}
	}
	free(poptr);
#endif /* DYNAMIC */
	return(status);
}
