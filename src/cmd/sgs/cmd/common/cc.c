#ident	"@(#)cc.c	23.2	1/10/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)cc.c	6.1		*/
static	char sccsid[] = "@(#) cc.c:	2.3 83/07/08";
/*
 * cc - front end for C compiler
 */
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/fs/s5dir.h>	/* dux0 */
#include <sys/stat.h>
#include "paths.h"
#include "sgs.h"	/* for -V version option, to get RELEASE */
#include "protos.h"		/* function prototypes */
#include <fcntl.h>

#ifndef	BINDIR				/* default /bin directory */
#define	BINDIR	""
#endif

#ifndef	LIBDIR				/* default /lib directory */
#define	LIBDIR	""
#endif
#ifndef	NAMEcpp
char *nameCPP = "cpp";                  /* base preprocessor name */
#else
char *nameCPP = NAMEcpp;
#endif
char *PATHcpp = LIBDIR;

#ifndef NAMEc0
#define	NAMEc0	"c0"			/* name of front end */
#endif
#ifndef NAMEcoptim
#define NAMEcoptim	"coptim"	/* name of high level optimizer */
#endif
#ifndef	NAMEc1
#define	NAMEc1	"c1"			/* name of code generator */
#endif

#ifndef NAMEas
char *nameAS =	"as";			/* base assembler name */
#else
char *nameAS =	NAMEas;			/* base assembler name */
#endif
char *PATHas = BINDIR;

#ifndef	NAMEoptim
char *nameOPTIM = "optim";              /* base optimizer name */ 
#else
char *nameOPTIM = NAMEoptim;
#endif
char *PATHoptim = LIBDIR;

#ifndef	NAMEld
#define	NAMEld	"ld"			/* base loader name */
#endif

/* its important to have a null character after the names of the libs to
   be able to append an 'n' if run with -i option */

char *nondy_str = "\0";

# ifndef NAMElibc
char *nameLIBC = "c\0";     /* ANSI library */
# else
char *nameLIBC = NAMElibc;
# endif

				
# ifndef NAMElibca
char *nameLIBCA = "ca\0";     /* STRICT ANSI library */
# else
char *nameLIBCA = NAMElibca;
# endif

# ifndef NAMElibc5
char *nameLIBC5 = "c5\0";     /* STRICT ANSI library */
# else
char *nameLIBC5 = NAMElibc5;
# endif

# ifndef NAMEcrt1
# define NAMEcrt1       "crt1.o"        /* default C startup name */
# endif
# ifndef NAMEmcrt1
# define NAMEmcrt1      "mcrt1.o"		/* for profiling */
# endif
# ifndef NAMEcrt2
# define NAMEcrt2       "crt2.o"        /* Strict ANSI C startup name */
# endif
# ifndef NAMEmcrt2
# define NAMEmcrt2      "mcrt2.o"		/* for profiling: Strict ANSI */
# endif
# ifndef NAMEcrt5
# define NAMEcrt5       "crt5.o"        /* SYSV startup routine */
# endif
# ifndef NAMEmcrt5
# define NAMEmcrt5      "mcrt5.o"		/* for profiling: SYSV */
# endif
/* ken#0: for port temporary -- what is crts.o  used for?
# ifndef NAMEcrts
# define NAMEcrts       "crts.o"
# endif
*/
# ifndef NAMEcrtn
# define NAMEcrtn       "crtn.o"        /* share libs base end name */
# endif

char	*cpp;				/* pointer to true cpp name */
char	*c0;				/* same for front end */
char	*c1;
char	*coptim;
char	*optim;
char	*as;				/* ... etc. */
char	*ld;
char	*libploc;

char    *crt1;
char    *crtn;

char *Ssave = (char *) 0;		/* bk */
char	*prefix;			/* prefixing string in cc command;
					** i.e., "mc68" in "mc68cc"
					*/
#define	Xc0	0			/* numbers for passes */
#define	Xc1	1
#define Xcoptim	2
#define	Xoptim	3
#define	Xcpp	4
#define	Xas	5
#define	Xld	6
#define	NPASS	7


char	*tmp0;		/* holds temp. file template */
char    *tmp1, *tmp2, *tmp3, *tmp4, *tmp5, *tmp6;
char	*outfile;
char	*savestr(), *strspl(), *setsuf(), *getpref(), *makename();
char	*strtok(), *stralloc();
char	*OpEnv;		/* OPTIM environment variable - controls whether
			** high level optimizer, peephole optimizer, or
			** both should be used */
char    *SvEnv;         /* SAVEIR environment variable - controls saving of
			   temporary work files for debuggin purposes */
char    *StEnv;         /* STATSFILE environment variable - controls file
			   name for coptim statistics */

int sys5flag = 0;	/** Default: don't generate SYS5 binaries **/
int iflag    = 0;	/* -b means to use the NON-DYMANIC libs */
int cuc_flag = 0;	/* TEMPORARY!!!!! Until "cuc" is no more */

#ifdef M68881
int     fflag;          /* generate Unix Conforming floating-point code */
			/* set when mathlib request "-lm" seen in list */
			/* passes -Xf to c0 */
#endif

/* Display format for "cc" version information (-V option) */
#if defined __STDC__
#define SHOW_VERSION()	fprintf(stderr,"%scc: %s (%s)\n", 	\
			prefix, cuc_flag ? CUC_RELEASE : RELEASE, __DATE__)
#else
#define SHOW_VERSION()	fprintf(stderr,"%scc: %s\n", 	\
			prefix, cuc_flag ? CUC_RELEASE : RELEASE)
#endif

