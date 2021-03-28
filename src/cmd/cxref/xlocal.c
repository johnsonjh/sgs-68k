/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) xlocal.c: version 25.1 created on 12/2/91 at 14:12:31	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)xlocal.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"cxref:xlocal.c	1.6"	*/
	
#ident	"@(#)cxref:xlocal.c	25.1"

#include <stdio.h>
#include "mfile1"
#ifndef FLEXNAMES
#	define LCHNM	8
#	define LFNM	15
#endif
#include "lmanifest"
/*
 * this file contains the functions local to CXREF
 * they put their output to outfp
 * the others contain lint's 1st pass with output thrown away
 * cgram.c has calls to these functions whenever a NAME is seen
 */

char infile[120];
FILE *outfp;

ref( i, line)
	int i, line;
{
#ifdef FLEXNAMES
	fprintf(outfp, "R%s\t%05d\n", stab[i].sname, line);
#else
	fprintf(outfp, "R%.8s\t%05d\n",stab[i].sname,line);
#endif
}

def( i, line)
	int i, line;
{
	if (stab[i].sclass == EXTERN)
		ref(i, line);
	else
#ifdef FLEXNAMES
		fprintf(outfp, "D%s\t%05d\n", stab[i].sname, line);
#else
		fprintf(outfp,"D%.8s\t%05d\n",stab[i].sname,line);
#endif
}


newf(i, line)
	int i, line;
{
#ifdef FLEXNAMES
	fprintf(outfp, "F%s\t%05d\n", stab[i].sname, line);
#else
	fprintf(outfp,"F%.8s\t%05d\n",stab[i].sname, line);
#endif
}
