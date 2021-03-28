#ident	"@(#)rewinddir.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1989 ARIX	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/rewinddir.c	1.4"	*/
	

/* rewinddir -- C library extension routine, removed as a macro
		     from <dirent.h> for POSIX 1003.1 conformance, mer */

#include	<sys/types.h>
#include	<dirent.h>

void
rewinddir (dirp)
register DIR	*dirp;		/* stream from opendir() */
{
	seekdir (dirp, 0L);
}
