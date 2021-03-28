#ident	"@(#)gethostnm.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)gethostnm.c	3.2 LAI System V NFS Release 3.2/V3	source"
#include <sys/utsname.h>
#include <stdio.h>

#define HOSTFILE	"/etc/hostname"

gethostname(name, namelen)
	char *name;
	int namelen;
{
	FILE *fp;
	char *cp, *strchr();
	int status;
	struct utsname utsname;

	/*
	 * First, try to get the hostname out of a file.  If this fails,
	 * then use the nodename as stored as part of the utsname.
	 */
	if ((fp = fopen(HOSTFILE, "r")) != NULL) {
		if (fgets(name, namelen, fp) != NULL) {
			if ((cp = strchr(name, '\n')) != NULL)
				*cp = '\0';
			fclose(fp);
			return(0);
		}
		fclose(fp);
	}

	status = uname(&utsname);
	if (status == 0)
		strncpy(name, utsname.nodename, sizeof(utsname.nodename));

	return(status);
}

sethostname(name, namelen)
	char *name;
	int namelen;
{
	FILE *fp;

	if ((fp = fopen(HOSTFILE, "w")) == NULL)
		return(-1);
	if (fprintf(fp, "%*.*s\n", namelen, namelen, name) != namelen + 1) {
		fclose(fp);
		return(-1);
	}
	fclose(fp);
	return(0);
}