int	ysflag = 0;		/* if > 0 the -YS has been specified */
int	hlflag;		/* if > 0 then execute coptim (intermediate optimizer)*/
int	peepflag;	/* if > 0 then execute optime (peephole optimizer) */
int	irflag;		/* if > 0 then save intermediate files */
int emuflag=0;	/* if > 0 then link with libFPSP.a */
int	idexit();
char	**av, **clist, **llist, **plist;

				/********* Pipeline Control ************/
typedef struct {
	char *proc;
	char **argv;
} t_proc;
t_proc	procs[7];	/* 7 entries (cpp, c0, coptim, c1, optim, as, 0) */
int pp_idx;
char **av_cpp, **av_c0, **av_coptim, **av_c1, **av_optim, **av_c1, **av_as;
char **av_gen;
int pipeit;
#define CHILD	0
#define FORKERR	(-1)

#if defined __STDC__
#define PASTE(x,y)	x ## y
#else
#define PASTE(x,y)	x/*paste*/y
#endif	/* __STDC__ */

/* Preparation for each component: 
 *				initialize argument vector (av[]), and
 *				set av[0] to name of component (e.g. NAMEc0)
 */

#define AV_PREP(x)	{						\
		na = 0;							\
		av = pipeit ? PASTE(av_,x) : av_gen;			\
		av[na++] = PASTE(NAME,x);				\
			}

#define AV_SAVE(x)	{						\
		av[na++] = 0; 						\
		if ( pipeit ) {						\
			procs[pp_idx].proc = x; 			\
			procs[pp_idx++].argv = av;			\
		}							\
			}
		/***************************************/

char	**list[NPASS];	/* list of lists */
int	nlist[NPASS];	/* current index for each list */
int	limit[NPASS];   /* length of each list */
int	argcount;	/* initial length of each list */

int	cflag, eflag, gflag, pflag, sflag, wflag = -1,
	exflag, proflag, oflag, debug, Gflag, mode = -1, bflag, foldflag = -1;

int 	ass_only_flg; /* assemble only flag . It's an internal flag */
int 	truncflag;	/* non-zero to truncate flexnames */
			/* Note: default is to compile for mpx(1) */
char	*dflag;
char	*chpass;
char	*npassname;
extern	char	*getenv();
extern	int	optind;
extern	int	opterr;
extern	char	*optarg;
extern	int	optopt;
extern  char    *tempnam();

int	nc, nl, np, nxo, na;

#define	cunlink(x)	if (x) unlink(x)

