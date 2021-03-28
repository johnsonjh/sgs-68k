#ident	"@(#)fclose.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "_stdio.h"
#include <syscall.h>

extern int fclose(FILE *fp)
{
	register int rtn = EOF;

#ifndef DYNAMIC
	if (fp == NULL)
#else /* DYNAMIC */
	if ((fp == NULL) || (fp->_flag & _IOFREE))
#endif /* DYNAMIC */
		return rtn;
	if (fp->_flag & (_IOREAD | _IOWRT | _IORW)) {

						/* For POSIX 1003.1, mer */
		if (fp->_flag & _IONBF) {
			rtn = 0;
			if ( SEEKCHK(fp) )
				lseek ( _fileno(fp), ftell(fp), 0L );
		} else {		/* note: fflush will call ftell & lseek */
			rtn = fflush(fp);
		}

		if ( close(_fileno(fp)) < 0 ) {
			rtn = EOF;
			errno = ENOENT;
		}
	}
	if (fp->_flag & _IOMYBUF) { 
		free((char *)fp->_base-4);
	}
#ifdef DYNAMIC
	if (fp == _firstbuf) {
		_firstbuf = fp->_next;
		if (_firstbuf != NULL) {
			_firstbuf->_prev = NULL;
		}
	} else {
		if (fp->_next != NULL) {
			fp->_next->_prev = fp->_prev;
		}
		fp->_prev->_next = fp->_next;
	}
	if (fp->_flag & _IOIOB) {
		_sortfree(fp);
	} else {
		fp->_next = _freemalloc;
		fp->_prev = NULL;
		if (_freemalloc != NULL) {
			_freemalloc->_prev = fp;
		}
		_freemalloc = fp;
	}
	fp->_flag |= _IOFREE;
#else /* DYNAMIC */
	fp->_flag = 0;
	fp->_cnt = 0;
	fp->_ptr = fp->_base;
#endif /* DYNAMIC */
	/*_bufend(fp) = NULL;	Fix for explicit fclose on duped stdout */
	return(rtn);
}
