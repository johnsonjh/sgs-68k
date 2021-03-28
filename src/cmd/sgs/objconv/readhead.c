#ident	"@(#)readhead.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

/* read the filehdr and get any information you may need */
void
readhead()
{
#ifdef DEBUG
	if(dflag){ 
		printf("BEGIN READHEAD\n");
		}
#endif /* DEBUG */
	FSEEK(symp, 0L, 0L);
	FREAD( &aheader, sizeof ( aheader ), 1, symp);

	/* Check the magic number */
	/* if you are changeing the magic number anyway
	 * don't bother to see if the current one is legal or not
	 */
	if(!mflag)
	if( !ISMAGIC(aheader) ){
	fprintf(stderr,"%s: Bad magic number [ %#o ] for input file %s\n",
		utilname, aheader.f_magic, infile);
		exit(1);
		}


	/* are there any symbols? */
	if( !(nsyms = aheader.f_nsyms)|| !(symptr = aheader.f_symptr)){
		fprintf(stderr,"%s: No symbols in input file %s\n",
			utilname, infile);
		exit(1);
		}
	/* save a pointer to the beginning of the string table */
	strtab=(nsyms*sizeof (struct syment)) + symptr;
#ifdef DEBUG
		if(dflag){ 
			printf("READHEAD nsyms[ %#x ] symptr[ %#x ]\n",
				nsyms, symptr);
			}
#endif /* DEBUG */
	FSEEK(symp, 0L, 0L); /* rewind the file */
} /* end readhead */
