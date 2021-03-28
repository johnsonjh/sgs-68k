#ident	"@(#)filemacros.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1989 ARIX	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ARIX:#ident	"libc-port:gen/filemacros.c	1.1"	*/
	

#ifdef	_POSIX_SOURCE	/* These functions are only included in POSIX libc's */

#include <stdio.h>

/* feof, fileno,  & ferror are required to be functions by POSIX 1003.1 
   & x/open, mr */


/* Returns file # of a stream */
int
fileno (stream_fd)
FILE	*stream_fd;
{
	return ((int)((stream_fd)->_file));
}

#endif	/* _POSIX_SOURCE */
