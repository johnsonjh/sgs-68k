#ident	"@(#)_findbuf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <stdlib.h>
#include <syscall.h>

/*
 * _findbuf, called only when iop->_base == NULL, locates a predefined buffer
 * or allocates a buffer using malloc.  If a buffer is obtained from malloc,
 * the _IOMYBUF flag is set in iop->_flag.
 */

extern void _findbuf(FILE *iop)
{
	register int fno = _fileno(iop); /* file number */

	/* allocate a small block for unbuffered, large for buffered */
	if (iop->_flag & _IONBF)  {
#ifndef DYNAMIC
		_bufend(iop) = (iop->_base = _smbuf[fno] + 4) + _SBFSIZ - 4;
#else /* DYNAMIC */
		iop->_base = (unsigned char *) malloc(_SBFSIZ);
		_bufend(iop) = &iop->_base[_SBFSIZ - 4];
#endif /* DYNAMIC */
	}  else  {
#if !u370
		if (fno < 2)  /* use existing bufs for stdin, stdout */
			_bufend(iop) = (iop->_base = 4 + 
				(fno == 0 ? _sibuf:_sobuf)) + BUFSIZ;
		else 
#endif
		if ((iop->_base = (unsigned char *) malloc(BUFSIZ+8)) != NULL)
		{
			/* if  we got a buffer */
			iop->_flag |= _IOMYBUF;
			_bufend(iop) = (iop->_base+=4) + BUFSIZ;
		} else
#ifndef DYNAMIC
			/* if no room for buffer, use small buffer */
			_bufend(iop) = 
			 (iop->_base = _smbuf[fno]+4) + _SBFSIZ - 4;
#else /* DYNAMIC */
		{
			/* if no room for buffer, use small buffer */
			iop->_base = (unsigned char *) malloc(_SBFSIZ);
			_bufend(iop) = &iop->_base[_SBFSIZ - 4];
		}
#endif /* DYNAMIC */
	}
	iop->_ptr = iop->_base;
	if ( isatty(fno) && !(iop->_flag & _IONBF) )
		iop->_flag |= _IOLBF;
}