main(argc, argv)
	int  argc;
	char **argv;
{
	char *t;
	char *assource;
	char *c0source;
	char *chpiece = NULL,	/* list of pieces whose default location */
				/* has been changed by -Y option	 */
	     *npath = NULL;     /* new path, for pieces changed by -Y    */
	int i, j, c, ic;
	int nargs = argc + 1;
	int justS = 1;

	/* There is a problem with nested makefiles on 4.2BSD that causes
	 * programs to run out of open file descriptors.  This hack
	 * closes enough files for the C compiler to work, but 'make'
	 * should really be fixed.
	 */

	for(i=5;i<20;i++)close(i);

	/* build initially assumed program names */

	prefix = getpref(argv[0]);	/* get prefix string */

	cpp = makename(LIBDIR,prefix,nameCPP);		/* C preprocessor */
	c0 = makename(LIBDIR,prefix,NAMEc0);		/* front end */
	c1 = makename(LIBDIR,prefix,NAMEc1);		/* code generator */
	coptim = makename(LIBDIR,prefix,NAMEcoptim);	/* intermediate optim.*/
	optim = makename(LIBDIR,prefix,nameOPTIM);	/* optimizer */
	as = makename(BINDIR,prefix,nameAS);		/* assembler */
	ld = makename(BINDIR,prefix,NAMEld);		/* loader */

	crt1 = makename(LIBDIR,prefix,NAMEcrt1);/* run-time start-up */
	crtn = makename(LIBDIR,prefix,NAMEcrtn);/* run-time link end */

							/* parallel execution of compiler phases */
	pipeit = getenv("CCPIPE") != NULL;

	/* ld currently adds upto 5 args; 10 is room to spare */
	/* I upped it anyway -- rob */

	if ( pipeit ) {
		/* arg vectors for phases participating in pipeline */
		av_cpp = (char **) stralloc((argc+20) * sizeof(char **));
		av_c0 = (char **) stralloc((argc+20) * sizeof(char **));
		av_coptim = (char **) stralloc((argc+20) * sizeof(char **));
		av_c1 = (char **) stralloc((argc+20) * sizeof(char **));
		av_optim = (char **) stralloc((argc+20) * sizeof(char **));
		av_as = (char **) stralloc((argc+20) * sizeof(char **));
		av_gen = (char **) stralloc((argc+20) * sizeof(char **));
	} else
		av_gen = (char **) stralloc((argc+20) * sizeof(char **));



	/* initialize arrays for extra arguments (-W) */

	argcount = argc + 6;
	c = sizeof(char *) * argcount;
	for (i = NPASS; i-- > 0; ) {
	    nlist[i] = 0;
	    list[i] = (char **) stralloc(c);
	    limit[i] = argcount;
	}
	clist = (char **) stralloc(c);
	llist = (char **) stralloc(c);
	plist = (char **) stralloc(c);
	opterr = 0;		/* Disable error messages from getopt */

/* option string for getopt() */

/* NOTE: order of options in list: general cc, cpp, optim, as, ld  */
#define OPTS "cgpAGOSfbiT:V#w:F:M:W:X:Y:Z:ECHPI:D:U:bB:KahmrstxzNQ0l:n:o:u:J:L:"

	while (optind < argc) {
		c = getopt(argc, argv, OPTS);
		justS++;
		switch (c) {

		case '0':	
			{
				/* Pass to ld for "allow deref of zero" */
				addopt(Xld,"-0");
				continue;
			}
		case 'i':	
			{	/* 
				   cpp with -D_NODYNAMIC link with 
				   -L/usr/lib/NODYNAMIC
				*/
				addopt(Xcpp,"-D_NODYNAMIC");
				nondy_str = "n";
				continue;

			}
		case 'T':
			{
				if ( strlen( optarg ) > 1 ) {
					error( "Invalid machine type %s. Use one of 2, 3, 4.",
						optarg );
				}
				switch ( *optarg ) {

				case '2':	/* M68020 */
					nameAS = "as2";
                                        nameCPP = "cpp2";
					nameOPTIM = "optim2";
					break;
				case '3':	/* M68030 */
					nameAS = "as3";
					break;
				case '4':	/* M68040 */
					nameAS = "as4";
                                        nameCPP = "cpp4";
					nameOPTIM = "optim4";
					break;

				default:
					error( "Invalid machine type %c. Use one of 2, 3, 4.",
						*optarg );
					exit(8);
					break;	/* Not reached */
				}

				/* Use as2, as3, or as4 as the assembler. */
				as = makename( PATHas, prefix, nameAS );
	                        cpp = makename(PATHcpp,prefix,nameCPP );
	                        optim = makename(PATHoptim,prefix,nameOPTIM);
			}
			continue;
		case 'B':				/* control optimizer inline function folding */
			if ( sscanf( optarg, "%d", &foldflag ) != 1 || foldflag < 0 ) {
				error( "Specify max function folding: -B[number]" );
				exit(8);
			}
				/* we'll add option below */
			continue;
		case 'b':				/* don't optimizer out frame pointer */
			bflag = 1;			/* for usage checks later on */
			addopt(Xoptim,"-a");
			continue;
		case 'K':				/* assembly listing */
			addopt(Xas,"-K");
			continue;
		case 'A':			/* produce A1000 binaries */
			addopt(Xc1,"-A0");			/* insert stack probes */
			addopt(Xas,"-A");			/* create A1000 .o */
			addopt(Xld,"-A");			/* link using A1000 section 
										   directives*/
			continue;
		case 'M':			/* compiler mode */
			if ( !isdigit(optarg[0]) ||
				 ( (mode = atoi(optarg)) < MODE_CUC ) || 
				 ( mode > MODE_STRICT_ANSI ) ) {
				error( "Specify ANSI C Mode: -M[0-2]" );
				exit(8);
			}
			continue;
		case 'w':			/* warning level */
			wflag = atoi(optarg);
			if ( ! W_LVLCHK( wflag ) ) {
				error( "Specify warning level: -w[0-%d]", W_LVLMAX );
				wflag = -1;
			}
			continue;
		case 'F':				/* floating point rounding precision */
			switch (optarg[0]) {
				case 's':		/* single precision */
				case 'd':		/* double precision */
					t = stralloc(20);
					sprintf( t, "-J_fp_prec=_fp%cprec", optarg[0] );
					addopt(Xld,t);
					break;
				case 'e':		/* extended precision (default) */
					break;
				default:
					error( "Specify floating point rounding: -F[s|d|e]", 0 );
					break;		/* extended is the default */
			}
			continue;
		case 'f':				/* floating point emulation of the 68881 instructions. Useful if you are on an 040.*/
				addopt(Xc1,"-F");
				emuflag++;
				continue;
		case 'G':				/* generate prototypes */
			Gflag++;
			continue;
		case 'S':
			sflag++;
			cflag++;
			justS = 0;			/* possibly -SYSV: only for CUC compiler */
			continue;
		case 'l':
#ifdef M68881
		/* if mathlib requested turn on Unix Conform flag for c0 */
			if (strcmp("m",optarg)==0) fflag++;
#endif
			llist[nl++] = strspl("-l", optarg);
			continue;
		case 'o':
			outfile = optarg;
			switch (getsuf(outfile)) {

				case 'c':
				case 'o':
					error("-o would overwrite %s", outfile);
					exit(8);
			}
			continue;
		case 'O':
			oflag++;
			continue;
		case 'p':
			proflag++;
			continue;
		case '#':
			debug++;
			continue;
		case 'g':
			gflag++;
			continue;
		case 'E':
			exflag++;
		case 'P':
			pflag++;
/*			if (optarg != 0)		*** bk: getopt doesn't work this way! **
			    plist[np++] = optarg; */
		case 'c':
			cflag++;
			continue;
		case 'H':					/* bk: somehow, cc was missing this one */
		case 'C':
			plist[np] = strspl("-", "x" );
			plist[np++][1] = c;
			continue;
		case 'D':
		case 'I':
		case 'U':
			plist[np] = strspl("-X", optarg);
			plist[np++][1] = c;
			continue;
		case 'W':
			if((optarg[1] != ',') || ((t = strtok(optarg, ",")) != optarg)){
				error("Invalid subargument: -W%s", optarg);
				continue;
			}
			if((i = getXpass((ic = *t), "-W")) == -1)
				continue;
			while((t = strtok(NULL, ",")) != NULL) {
				if(nlist[i] >= nargs){
					error("Too many arguments for pass -W%c", ic);
					continue;
				}
				list[i][nlist[i]++] = t;
			}
			continue;

		case 'Y':	
			if ( ( justS == 1 ) && ( strcmp(optarg,"SV") == 0 ) ) {
				sflag--;		/* kludge! */
				cflag--;
				sys5flag = 1;
			} else {		/* normal code follows... */
				if (((chpiece=strtok(optarg,",")) != optarg) ||
					((npath=strtok(NULL,",")) == NULL)) {
					fprintf(stderr,"Invalid argument: -Y %s\n",optarg);
					exit(1);
				} else if ((t=strtok(NULL," ")) != NULL) {
					fprintf(stderr,"Too many arguments: -Y %s,%s,%s\n",
						chpiece, npath,t);
					exit(1);
					}
				chg_pathnames(prefix, chpiece, npath);
			}
			continue;

		case 'Z':					/* ken#1 */
			{char *zptr;
			zptr=(char *)malloc(strlen(optarg)+3);
			strcpy(zptr,"-Z");
			strcat(zptr,optarg);
			addopt(Xas,zptr);
			addopt(Xld,zptr);
			}
			continue;

		case 'V':	/* version flag or ld VS flag */
			if(strncmp("-VS",argv[optind],3)){
				addopt(Xas,"-V");
				addopt(Xld,"-V");
				SHOW_VERSION();
			} else {
				/* to set the static variable in getopt to 0
				   so that we can read the next argument */
				c=getopt(argc,argv,"S");
				c=getopt(argc,argv,NULL);
				addopt(Xld,"-VS");
				addopt(Xld,argv[optind++]);
			}
			break;
		case 'd':
			dflag = optarg;
			continue;
		case 'X':	/* bk: command line pragma directives */
			t = stralloc( strlen(optarg) + 3 );
			sprintf( t, "-X%s", optarg );
			addopt(Xcpp,t);
			continue;
		case 'J':			/* bk: Linker Symbolic Equate */
		case 'n':			/* bk: Linker Namespace File */
		case 'L':
			t = stralloc(strlen(optarg) + 3);
			sprintf( t, "-%c%s", optopt, optarg );
			addopt(Xld,t);
			continue;

		case '?':	/* opt char not in optstr: EXIT */
			switch ( optopt ) {			/* first, check for missing optarg */
				case 'o':
					error("No output file found for '-o' option\n",outfile);
					exit(8);
				case 'I':
				case 'D':
				case 'U':
				case 'W':
				case 'Y':
				case 'Z':
				case 'l':
				case 'u':
				case 'L':
					error( "Missing argument for '%c' option\n", optopt );
					exit(8);
				default:			/* sorry, not in OPTString */
					break;
			}
			error( "Invalid option '%c'\n", optopt );
			exit(8);

		case 'u':					/* force undefined symbol in linker */
			addopt(Xld,strspl("-u", optarg));
			continue;

		case 'a': case 'm': case 'r': case 't':	/* miscellaneous linker opts */
		case 'x': case 'z': case 's': case 'N': case 'Q':
		case 'h':
			t = strspl("-", "x");
			t[1] = optopt;
			addopt(Xld,t);
			continue;
	
		case EOF:
			t = argv[optind];
			optind++;
			c = getsuf(t);
			if (c=='c' || c=='s' || c=='i' || exflag) {
				clist[nc++] = t;
				t = setsuf(t, 'o');
			}
			if (nodup(llist, t)) {
				llist[nl++] = t;
				if (getsuf(t)=='o')
					nxo++;
			}
		}
    	}
	

	{	 /*** bk: set ANSI conformance mode and related flags ***/

		if ( cuc_flag ) {
			wflag = -1;
			mode = MODE_CUC;
		}

		if ( sys5flag ) {				/* special case: SYSV OS compatibility*/
			if ( mode >= 0 && mode != MODE_CUC )
				fprintf(stderr, "Warning, SYSV option forces CUC mode\n" );
			mode = MODE_CUC;
												/* SYS5 runtime startup */
			crt1 = makename(Ssave ? Ssave : LIBDIR, prefix,NAMEcrt5);
		} else {
			if ( mode < 0 )					/* not set? */
				mode = MODE_ANSI;			/* default mode */
			if ( fflag ) {
				fprintf(stderr, "Warning, superfluous -lm ignored\n" );
				fflag = 0;
			}
		}

		if ( mode != MODE_ANSI && !cuc_flag ) {	/* non-default mode? */
			t = stralloc( 5 );				/* alert components */
			sprintf( t, "-XM%d", mode );
			addopt(Xc0,t);
			t = stralloc( 4 );
			sprintf( t, "-M%d", mode );
			addopt(Xcpp,t);
			addopt(Xld,t);

			if ( mode == MODE_STRICT_ANSI ) {	/* set warnings to highest lvl*/
				if ( wflag == -1 )			/* user hasn't set it */
					wflag = W_STRICT;		/* make stricter warning level */
											/* Strict ANSI runtime startup */
				crt1 = makename(Ssave ? Ssave : LIBDIR, prefix,NAMEcrt2);
			}
		}

		if (wflag >= 0) {			/* warning level */
			t = stralloc( 4 );
			sprintf( t, "-L%d", wflag );
			addopt(Xcpp,t);
			t = stralloc( 5 );
			sprintf( t, "-Xw%d", wflag );
			addopt(Xc0,t);
			t = stralloc( 4 );
			sprintf( t, "-w%d", wflag );
			addopt(Xld,t);
		}
	}

									/* optimizer options w/out -O? */
	if ( !oflag && (bflag || foldflag >= 0) ) {
		if ( gflag )
	        error("Warning, -g overrides optimizer flags", "");
		else {
			error( "Warning, optimizer flags activate -O option", "" );
			oflag = 1;
		}
		eflag--;	/* don't cause error exit */
	}
	if ( bflag ) {			/* NOTE: b flag has precedence over B flag */
		if ( foldflag > 0 ) {
			error( "Warning, -b option eliminates function folding", "" );
			eflag--;	/* don't cause error exit */
		}
	} else {
		if ( foldflag >= 0 ) {
			t = stralloc( strlen(optarg) + 3 );
			sprintf( t, "-z%d", foldflag );
			addopt(Xoptim, t );
		}
	}

	hlflag = 0;
	peepflag = 0;
	if (oflag && !gflag)
	{ 
	   /* coptim used if:
		-O specified and it exists, unless OPTIM environment flag
		is present and specifies neither "BOTH" or "HL".     */

	   struct stat statbuf;

	   OpEnv = getenv("OPTIM"); /* controls which optimizer is invoked */

	   if (stat(coptim,&statbuf)==0) {  /* it exists */
			hlflag = 1;
			if ((OpEnv != NULL) && ((strcmp(OpEnv,"BOTH") != 0) &
			       (strcmp(OpEnv,"HL") != 0))) hlflag = 0;
		}

	   peepflag = 1;
	   if ((OpEnv != NULL) && (strcmp(OpEnv,"HL") == 0)) peepflag = 0;

	   /* Should intermediate files be saved? */

	   irflag = 0;
	   SvEnv = getenv("SAVEIR");
	   if ((SvEnv != NULL) && (strcmp(SvEnv, "TRUE") == 0))
	      irflag = 1;
	}
	if ( oflag && gflag ) {
	        error("Warning, -g overrides -O", "");
		oflag = 0;
		eflag--;	/* don't cause error exit */
		cflag--;
	}

				/* profiling: prepare name of startup */
	if (proflag) {
		if ( sys5flag )
			crt1 = makename(Ssave ? Ssave : LIBDIR,prefix, NAMEmcrt5 );
		else if ( mode == MODE_STRICT_ANSI )
			crt1 = makename(Ssave ? Ssave : LIBDIR,prefix, NAMEmcrt2 );
		else
			crt1 = makename(Ssave ? Ssave : LIBDIR,prefix, NAMEmcrt1);
	}

	if (oflag && hlflag)
		++gflag;	/* sdb information needed for coptim */
	if (nc==0)
		if ( nl == 0 )
			error("No files supplied!!", "");
		else
			goto nocom;
	if (eflag)
		dexit();
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, idexit);
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		signal(SIGTERM, idexit);
	tmp0 = tempnam(NULL,"ctm");	/* form temporary filename template */
	if (pflag==0) {
		if ( Gflag == 0 ) {
			tmp3 = strspl(tmp0, "3");
		}
		tmp1 = strspl(tmp0, "1");
		tmp4 = strspl(tmp0, "4");
		tmp6 = strspl(tmp0, "6");
	}
	if (oflag)
		{
		if (hlflag)
		   tmp2 = strspl(tmp0, "2");
		if (peepflag)
		   tmp5 = strspl(tmp0, "5");
		}
				/* OBJECT FILE LOOP */
	for (i=0; i<nc; i++) {
		if (nc > 1) {
			printf("%s:\n", clist[i]);
			fflush(stdout);
		}

		pp_idx = 0;							/* reset pipeline process index */

		switch (getsuf(clist[i]) )
		{
		case 'i':							/* skip preprocessor phase */
			assource = tmp3;
			c0source = clist[i];
			goto compmain;
		case 's':
			if (!pflag && !Gflag) {
				assource = clist[i];
				ass_only_flg = 1;
				goto assemble;
			}
			/* fallthrough */
		default:
			assource = tmp3;
			if (pflag)
				tmp4 = setsuf(clist[i], 'i');
			c0source = tmp4;
			break;
		}

		/* PREPROCESSOR */

		AV_PREP(cpp)				/* start building arguments */
#ifdef  PORTAR 
		av[na++] = "-DPORTAR";		/* Archive version */
#endif

#ifdef	INT16BIT
	 /*     av[na++] = "-Dmc68k16";         /* signify 16-bit version */
#else
	 /*     av[na++] = "-Dmc68k32";         /* signify 32-bit version */
#endif

		if ( sys5flag )
			av[na++] = "-D_SYSV";			/* bk: SYSTEM V header info */
		else
			av[na++] = "-D_POSIX_SOURCE";	/* bk: POSIX header info */

		if ( mode == MODE_STRICT_ANSI )		/* strictly conforming program */
			av[na++] = "-D__STRICT_STDC__";	/* bk: restrict header info */

		for (j = 0; j < np; j++)
			av[na++] = plist[j];
#ifdef	USR_INC
		av[na++] = USR_INC;	/* default search dirs for cpp */
#endif
		if (truncflag)			/* pass -XT if truncating */
		    av[na++] = "-T";

		for (j = 0; j < nlist[Xcpp]; j++)
		    av[na++] = list[Xcpp][j];

		av[na++] = clist[i];
		if ( pflag || !pipeit ) {
			av[na++] = exflag ? "-" : tmp4;
			AV_SAVE(cpp)
			if (callsys(cpp, av)) {
				cflag++;
				eflag++;
				continue;
			}
			if ( pflag ) {
				cflag++;
				continue;
			}
		} else { 		/** else, build next component's pipeline arg vector */
			AV_SAVE(cpp)
		}

compmain:
		if (sflag)
			assource = tmp3 = setsuf(clist[i], 's');
		if (irflag)
		   tmp1 = setsuf(clist[i], 'x');

		   /* PARSER */

		AV_PREP(c0)				/* start building arguments */
		if (gflag)		/* must be first arg */
		    av[na++] = (hlflag && !cuc_flag) ? "-O" : "-g";
		if (truncflag)		/* if truncating names */
		    av[na++] = "-XT";	/* pass flag */
#ifdef M68881
		if (fflag)
		    av[na++] = "-Xf"; /* Unix Conforming floating-point */
#endif
		for (j = 0; j < nlist[Xc0]; j++)
		    av[na++] = list[Xc0][j];

		if ( !pipeit ) {
			av[na++] = c0source;
			av[na++] = tmp1;
			av[na++] = tmp6;        /* pass in-line temporary file */
		}

		if (proflag)
			av[na++] = "-XP";


		if ( Gflag ) { 				/* pass prototype output file */
			if ( pipeit ) {
				av[na++] = "+"; /* input file for c0 = stdin */
				av[na++] = "+"; /* output file for c0 = stdout */
				av[na++] = "+"; /* asm macro temp file for c0 = NULL */
				av[na++] = setsuf(clist[i], 'p');
				AV_SAVE(c0)
				if (pipeline(procs, pp_idx)) {
					cflag++;
					eflag++;
					continue;
				}		/* no more phases after protogen */
			} else {
				av[na++] = setsuf(clist[i], 'p');
				AV_SAVE(c0)
				if (callsys(c0, av)) {
					cflag++;
					eflag++;
					continue;
				}
			}
			cflag++;
			continue;
		}

		AV_SAVE(c0)
		if ( !pipeit ) {
			if (callsys(c0, av)) {
				cflag++;
				eflag++;
				continue;
			}
		}

		/* INTERMEDIATE OPTIMIZER */

		if (oflag && hlflag)
		{
		   AV_PREP(coptim)				/* start building arguments */
		   if (irflag)
		      tmp2 = setsuf(clist[i], 'y');
		   for (j = 0; j < nlist[Xcoptim]; j++)
		      av[na++] = list[Xcoptim][j];
		   StEnv = getenv("STATS");
		   if ((StEnv != NULL) && (strcmp(StEnv, "COLLECT") == 0))
		      av[na++] = "-S";
			if ( !pipeit ) {
				av[na++] = tmp1;
				av[na++] = tmp2;
			}

			AV_SAVE(coptim)
			if ( !pipeit ) {
				if (callsys(coptim, av)) {
					cflag++;
					eflag++;
					continue;
				}
			}

		}

		/* CODE GENERATOR */
		AV_PREP(c1)				/* start building arguments */
		for (j = 0; j < nlist[Xc1]; j++)
		    av[na++] = list[Xc1][j];

		if ( !pipeit ) {
			av[na++] = oflag && hlflag ? tmp2 : tmp1;
			av[na++] = oflag && peepflag ? tmp5 : tmp3;
			av[na++] = tmp6;    /* pass temporary in-line file */
		}
		AV_SAVE(c1)
		if ( !pipeit ) {
			if (callsys(c1, av)) {
				cflag++;
				eflag++;
				dexit(); 		/* ken#2 */
			}
		}

		/* BACKEND OPTIMIZER */
		if (oflag && peepflag) {
			AV_PREP(optim)				/* start building arguments */
			for (j = 0; j < nlist[Xoptim]; j++)
			    av[na++] = list[Xoptim][j];

			if ( !pipeit ) {
				av[na++] = tmp5;
				av[na++] = tmp3;
			}
			AV_SAVE(optim)

			if ( !pipeit ) {
				if (callsys(optim, av)) {
					unlink(tmp3);
					tmp3 = assource = tmp5;
				} else
					unlink(tmp5);
			}
		}


		if (sflag) /* produce assembly code, DO NOT assemble */
		{
		 if (pipeit) pipeline( procs, pp_idx );
		 continue;
		}

			/* ASSEMBLER */

	assemble:
		cunlink(tmp4); 
		if (! irflag) {
		   cunlink(tmp1); 
		   cunlink(tmp6);
		}
		if (oflag && hlflag && ! irflag)
		   cunlink(tmp2);
		AV_PREP(as)				/* start building arguments */
		for (j = 0; j < nlist[Xas]; j++)
		    av[na++] = list[Xas][j];
		av[na++] = "-o";
		av[na++] = setsuf(clist[i], 'o');
		if (dflag)
			av[na++] = dflag;

		if ( !pipeit || ass_only_flg )
			av[na++] = assource; 
		AV_SAVE(as)

		if ( !pipeit || ass_only_flg ) {
			ass_only_flg = 0;
			if (callsys(as, av) > 0) {
				cflag++;
				eflag++;
				continue;
			}
		} else {
			pipeline( procs, pp_idx );
		}
	}
