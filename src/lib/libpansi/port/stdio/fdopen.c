#ident	"@(#)fdopen.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:stdio/fdopen.c	1.9"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
/*
 * Unix routine to do an "fopen" on file descriptor
 * The mode has to be repeated because you can't query its
 * status
 */

#include <stdio.h>
#include <_stdio.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

extern long lseek();
extern FILE *_findiop();

FILE *
fdopen(fd, mode)
int	fd;
register char *mode;
{
	register FILE *iop;
	int	status;
	struct stat stat_buf;

	if( (status = fcntl(fd,F_GETFL, 0)) < 0 )
		return(NULL);
	if( fstat(fd,&stat_buf) < 0 )
		return(NULL);

#ifndef DYNAMIC
	if((iop = _findiop()) == NULL)
#else /* DYNAMIC */
	if ((iop = _findiop(NULL)) == NULL)
#endif /* DYNAMIC */
		return(NULL);

	iop->_cnt = 0;
	iop->_file = fd;
	_bufend(iop) = iop->_base = iop->_ptr = NULL;
#ifndef DYNAMIC
	iop->_flag = 0;
	_XFLAGS(iop) = 0;
#endif /* DYNAMIC */
	switch(*mode) {

		case 'r':
			if((status & O_ACCMODE) == O_WRONLY ) 
				return(NULL);
			iop->_flag |= _IOREAD;
			break;
		case 'a':
			if((status & O_ACCMODE) == O_RDONLY )
				return(NULL);
			(void) lseek(fd, 0L, 2);
			/* No break */
		case 'w':
			if((status & O_ACCMODE) == O_RDONLY)
				return(NULL);
			iop->_flag |= _IOWRT;
			break;
		default:
			return(NULL);
	}

	if ( S_ISFIFO(stat_buf.st_mode) )	
	/* barryk: to prevent seeks */
#ifndef DYNAMIC
		_XFLAGS(iop) |= _IOXPIPE;
#else /* DYNAMIC */
		iop->_flag |= _IOXPIPE;
#endif /* DYNAMIC */

	if(mode[1] == '+') {
		iop->_flag &= ~(_IOREAD | _IOWRT);
		iop->_flag |= _IORW;
	}

	return(iop);
}
