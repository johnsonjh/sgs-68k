#ident	"@(#)closedir.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/closedir.c	1.4"	*/
	

/*
	closedir -- C library extension routine

*/

#include	<sys/types.h>
#include	<dirent.h>
#include 	<errno.h>


extern void	free();
extern int	close();

int
closedir( dirp )
register DIR	*dirp;		/* stream from opendir() */
{
register int rval;

	free( dirp->dd_buf );
	free( (char *)dirp );

	/* JTOF - fix for NIST test suite */
	if ( dirp->dd_fd ) {
		rval = close( dirp->dd_fd );
		dirp->dd_fd = -1; 
		return rval;
	}
	else {
		errno = EBADF;
		return -1;
	}
}
