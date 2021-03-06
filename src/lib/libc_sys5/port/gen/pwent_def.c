#ident	"@(#)pwent_def.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/pwent_def.c	1.1"	*/
	

/*
 * Contains defintion
 * of pointers to the
 * functions getpwent(),
 * setpwent(), endpwent() and
 * fgetpwent.
 */
#if SHLIB
#include <pwd.h>

 struct passwd * (* _libc_getpwent)() = 0;
 struct passwd * (* _libc_fgetpwent)() = 0;
 void  (*_libc_setpwent)() = 0;
 void  (*_libc_endpwent)() = 0;
#endif
