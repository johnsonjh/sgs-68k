#ident	"@(#)getpwuid.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/getpwuid.c	1.8"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#ifndef NFSMNT
#include <pwd.h>
extern struct passwd *getpwent();
extern void setpwent(), endpwent();

struct passwd *
getpwuid(uid)
register int uid;
{
	register struct passwd *p;

	setpwent();
	while((p = getpwent()) && p->pw_uid != uid)
		;
	endpwent();
	return(p);
}
#endif