nocom:
	if (cflag==0 && nl!=0) {
		av = av_gen;						/* ld is never part of pipeline */
		av[0] = NAMEld; na=1;

		if (sys5flag)
			av[na++] = "-SYSV";

		if (outfile) {
			av[na++] = "-o";
			av[na++] = outfile;
		}

		if ( proflag )
			av[na++] = strspl("-L",
					strspl(libploc ? libploc : LIBDIR,"/libp"));

		for (j = 0; j < nlist[Xld]; j++)
		{
				av[na++] = list[Xld][j];
		}

		av[na++] = crt1;		/* bk: note that this could be crt5 */

		if(gflag && !oflag)				/* sdb needs "libg.a" */
			llist[nl++] = "-lg";


		for ( i = 0; i < nl; i++ ) {
			if ( !sys5flag || emuflag )
				if ( strcmp( llist[i], "-lm" ) == 0 )
					continue;
			av[na++] = llist[i];
		}

					
		switch ( mode ) {				/* pick correct C library version */ 
			case MODE_CUC:
				if ( sys5flag )
				{
					nameLIBC5 = strspl(nameLIBC5,nondy_str);
					av[na++] = strspl( "-l", nameLIBC5 );
					if ( sys5flag && emuflag) av[na++] = "-lme";
					break;
				}
			default:
			case MODE_ANSI:
				nameLIBC = strspl(nameLIBC,nondy_str);
				av[na++] = strspl( "-l", nameLIBC );
				if (emuflag) av[na++] = "-lcme";
				else av[na++] = "-lcm";
				break;
			case MODE_STRICT_ANSI:
				nameLIBCA = strspl(nameLIBCA,nondy_str);
				av[na++] = strspl( "-l", nameLIBCA );
				if (emuflag) av[na++] = "-lcame";
				else av[na++] = "-lcam";
				break;
		}
		if (emuflag) av[na++] = "-lFPSP";
		av[na++] = crtn;        /* shared library end init stub */

		av[na++] = 0;
		eflag |= callsys(ld, av);
		if (nc==1 && nxo==1 && eflag==0)
			unlink(setsuf(clist[0], 'o'));
	}
	dexit();
}

