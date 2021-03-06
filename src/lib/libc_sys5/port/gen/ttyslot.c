#ident	"@(#)ttyslot.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/ttyslot.c	1.9"	*/
	

/*LINTLIBRARY*/
/*
 * Return the number of the slot in the utmp file
 * corresponding to the current user: try for file 0, 1, 2.
 * Returns 0 if slot not found.  - jc0
 */
#include <sys/types.h>
#include "utmp.h"
#define	NULL	0

extern char *ttyname(), *strrchr();
extern int strncmp(), open(), read(), close();

static char utmp_file[] = "/etc/utmp";

int
ttyslot()
{
	struct utmp ubuf;
	register char *tp, *p;
	register int s, fd;

	if((tp=ttyname(0)) == NULL && (tp=ttyname(1)) == NULL &&
					(tp=ttyname(2)) == NULL)
		return(0); /* jc0 */

	if((p=strrchr(tp, '/')) == NULL)
		p = tp;
	else
		p++;

	if((fd=open(utmp_file, 0)) < 0)
		return(0); /* jc0 */
	s = 0;
	while(read(fd, (char*)&ubuf, sizeof(ubuf)) == sizeof(ubuf)) {
		if(    (ubuf.ut_type == INIT_PROCESS ||
			ubuf.ut_type == LOGIN_PROCESS ||
			ubuf.ut_type == USER_PROCESS ||
			ubuf.ut_type == DEAD_PROCESS ) &&
			strncmp(p, ubuf.ut_line, sizeof(ubuf.ut_line)) == 0){

			(void) close(fd);
			return(s);
		}
		s++;
	}
	(void) close(fd);
	return(0); /* jc0 */
}
