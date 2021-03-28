#ident	"@(#)t_btassem.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/




#include <stdio.h>
#include "filehdr.h"
#include "ldfcn.h"
#include "shlib.h"
#include "syms.h"
#include "trg.h"


/* This function prints the leading lines of assemble code for the branch table */
void
prlbtassem(assembt)
FILE	*assembt;
{
	/* Print leading lines of assembly file.
	 * The beginning of the assemble file should look like:
	 *
	 *	file	"<filename>"
	 *	ident	"<ident string>"  #if ident string exists
	 *	text
	 */
/***********************************************************************/
/* dux#1 - removed what was apparently debug stuff */
/*	(void)fprintf(stderr,"\tfile\t\"branchtab\"\n");
	if (idstr != 0)
		(void)fprintf(stderr,"\n\tident\t\"%s\"\n",idstr);
	(void)fprintf(stderr,"\n\ttext\n");
***********************************************************************/

	(void)fprintf(assembt,"\tfile\t\"branchtab\"\n");
	if (idstr != 0)
		(void)fprintf(assembt,"\n\tident\t\"%s\"\n",idstr);
	(void)fprintf(assembt,"\n\ttext\n");
}


/* This function generates the assembly code for a label in the branch table */
void
labelassem(symname, assembt)
char	*symname;
FILE	*assembt;
{
	(void)fprintf(assembt,"\tglobal\t%s\n%s:\n",symname,symname);
/*	(void)fprintf(stderr,"\tglobal\t%s\n%s:\n",symname,symname); dux#1 */
}

/* This function generates the assembly code for the branch table */
void
genbtassem(label, assembt)
char	*label;
FILE	*assembt;
{
	/* Each entry in the branch table will have the following format:
	 *
	 *		jmp	<label name>
	 */
	(void)fprintf(assembt,"\tjmp\t%s\n",label);
/*	(void)fprintf(stderr,"\tjmp\t%s\n",label); 	dux#1 */
}


/* This function generates the assembly code for the .lib section */
void
genlibassem(assembt)
FILE	*assembt;
{
	int	size;	/* length of target pathname */
	int	i;
	char	*p;
	int	slong,
		len;

	/* The .lib contents will have the following format:
	 *
	 *	section	lib,"l"
	 *	word	num,2	# where num is the size of the current entry
	 *			# i.e. length of the target pathname + other info.
	 *	byte	<target pathname in hex>
	 */
	(void)fprintf(assembt,"\n\n\tsection\tlib,\"l\"\n\n");
/*	(void)fprintf(stderr,"\n\tsection\tlib,\"l\"\n\n");	dux#1 */
	len= strlen(trgpath)+1;
	slong= sizeof(long);
	size= (len+(slong-1))/slong + 2; 	/* the last term represents the two
						   leading words- the 1st describes
						   the total length and the 2nd
						   describes the offset of the
						   non-optional information */
/**************************************************************************/
/* dux#1 - removed what was apparently debug stuff */
/*	(void)fprintf(stderr,"\tlong\t%d,2\n",size);
	(void)fprintf(stderr,"\tbyte\t");
	for (p=trgpath;*p!='\0';p++) {
		(void)fprintf(stderr,"0x%x,",*p);
	}
	(void)putc('0',stderr);
**************************************************************************/

	(void)fprintf(assembt,"\tlong\t%d,2\n",size);
	(void)fprintf(assembt,"\tbyte\t");
	for (p=trgpath;*p!='\0';p++) {
		(void)fprintf(assembt,"0x%x,",*p);
	}
	(void)putc('0',assembt);
	(void)fprintf(assembt,"\n");
}