idexit()
{

	eflag = 100;
	dexit();
}

dexit()
{

	if (!pflag) {
		if ( ! irflag)
		   cunlink(tmp1);
		if (oflag && hlflag && ! irflag)
		   cunlink(tmp2);
		if (sflag==0 && Gflag==0)
			cunlink(tmp3);
		cunlink(tmp4);
		if (oflag && peepflag)
		   cunlink(tmp5);
		cunlink(tmp6);
	}
	exit(eflag);
}

error(s, x)
char *s, *x;
{
	FILE *diag = exflag ? stderr : stdout;

	fprintf(diag, "%scc:  ", prefix);
	fprintf(diag, s, x);
	putc('\n', diag);
	cflag++;
	eflag++;
}

getsuf(as)
char as[];
{
	register int c;
	register char *s;
	register int t;

	s = as;
	c = 0;
	while (t = *s++)
		if (t=='/')
			c = 0;
		else
			c++;
	s -= 3;
	if ( c > 2 && *s++ == '.')
		return (*s);
	return (0);
}

char *
setsuf(as, ch)
	char *as;
{
	register char *s, *s1;

	s = s1 = savestr(as);
	while (*s)
		if (*s++ == '/')
			s1 = s;
	s[-1] = ch;
	return (s1);
}

callsys(f, v)
	char *f, **v;
{
	int t, status;

	if (debug) {
		debug_show(f,v);
		if ( debug >= 3 )
			return 0;
	}

	t = fork();
	if (t == -1) {
		printf("No more processes\n");
		return (100);
	}
	if (t == 0) {
		execv(f, v);
		printf("Can't find %s\n", f);
		fflush(stdout);
		_exit(100);
	}
	while (t != wait(&status))
		;
	if ((t=(status&0377)) != 0 && t!=14) {
		if (t!=2) {
			printf("Fatal error in %s\n", f);
			eflag = 8;
		}
		dexit();
	}
	return ((status>>8) & 0377);
}

