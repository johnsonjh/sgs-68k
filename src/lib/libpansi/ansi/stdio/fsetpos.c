#ident	"@(#)fsetpos.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <syscall.h>

extern int fsetpos(FILE *fp,const fpos_t *pos)
{
	int ret;

	ret = fflush(fp);
	if ( ret == 0 ) ret = lseek(_fileno(fp),(long) *pos,0);
	fp->_cnt=0;
	fp->_ptr = fp->_base;
	fp->_flag &= ~(_IOERR | _IOEOF);
	if (fp->_flag & _IORW) fp->_flag &= ~(_IOREAD|_IOWRT);
	return ( ret == -1 ? ret : 0 );
	/* lseek sets errno, further see fgetpos */
}
