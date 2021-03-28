#ident	"@(#)_endopen.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <errno.h>
#include <stdio.h>
#include "_stdio.h"
#include "fcntl.h"
#include <syscall.h>

extern FILE *_endopen(const char *file,const char *mode,FILE *iop)
{
	register int	plus, oflag, fd;

#ifndef DYNAMIC
	if (iop == NULL || file == NULL ||
  	    file[0] == '\0' || mode[0] == '\0') {
#else /* DYNAMIC */
	if (file == NULL || file[0] == '\0' || mode == '\0') {
#endif /* DYNAMIC */
		errno=ENOENT;
		return (NULL);
	}	

    /* mode[0]!='\0', so mode has at least length 1, 
       so mode[1] can be addressed */
	plus   = (mode[1] == '+') || ((mode[1] != '\0') && (mode[2] == '+'));
	/* Note: No distinction between binary and text streams is being made */

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
			   of the file, else leave it at the beginning */
			if ((lseek(fd,0L,2)) < 0)  {
				return NULL;
			}
		}
	}
	iop->_cnt = 0;
	_bufend(iop) = iop->_base = iop->_ptr = NULL;
#ifndef DYNAMIC
	_XFLAGS(iop) = 0;
#endif /* DYNAMIC */
	return (iop);
}
