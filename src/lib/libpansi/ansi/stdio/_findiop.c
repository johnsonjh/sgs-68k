#ident	"@(#)_findiop.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"

#ifndef DYNAMIC
extern FILE *_findiop(void)
#else /* DYNAMIC */
extern FILE *_findiop(iop)
register FILE *iop;
#endif /* DYNAMIC */
{
#ifndef DYNAMIC
	register FILE *iop;

	for(iop = _iob; iop->_flag & (_IOREAD | _IOWRT | _IORW); iop++)
		if(iop >= _lastbuf) return(NULL);
	if(iop >= _lastbuf) return(NULL);
#else /* DYNAMIC */
	if (iop == NULL) {
		if (_freeiob) {
			iop = _freeiob;
			iop->_flag = _IOIOB;
			_freeiob = _freeiob->_next;
			if (_freeiob) {
				_freeiob->_prev = NULL;
			}
		} else {
			if (_freemalloc) {
				iop = _freemalloc;
				_freemalloc = _freemalloc->_next;
				if (_freemalloc) {
					_freemalloc->_prev = NULL;
				}
			} else {
				if ((iop = (FILE *) malloc(sizeof(FILE))) < 0) {
					return (NULL);
				}
				iop->_prev = NULL;
			}
			iop->_flag = 0;
		}
	} else {
		if (_freeiob == iop) {
			_freeiob = _freeiob->_next;
			iop->_flag = _IOIOB;
			if (_freeiob) {
				_freeiob->_prev = NULL;
			}
		} else {
			if (_freemalloc == iop) {
				iop->_flag = 0;
				_freemalloc = _freemalloc->_next;
				if (_freemalloc) {
					_freemalloc->_prev = NULL;
				}
			}
		}
	}

	/* This will only be true for freopen of an iop in the middle *
	 * of the list.						      *
	 */

	if (iop->_prev != NULL) {
		if ((iop->_prev->_next = iop->_next) != NULL) {
			iop->_next->_prev = iop->_prev;
		}
		if (iop->_flag & _IOIOB) {
			iop->_flag = _IOIOB;
		} else {
			iop->_flag = 0;
		}
	}

	if (_firstbuf) {
		_firstbuf->_prev = iop;
	}
	iop->_next = _firstbuf;
	iop->_prev = NULL;
	_firstbuf = iop;
#endif /* DYNAMIC */
	return(iop);
}
