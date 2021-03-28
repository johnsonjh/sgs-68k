#ident	"@(#)getdom.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)getdom.c	3.3 LAI System V NFS Release 3.2/V3	source"
#include <stdio.h>

#define DOMAINFILE	"/etc/domainname"

getdomainname(name, length)
	char *name;
	int length;
{
	FILE *fp;
	char *cp, *strchr();

	if (length <= 0)
		return(0);
	/*
	 * Null domain names are OK on the Sun, so why not for us?
	 */
	if ((fp = fopen(DOMAINFILE, "r")) == NULL) {
		*name = '\0';
		return(0);
	}
	if (fgets(name, length, fp) == NULL) {
		*name = '\0';
		return(0);
	}
	if ((cp = strchr(name, '\n')) != NULL)
		*cp = '\0';
	fclose(fp);
	return (0);
}

setdomainname(name, length)
	char *name;
	int length;
{
	FILE *fp;

	if ((fp = fopen(DOMAINFILE, "w")) == NULL)
		return(-1);
	if (fprintf(fp, "%*.*s\n", length, length, name) != length + 1) {
		fclose(fp);
		return(-1);
	}
	fclose(fp);
	return(0);
}
