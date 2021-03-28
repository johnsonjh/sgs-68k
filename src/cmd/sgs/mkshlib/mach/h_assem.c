#ident	"@(#)h_assem.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/




#include <stdio.h>
#include "filehdr.h"
#include "ldfcn.h"
#include "reloc.h"
#include "syms.h"
#include "scnhdr.h"
#include "shlib.h"
#include "hst.h"

/* prlal() prints the leading lines of an assembly file */
void
prlal(assemf, curname)
FILE	*assemf;
char	*curname;
{
	/* Print leading lines of assembly file.
	 * The beginning of the assembly file should look like:
	 *
	 *      file   "<filename>"
	 *      ident  "<ident string>"
	 *      section	init,"x"
	 */
/*(void)fprintf(stderr,"\tfile   \"%s\"\n",curname);
	if (idstr != 0)
		(void)fprintf(stderr,"\n\tident  \"%s\"\n",idstr);
	(void)fputs("\n\tsection\tinit,\"x\"\n",stderr);	dux#1 */

	(void)fprintf(assemf,"\n\tfile   \"%s\"\n",curname);
	if (idstr != 0)
		(void)fprintf(assemf,"\n\tident  \"%s\"\n",idstr);
	(void)fputs("\n\tsection\tinit,\"x\"\n",assemf);
}


/* Print initialization code */
void
initpr(assemf, import, pimport)
FILE	*assemf;
char	*import;
char	*pimport;
{
	/* Generate initialization code.
	* Each initialization specification line should result in
	* the generation of the following code:
	*
	*      mov.l    &<import>, <pimport>
	*/
/*	(void)fprintf(stderr,"\tmov.l\t&%s,%s\n",import,pimport); */
	(void)fprintf(assemf,"\tmov.l\t&%s,%s\n",import,pimport);
}
