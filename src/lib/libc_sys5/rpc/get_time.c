#ident	"@(#)get_time.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)get_time.c	3.2 LAI System V NFS Release 3.2/V3	source"
	/*
	 *	  Compatibility function to simulate the missing
	 *	gettimeofday(2) system call.   Returns time in
	 *	seconds in now -> tv_sec, and sets microseconds to
	 *	an arbitrarily chosen unique value.
	 */

#include <stdio.h>

#include <sys/fs/nfs/time.h>
static	int	usecs;

gettimeofday (now, dummy)
struct	timeval	*now;
{
long	secs;
long	time ();

	time (&secs);

	now -> tv_sec = secs;
	now -> tv_usec = usecs++;
}
