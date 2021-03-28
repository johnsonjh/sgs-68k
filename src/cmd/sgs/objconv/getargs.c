#ident	"@(#)getargs.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

/* print out usage error message */
usage(){
#ifdef DEBUG
	if(dflag) printf("usage\n");
#endif /* DEBUG */

	fprintf(stderr,"usage: %s [ -inrvw ] [ -m magic ] [-o outfile ] infile\n",
	utilname);
	exit(1);
} /* end usage */

/* process the arguement line */
getargs(ac,av)
int ac;
char *av[];
{
int c;
extern char *optarg;	
extern int optind;
#ifdef DEBUG
	if(dflag) printf("getargs\n");
#endif /* DEBUG */

	/* are there enough arguments? */
	if(ac < 2)usage();
while(optind < ac){
	while ((c = getopt(ac, av, "idm:nprvwo:")) != EOF)
		switch(c){
		case 'i':iflag++; break;	/* interactive */
		case 'o':
			outfile = optarg;	break; /* name an output file */
		case 'm':
			mflag++;
			localmagic = myatol(optarg);break;/* change magic no only */
		case 'd': dflag++; break;
		case 'p': pflag++; break;
		case 'n': vflag++;nflag++; break;
		case 'r': rflag++; break;	/* change reloc info only */
		case 'v': vflag++; break;	/* verbose */
		case 'w': wflag++; break;
		case '?':
		default:
			exit(1);
			
		} /* end switch */
		if(&infile[0] == '\0')
			infile = av[optind];
		optind++;
	}
	if (infile == NULL)usage();
	if( !(strcmp(infile,outfile))){
		fprintf(stderr,"%s: Input file name must be different from Output file name\n",utilname);
		exit(1);
		}
} /* end getargs */
