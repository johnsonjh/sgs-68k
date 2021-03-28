#ident	"@(#)putc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"

extern int putc(int x,FILE *p)
{
	return (--p->_cnt < 0 ? _flsbuf((unsigned char) x, p) : 
			(int) (*p->_ptr++ = (unsigned char) x));
}
