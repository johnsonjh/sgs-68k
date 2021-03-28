#ident	"@(#)_flsbuf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <syscall.h>

/* The routine _flsbuf may or may not actually flush the output buffer.  If
 * the file is line-buffered, the fact that iop->_cnt has run below zero
 * is meaningless: it is always kept below zero so that invocations of putc
 * will consistently give control to _flsbuf, even if the buffer is far from
 * full.  _flsbuf, on seeing the "line-buffered" flag, determines whether the
 * buffer is actually full by comparing iop->_ptr to the end-of-buffer pointer
 * _bufend(iop).  If it is full, or if an output line is completed (with a
 * newline), the buffer is flushed.  (Note: the character argument to _flsbuf
 * is not flushed with the current buffer if the buffer is actually full--
 * it goes into the buffer after flushing.)
 */

extern int _flsbuf(unsigned char c,FILE *iop)
{
    unsigned char c1;

	if (_WRTCHK(iop))			/* is writing legitimate? */
		return(EOF);
    do {
	/* check for linebuffered with write perm, but no EOF */
	if ( (iop->_flag & (_IOLBF | _IOWRT | _IOEOF)) == (_IOLBF | _IOWRT) ) {
		if ( iop->_ptr >= _bufend(iop) )  /* if buffer full, */
			break;		    /* exit do-while, and flush buf. */
		if ( (*iop->_ptr++ = c) != '\n' ) return(c);
		return(_xflsbuf(iop) == EOF ? EOF : c);
	}
	/* write out an unbuffered file, if have write perm, but no EOF */
	if ( (iop->_flag & (_IONBF | _IOWRT | _IOEOF)) == (_IONBF | _IOWRT) ) {
		c1 = c;
		iop->_cnt = 0;
		if (write(_fileno(iop),(char *) &c1,1) == 1) return(c);
		iop->_flag |= _IOERR;
		return(EOF);
	}
    } while ( (iop->_flag & (_IONBF | _IOLBF)) );

    _xflsbuf(iop);   /* full buffer:  flush buffer */
	putc((char) c,iop);
	/* put c in newly emptied buffer */
	/* (which, because of signals, may NOT be empty) */
    return( ferror(iop) ? EOF : c);
}
