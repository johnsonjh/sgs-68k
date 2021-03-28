#ident	"@(#)fseek.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <syscall.h>

extern int fseek(FILE *fp,long int offset,int whence)
{
	register int c;
	long int     p;

	fp->_flag &= ~_IOEOF;
	/* clear the EOF-flag */
	if(fp->_flag & _IOREAD) {
		if(((whence==SEEK_CUR) || (whence==SEEK_SET)) && 
		   fp->_base && 
		   !(fp->_flag&_IONBF)
		  ) {
			c = fp->_cnt;
			p = offset;
			if (whence == SEEK_SET)
				p += (long int) c - lseek((int) _fileno(fp), 0L, 1);
			else
				offset -= (long int) c;
			if(!(fp->_flag&_IORW) && c > 0 && p <= c &&
					p >= fp->_base - fp->_ptr) {
				fp->_ptr += (int) p;
				fp->_cnt -= (int) p;
				return(0);
			}
		}
		if(fp->_flag & _IORW) {
			fp->_ptr = fp->_base;
			fp->_flag &= ~_IOREAD;
		}
		switch (whence) {
		case SEEK_SET : p = lseek(_fileno(fp), offset, 0); break;
		case SEEK_CUR : p = lseek(_fileno(fp), offset, 1); break;
		case SEEK_END : p = lseek(_fileno(fp), offset, 2); break;
		};
		fp->_cnt = 0;
	} else if(fp->_flag & (_IOWRT | _IORW)) {
		if (fp->_flag & _IOWRT) {
			fflush(fp);
			if ( fp->_flag & _IORW )
				fp->_flag &= ~_IOWRT;	/* bk: mvdk did this in fflush */
		}
		fp->_cnt = 0;
		if (fp->_flag & _IORW) fp->_ptr = fp->_base;
		switch (whence) {
		case SEEK_SET : p = lseek(_fileno(fp), offset, 0); break;
		case SEEK_CUR : p = lseek(_fileno(fp), offset, 1); break;
		case SEEK_END : p = lseek(_fileno(fp), offset, 2); break;
		};
	}
	return((p == -1)? -1: 0);
}