debug_show(f,v)
char *f, **v;
{
	char	**p;

	if (debug) {
		if ( debug >= 2 )
			fprintf(stderr,"callsys %s: ", f);
		for (p=v; *p != NULL; p++)
			  fprintf(stderr,"'%s' ", *p);
		fprintf(stderr,".\n");
	}
}

nodup(l, os)
	char **l, *os;
{
	register char *t, *s;
	register int c;

	s = os;
	if (getsuf(s) != 'o')
		return (1);
	while (t = *l++) {
		while (c = *s++)
			if (c != *t++)
				break;
		if (*t==0 && c==0)
			return (0);
		s = os;
	}
	return (1);
}

#define	NSAVETAB	1024
char	*savetab;
int	saveleft;

char *
savestr(cp)
	register char *cp;
{
	register int len;

	len = strlen(cp) + 1;
	if (len > saveleft) {
		saveleft = NSAVETAB;
		if (len > saveleft)
			saveleft = len;
		savetab = (char *)malloc(saveleft);
		if (savetab == 0) {
			fprintf(stderr, "ran out of memory (savestr)\n");
			exit(1);
		}
	}
	strncpy(savetab, cp, len);
	cp = savetab;
	savetab += len;
	saveleft -= len;
	return (cp);
}

char *
strspl(left, right)
	char *left, *right;
{
	char buf[BUFSIZ];

	strcpy(buf, left);
	strcat(buf, right);
	return (savestr(buf));
}

