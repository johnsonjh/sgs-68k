#ident	"@(#)_filbuf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include <errno.h>
#include "_stdio.h"
#include <syscall.h>


extern int _filbuf(FILE *iop)
{
	register FILE *diop;

	if (iop->_base == NULL)  /* get buffer if we don't have one */
		_findbuf(iop);

	if ( !(iop->_flag & _IOREAD) )
		if (iop->_flag & _IORW)
			iop->_flag |= _IOREAD;
		else {
			errno = EBADF;
			return(EOF);
		}

	/* if this device is a terminal (line-buffered) or unbuffered, then */
	/* flush buffers of all line-buffered devices currently writing */

	if (iop->_flag & (_IOLBF | _IONBF))
#ifndef DYNAMIC
		for (diop = _iob; diop < _lastbuf; diop++ )
#else /* DYNAMIC */
		for (diop = _firstbuf; diop != NULL; diop = diop->_next)
#endif /* DYNAMIC */
			if (diop->_flag & _IOLBF)
				fflush(diop);	/* bk: mvdk had a static version here */

	iop->_ptr = iop->_base;
	iop->_cnt = read(_fileno(iop),(char *) iop->_base,
	    (unsigned int) ((iop->_flag & _IONBF)?1:_bufsiz(iop)));
	if (--iop->_cnt >= 0)		/* success */
		return (*iop->_ptr++);
	if (iop->_cnt != -1)		/* error */
		iop->_flag |= _IOERR;
	else {				/* end-of-file */
		iop->_flag |= _IOEOF;
		/* if ( iop->_flag & _IORW ) 
			iop->_flag &= ~_IOREAD; */
	}
	iop->_cnt = 0;
	return (EOF);
}
