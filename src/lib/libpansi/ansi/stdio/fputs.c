#ident	"@(#)fputs.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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

extern int fputs(char *ptr,FILE *iop)
{
	register int ndone = 0, n;
	register unsigned char *cptr, *bufend;
	char *p;

	if (_WRTCHK(iop)) {
		errno = EBADF;
		return (EOF);
	}
	bufend = _bufend(iop);

	if ((iop->_flag & _IONBF) == 0)  {
		for ( ; ; ptr += n) {
			while ((n = bufend - (cptr = iop->_ptr)) <= 0)  
				/* full buf */
				if (_xflsbuf(iop) == EOF)
					return(EOF);
#ifdef _ANSI 
			if ((p = _memccpy((char *) cptr, ptr, '\0', n)) != NULL)
#else
			if ((p = memccpy((char *) cptr, ptr, '\0', n)) != NULL)
#endif /* _ANSI */
				n = (p - (char *) cptr) - 1;
			iop->_cnt -= n;
			iop->_ptr += n;
			_BUFSYNC(iop);
			ndone += n;
			if (p != NULL)  { 
				/* done; flush buffer if line-buffered */
	       			if (iop->_flag & _IOLBF)
	       				if (_xflsbuf(iop) == EOF)
	       					return(EOF);
	       			return(ndone);
	       		}
		}
	}  else  {
		/* write out to an unbuffered file */
		register size_t cnt = strlen(ptr);
									/* barryk: before, always returned cnt */
		return( write(_fileno(iop),ptr,cnt) );	
	}
}
