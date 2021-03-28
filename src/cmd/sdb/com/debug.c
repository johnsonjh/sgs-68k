/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) debug.c: version 23.1 created on 11/15/90 at 13:18:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)debug.c	23.1	11/15/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:debug.c	23.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:debug.c	6.1		*/

#include "head.h"
#include "coff.h"
/*
 * debugging for sdb
 */
extern MSG		NOPCS;

prstentry(stp)	/* debug */
register struct syment *stp;
{
#if DEBUG > 1
	fprintf( FPRT2, "debug: print symbol table entry:\n" );
#ifdef	FLEXNAMES	/* dd0 */
	if ( !stp->n_zeroes )
		fprintf( FPRT2, "	n_name=  %s;\n", stp->n_nptr );
	else
		fprintf( FPRT2, "	n_name=  %s;\n", stp->n_name );
#else
	fprintf( FPRT2, "	n_name=  %s;\n", stp->n_name );
#endif	/*	FLEXNAMES	*/	/* dd0 */
	fprintf( FPRT2, "	n_value= %#lx;\n", stp->n_value );
	fprintf( FPRT2, "	n_scnum= %d;\n", (int)stp->n_scnum );
	fprintf( FPRT2, "	n_type=  %#x;\n", (int)stp->n_type );
	fprintf( FPRT2, "	n_sclass=%d;\n", (int)stp->n_sclass );
	fprintf( FPRT2, "	n_numaux=%d;\n", (int)stp->n_numaux );
#endif
}

prprocaux(axp, name)	/* debug */
AUXENT *axp;
char *name;
{
#if DEBUG > 1
#define AX	axp->x_sym
#define AXF	AX.x_fcnary
	fprintf(FPRT2, " proc auxent for %s:\n", name);
	fprintf(FPRT2, "	x_tagndx= 0x%8.8lx\n", AX.x_tagndx);
	fprintf(FPRT2, "	x_fsize=  0x%8.8lx\n", AX.x_misc.x_fsize);
	fprintf(FPRT2, "	x_lnnoptr=0x%8.8lx\n", AXF.x_fcn.x_lnnoptr);
	fprintf(FPRT2, "	x_endndx= 0x%8.8lx\n", AXF.x_fcn.x_endndx);
#endif
}

prprstk()	/* debug */
{
#if DEBUG > 1
	register int n, i;
	fprintf(FPRT2, "debug process stack:\n");
	if( pid == 0 && PC < 0 )
	{
		error( NOPCS );
		return( -1 );
	}
	else
	{
		fprintf(FPRT2, "	SDBREG(PC) = %#o\n", SDBREG(PC));
	}
#endif
}
