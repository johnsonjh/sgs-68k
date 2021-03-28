/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) m68kx.c: version 25.1 created on 12/2/91 at 17:38:24	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)m68kx.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	m68kx.c	7.1		*/

#ident	"@(#)f77/f77/mach:m68kx.c	25.1"

#include <stdio.h>
#include "defines"
#include "machdefs"


prjunk(fp, infname)
FILEP fp;
char *infname;
{
register char *t;
register int i;
	t = infname + (i = strlen(infname));
	while (*--t != '/' && i-- > 0)
		;
	fprintf(fp, "\tfile\t\"%s\"\n", ++t);
}

prchars(fp, s)
FILEP fp;
int *s;
{

fprintf(fp, "\tbyte 0%o,0%o\n", s[0], s[1]);
}



pruse(fp, s)
FILEP fp;
char *s;
{
fprintf(fp, "\t%s\n", s);
}



prskip(fp, k)
FILEP fp;
ftnint k;
{
fprintf(fp, "\tspace\t%ld\n", k);
}





prcomblock(fp, name)
FILEP fp;
char *name;
{
fprintf(fp, LABELFMT, name);
}
