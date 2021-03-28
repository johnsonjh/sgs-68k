#ident	"@(#)zprnt_def.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:print/zprnt.c	1.1"	*/
	
int (*_libc__doprnt)() =0;
int (*_libc__cerror)() =0;
int (*_libc_isnand)() =0;
