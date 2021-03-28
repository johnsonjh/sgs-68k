#ident	"@(#)mall_def.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/mall_def.c	1.3"	*/
/*
 * Contains the definitions
 * of the pointers to the
 * malloc, and _assert
 */
#if SHLIB
 char * (* _libc_malloc)() = 0;
 void (* _libc_free)()   = 0;
 char * (* _libc_realloc)()= 0;
#endif
