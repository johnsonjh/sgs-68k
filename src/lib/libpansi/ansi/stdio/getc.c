#ident	"@(#)getc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"

extern int getc(FILE *p)
{
	int i;

	i = --p->_cnt < 0 ? _filbuf(p) : (int) *p->_ptr++;

	return i;
}
