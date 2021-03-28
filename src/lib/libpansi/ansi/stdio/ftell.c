#ident	"@(#)ftell.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <errno.h>
#include <syscall.h>

extern long int ftell(FILE *iop)
{
	long int	tres;
	register int adjust;

	if ( SEEKCHK(iop) == 0 ) {	/* we shouldn't be here for pipes */
		errno = EIO;
		return( -1 );
	}

	if(iop->_cnt < 0)
		iop->_cnt = 0;
	if(iop->_flag & _IOREAD)
		adjust = - iop->_cnt;
	else if(iop->_flag & (_IOWRT | _IORW)) {
		adjust = 0;
		if(iop->_flag & _IOWRT && iop->_base &&
					(iop->_flag & _IONBF) == 0)
			adjust = iop->_ptr - iop->_base;
	} else {
		errno = EIO;
		return(-1L);
	}
	tres = lseek(_fileno(iop), 0L,1);
	if(tres >= 0)
		tres += (long int) adjust;
	return(tres);
}
