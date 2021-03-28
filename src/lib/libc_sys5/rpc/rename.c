#ident	"@(#)rename.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	System V NFS - Release 3.2/V3
 *
 *	Copyright 1986, 1987, 1988 Lachman Associates, Incorporated (LAI)
 *
 *	All Rights Reserved.
 *
 *	The copyright above and this notice must be preserved in all
 *	copies of this source code.  The copyright above does not
 *	evidence any actual or intended publication of this source
 *	code.
 *
 *	This is unpublished proprietary trade secret source code of
 *	Lachman Associates.  This source code may not be copied,
 *	disclosed, distributed, demonstrated or licensed except as
 *	expressly authorized by Lachman Associates.
 */
#ident	"@(#)rename.c	3.2 LAI System V NFS Release 3.2/V3	source"
#ifdef EXOS
#include <sys/errno.h>
#else
#include <errno.h>
#include <CMC/errno.h>
#endif

rename(from, to)
	char *from, *to;
{
	int ret, serrno;
	extern int errno;

	(void) unlink(to);
	if (link(from, to) < 0)
		return(-1);
	if ((ret = unlink(from)) < 0) {
		serrno = errno;
		(void) unlink(to);
		errno = serrno;
		}
	return(ret);
}
