#ident	"@(#)getfstyp.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1989 ARIX	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ARIX:#ident	"libc:gen/getfstyp.c	1.1"	*/
	

/* getfstyp - Returns an integer representing file system type, of an 
   unmounted file system.  This call does not replace statfs, since in 
   statfs you must provide that information */

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fs/s5param.h>
#include <sys/fs/s5filsys.h>
#include <errno.h>

int
getfstyp (path)
char	*path;
{

	int	dev_fd;
	long	bsize = 0;
	struct	stat	buf;
	struct	filsys	*sblock;
	char	first_1k [1024];	/* We need to read in 1k, since raw disk will
					   require at least a 1k read */

	if (stat(path, &buf) < 0){

		errno = EACCES;
		return (-1);
	}

	if (((buf.st_mode & S_IFMT) != S_IFBLK) 
		&& ((buf.st_mode & S_IFMT) != S_IFCHR)) {

		errno = ENOTBLK;
		return (-1);
	}

	if ((dev_fd = open(path, O_RDONLY)) < 0) {

		errno = EACCES;
		return (-1);
	}
	
	if ( read (dev_fd, first_1k, 1024) != 1024 ) {

		errno = ESPIPE;
		close(dev_fd);
		return (-1);
	}

	sblock = (struct filsys *)(first_1k + SUPERBOFF);

	close(dev_fd);
	

	return (sblock->s_type);
}
