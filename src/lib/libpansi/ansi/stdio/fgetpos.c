#ident	"@(#)fgetpos.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>

extern int fgetpos(FILE *stream,fpos_t *pos)
{
	return (*pos=(fpos_t) ftell(stream)) == -1L ? EOF : 0;
	/* Our filesystem cannot handle files larger than 4GB */
}
