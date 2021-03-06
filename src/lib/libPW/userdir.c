/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) userdir.c: version 25.1 created on 12/2/91 at 18:45:27	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)userdir.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libPW:userdir.c	3.3"	*/
	
#ident	"@(#)libPW:userdir.c	25.1"

/*
	Gets user's login directory.
	The argument must be an integer.
	Note the assumption about position of directory field in
	password file (no group id in password file).
	Returns pointer to login directory on success,
	0 on failure.
        Remembers user ID and login directory for subsequent calls.
*/

char *
userdir(uid)
register int uid;
{
	char pw[200];
	static int ouid;
	static char ldir[33];
	register int i;
	register char *cp;

	if (ouid!=uid || ouid==0) {
		if (getpw(uid,pw))
			return((char *)0);
		cp = pw;
		while (*cp++ != ':') ; /* login name */
		while (*cp++ != ':') ; /* passwd */
		while (*cp++ != ':') ; /* user ID */
		while (*cp++ != ':') ; /* comment */
		for (i=0; i<32; i++) {
			if ((ldir[i] = *cp)=='\0' || *cp==':') break;
			cp++;
		}
		ldir[i] = '\0';
	}
	return(ldir);
}
