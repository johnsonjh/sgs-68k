#ident	"@(#)fflush.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <errno.h>

/*
	The fflush() routine must take care because of the
	possibility for recursion. The calling program might
	do IO in an interupt catching routine that is likely
	to interupt the write() call within fflush()
 */

extern int fflush(FILE *fp)
{

	if (fp == (FILE *) NULL) {
		register FILE *iop;
		for(iop = _iob; iop->_flag & (_IOREAD | _IOWRT | _IORW); iop++)
			if(iop->_flag & _IOWRT ) fflush(iop);
		return 0;
	}

	if ( !(fp->_flag & _IOWRT) ) {
		int ret;

		if ( SEEKCHK(fp) ) {
			long pos;

			pos = ftell(fp);
			if ( pos == -1 ) { 				/* For POSIX 1003.1, barryk */
				errno = EBADF;
				return(EOF);
			}

			lseek ( _fileno (fp), pos, 0);	/* For POSIX 1003.1, mer */
		}
		fp->_cnt = 0; 
		return(0); 
	}

	while(!(fp->_flag & _IONBF) && (fp->_flag & _IOWRT) &&
			(fp->_base != NULL) && (fp->_ptr > fp->_base) )
		 _xflsbuf(fp);

	if ( SEEKCHK(fp) )
		lseek ( _fileno (fp), ftell (fp), 0);	/* For POSIX 1003.1, mer */

/***					** barryk: YIKES!!!! ****
	if ( fp->_flag & _IORW ) fp->_flag &= ~( _IOWRT | _IOREAD );
	***/
	/* It is now indeterminate whether the file (that is opened in update
	   mode) is open for input or output, MK*/

	return(ferror(fp) ? EOF : 0);
}