char *
getpref(s)				/* get prefix of "cc" */
char * s;				/* cc command name */
{
    static char prefix[15];		/* enough room for prefix and \0 */
    char * last = s + strlen(s) - 1;	/* point at last char */
    char * cp;				/* used later */

    prefix[0] = '\0';			/* initialize prefix static */

/* verify we have "cc" */

    if ( *last == 'c' ) {
		if ( *--last == 'c') {
			cp = last - 1;			/* point at char. preceding cc */
			while (cp >= s && *cp != '/')	/* back up to / or beginning */
				cp--;
			
			cp++;				/* compensate for going too far */
			(void) strncpy(prefix,cp,last-cp); /* copy prefix string */
		} else {					/* bk: check for "cuc" anomaly */
			if ( *last == 'u' ) {
				cp = last - 1;			/* point at char. preceding cc */
				if ( *cp == 'c' ) {
					cp--;
					if ( *cp == '/' || cp < s ) {
						(void) strcpy(prefix,"cu"); /* hard-code prefix string*/
					}
				}
			}
		}
	}
	if ( strcmp(prefix, "cu" ) == 0 )
		cuc_flag = 1;
    return(prefix);			/* return prefix */
}

/* make absolute program name from path, prefix, and base program name */

char *
makename(path,prefix,name)
char * path;				/* initial path name */
char * prefix;				/* string to prepend to name */
char * name;				/* command name, as in "ccom" */
{
    static char buf[BUFSIZ];		/* for building name */

    (void) strcpy(buf,path);		/* concatenate pieces */
    (void) strcat(buf,"/");
    (void) strcat(buf,prefix);
    (void) strcat(buf,name);
    return( savestr(buf) );		/* return completed string */
}

getXpass(c, opt)
char	c, *opt;
{
	switch (c) {
	default:
		error("Unrecognized pass name: '%s%c'", opt, c);
		return(-1);
	case '0':
		return(Xc0);
	case '1':
		return(Xc1);
	case '2':
		return(Xcoptim);
	case '3':
		return(Xoptim);
	case 'p':
		return(Xcpp);
	case 'a':
		return(Xas);
	case 'l':
		return(Xld);
	}
}

