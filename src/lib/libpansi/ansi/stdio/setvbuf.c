#ident	"@(#)setvbuf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <stdlib.h>

extern int setvbuf(FILE *fp,char *buffer,int mode,size_t size)
{
	if(fp->_base != NULL && fp->_flag & _IOMYBUF)
	free((char*)fp->_base-4);
	fp->_flag &= ~(_IOMYBUF | _IONBF | _IOLBF);
	/* This resets the _IOMYBUF,_IONBF and _IOLBF flags to 0 */
	fp->_flag |= mode;
	switch (mode)  {
	    /*note that the flags are the same as the possible values for mode
		  This is only the case because of our smart implementation
		*/
	    case _IONBF: /* file is to be unbuffered */ break;
	    case _IOLBF: /* file is to be line-buffered */ break;
	    case _IOFBF: /* file is to be buffered */
			/* need eight characters beyond _bufend for stdio slop */
			if (size <= 8) {size = BUFSIZ;buffer = NULL;}
			/* This is against ANSI, but takes some time to be changed */
			if (buffer == NULL) {
				size += 8; fp->_base = (unsigned char *) malloc(size);
			}
			else fp->_base = (unsigned char *) buffer;
			_bufend(fp) = fp->_base + size -8;
			break;
	    default:
			return -1;
	}
	fp->_ptr = fp->_base;
	fp->_cnt = 0;
	return 0;
}
