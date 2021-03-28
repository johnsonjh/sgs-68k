#ident	"@(#)getgrnam.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/getgrnam.c	1.8"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#ifndef NFSMNT
#include <grp.h>

extern struct group *getgrent();
extern void setgrent(), endgrent();
extern int strcmp();

struct group *
getgrnam(name)
register char *name;
{
	register struct group *p;

	setgrent();
	while((p = getgrent()) && strcmp(p->gr_name, name))
		;
	endgrent();
	return(p);
}
#endif
