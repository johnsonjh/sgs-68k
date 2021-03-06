#ident	"@(#)fread.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * This version reads directly from the buffer rather than looping on getc.
 * Ptr args aren't checked for NULL because the program would be a
 * catastrophic mess anyway.  Better to abort than just to return NULL.
 */

#include <stdio.h>
#include <errno.h>
#include "_stdio.h"

#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"

#define MIN(x, y)	(x < y ? x : y)

extern size_t fread(void *ptr,size_t size,size_t count,FILE *iop)
{
	register unsigned long nleft;
	register int n;
	struct	stat	iop_buf;

	if ( (iop->_flag & _IOWRT)){
		errno = EBADF;
		return(NULL);
	}

	if ( count <= 0) return 0;

	nleft = (unsigned long) count * size; /* may overflow */
	if (nleft < count || nleft < size)	/* overflow occured */
		return (0);

	/* Put characters in the buffer */
	/* note that the meaning of n when just starting this loop is
	   irrelevant.  It is defined in the loop */
	for ( ; ; ) {
		if (iop->_cnt <= 0) { /* empty buffer */
			if (_filbuf(iop) == EOF)
				return (count - (nleft + size - 1)/size);
			iop->_ptr--;
			iop->_cnt++;
		}
		n = MIN(nleft, iop->_cnt);
		ptr = (char *) memcpy(ptr, (char *) iop->_ptr, n) + n;
		iop->_cnt -= n;
		iop->_ptr += n;
		_BUFSYNC(iop);
		if ((nleft -= n) <= 0)
			return (count);
	}
}
