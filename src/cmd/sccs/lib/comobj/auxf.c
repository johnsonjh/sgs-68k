/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) auxf.c: version 25.1 created on 12/2/91 at 17:09:39	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)auxf.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
/*	ATT: #ident	"sccs:lib/comobj/auxf.c	6.2"		*/

#ident	"@(#)sccs:auxf.c	25.1"

# include	"../../hdr/defines.h"


/*
	Figures out names for g-file, l-file, x-file, etc.

	File	Module	g-file	l-file	x-file & rest

	a/s.m	m	m	l.m	a/x.m

	Second argument is letter; 0 means module name is wanted.
*/

char *
auxf(sfile,ch)
register char *sfile;
register char ch;
{
	static char auxfile[FILESIZE];
	register char *snp;

	snp = sname(sfile);

	switch(ch) {

	case 0:
	case 'g':	copy(&snp[2],auxfile);
			break;

	case 'l':	copy(snp,auxfile);
			auxfile[0] = 'l';
			break;

	default:
			copy(sfile,auxfile);
			auxfile[snp-sfile] = ch;
	}
	return(auxfile);
}
