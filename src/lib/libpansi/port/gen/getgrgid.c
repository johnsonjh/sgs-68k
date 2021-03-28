#ident	"@(#)getgrgid.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/getgrgid.c	1.7"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#ifndef NFSMNT
#include "shlib.h"
#include <grp.h>

extern struct group *getgrent();
extern void setgrent(), endgrent();

/* gid_t added for POSIX 1003.1 compliance, mr */

struct group *
getgrgid(gid)
register gid_t gid;
{
	register struct group *p;

	setgrent();
	while((p = getgrent()) && p->gr_gid != gid)
		;
	endgrent();
	return(p);
}
#endif
