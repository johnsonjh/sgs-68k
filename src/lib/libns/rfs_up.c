/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rfs_up.c: version 25.1 created on 12/2/91 at 19:43:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rfs_up.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"libns:rfs_up.c	1.1.1.1"		*/

#ident	"@(#)lib/libns:rfs_up.c	25.1"

#include <sys/rfsys.h>
#include <errno.h>
#include <nserve.h>

rfs_up()
{
	char dname[MAXDNAME];


	/*
	 *	Determine if RFS is running by first obtaining the
	 *	domain name and then trying to push it into the
	 *	kernel.
	 */

	if (rfsys(RF_GETDNAME, dname, MAXDNAME) < 0) {
		errno=ENONET;
		return(-1);
	}
	if (rfsys(RF_SETDNAME, dname, strlen(dname)+1) >= 0) {
		errno=ENONET;
		return(-1);
	}
	return(0);
}
