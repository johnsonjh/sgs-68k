#ident	"@(#)rewind.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <syscall.h>

extern void rewind(FILE *fp)
{
	fflush(fp);
	lseek(_fileno(fp),0L,0);
	fp->_cnt=0;
	fp->_ptr = fp->_base;
	fp->_flag &= ~(_IOERR | _IOEOF);
	if (fp->_flag & _IORW) fp->_flag &= ~(_IOREAD|_IOWRT);
	return;
}
