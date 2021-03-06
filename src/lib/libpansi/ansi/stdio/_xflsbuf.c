#ident	"@(#)_xflsbuf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include <errno.h>
#include "_stdio.h"
#include <syscall.h>

/* The function _xflsbuf writes out the current contents of the output
 * buffer delimited by iop->_base and iop->_ptr.
 * iop->_cnt is reset appropriately, but its value on entry to _xflsbuf
 * is ignored.
 *
 * The following code is not strictly correct.  If a signal is raised,
 * invoking a signal-handler which generates output into the same buffer
 * being flushed, a peculiar output sequence may result (for example,
 * the output generated by the signal-handler may appear twice).  At
 * present no means has been found to guarantee correct behavior without
 * resorting to the disabling of signals, a means considered too expensive.
 * For now the code has been written with the intent of reducing the
 * probability of strange effects and, when they do occur, of confining
 * the damage.  Except under extremely pathological circumstances, this
 * code should be expected to respect buffer boundaries even in the face
 * of interrupts and other signals.
 */

extern int _xflsbuf(FILE *iop)
{
	register unsigned char *base;
	register int n;

	n = iop->_ptr - (base = iop->_base);
	iop->_ptr = base;
	iop->_cnt = (iop->_flag &(_IONBF | _IOLBF)) ? 0 : _bufsiz(iop);
	_BUFSYNC(iop);

	if (n > 0 &&
		n != write(_fileno(iop),(char *) base,(unsigned) n))  {

		iop->_flag |= _IOERR;
/*		errno = EIO;			** barryk: don't know why this is here! */
						/* we must leave errno set as per underlying function */
		return(EOF);
	}
	return(0);
}
