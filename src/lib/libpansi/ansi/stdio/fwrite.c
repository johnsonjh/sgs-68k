#ident	"@(#)fwrite.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include <errno.h>
#include "_stdio.h"
#include <string.h>
#include <syscall.h>

extern size_t fwrite(const void *ptr,size_t size,size_t count,FILE *iop)
{
	register unsigned long nleft;
	register int n;
	register unsigned char *cptr, *bufend;

	if (count <= 0 || size <= 0 || _WRTCHK(iop)) {
		errno = EBADF;
		return (0);
	}

	bufend = _bufend(iop);
	nleft = (unsigned long) count * size;	/* may overflow */
	if (nleft < count || nleft < size)	/* overflow occured */
		return (0);


	/* if the file is unbuffered, or if the iop->ptr = iop->base, and there
	   is > BUFSIZ chars to write, we can do a direct write */
	if (iop->_base >= iop->_ptr)  {	/*this covers the unbuffered case, too*/
		if (((iop->_flag & _IONBF) != 0) || (nleft >= BUFSIZ))  {
			if ((n=write(_fileno(iop),ptr,nleft)) != nleft)
			    {
				iop->_flag |= _IOERR;
				n = (n >= 0) ? n : 0;
			}
			return (size_t) n/size;
		}
	}
	/* Put characters in the buffer */
	/* note that the meaning of n when just starting this loop is
	   irrelevant.  It is defined in the loop */
	for (; ; ptr =(char *)ptr + n) {
	        while ((n = bufend - (cptr = iop->_ptr)) <= 0)  /* full buf */
	                if (_xflsbuf(iop) == EOF)
	                        return (count - (nleft + size - 1)/size);
	        n = (nleft < n ? nleft : n);
	        memcpy((void *) cptr,ptr, n);
	        iop->_cnt -= n;
	        iop->_ptr += n;
	        _BUFSYNC(iop);
		/* done; flush if linebuffered with a newline */
	        if ((nleft -= n) == 0)  { 
			if (iop->_flag & (_IOLBF | _IONBF)) {
	               		if ((iop->_flag & _IONBF) || 
							(memchr((void *)cptr,(int)'\n',count * size)!=NULL)
						   )  { _xflsbuf(iop); }
			}
	                return (count);
	        }
	}
}
