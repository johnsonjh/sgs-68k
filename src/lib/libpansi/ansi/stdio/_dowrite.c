#ident	"@(#)_dowrite.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <string.h>

/* The function _dowrite carries out buffer pointer bookkeeping surrounding */
/* a call to fwrite.  It is called only when the end of the file output */
/* buffer is approached or in other unusual situations. */

extern int _dowrite(char *p,int n,FILE *iop,unsigned char **ptrptr)
{
	int ret;

#ifndef DYNAMIC
	if (iop->_file != FOPEN_MAX) {
#else /* DYNAMIC */
	if (!(iop->_flag & _IOSTRING)) {
#endif /* DYNAMIC */
		iop->_cnt -= (*ptrptr - iop->_ptr);
		iop->_ptr = *ptrptr;
		_BUFSYNC(iop);
		ret = fwrite(p, 1, n, iop);
		*ptrptr = iop->_ptr;
	} else {
		*ptrptr = (unsigned char *) memcpy((char *) *ptrptr, p, n) + n;
		ret = n;
	}
	/**
	if ( ret < 0 )
		iop->_flag |= _IOERR;
		**/
	return ret;
}
