#ident	"@(#)tmpfile.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include <syscall.h>

extern FILE *tmpfile(void)
{
	char	tfname[L_tmpnam];
	register FILE	*p;

	if((p = fopen(tmpnam((char *) tfname), "wb+")) == NULL) return NULL;
		else remove((char *) tfname);
		/* removes directory entry of tfname, file remains in existence
		   until the descriptor of it dies */
	return(p);
}
