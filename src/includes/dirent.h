/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"head:dirent.h	1.1"	*/
	

#ifndef	DIRENT_H
#ident	"@(#)dirent.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define	DIRENT_H


#define MAXNAMLEN	512		/* maximum filename length */
#define DIRBUF		1048		/* buffer size for fs-indep. dirs */


typedef struct
	{
	int	dd_fd;			/* file descriptor */
	int	dd_loc;			/* offset in block */
	int	dd_size;		/* amount of valid data */
	char	*dd_buf;		/* directory block */
	}	DIR;			/* stream data from opendir() */

extern DIR		*opendir();
extern struct dirent	*readdir();
extern long		telldir();
extern void		seekdir();
extern int		closedir();

/* The following macro was removed for POSIX 1003.1 compliance, added as a
   library call in libc instead, mer */

#ifndef	_POSIX_SOURCE

#define rewinddir( dirp )	seekdir( dirp, 0L )

#endif /* _POSIX_SOURCE */

#include <sys/dirent.h>

#endif	/* DIRENT_H */
