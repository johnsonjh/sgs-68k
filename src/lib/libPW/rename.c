/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rename.c: version 25.1 created on 12/2/91 at 18:44:59	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rename.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libPW:rename.c	3.4"	*/
	
#ident	"@(#)libPW:rename.c	25.1"

# include "errno.h"
# include "fatal.h"
# include "signal.h"
/*
	rename (unlink/link)
	Calls xlink() and xunlink().
*/

rename(oldname,newname)
char *oldname, *newname;
{
	extern int errno;
	int (* holdsig[3])();
	int retval;
	
	/*Ignor signals 01 02 03 */
	holdsig[0] = signal(SIGHUP,SIG_IGN);
	holdsig[1] = signal(SIGINT,SIG_IGN);
	holdsig[2] = signal(SIGQUIT,SIG_IGN);
	if (unlink(newname) < 0 && errno != ENOENT)
		retval = xunlink(newname);

	if (xlink(oldname,newname) == Fvalue)
		retval = -1;
	retval = (xunlink(oldname));
	/*re establish signals */
	signal(SIGHUP,holdsig[0]);
	signal(SIGINT,holdsig[1]);
	signal(SIGQUIT,holdsig[2]);
	return(retval);
}
