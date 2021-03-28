#ident	"@(#)main.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	main.c	7.2		*/
/*	Copyright 1985 Motorola Inc.			*/

# include "defin.h"
extern char* getenv();

/*F*********************** FUNCTION HEADER *******************************

 main

	Controls the execution of the optimizer.

PARAMETERS:

	Receives:	argc: number of arguments on command line
			argv: array of pointers to command line entries

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	symflag
	clseonly
	bbflag
	labelsht
	lcseht
	constht
	atendofblock

FUNCTIONS CALLED:

	p2init
	hcreat
	initfunc
	gatherfunc	- gather.c
	scopedump	- symtab.c
	optimize	- hloptim.c
	writefunc
	writestats
	prflowgraph

CALLED BY:

	no body

*FE********************************************************************/

main( argc, argv ) 

int argc;
char *argv[]; 
{
	register files;
	int noteof;
	int fdef=0;

	files = p2init( argc, argv );
	tinit();

	if( files ){
		while( files < argc ) {
			if( *(argv[files]) != '-' ) switch( ++fdef )
			{
			case 1:
			case 2:
				if( freopen(argv[files], fdef==1 ? "r" : "w", fdef==1 ? stdin : stdout) == NULL)
				{
					fprintf(stderr, "ccom:can't open %s\n", argv[files]);
					exit(1);
				}
				break;

			default:
				cerror("usage error: coptim [<options>] [<source file> [<destination file>]] : \"%s\" purpose unknown", argv[files]);
			}
			++files;
		}
	    }
	hcreat(&labelsht, 64);
	hcreat(&lcseht, 31);
	hcreat(&constht, 31);
	for (;;)
	   {
	   initfunc();
	   if (gatherfunc())
	      {
# ifndef NODBG
	      if (symflag)
		 scopedump(); /* dump scope information */
# endif
	      if ( ! clseonly)
		 {
		 atendofblock = TRUE;
	         optimize();
		 atendofblock = FALSE;
		 }
	      writefunc();
# ifndef NODBG
	      writestats();
	      if (bbflag)
		 prflowgraph();
# endif
	      }
	   else    /* end of file */
	      break;
	   }

	return ( 0 );

}


/*F*********************** FUNCTION HEADER *******************************

 p2init

	Process command line options

PARAMETERS:

	Receives:	argc
			argv

	Returns:	number of file names encountered

EXTERNAL DATA REFERENCES:

	regflag
	lcseflag
	statsflag
	bbflag
	lcsedbflg
	symflag
	odebug
	align_val (if m68k defined)

FUNCTIONS CALLED:

	mkdope
	getenv
	strcmp

CALLED BY:

	main

*FE********************************************************************/

int
p2init( argc, argv )
char *argv[];
{
	register int c;
	register char *cp;
	register files;
# ifndef NODBG
	register char* statsname;
# endif
#if defined( M68881 ) || defined( m68k ) 
	char *strval;
#endif

	files = 0;

	for( c=1; c<argc; ++c )
	{
		if( *(cp=argv[c]) == '-' )
		{
			while( *++cp )
			{
				switch( *cp )
				{
				case 'C':  /* Turn off constant to register promotion */
					conflag++;
					break;

				case 'R':  /* Turn off register allocation */
					regflag = FALSE;
					break;

				case 'L':  /* Turn off lcse */
					lcseflag = FALSE;
					break;

				case 'S':  /* Statistics */
					++statsflag;
# ifndef NODBG
					if (statsflag == 1)
					   {
					   if ((statsname = getenv("STATSFILE")) == NULL)
					      cerror("STATSFILE not defined within current environment,\n\t statistics can not be collected.");
					   if ((statsfile = fopen(statsname,"a")) == NULL)
					      cerror("%s can not be opened",statsname);
					   fprintf(statsfile, "\n\tfuncname          nodesout    aregstat    fpregstat   lcsestat    scopestat   loopstat\n");
					   fprintf(statsfile, "\t            nodesin     bbcnt       dregstat    symtabndx   valueno     tempstat\n");
					   }
# endif
					break;

				case 'b':  /* basic blocks */
					++bbflag;
					break;

				case 'l':  /* local common subexpression flag */
					++lcsedbflg;
					break;

				case 's':  /* symbol table */
					++symflag;
					break;

				case 'o':  /* orders */
					++odebug;
					break;
				default:
					cerror( "bad option: %c", *cp );
				}
			}
		}
		else files = 1;  /* assumed to be a ftitle */
	}

#ifdef m68k
# ifdef DFLTDBLALIGN
	if( ( ( strval = getenv( "DBLALIGN" ) ) != NULL ) &&
	    ( ! strcmp( strval, "NO" ) ) )
	{
		align_val = 16; /* word align */
	}
# else
	if( ( ( strval = getenv( "DBLALIGN" ) ) != NULL ) &&
	    ( ! strcmp( strval, "YES" ) ) )
	{
		align_val = 32;	/* double word align */
	}
# endif
#endif

	mkdope();
	return( files );
}
