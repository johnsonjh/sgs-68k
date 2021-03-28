/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mkdirp.c: version 25.1 created on 12/2/91 at 19:33:09	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mkdirp.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:mkdirp.c	1.4"	*/
	
#ident	"@(#)libgen:mkdirp.c	25.1"


/* Creates directory and it's parents if the parents do not
** exist yet.
**
** Returns -1 if fails for reasons other than non-existing
** parents.
** Does NOT compress pathnames with . or .. in them.
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int errno;
char *strrchr(), *strchr();
static int pperm(), makedir();

int
mkdirp(d, ms)
char *d;
char *ms;
{
	char  *endptr, *ptr, *p, *slash;
	int i;

	ptr = d;


		/* Try to make the directory */

	if ((i=makedir(d, ms)) == 0)
		return(0);
	if (errno != ENOENT)
		return(-1);
	endptr = strrchr(d, '\0');
	ptr = endptr;
	slash = strrchr(d, '/');

		/* Search upward for the non-existing parent */

	while (slash != NULL) {

				/* Skip extra slashes */

		if ((p = (slash+1)) == ptr) {
			ptr = slash;
			p = slash;
			*p = '\0';
			slash = strrchr(d, '/');
			*p = '/';
			continue;
		} else
			ptr = slash;
		*ptr = '\0';

			/* If reached an existing parent, break */

		if (access(d, 00) ==0)
			break;

			/* If non-existing parent*/

		else {
			slash = strrchr(d,'/');

				/* If under / or current directory, make it. */

			if (slash  == NULL || slash== d) {
				if (makedir(d, ms))
					return(-1);
				break;
			}
		}
	}
		/* Create directories starting from upmost non-existing parent*/

	while ((ptr = strchr(d, '\0')) != endptr){
		*ptr = '/';
		if (makedir(d, ms))
			return(-1);
	}
	return(0);
}

/* Check write permission of parent of dir */

static int
pperm(dir)
char *dir;
{
	char *dp, *pp, *malloc();

	if ((dp=malloc((unsigned)strlen(dir) +4)) == NULL)
		return(-1);
	strcpy(dp, dir);
	if ((pp=strrchr(dp,'/')) == NULL) {
		if (access(".",02))
			return(-1);
		return(0);
	}
	if (pp == dp) {
		if (access("/", 02))
			return(-1);
		return(0);
	}
	*pp = '\0';
	if (access(dp, 02))
		return(-1);
	return(0);
}

/* Make directory if permitted, and change uid and gid to real uid and gid */

static int
makedir(dir, ms)
char *dir;
char *ms;
{
	int dfault = 0777;

	if (pperm(dir))
		return(-1);
	if (setuid(getuid()) || setgid(getgid()))
		return(-1);
	if (mkdir(dir,newmode(dfault, dir, ms)))
		return(-1);
	return(0);
}