char *
stralloc(n)
	int n;
{
	register char *s;

	if ((s = (char *) malloc(n+1)) == NULL) {
		error("out of space", (char *) NULL, (char *) NULL);
		dexit();
	}
	return(s);
}

/* Add the string pointed to by opt to the list given by list[lidx]. */
addopt(lidx, opt)
int	lidx;	/* index of list */
char	*opt;  /* new argument to be added to the list */
{
	/* check to see if the list is full */
	if ( nlist[lidx] == limit[lidx] - 1 ) {
		limit[lidx] += argcount;
		if ((list[lidx]= (char **)realloc(list[lidx],
					limit[lidx]*sizeof(char *))) == NULL) {
			fprintf(stderr, "Out of space\n");
			dexit();
		}
	}

	list[lidx][nlist[lidx]++]= opt;
}


/* 
   chg_pathnames() overrides the default pathnames as specified by the -Y option
*/
chg_pathnames(prefix, chpiece, npath)
char *prefix;
char *chpiece;
char *npath;
{
	char	*t;
	char	*topt;

	for (t = chpiece; *t; t++)
		switch (*t) {
		case 'p':
			cpp = makename( npath, prefix, nameCPP );
			PATHcpp = savestr( npath );
			break;
		case '0':
			c0 = makename( npath, prefix, NAMEc0 );
			break;
		case '1':
			c1 = makename( npath, prefix, NAMEc1 );
			break;
		case '2':
			coptim = makename( npath , prefix, NAMEcoptim);
			break;
		case '3':
			optim = makename( npath, prefix, nameOPTIM );
			PATHoptim = savestr( npath );
			break;
		case 'a':
			as = makename( npath, prefix, nameAS );
			PATHas = savestr( npath );
			break;
		case 'l':
			ld = makename( npath, prefix, NAMEld );
			break;
		case 'S':
			Ssave = savestr(npath);						/* bk */
			crt1 = makename(npath,prefix,NAMEcrt1);/* start-up */
			crtn = makename(npath,prefix,NAMEcrtn);/* link end */
			break;
		case 'I':
			topt = stralloc(strlen(npath)+4);
			sprintf(topt,"-Y%s",npath);
			addopt(Xcpp,topt);
			break;
		case 'L':
			libploc = savestr(npath);						/* bk */
		case 'U':					/* bk */
			addopt(Xld,"-Y");
			topt = stralloc(strlen(npath)+3);
			sprintf(topt,"%c,%s",*t,npath);
			addopt(Xld,topt);
			break;
#ifdef M32MAU
		case 'F':
			fplibdir= stralloc(strlen(npath));
			strcpy(fplibdir,npath);
			break;
#endif
			
		default: /* error */
			fprintf(stderr,"Bad option -Y %s,%s\n",chpiece, npath);
			exit(1);
		} /* end switch */
}


/************************************************************************/
/************************************************************************/

pipeline( procs, numprocs )
t_proc procs[];
int numprocs;
{
	int pid, ret,i;

	if (debug)
	{ 
	 fprintf(stderr,"Ready to start Pipeline.Processes spawned will be:\n");
	 for (i= numprocs - 1; i >= 0; --i) 
		debug_show(procs[i].proc,procs[i].argv);
	 fprintf(stderr,"============= PIPE LINE STARTING ==========\n");
	}

	switch ( pid = fork() ) {
		default: 
			while ( wait(&ret) != pid )
				;
			break;
		case 0:
			if (sflag) freopen(tmp3,"w",stdout);
			if (Gflag) freopen("/dev/null","w",stdout);
	 		if ( debug >= 3 ) exit(0);
			pipe_ctrl( procs, numprocs-1 );
			break;
		case FORKERR:
			break;
	}
	if (debug)
	   fprintf(stderr,"============= PIPE LINE FINISHED ==========\n");
}

#define 	HOOK_STDIN_TO_PIPE(fd)	stdio_hookup(fd, 0)
#define 	HOOK_STDOUT_TO_PIPE(fd)	stdio_hookup(fd, 1)
#define		EXEC(p)	execit(p)
#define		CLEANUP(fd) 	(close(fd[0]) || close(fd[1]))

stdio_hookup(fd,out)
int fd[2];
int out;
{
	close(out); 
	fcntl(fd[out], F_DUPFD, out);
}

pipe_ctrl( proc, numpipes )
t_proc proc[];
int numpipes;
{
	int pid, i;
	int fdv[2];
	int t, status;
	char	**p;
	

	for ( i = numpipes; i > 0; --i ) {
		if ( pipe(fdv) < 0 ) {
			perror( "Pipe failed" );
			exit( 2 );
		}
		pid = fork();
		if ( pid == FORKERR ) {
			printf( "No more processes\n" );
			return (100);
		}
		if (pid != CHILD)
			HOOK_STDIN_TO_PIPE(fdv);
		else
			HOOK_STDOUT_TO_PIPE(fdv);
		CLEANUP(fdv);
		if (pid != CHILD)
			break;
	}


	EXEC( &proc[i] );
	printf( "Failed to exec %s\n", proc[i].proc );
	fflush(stdout);
	_exit(100);
}

execit(proc)
t_proc *proc;
{
	char **p;

	/*
	if (debug) {
		if ( debug >= 2 )
			printf("callsys %s: ", proc->proc);
		for (p=proc->argv; *p != NULL; p++)
			printf("'%s' ", *p);
		printf(".\n");
		if ( debug >= 3 )
			return(0);
	}
	*/
	execv( proc->proc, proc->argv );
}
