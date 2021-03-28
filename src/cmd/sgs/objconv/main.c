#ident	"@(#)main.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

main(argc, argv)
register int argc;
register char *argv[];
{

#ifdef DEBUG
	if(dflag) printf("main\n");
#endif /* DEBUG */

	utilname=argv[0];
	getargs(argc,argv);

	/* open the input file for reading */
	if ( !( symp = fopen( infile, "r" )) 
	  || !( strp = fopen( infile, "r" ))){
		fprintf(stderr,"%s: cannot open input file %s for reading\n",
			utilname,infile);
		exit(1);
		}
	/* read the file header for any pertinent info */
	readhead();
	readsyms();
	if(dflag && pflag) printsyms();
	if(!mflag && iflag)interactive();
	else if(!rflag && !mflag) fixsyms(); /* alter names */

	if(pflag && dflag) printsyms();

	fseek(symp,0L,0L);
	/* write out the file */
	if(!nflag)
		writesyms(outfile,symp);
	exit(0);
}
