/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) main.c: version 25.1 created on 12/2/91 at 17:38:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)main.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	main.c	7.2		*/

#ident	"@(#)f77/f77/mach:main.c	25.1"

/*  main.c	7.2	"*/

#include "defs"
#include <signal.h>

#ifdef HOG			/* plock compiler */
#include <sys/lock.h>
extern int errno;
#endif

#include <a.out.h>

#ifdef m68k
int align_val = 16;	/* m68k default variable alignment; word */
#endif

int is_m68881 = 0; 	/* TRUE if system has an M68881 coprocessor;
			** double/float return values returned in
			** %fp0 only				 */
int is_m68881u= 0; 	/* TRUE if system has an M68881 coprocessor,
			** and double/float return values returned in 
			** both %fp0 and %d0/%d1 for compatibility
			** with old object code.  (u="u"niversal) */

extern char *getenv();
int stkckflg = 0;		/* global 'include stack checking code' flag */

main(argc, argv)
int argc;
char **argv;
{
char *s;
int k, retcode, *ip;
register char *strval;
FILEP opf();
int flovflo();

#define DONE(c)	{ retcode = c; goto finis; }

signal(SIGFPE, flovflo);  /* catch overflows */

#if HERE == PDP11
	ldfps(01200);	/* trap on overflow */
#endif


#ifdef HOG
	if (plock(PROCLOCK) == -1)
		fatali("plock fails, errno = %d\n", errno);
#endif


--argc;
++argv;

while(argc>0 && argv[0][0]=='-')
	{
	for(s = argv[0]+1 ; *s ; ++s) switch(*s)
		{
		case 'w':
			if(s[1]=='6' && s[2]=='6')
				{
				ftn66flag = YES;
				s += 2;
				}
			else
				nowarnflag = YES;
			break;

		case 'U':
			shiftcase = NO;
			break;

		case 'u':
			undeftype = YES;
			break;

		case 'O':
			optimflag = YES;
			if( isdigit(s[1]) )
				{
				k = *++s - '0';
				if(k > MAXREGVAR)
					{
					warn1("-O%d: too many register variables", (char *)k);
					maxregvar = MAXREGVAR;
					}
				else
					maxregvar = k;
				}
			break;

		case 'd':
			debugflag = YES;
			break;

		case 'p':
			profileflag = YES;
			break;

		case 'C':
			checksubs = YES;
			break;

		case '6':
			no66flag = YES;
			noextflag = YES;
			break;

		case '1':
			onetripflag = YES;
			break;

#ifdef SDB
		case 'g':
			sdbflag = YES;
			break;
#endif

		case 'N':
			switch(*++s)
				{
				case 'q':
					ip = &maxequiv; goto getnum;
				case 'x':
					ip = &maxext; goto getnum;
				case 's':
					ip = &maxstno; goto getnum;
				case 'c':
					ip = &maxctl; goto getnum;
				case 'n':
					ip = &maxhash; goto getnum;

				default:
					fatali("invalid flag -N%c", *s);
				}
		getnum:
			k = 0;
			while( isdigit(*++s) )
				k = 10*k + (*s - '0');
			if(k <= 0)
				fatal("Table size too small");
			*ip = k;
			break;

		case 'I':
			if(*++s == '2')
				tyint = TYSHORT;
			else if(*s == '4')
				{
				shortsubs = NO;
				tyint = TYLONG;
				}
			else if(*s == 's')
				shortsubs = YES;
			else
				fatali("invalid flag -I%c\n", *s);
			tylogical = tyint;
			break;

		default:
			fatali("invalid flag %c\n", *s);
		}
	--argc;
	++argv;
	}

if(argc != 4)
	fatali("arg count %d", argc);
asmfile  = opf(argv[1]);
initfile = opf(argv[2]);

/*#ifdef FONEPASS*/
	if (!(textfile = freopen(argv[3], "w", stdout)))
	{
		perror(argv[3]);
		fatal("cannot reopen stdout");
		/* NOTREACHED */
	}
/*#else*/
/*textfile = opf(argv[3]);*/
/*#endif /*FONEPASS*/

	if( ((strval=getenv("STACKCHECK")) != NULL) 
		&& (!strcmp(strval,"ON")) )
	{
		++stkckflg;
	}


#ifdef M68881

	if ( ( strval = getenv( "FP" ) ) != NULL ) 
	{
		if (( ! strcmp( strval, "M68881" ) )||
		    ( ! strcmp( strval, "m68881" ) ))
		{
			++is_m68881;	/* generate code for M68881 */
					/* return float/doubles in %fp0 only */
		}

		if (( ! strcmp( strval, "M68881u" ) )||
		    ( ! strcmp( strval, "m68881u" ) ))
		{
			++is_m68881;	/* generate code for M68881 */
			++is_m68881u;	/* generate code for M68881 */
					/* return float/doubles in %fp0 */
					/* and also in %d0/%d1 */
		}
	}
#endif

initkey();
if(inilex( copys(argv[0]) ))
	DONE(1);
fprintf(diagfile, "%s:\n", argv[0]);


#ifdef SDB
#	if TARGET == PDP11
	for(s = argv[0] ; ; s += 8)
	{
	prstab(s,N_SO,0,NULL);
	if( strlen(s) < 8 )
		break;
	}
#	else
	if (sdbflag)		/* put out Complex and Dcomplex */
		{		/* structure definitions */
#		if TARGET==M68K
		p2pass("\ttext");
		p2pass("\tdef\tComplex;\tscl\t10;\ttype\t010;\tsize\t8;\tendef");
		p2pass("\tdef\treal;\tval\t0;\tscl\t8;\ttype\t6;\tendef");
		p2pass("\tdef\timag;\tval\t4;\tscl\t8;\ttype\t6;\tendef");
		p2pass("\tdef\teos;\tval\t8;\tscl\t102;\ttag\tComplex;\tendef");
		p2pass("\tdef\tDcomplex;\tscl\t10;\ttype\t010;\tsize\t16;\tendef");
		p2pass("\tdef\treal;\tval\t0;\tscl\t8;\ttype\t7;\tendef");
		p2pass("\tdef\timag;\tval\t8;\tscl\t8;\ttype\t7;\tendef");
		p2pass("\tdef\teos;\tval\t16;\tscl\t102;\ttag\tDcomplex;\tendef");
#		else
  		p2pass("\t.def\tComplex;\t.scl\t10;\t.type\t010;\t.size\t8;\t.endef");
  		p2pass("\t.def\treal;\t.val\t0;\t.scl\t8;\t.type\t6;\t.endef");
  		p2pass("\t.def\timag;\t.val\t4;\t.scl\t8;\t.type\t6;\t.endef");
  		p2pass("\t.def\t.eos;\t.val\t8;\t.scl\t102;\t.tag\tComplex;\t.endef");
  		p2pass("\t.def\tDcomplex;\t.scl\t10;\t.type\t010;\t.size\t16;\t.endef");
  		p2pass("\t.def\treal;\t.val\t0;\t.scl\t8;\t.type\t7;\t.endef");
  		p2pass("\t.def\timag;\t.val\t8;\t.scl\t8;\t.type\t7;\t.endef");
  		p2pass("\t.def\t.eos;\t.val\t16;\t.scl\t102;\t.tag\tDcomplex;\t.endef");
#		endif
		}
#	endif
#endif

fileinit(debugflag);
procinit();
if(k = yyparse())
	{
	fprintf(diagfile, "Bad parse, return code %d\n", k);
	DONE(1);
	}
if(nerr > 0)
	DONE(1);
if(parstate != OUTSIDE)
	{
	warn("missing END statement");
	endproc();
	}
doext();
preven(ALIDOUBLE);
prtail();
#if FAMILY==PCC
	puteof();
#endif

if(nerr > 0)
	DONE(1);
DONE(0);


finis:
	done(retcode);
	return(retcode);
}



done(k)
int k;
{
void exit();
static int recurs	= NO;

if(recurs == NO)
	{
	recurs = YES;
	clfiles();
	}
exit(k);
}


LOCAL FILEP opf(fn)
char *fn;
{
FILEP fp;
if( fp = fopen(fn, "a") )
	return(fp);

fatalstr("cannot open intermediate file %s", fn);
/* NOTREACHED */
}



LOCAL clfiles()
{
fprintf(textfile,"#B1.text\n\ttext\n");
clf(&textfile);
fprintf(asmfile,"#B1.asm\n\ttext\n");
clf(&asmfile);
/*fprintf(initfile,"#B1.init\n\ttext\n");*/
clf(&initfile);
}


clf(p)
FILEP *p;
{
if(p!=NULL && *p!=NULL && *p!=stdout)
	{
	if(ferror(*p))
		fatal("writing error");
	fclose(*p);
	}
*p = NULL;
}




flovflo()
{
err("floating exception during constant evaluation");
#if HERE == VAX
	fatal("vax cannot recover from floating exception");
	/* vax returns a reserved operand that generates
	   an illegal operand fault on next instruction,
	   which if ignored causes an infinite loop.
	*/
#endif
signal(SIGFPE, flovflo);
}
