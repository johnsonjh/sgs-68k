#ident	"@(#)execle.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:sys/execle.c	1.6"	*/
	

/*
 *	execle(file, arg1, arg2, ..., 0, envp)
 */

execle(file, args)
	char	*file;
	char	*args;			/* first arg */
{
	register  char  **p;

	p = &args;
	while(*p++);
	return(execve(file, &args, *p));
}
