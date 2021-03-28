#ident	"@(#)fgets.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

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

#include <errno.h>
#include <stdio.h>
#include "_stdio.h"

#include "sys/stat.h"
#include "fcntl.h"

#define MIN(x, y)	(x < y ? x : y)

extern char *fgets(char *ptr,int size,FILE *iop)
{
	char *p, *ptr0 = ptr;
	register int n;
	struct	stat	sfd_buf;

	if ( !(iop->_flag & (_IOREAD | _IORW)))  {

		errno = EBADF;
		return (NULL);
		}

	if ( size <= 0)

		return (NULL);

	for (size--; size > 0; size -= n) {
		if (iop->_cnt <= 0) { /* empty buffer */
			if (_filbuf(iop) == EOF) {
				if (ptr0 == ptr)
					return (NULL);
				break; /* no more data */
			}
			iop->_ptr--;
			iop->_cnt++;
		}
		n = MIN(size, iop->_cnt);
#ifdef  _ANSI 
		if ((p = _memccpy(ptr, (char *) iop->_ptr, '\n', n)) != NULL)
#else
		if ((p = (char *) memccpy(ptr, (char *) iop->_ptr, '\n', n)) != NULL)
#endif /* _ANSI */
			n = p - ptr;
		ptr += n;
		iop->_cnt -= n;
		iop->_ptr += n;
		_BUFSYNC(iop);
		if (p != NULL)
			break; /* found '\n' in buffer */
	}
	*ptr = '\0';
	return (ptr0);
}
