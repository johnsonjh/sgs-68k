/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) p2open.c: version 25.1 created on 12/2/91 at 19:33:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)p2open.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:p2open.c	1.5"	*/
	
#ident	"@(#)libgen:p2open.c	25.1"

/*
	Similar to popen(3S) but with pipe to cmd's stdin and from stdout.
*/

/*LINTLIBRARY*/
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>

extern FILE	*fdopen();
extern int	close(),
		execl(),
		fcntl(),
		fork(),
		pipe();
static int	popen_pid[_NFILE]; /* sw0 */


p2open(cmd, fpin, fpout)
char	*cmd;
FILE	**fpin;	/* file pointer to cmd stdin */
FILE	**fpout;	/* file pointer from cmd stdout */
{
	int		tocmd[2];
	int		fromcmd[2];
	register int	pid;

	if( pipe(tocmd) < 0  ||  pipe(fromcmd) < 0 )
		return  0;
	if( (pid = fork()) == 0 ) {
		(void) close( tocmd[1] );
		(void) close( 0 );
		(void) fcntl( tocmd[0], F_DUPFD, 0 );
		(void) close( tocmd[0] );
		(void) close( fromcmd[0] );
		(void) close( 1 );
		(void) fcntl( fromcmd[1], F_DUPFD, 1 );
		(void) close( fromcmd[1] );
		(void) execl("/bin/sh", "sh", "-c", cmd, 0);
		_exit(1);
	}
	if(pid == -1)
		return  0;
	popen_pid[ tocmd[1] ] = pid;
	popen_pid[ fromcmd[0] ] = pid;
	(void) close( tocmd[0] );
	(void) close( fromcmd[1] );
	*fpin = fdopen( tocmd[1], "w" );
	*fpout = fdopen( fromcmd[0], "r" );
	return  pid;
}

int
p2close( fp1, fp2 )
FILE	*fp1;
FILE	*fp2;
{
	register int	pid,
			r;
	int		status,
			(*hstat)(),
			(*istat)(),
			(*qstat)();

	pid = popen_pid[ fileno(fp1) ];
	if( pid != popen_pid[ fileno(fp2) ] ) {
		fprintf( stderr,
			"p2close(fp1, fp2):  fp1 not from same p2open() as fp2\n" );
		exit( 1 );
	}
	(void) fclose(fp1);
	(void) fclose(fp2);
	istat = signal(SIGINT, SIG_IGN);
	qstat = signal(SIGQUIT, SIG_IGN);
	hstat = signal(SIGHUP, SIG_IGN);
	while( (r = wait(&status)) != pid  &&  r != -1 )
		;
	if( r == -1 )
		status = -1;
	(void) signal(SIGINT, istat);
	(void) signal(SIGQUIT, qstat);
	(void) signal(SIGHUP, hstat);
	return  status;
}
