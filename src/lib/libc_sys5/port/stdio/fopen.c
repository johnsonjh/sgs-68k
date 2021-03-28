#ident	"@(#)fopen.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:stdio/fopen.c	1.13"	*/
	

/*LINTLIBRARY*/
#include "shlib.h"
#include "stdio.h"
#include <fcntl.h>

extern int open(), fclose();
extern long lseek();
extern FILE *_findiop(), *_endopen();

FILE *
fopen(file, mode)
char	*file, *mode;
{
#ifndef DYNAMIC
	return (_endopen(file, mode, _findiop()));
#else /* DYNAMIC */
	return (_endopen(file, mode, NULL));
#endif /* DYNAMIC */
}

FILE *
freopen(file, mode, iop)
char	*file, *mode;
register FILE *iop;
{
	(void) fclose(iop); /* doesn't matter if this fails */
	return (_endopen(file, mode, iop));
}

static FILE *
_endopen(file, mode, iop)
char	*file, *mode;
register FILE *iop;
{
	register int	plus, oflag, fd;

#ifndef DYNAMIC
	if (iop == NULL || file == NULL || file[0] == '\0')
#else /* DYNAMIC */
	if (file == NULL || file[0] == '\0')
#endif /* DYNAMIC */
		return (NULL);
	plus = (mode[1] == '+');
	switch (mode[0]) {
	case 'w':
		oflag = (plus ? O_RDWR : O_WRONLY) | O_TRUNC | O_CREAT;
		break;
	case 'a':
		oflag = (plus ? O_RDWR : O_WRONLY) | O_APPEND | O_CREAT;
		break;
	case 'r':
		oflag = plus ? O_RDWR : O_RDONLY;
		break;
	default:
		return (NULL);
	}
	if ((fd = open(file, oflag, 0666)) < 0)
		return (NULL);
#ifdef DYNAMIC
	iop = _findiop(iop);
	if (iop == NULL) {
		return (NULL);
	}
#endif /* DYNAMIC */
	iop->_file = fd;
	iop->_flag |= plus ? _IORW : (mode[0] == 'r') ? _IOREAD : _IOWRT;
	if (mode[0] == 'a')   {
		if (!plus)  {
			/* if update only mode, move file pointer to the end
			   of the file */
			if ((lseek(fd,0L,2)) < 0)  {
				return NULL;
			}
		}
	}
	iop->_cnt = 0;
	_bufend(iop) = iop->_base = iop->_ptr = NULL;
	return (iop);
}
