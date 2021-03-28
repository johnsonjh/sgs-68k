#ident	"@(#)ungetc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"

extern int ungetc(int c,FILE *iop)
{
	if(c == EOF) return(EOF);
	if( (iop->_flag & _IOREAD) == 0 || iop->_ptr <= iop->_base )
		if(iop->_ptr == iop->_base && iop->_cnt == 0)
			++iop->_ptr;
		 else 
			return(EOF);
	if (iop->_base == NULL) _findbuf(iop);
	--iop->_ptr;
	if ( RDONLY(iop) == 0 )			/* "stream" could be a string literal */
		*iop->_ptr =(unsigned char) c;
	++iop->_cnt;
	iop->_flag &= ~_IOEOF;
	return(unsigned char) c;
}
