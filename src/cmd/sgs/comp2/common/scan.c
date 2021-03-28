#ident	"@(#)scan.c	25.1	12/3/91 Copyright (c) 1991 by Arix Corp."
/*
    1.4	89/10/02 09:59:34 hossein
	Added support for M68040.

    1.3	89/06/26 16:17:55 hossein
	changes related to "const" type qualifier for POSIX.

    1.2	89/06/15 14:39:18 hossein
	fixed scan.c to recognize 8-bit characters.

    1.1	89/06/08 00:38:34 root
	date and time created 89/06/08 00:38:34 by root

*/
/*	@(#)scan.c	7.3		*/
static	char	SCCSID[] = "@(#) scan.c:	@(#)scan.c	7.3	";

# include "mfile1.h"
# include <ctype.h>
# include <errno.h>			/* bk: for ERANGE */
# include <math.h>			/* bk: for MAXFLOAT */
# include "modes.h"			/* bk: for pragma control */

#define MAX_ULONG_DEC	"4294967295"			/* bk: for overflow checks */
#define MAX_ULONG_OCT	"037777777777"
/* temporarily */

/* assembler directive for #ident or "Slist" feature:  default, make it a comment */
# ifndef IDENTSTR
# define IDENTSTR COMMENTSTR
# endif

/* character-set translation */
# ifndef CCTRANS
# define CCTRANS(x) (x)
# endif

extern int hlflag;		/* Motorola 4.1: high level optimizer follows c0 */
extern warn_t warn_lvl;
extern int fflag; /*MC68881 UNIX conformace (bounds check for sin/cos/tan) */
int flflag = 0;
/* hoss#5 */
int intrflg = 0; /* if raised, expand intrinsics inline */
int protoflg;	/* if -XG, generate prototypes for old-style ftn definitions */
/*bk*/
int seen_typespec;	/* bk: helps with ANSI 3.5.6: "...type specifiers shall */
					/* not be omitted in the inner declaration." */
static int wideflg;	/*bk: flags presence of wide-character prefix */
int vaflag;		/* bk */
int null_input;

# ifndef FLOATCVT
double atof();
# define FLOATCVT(x) atof(x)
# endif

# ifndef ASMBUF
# define ASMBUF 50
# endif
char asmbuf[ASMBUF];
char *asmp;
int asm_esc = 0; /* asm escaped used in file */
/* lexical actions */

# define A_ERR 0		/* illegal character */
# define A_LET 1		/* saw a letter */
# define A_DIG 2		/* saw a digit */
# define A_1C 3			/* return a single character */
# define A_STR 4		/* string */
# define A_CC 5			/* character constant */
# define A_BCD 6		/* GCOS BCD constant */
# define A_SL 7			/* saw a / */
# define A_DOT 8		/* saw a . */
# define A_PL 9		/* + */
# define A_MI 10		/* - */
# define A_EQ 11		/* = */
# define A_NOT 12		/* ! */
# define A_LT 13		/* < */
# define A_GT 14		/* > */
# define A_AND 16		/* & */
# define A_OR 17		/* | */
# define A_WS 18		/* whitespace (not \n) */
# define A_NL 19		/* \n */
# define A_MOD 20		/* % */			/*bk*/
# define A_ER 21		/* ^ */			/*bk*/
# define A_MUL 22		/* * */			/*bk*/

/* character classes */

# define LEXLET 01
# define LEXDIG 02
# define LEXOCT 04
# define LEXHEX 010
# define LEXWS 020
# define LEXDOT 040

/* reserved word actions */

# define AR_TY 0		/* type (and type qualifier) word */
# define AR_RW 1		/* simple reserved word */
# define AR_CL 2		/* storage class word */
# define AR_S 3		/* struct */
# define AR_U 4		/* union */
# define AR_E 5		/* enum */
# define AR_A 6		/* asm */
# define AR_TQ 7


/* text buffer */
# define LXTSZ BUFSIZ		/* lots of room for FLEXNAMES */
char yytext[LXTSZ];
char * lxgcp;
/* If FLEXNAMES defined, let -T determine whether symbols are truncated.
** Otherwise, forcibly truncate all names.
*/

static int truncate_flag =
#ifdef	FLEXNAMES
0;		/* truncate based on -T (non-zero to truncate */
#else
1;		/* force all names <= NCHNAM chars */
#endif

#ifdef	PROFILE
	int proflg;	/* global - referenced again in local.c - bfcode() */
#endif

#ifdef m68k

# ifdef DFLTDBLALIGN
int align_val = 32;     /* m68k default variable alignment: double word */
# else
int align_val = 16;	/* m68k default variable alignment; word */
# endif

int align_struc = 16;	/* m68k default variable alignment inside
			** structures; word */
#endif

int is_m68881 = 0;  	/* true if compiling for system with 881;
			** double/float return values returned in
			** %fp0 only				 */
int is_m68881u= 0; 	/* TRUE if system has an M68881 coprocessor;
			** and double/float return values returned in 
			** both %fp0 and %d0/%d1 for compatibility
			** with old object code.  (u="u"niversal) */
extern char *getenv();
int compiler_mode = MODE_ANSI;	/* compiler mode: default = ANSI */
FILE *protofp;   /* file to store extracted function prototypes */


/* ARGSUSED */
mainp1( argc, argv )
int argc;
char *argv[];
{
	/* control multiple files */

	register i;
	register char *cp;
#ifdef m68k
	register char *strval;
#endif
	extern int idebug, bdebug, tdebug, edebug, ddebug, 
			xdebug, opdebug, pdebug;

#ifdef IN_LINE
extern int asmdebug;  /* set if env variable ASMDEBUG=YES */
extern FILE *inlfp;   /* file to store asm 'function' bodies */
#endif

#ifndef TWOPASS
	extern int zflag;
#endif
	extern unsigned int maxoffset;
	int fdef = 0;

	unsigned int temp = 1;
	i = 0;
	do
	{
		temp <<= 1;
		++i;
	} 	while ( temp != 0 );
	maxoffset = 1 << (i-1);

#ifdef IN_LINE
	if( ( ( strval = getenv( "ASMDEBUG" ) ) != NULL ) &&
	    ( ! strcmp( strval, "YES" ) ) )
	{
		++asmdebug;   /* debug in-line expansion */
	}
#endif

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
	if( ( ( strval = getenv( "STALIGN" ) ) != NULL ) &&
	    ( ! strcmp( strval, "YES" ) ) &&
	    ( align_val == 32)) /* only allow longword struc align when
				** all else is long-aligned. */
		align_struc = 32;
#endif
#ifdef M68881

	if ( ( strval = getenv( "FP" ) ) != NULL ) 
	{
		if (strval[0] == 'm')
			strval[0] = 'M'; /* reduce # of cases to cover */

		if ( ! strcmp( strval, "M68881" ) )
		{
			++is_m68881;	/* generate code for M68881 */
					/* return float/doubles in %fp0 only */
		}

		if ( (! strcmp( strval, "M68881u" ) ) ||
		     (! strcmp( strval, "M68881U" ) ))
		{
			++is_m68881;	/* generate code for M68881 */
			++is_m68881u;	/* generate code for M68881 */
					/* return float/doubles in %fp0 */
					/* and also in %d0/%d1 */
		}
	}
#ifdef FORCE881
	++is_m68881;            /* always produce '881 code */
#endif
#endif
	

# ifdef SDB
	/* Motorola 4.1 */
	/* check specifically for -g  or -O as first argument */
	if( argc>1 && *(cp=argv[1])=='-')
	{
		extern int gdebug;

		if (cp[1]=='g' && !cp[2])
		  {	gdebug = 1;
			++argv;
			--argc;
			}
		else
		if (cp[1]=='O' && !cp[2])
		  {	hlflag = 1;			/* set coptim flag */
			gdebug = 1;
			++argv;
			--argc;
			}
	}
# endif

	for( i=1; i<argc; ++i )
	{
		if( *(cp=argv[i]) == '-' )	/* beg hoss#1 */
		{	
			if( *++cp == 'X' )
			{
				while( *++cp )
				{
					switch( *cp )
					{

					case 'd':
						++ddebug;
						break;
					case 'M':			/* bk: compiler mode */
						compiler_mode = atoi( ++cp );	
						break;
					case 'w':			/* bk */
						warn_lvl = (warn_t) atoi( ++cp );	/* warning level */
						if ( !W_LVLCHK(warn_lvl) )
							cerror( "bad option: X%c", *cp );
						break;
					case 'p':			/* bk */
						++pdebug;		/* prototype debug */
						break;
					case 'i':
						++idebug;
						break;
					case 'b':
						++bdebug;
						break;

					/* hoss#5 */
					/* inline expansion of intrinsics */
					case 'k':
						++intrflg;
						break;

					case 't':
						++tdebug;
						break;
					case 'f':
nofloat:											
						++fflag; /* Unix conforming float */
						break;
					case 'e':
						++edebug;
						break;
#ifndef NODBG
					case 'S':	/* ken#4 */
						{extern int printsize;
						++printsize;
						}
						break;
#endif
					case 'x':
						++xdebug;
						break;
					case 'o':
						++opdebug;
						break;
#ifdef ASSYCOMMENT
					case 's':
						{
							 extern int asdebug;
							++asdebug;
							break;
						}
#ifndef TWOPASS
					case 'z':
						++zflag; /* stin file debug info */
						break;
#endif
#endif
					case 'T':	/* truncate names */
						++truncate_flag;
						break;

					case 'A':	/* Arix-specific boolean flags */
						Arix[ atoi( ++cp ) ] = 1;
						break;

#ifdef	PROFILE
					case 'P':	/* profile code */
						++proflg;
						break;
#endif
						default:
							cerror( "bad option: X%c", *cp );
					}
				}
			}
			else 
			{
				cerror("bad option: -%c", *cp);
			}
		}
		else
		{
		 	if ( cp[0] == '+') fdef++;
		 	else switch( fdef++ )   /* end hoss#1 */
			{
			case 0:
			case 1:
				if( freopen(argv[i], fdef==1 ? "r" : "w", fdef==1 ? stdin : stdout) == NULL)
				{
					fprintf(stderr, "ccom:can't open %s\n", argv[i]);
					exit(1);
				}
				break;
			case 2: if ((inlfp=fopen(argv[i],"w+"))==NULL)
				{
					fprintf(stderr, "can't open asm macro tmp file\n");
					exit(1);
				}
				break;

				/* bk: function prototype generation output */
			case 3: if ((protofp=fopen(argv[i],"w+"))==NULL)
				{
					fprintf(stderr, "can't open prototype output file\n");
					exit(1);
				}
				++protoflg;
				break;

			default:
				;
			}
		}
	}

	if ( compiler_mode != MODE_CUC ) {
		if ( fflag || intrflg ) {
			werror( W_DFLT, 
				"Floating point intrinsics incompatible with ANSI: ignored" );
			fflag = intrflg = 0;
		}
	}

	if (opdebug>0) setbuf(stdout,NULL);  /* no buffering if debugging */


#ifdef ONEPASS
	p2init( argc, argv );
#endif

#if 0
/* Motorola 4.1 */
/* WHY DO THIS? stab is external and therefore must be zero! */
	for( i=0; i<SYMTSZ; ++i ) stab[i].stype = TNULL;
#endif

	if ( compiler_mode != MODE_ANSI )
		/* must come before lxtitle, called in lxinit */
		mode_init(compiler_mode);	

	lxinit();
	tinit();
	mkdope();

	lineno = 1;

	/* dimension table initialization */

	dimtab[NULL] = 0;
	dimtab[VOID] = 0;
	dimtab[CHAR] = SZCHAR;
	dimtab[INT] = SZINT;
	dimtab[FLOAT] = SZFLOAT;
	dimtab[DOUBLE] = SZDOUBLE;
	dimtab[LONG] = SZLONG;
	dimtab[SHORT] = SZSHORT;
	dimtab[UCHAR] = SZCHAR;
	dimtab[USHORT] = SZSHORT;
	dimtab[UNSIGNED] = SZINT;
	dimtab[ULONG] = SZLONG;
	/* starts past any of the above */
	curdim = 17;			/*bk: changed from 16 */
	reached = 1;
	typequal = 0;
	indcl = 0;

	null_input = 1;			/* so we can check for empty input files */

	yyparse();
	yyaccpt();

	if ( null_input )
		werror(W_STRICT,"file must contain at least one external def"); 

# ifdef SDB
	ejsdb();
# endif
# ifdef ENDJOB
	ENDJOB(nerrors?1:0);
# endif

	return(nerrors?1:0);

}

/* Motorola 4.1 (CSSZ used to be 128) */
/* Handle all possible 256 characters (bomb fix for European chars d.t.) */
# define CSMASK 0377
# define CSSZ 256

#ifdef	ASSYCOMMENT
/*
*	We buffer the input on a line by line basis
*	so that we can output the ``C'' source as comments
*	in the assembly language
*/
int	asdebug = 0;
static	char	linebuf[BUFSIZ] =
{
	'\n','\n','\n'
};
static	char	*linecp = &linebuf[0];
#undef	ungetc
#undef	getchar

#define	getchar()	(*linecp ? *linecp++ : eatline() )
#define	ungetc(c, ioptr)	*--linecp=(c)

eatline()
{
	linecp = &linebuf[0];
	if (fgets(linebuf, BUFSIZ, stdin) == NULL)
	{
		*linecp = '\0';
		return( EOF );
	}
	if (asdebug)
		spitline(linebuf);

	return( *linecp++ );
}


/*
** add the original C line to the assembler output as comment,
** unless it is empty.
*/
#define	MARKERCNT	20	/* how often to spit out line markers */
spitline(l)
register char *l;
{
	extern int lineno;

	if( !(lineno%MARKERCNT) )
		printf( "%s -- line %d %s\n", COMMENTSTR, lineno, ftitle );
				/* line marker */
	while( isspace( *l ) )
		l++;
	if( *l )
		printf( "%s %s", COMMENTSTR, l );
}
#endif

short lxmask[CSSZ+1];

lxenter( s, m )
register char *s;
register short m;
{
	/* enter a mask into lxmask */
	register c;

	while( c= *s++ ) lxmask[c+1] |= m;

}


# define lxget(c,m) (lxgcp=yytext,lxmore(c,m))

lxmore( c, m )
register c, m;
{
	register char *cp;

	*(cp = lxgcp) = c;
	while( (c = getchar()), lxmask[c+1]&m )
	{
		if( cp < &yytext[LXTSZ-1] )
		{
			*++cp = c;
		}
	}
	ungetc(c,stdin);
	*(lxgcp = cp+1) = '\0';
}

struct lxdope
{
	short lxch;	/* the character */
	short lxact;	/* the action to be performed */
	short lxtok;	/* the token number to be returned */
	short lxval;	/* the value to be returned */
} lxdope[] =
{
	'$',	A_ERR,	0,	0,	/* illegal characters go here... */
	'_',	A_LET,	0,	0,	/* letters point here */
	'0',	A_DIG,	0,	0,	/* digits point here */
	' ',	A_WS,	0,	0,	/* whitespace goes here */
	'\n',	A_NL,	0,	0,
	'"',	A_STR,	0,	0,	/* character string */
	'\'',	A_CC,	0,	0,	/* character constant */
	'`',	A_BCD,	0,	0,	/* GCOS BCD constant */
	'(',	A_1C,	LP,	0,
	')',	A_1C,	RP,	0,
	'{',	A_1C,	LC,	0,
	'}',	A_1C,	RC,	0,
	'[',	A_1C,	LB,	0,
	']',	A_1C,	RB,	0,
	'*',	A_MUL,	MUL,	MUL,
	'?',	A_1C,	QUEST,	0,
	':',	A_1C,	COLON,	0,
	'+',	A_PL,	PLUS,	PLUS,
	'-',	A_MI,	MINUS,	MINUS,
	'/',	A_SL,	DIVOP,	DIV,
	'%',	A_MOD,	DIVOP,	MOD,
	'&',	A_AND,	AND,	AND,
	'|',	A_OR,	OR,	OR,
	'^',	A_ER,	ER,	ER,
	'!',	A_NOT,	UNOP,	NOT,
	'~',	A_1C,	UNOP,	COMPL,
	',',	A_1C,	CM,	CM,
	';',	A_1C,	SM,	0,
	'.',	A_DOT,	STROP,	DOT,
	'<',	A_LT,	RELOP,	LT,
	'>',	A_GT,	RELOP,	GT,
	'=',	A_EQ,	ASSIGN,	ASSIGN,
	-1,	A_1C,	0,	0,
};

struct lxdope *lxcp[CSSZ+1];

lxinit()
{
	register struct lxdope *p;
	register i;
	register char *cp;
	/* set up character classes */

	lxenter( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", LEXLET );
	lxenter( "0123456789", LEXDIG );
	lxenter( "0123456789abcdefABCDEF", LEXHEX );
	/* \013 should become \v someday; \013 is OK for ASCII and EBCDIC */
	/* hoss#21 */
	lxenter( " \t\r\b\a\f\013", LEXWS );
	lxenter( "01234567", LEXOCT );
	lxmask['.'+1] |= LEXDOT;

	/* make lxcp point to appropriate lxdope entry for each character */

	/* initialize error entries */

	for( i= 0; i<=CSSZ; ++i ) lxcp[i] = lxdope;

	/* make unique entries */

	for( p=lxdope; ; ++p )
	{
		lxcp[p->lxch+1] = p;
		if( p->lxch < 0 ) break;
	}

	/* handle letters, digits, and whitespace */
	/* by convention, first, second, and third places */

	cp = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	while( *cp ) lxcp[*cp++ + 1] = &lxdope[1];
	cp = "123456789";
	while( *cp ) lxcp[*cp++ + 1] = &lxdope[2];
	cp = "\t\b\r\f\013";
	while( *cp ) lxcp[*cp++ + 1] = &lxdope[3];

	/* first line might have title */
	lxtitle();

}

int lxmatch;  /* character to be matched in char or string constant */

lxstr(ct)
register ct;
{
	/* match a string or character constant, up to lxmatch */

	register c;
	register val;
	register i;
	int ovflow_warned = 0;

	i=0;
	while (1) 	/*bk*/
	{
		while( (c = getchar()) != lxmatch )
		{
			switch( c )
			{

			case EOF:
				uerror( "lxstr() unexpected EOF" );
				break;

			case '\n':
				uerror( "newline in string or char constant" );
				++lineno;
				break;

			case '\\':
				switch( c = getchar() )
				{

				case '\n':
					++lineno;
					continue;

				default:
					val = c;
					goto mkcc;

				case 'n':
					val = '\n';
					goto mkcc;

				case 'r':
					val = '\r';
					goto mkcc;

				case 'b':
					val = '\b';
					goto mkcc;

				case 't':
					val = '\t';
					goto mkcc;

				/* hoss#21 */
				case 'a':
					if ( compiler_mode == MODE_CUC )
						break;
					val = '\007';
					goto mkcc;

				case 'f':
					val = '\f';
					goto mkcc;

				case 'v':
					val = '\013';
					goto mkcc;
				
				case 'x':			/*bk*/
					if ( compiler_mode == MODE_CUC )
						break;
					lxget( c, LEXHEX );
					if ( yytext[1] == 0 ) {
						val = c;
						goto mkcc;
					}
					/* convert the value */
					{
						char *cp;
						val = 0;
						for( cp = yytext+1; *cp; ++cp )
						{
							/* this code won't work for all character sets,
							** but seems ok for ascii and ebcdic
							*/
							val <<= 4;
							if( isdigit( *cp ) )
								val += *cp-'0';
							else if( isupper( *cp ) )
								val += *cp - 'A'+ 10;
							else
								val += *cp - 'a'+ 10;
						}
						if ( cp > yytext+3 )	/* e.g. \x07Alert */
							werror( W_DFLT, 
							"hex escape sequence interpreted as \\x%x", val );
					}
					if ( val > 0xFF )
						werror( W_STRICT, "hex character constant overflow" );
					goto mkcc1;

				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					val = c-'0';
					c = getchar();  /* try for 2 */
					if( lxmask[c+1] & LEXOCT )
					{
						val = (val<<3) | (c-'0');
						c = getchar();  /* try for 3 */
						if( lxmask[c+1] & LEXOCT )
						{
							val = (val<<3) | (c-'0');
						}
						else ungetc( c ,stdin);
					}
					else ungetc( c ,stdin);

					if ( val > 0377 )
						werror( W_STRICT, "octal character constant overflow" );
					goto mkcc1;

				}
			default:
				val =c;
mkcc:
				val = CCTRANS(val);
mkcc1:
				if( lxmatch == '\'' )
				{
					if ( wideflg ) {
						/* reflects definition of wchar_t as unsigned char */
						val = ccast((CONSZ)val,UCHAR);
					} else {
						/* it is, after all, a "character" constant */
						val = ccast((CONSZ)val,CHAR);
					}
					if( i==0 ) lastcon = val;
					else
					{
# ifdef MAKECC
						MAKECC( val, i );
# else
# ifndef RTOLBYTES
						lastcon <<= SZCHAR;
						lastcon |= (val&BITMASK(SZCHAR));
# else
						lastcon &= BITMASK(SZCHAR*i);
						lastcon |= val << (SZCHAR*i);
# endif
# endif
					}
				}
				else
				{
					 /* stash the byte into the string */
					if( strflg == 1 )
					{
						if( ct==0 || i<ct ) putbyte( val );
						else if( i == ct ) {
							werror( W_DFLT,
						"string initializer too long; overflow bytes ignored" );
							ovflow_warned = 1;
						}
					}
					else if( strflg == 2 )
					{
						if( ct==0 || i<ct ) ary_strbyte( val );
						else if( i == ct ) {
							werror( W_DFLT,
						"string initializer too long; overflow bytes ignored" );
							ovflow_warned = 1;
						}
					}
					else bycode( 0xff&val, i ); /* hoss#22 */
				}
				++i;
				continue;
			}
			break;
		}

				/****** bk ******/
		if( lxmatch == '"' ) {			/* string catenation */
			while ( isspace(c = getchar()) ) {
				if ( c == '\n' ) {
					++lineno;
					lxtitle();
				}
			}
			if ( c == '"' )
				continue;
			ungetc( c ,stdin);
		}
		break;
	}

	/* end of string or char constant */

	if( lxmatch == '"' )
	{
		if( strflg == 1 )
		{
			 /* end the string */
			if( ct==0 || i<ct ) putbyte( 0 );  /* the null at the end */
			else
				if ( !ovflow_warned )		/* bk: ANSI quiet change */
					werror( W_DFLT, "array too small for string terminator" );
		}
		else if( strflg == 2 )
		{
			 /* end the string */
			if( ct==0 || i<ct ) ary_strbyte( 0 );  /* the null at the end */
			else
				if ( !ovflow_warned )		/* bk: ANSI quiet change */
					werror( W_DFLT, "array too small for string terminator" );
			strflg = 0;
		}
		else
		{
			  /* the initializer gets a null byte */
			bycode( 0, i++ );
			bycode( -1, i );
			dimtab[curdim] = i;  /* in case of later sizeof ... */
		}
	}
	else
	{
		 /* end the character constant */
		if( i == 0 ) uerror( "empty character constant" );
		if( i>(SZINT/SZCHAR) )
			uerror( "too many characters in character constant" );
		else if ( i > 1 )
			werror( W_LINT, "multiple characters in character constant" );
	}

	wideflg = 0;				/* in case it was turned on, reset */
}

lxcom()
{
	register c;
	/* saw a /*: process a comment */

	for(;;)
	{

		switch( c = getchar() )
		{

		case EOF:
			uerror( "lxcom() unexpected EOF" );
			return;

		case '\n':
			++lineno;

		default:
			continue;

		case '*':
			if( (c = getchar()) == '/' ) return;
			else ungetc( c ,stdin);
			continue;

/* # ifdef LINT *** bk ***/
		case 'V':
			lxget( c, LEXLET|LEXDIG );
			{
				extern int vaflag;
				register i;
				i = yytext[7]?yytext[7]-'0':0;
				yytext[7] = '\0';
				if( strcmp( yytext, "VARARGS" ) ) continue;
				vaflag = i;
				continue;
			}
# ifdef LINT
		case 'L':
			lxget( c, LEXLET );
			if( strcmp( yytext, "LINTLIBRARY" ) ) continue;
			{
				extern int libflag;
				libflag = 1;
			}
			continue;

		case 'A':
			lxget( c, LEXLET );
			if( strcmp( yytext, "ARGSUSED" ) ) continue;
			{
				extern int argflag, vflag;
				argflag = 1;
				vflag = 0;
			}
			continue;

		case 'N':
			lxget( c, LEXLET );
			if( strcmp( yytext, "NOTREACHED" ) ) continue;
			reached = 0;
			continue;
# endif
		}
	}
}

yylex()
{
	for(;;)
	{

		register lxchar;
		register struct lxdope *p;
		register struct symtab *sp;
		register char *cp;
		register id;

		switch( (p = lxcp[(lxchar = getchar())+1])->lxact )
		{

onechar:
			ungetc( lxchar ,stdin);
			if ((!indcl) && (p->lxtok == ASSIGN)) constid = 1;

		case A_1C:
			/* eat up a single character, and return an opcode */

			yylval.intval = p->lxval;
			return( p->lxtok );

		case A_ERR:
			uerror( "illegal character: 0x%x (hex)", lxchar );
			break;

		case A_LET:
			/* collect an identifier, and check for reserved word */
			lxget( lxchar, LEXLET|LEXDIG );

			/* 0 means some kind of low level error, >0 reserved,
			** <0 means id
			*/
			if( (lxchar=lxres()) > 0 ) return( lxchar );
			if( lxchar== 0 ) continue;

			if (truncate_flag )
				yytext[NCHNAM] = '\0';	/* truncate name */

			/* bk: this requires that prefix immediately precede string */
			if ( yytext[1] == 0 && yytext[0] == 'L' ) {	/* test for 'L' prefix*/
				switch ( lxcp[ (lxchar = getchar())+1]->lxact ) {
					case A_STR:			/* wide-char string literal */
						wideflg = 1;
						goto getstring;
					case A_CC:			/* wide-char character constant */
						wideflg = 1;
						goto getccon;
					default:
						ungetc(lxchar,stdin);
						break;
				}
			}

			id = lookup( hash(yytext),
					/* tag name for struct/union/enum */
				(stwart&TAGNAME)? STAG:
					/* member name for struct/union */
				(stwart&(INSTRUCT|INUNION|FUNNYNAME))?SMOS:0 );
			sp = &stab[id];
			if(  sp->sclass == TYPEDEF && !seen_typespec )
			/* &&  !stwart && !tdef_look() )
			*/
			{
				extern NODE *type_def;
				int testqual;	/* prevent "const CONST_TYPEDEF x" */

				stwart = instruct;
				yylval.nodep = mkty( sp->stype, sp->dimoff, 
									sp->sizoff, sp->stypeq );
				type_def = yylval.nodep;
						/* bk: inherit typedef's prototype list, if any. */
						/* If typedef is for non-prototyped function, */
						/* flag it - otherwise ProtoType() will get confused. */
				yylval.nodep->in.proto = sp->sproto;
				if ( ISFTN( sp->stype ) || ISFTN(DECREF(sp->stype))) {
#ifndef NODBG					
					extern int pdebug;
					if ( pdebug )
						printf( "yylex: typedefed ftn %s, proto=%d\n", 
												sp->sname, sp->sproto );
#endif
					if (sp->sproto == 0)
						yylval.nodep->in.proto = -1;
				}
						/* bk: also inherit typedef's type-qualifier info */
				testqual = sp->stypeq;
/*				if ( ISPTR(sp->stype) ) {		/ Plum Hall errtests/m35041 /
					if ( typequal & (CONSTVAR | VOLVAR) )	
						typequal <= 2;
				} */
				if ( testqual & typequal )
					uerror( "illegal type qualifier combination" );
				else
					typequal |= sp->stypeq;
				seen_typespec = 1;
				return( TYPE );
			}
			stwart = (stwart&SEENAME) ? instruct : 0;
			yylval.intval = id;
			return( NAME );

		case A_DIG:
			/* collect a digit string, then look at last one... */
			lastcon = 0;
			lxget( lxchar, LEXDIG );
			switch( lxchar = getchar() )
			{

			case 'x':
			case 'X':
				if( yytext[0] != '0' || yytext[1] ) 	/*bk */
					uerror( "illegal hex constant" );
				lxmore( lxchar, LEXHEX );
				/* convert the value */
				{
					if ( yytext[2] == 0 )			/* i.e. "0x" */
						uerror( "illegal hex constant" );
					else {
							
						for( cp = yytext+2; *cp; ++cp )
						{
							/* this code won't work for all character sets,
							** but seems ok for ascii and ebcdic
							*/
							lastcon <<= 4;
							if( isdigit( *cp ) )
								lastcon += *cp-'0';
							else if( isupper( *cp ) )
								lastcon += *cp - 'A'+ 10;
							else
								lastcon += *cp - 'a'+ 10;
						}
									 /* bk: strlen("0xFFFFFFFF")==10  */
						if ( strlen(yytext) > 10 )
							werror( W_STRICT, "hex integer constant overflow" );
					}
				}
				goto iconstant_type;

			case '.':
				lxmore( lxchar, LEXDIG );

getfp:
				if( (lxchar = getchar()) == 'e' || lxchar == 'E' )
				{
					 /* exponent */
				case 'e':
				case 'E':
					if( (lxchar = getchar()) == '+' ||
					    lxchar == '-' )
					{
						*lxgcp++ = 'e';
					}
					else
					{
						ungetc(lxchar,stdin);
						lxchar = 'e';
					}
				/* bk: skip whitespace (fixes ansi cpp problem: 1.0e+MACRO) */
					{
						int c;
						while( (c = getchar()), lxmask[c+1]&LEXWS )
							;
						ungetc(c,stdin);
					}
					lxmore( lxchar, LEXDIG );
					/* now have the whole thing... */
				}
				else
				{
					  /* no exponent */
					ungetc( lxchar ,stdin);
				}
# ifdef NOFLOAT
				if( !flflag )
				{
					flflag = 1;
					werror( W_DFLT, "sorry, no floating point" );
				}
				for( cp = yytext; *cp; ++cp )
				{
					if( *cp == 'e' || *cp == 'E' ||
					    *cp == '.' )
					{
						*cp = '\0';
						break;
					}
				}
				goto conv10;
# else
				flflag = 1;
					/* library conversion (atof) forgives (e.g.) "1e+" */
					/* but we can't */
				if ( *(yytext + strlen(yytext)-1) == '+' ) {
					uerror( "misformed floating constant" );
					dcon = 0;
				} else {
					dcon = FLOATCVT(yytext);
					if ( errno == ERANGE )		/* bk */
						werror( W_STRICT, "floating constant overflow" );
				}

				/* finally, look for trailing L or F */	/*bk*/
				yylval.intval = 0;			/* double precision by default */
				if( (lxchar=getchar()) == 'L' || lxchar == 'l' )
					;			/* note: 'l' suffix ignored */
				else if ( lxchar == 'F' || lxchar == 'f' ) {
					yylval.intval = 1;		/* single precision */
					if ( dcon > MAXFLOAT )
						werror( W_STRICT, "floating constant overflow" );
				}
				else ungetc( lxchar ,stdin);
				return( FCON );
# endif

			default:
				ungetc( lxchar ,stdin);
				if( yytext[0] == '0' )
				{
					/* convert in octal */
					int i;
					for( cp = yytext+1; *cp; ++cp )
					{
						if ( *cp == '8' || *cp == '9' )
							werror( W_DFLT, "misformed octal constant" );
						lastcon <<= 3;
						lastcon += *cp - '0';
					}
							/* bk: check for > 037777777777 */
					i=strlen(yytext);
					if ( i > 12 || 
						( i==12 && strcmp(yytext,MAX_ULONG_OCT)>0 ) )
						werror( W_STRICT, "octal integer constant overflow" );
					goto iconstant_type;
				}
				else
				{
					/* convert in decimal */
# ifdef NOFLOAT
conv10:
# endif
					/* bk: check for overflow boundary condition */
					int yyleng;
					for( cp = yytext ; *cp ; ++cp )
					{
						lastcon = lastcon*10 + *cp - '0';
					}
							/* 11 = max # of digits in dec constant */
					yyleng = cp - yytext;
					if ( yyleng > 11 || 
						( yyleng==11 && strcmp(yytext,MAX_ULONG_DEC)>0 ))
						werror( W_STRICT, "decimal integer constant overflow" );
				}

iconstant_type:				/* bk: properly type an integer constant */
				/* (bk) Decide if integer constant is unsigned or not. */
				/* Since, on our machine sizeof int == sizeof long */
				/* the requirements of ANSI 3.1.2.2 reduce to a simple */
				/* decision between int and uint. The "L" suffix */
				/* is ignored, except for syntax checking (e.g. 20LLU). */
				/* A "U" suffix of a value > INT_MAX will force an */
				/* unsigned int. Note that we have already checked for */
				/* overflow above UINT_MAX while converting yytext. */

				/* look for trailing long/unsigned suffixes */
				yylval.intval = 0;
				while (1)
				{	
					lxchar = getchar();
					if ( lxchar == 'L' || lxchar == 'l' ) {
						if ( yylval.intval & 1 )
							goto getout;
						yylval.intval |= 1;
					} else {
						if ( lxchar == 'U' || lxchar == 'u' ) {
							if ( yylval.intval & 2 )
								goto getout;
							yylval.intval |= 2;
						} else {
getout:
							ungetc( lxchar ,stdin);
							break;
						}
					}
				} 
				yylval.intval = ( yylval.intval & 2 ) ? 1 : 0;
				if ( lastcon & ONEBIT(SZLONG-1) )
					yylval.intval = 1;
				return( ICON );
			}

		case A_DOT:
			/* a dot: if followed by a digit, floating point */
			lxchar = getchar();
			if( lxmask[lxchar+1] & LEXDIG )
			{
				ungetc(lxchar,stdin);
				lxget( '.', LEXDIG );
				goto getfp;
			} 
			if ( lxmask[lxchar+1] & LEXDOT ) {		/* try for ellipsis */
				lxchar = getchar();
				if (!( lxmask[lxchar+1] & LEXDOT )) { 	/* nope */
					uerror( "syntax error" );
					goto onechar;
				}
				return( ELLIPSIS );
			}
			stwart = FUNNYNAME;
			goto onechar;

		case A_STR:
			/* string constant */
getstring:
			lxmatch = '"';
			return( STRING );

		case A_CC:
			/* character constant */
getccon:
			lxmatch = '\'';
			lastcon = 0;
			lxstr(0);
			yylval.intval = 0;
			return( ICON );

		case A_BCD:
			{
				register i;
				register j;
				for( i=0; i<LXTSZ; ++i )
				{
					if( ( j = getchar() ) == '`' ) break;
					if( j == '\n' )
					{
						uerror(
						"newline in BCD constant" );
						break;
					}
					yytext[i] = j;
				}
				yytext[i] = '\0';
				if( i>6 )
					uerror(
					"BCD constant exceeds 6 characters" );
# ifdef gcos
				else strtob( yytext, &lastcon, i );
				lastcon >>= 6*(6-i);
# else
				if( yytext[0] == 'u' )
				{
					yylval.intval = UOP0 + yytext[1]-'0';
					return( UNOP );
				}
				uerror( "gcos BCD constant illegal" );
# endif
				yylval.intval = 0;  /* not long */
				return( ICON );
			}

		case A_SL:
			/* / */
			if( (lxchar = getchar()) != '*' ) {
				if (lxchar == '=' ) {
					yylval.intval = DIV;
					return( DIVOPASG );
				}
				goto onechar;
			}
			lxcom();
		case A_WS:
			continue;

		case A_NL:
			++lineno;
			lxtitle();
			continue;

		case A_MUL:
			/* * */
			if( (lxchar = getchar()) != '=' ) goto onechar;
			return( yylval.intval = MULASG );

		case A_MOD:
			/* % */
			if( (lxchar = getchar()) != '=' ) goto onechar;
			yylval.intval = MOD;
			return( DIVOPASG );

		case A_NOT:
			/* ! */
			if( (lxchar = getchar()) != '=' ) goto onechar;
			yylval.intval = NE;
			return( EQUOP );

		case A_MI:
			/* - */
			if( (lxchar = getchar()) == '-' )
			{
				yylval.intval = DECR;
				return( INCOP );
			}
			if( lxchar == '=' ) 
				return( yylval.intval = MINUSASG );
			if( lxchar != '>' ) goto onechar;
			stwart = FUNNYNAME;
			yylval.intval=STREF;
			return( STROP );

		case A_PL:
			/* + */
			if( (lxchar = getchar()) == '+' )  {
				yylval.intval = INCR;
				return( INCOP );
			}
			if( lxchar == '=' ) 
				return( yylval.intval = PLUSASG );
			goto onechar;

		case A_ER:
			/* ^ */
			if( (lxchar = getchar()) != '=' ) goto onechar;
			return( yylval.intval = ERASG );

		case A_AND:
			/* & */
			if( (lxchar = getchar()) != '&'  &&  lxchar != '=' )
				goto onechar;
			return( yylval.intval = (lxchar == '&') ? ANDAND : ANDASG );

		case A_OR:
			/* | */
			if( (lxchar = getchar()) != '|'  &&  lxchar != '=' )
				goto onechar;
			return( yylval.intval = (lxchar == '|') ? OROR : ORASG );

		case A_LT:
			/* < */
			if( (lxchar = getchar()) == '<' )
			{
				yylval.intval = LS;
				if( (lxchar = getchar()) == '=' )
					return( SHIFTOPASG );
				ungetc( lxchar, stdin );
				return( SHIFTOP );
			}
			if( lxchar != '=' ) goto onechar;
			yylval.intval = LE;
			return( RELOP );

		case A_GT:
			/* > */
			if( (lxchar = getchar()) == '>' )
			{
				yylval.intval = RS;
				if( (lxchar = getchar()) == '=' )
					return( SHIFTOPASG );
				ungetc( lxchar, stdin );
				return(SHIFTOP );
			}
			if( lxchar != '=' ) goto onechar;
			yylval.intval = GE;
			return( RELOP );

		case A_EQ:
			/* = */
			switch( lxchar = getchar() )
			{

			case '=':
				yylval.intval = EQ;
				return( EQUOP );

			default:
				goto onechar;

			}


		default:
			cerror( "yylex error, character %03o (octal)", lxchar );

		}

		/* ordinarily, repeat here... */
		cerror( "out of switch in yylex" );

	}

}


/************************************************************************
 * Resolve ambiguity between typedef name and identifier.  Return 1     *
 * if identifier, 0 if typedef name. (bk)                               *
 *                                                                      *
 * Consider the following declarations involving a typedef name, UINT:  *
 * typedef unsigned int UINT;       ** initial declaration              *
 * UINT x;                          ** yylex lookup, sclass=TYPEDEF		*
 * short UINT y;                    ** same                             *
 * typedef unsigned in UINT;        ** here, we return NAME from yylex, *
 *                                  ** since tdef_look finds that       *
 *                                  ** lookahead = semicolon.           *
 * int foo( UINT, int );            ** tdef_look returns 0, since we    *
 *                                  ** haven't seen a type-specifier.   *
 * int foo( UINT x, int );          ** tdef_look returns 1              *
/************************************************************************/
#ifdef NEVER
int
tdef_look()
{
	register lxchar;
	register struct lxdope *p;
	int ws = 1;				/* continue while there's whitespace */

	do {
		switch( (p = lxcp[(lxchar = getchar())+1])->lxact ) 
		{
			case A_SL:
				/* / */
				if( (lxchar = getchar()) != '*' ) {
					ungetc( lxchar, stdin );
					ungetc( '/', stdin );
					return 0;
				} else {
					lxcom();
				}
				/* Fall-through */
			case A_WS:
				continue;
			default:
				ws = 0;
				break;
		}
	} while ( ws );

	ungetc( lxchar, stdin );

/***	if ( seen_typespec && (lxchar == ',') ) {
		seen_typespec = 0;
		return 1;
	}  ****/
	return (( lxchar == ';' ) || 
			( lxchar == '=' ) ||
			( seen_typespec && (lxchar == ',') ) );
}
#endif


struct lxrdope
{
	/* dope for reserved, in alphabetical order */

	char *lxrch;	/* name of reserved word */
	short lxract;	/* reserved word action */
	short lxrval;	/* value to be returned */
} lxrdope[] =
{
	"__argtype",AR_RW,	ARGTYPE,	/* bk */
	"_asm",		AR_A,	0,			/* bk: added leading underscore for ANSI */
	"_fortran",	AR_CL,	FORTRAN,	/* bk: added leading underscore for ANSI */
	"asm",		AR_A,	0,
	"auto",		AR_CL,	AUTO,
	"break",	AR_RW,	BREAK,
	"char",		AR_TY,	CHAR,
	"case",		AR_RW,	CASE,
	"const",	AR_TQ,	CONST,
	"continue",	AR_RW,	CONTINUE,
	"double",	AR_TY,	DOUBLE,
	"default",	AR_RW,	DEFAULT,
	"do",		AR_RW,	DO,
	"extern",	AR_CL,	EXTERN,
	"else",		AR_RW,	ELSE,
	"enum",		AR_E,	ENUM,
# ifdef ONEFLOAT
	"float",	AR_TY,	DOUBLE,
# else
	"float",	AR_TY,	FLOAT,
# endif
	"for",		AR_RW,	FOR,
	"fortran",	AR_CL,	FORTRAN,
	"goto",		AR_RW,	GOTO,
	"if",		AR_RW,	IF,
	"int",		AR_TY,	INT,
	"long",		AR_TY,	LONG,
	"return",	AR_RW,	RETURN,
	"register",	AR_CL,	REGISTER,
	"switch",	AR_RW,	SWITCH,
	"struct",	AR_S,	0,
	"sizeof",	AR_RW,	SIZEOF,
	"short",	AR_TY,	SHORT,
	"signed",	AR_TY,	SIGNED,	
	"static",	AR_CL,	STATIC,
	"typedef",	AR_CL,	TYPEDEF,
	"unsigned",	AR_TY,	UNSIGNED,
	"union",	AR_U,	0,
	"void",		AR_TY,	VOID,
	"volatile",	AR_TQ,	VOLATILE,
	"while",	AR_RW,	WHILE,
	"",		0,	0,	/* to stop the search */
};

lxres()
{
	/* check to see of yytext is reserved; if so,
	** do the appropriate action and return
	*/
	/* otherwise, return -1 */

	register c, ch;
	register struct lxrdope *p;

	ch = yytext[0];

	if( !islower(ch) && ch!='_' ) return( -1 );

	switch( ch )
	{

	case '_':
		c=0;
		break;
	case 'a':
		c=3;
		break;
	case 'b':
		c=5;
		break;
	case 'c':
		c=6;
		break;
	case 'd':
		c=10;
		break;
	case 'e':
		c=13;
		break;
	case 'f':
		c=16;
		break;
	case 'g':
		c=19;
		break;
	case 'i':
		c=20;
		break;
	case 'l':
		c=22;
		break;
	case 'r':
		c=23;
		break;
	case 's':
		c=25;
		break;
	case 't':
		c=31;
		break;
	case 'u':
		c=32;
		break;
	case 'v':
		c=34;
		break;
	case 'w':
		c=36;
		break;

	default:
		return( -1 );
	}

	for( p= lxrdope+c; p->lxrch[0] == ch; ++p )
	{
		if( !strcmp( yytext, p->lxrch ) )
		{
			 /* match */
			switch( p->lxract )
			{

			case AR_TQ:		/* bk */
				if ( ! mode_chk( MODE_ANSIKEYW ) )
					return( -1 );
				yylval.intval = p->lxrval;
				return( TYPEQ );
				break;

			case AR_TY:
				/* type word */
				if ( p->lxrval == SIGNED && !mode_chk(MODE_ANSIKEYW) )
					return( -1 );
				stwart = instruct;
				if ( p->lxrval <= DOUBLE ) seen_typespec = 1;
				else seen_typespec = 0;
				yylval.nodep = mkty( (TWORD)p->lxrval, 0, p->lxrval, 0 );
				if( p->lxrval==FLOAT || p->lxrval==DOUBLE )
				{
# ifdef NOFLOAT
					if( !flflag )
					{
						flflag = 1;
						werror( W_DFLT, "sorry, no floating point" );
					}
					yylval.nodep->tn.type = LONG;
# else
					flflag = 1;
# endif
				}
				return( TYPE );

			case AR_RW:
				/* ordinary reserved word */
				return( yylval.intval = p->lxrval );

			case AR_CL:
				/* class word */
				/* fortran or _fortran */
				if ( p->lxrval == FORTRAN )
					if ( mode_chk( MODE_ASM ) ^ ch == 'f' )
						return( -1 );
				yylval.intval = p->lxrval;
				return( CLASS );

			case AR_S:
				/* struct */
				stwart = INSTRUCT|SEENAME|TAGNAME;
				yylval.intval = INSTRUCT;
				return( STRUCT );

			case AR_U:
				/* union */
				stwart = INUNION|SEENAME|TAGNAME;
				yylval.intval = INUNION;
				return( STRUCT );

			case AR_E:
				/* enums */
				stwart = SEENAME|TAGNAME;
				return( yylval.intval = ENUM );

			case AR_A:
				/* asm or _asm */
				if ( mode_chk( MODE_ASM ) ^ ch == 'a' )	/* bk */
					return( -1 );
				asm_esc = 1; /* warn the world! */
				lxget( ' ', LEXWS );
#ifdef IN_LINE
				if ((c=getchar()) != '(')
				{
					ungetc(c,stdin);
					yylval.intval = INLINE;
					return (CLASS);
				}
#else
				if( getchar() != '(' ) goto badasm;
#endif
				lxget( ' ', LEXWS );
				if( getchar() != '"' ) goto badasm;
				asmp = asmbuf;
				while( (c = getchar()) != '"' )
				{
					if( c=='\n' || c==EOF ) goto badasm;
					*asmp++ = c;
					if( asmp >= &asmbuf[ASMBUF-1] )
					{
						uerror( "asm > %d chars",
						ASMBUF);
					}
				}
				lxget( ' ', LEXWS );
				if( getchar() != ')' ) goto badasm;
				*asmp++ = '\0';
				return( ASM );

badasm:
				uerror( "bad asm construction" );
				return( 0 );

			default:
				cerror( "bad AR_?? action" );
			}
		}
	}
	return( -1 );
}

lxtitle()
{
	/* called after a newline; set linenumber and file name */

	register c, val;
	register char *cp;

	for(;;)
	{
		 /* might be several such lines in a row */

#ifdef ASSYCOMMENT
		c = eatline();
#else
		c = getchar();
#endif

		while( lxmask[c+1]&LEXWS )		/* bk: whitespace can precede # */
			c = getchar();

		if( c != '#' )
		{
			if( c != EOF ) ungetc(c,stdin);
			return;
		}

		lxget( ' ', LEXWS );

		c = getchar();

		/* "S-lists" feature:  Recognition of #ident <string> line.
		/* Information passed through to assembly code. 
		*/
		if (!isdigit(c))
		{
			ppcontrol(c);
			continue;
		}

		for( val = 0; isdigit(c); c=getchar() )
		{
			val = val*10+ c - '0';
		}
		ungetc( c, stdin );
		lineno = val;
		lxget( ' ', LEXWS );
		if( (c = getchar()) != '\n' )
		{
			for( cp = ftitle; c != '\n'; c = getchar(),++cp )
			{
				*cp = c;
			}
			*cp = '\0';

			bg_file();	/* having seen filename, do start-of-file
					** stuff
					*/
		}
	}
}

# ifndef MYASMOUT
asmout()
{
#ifdef ASM_COMMENT
	printf( "%s\n", ASM_COMMENT);
#endif
	printf( "%s\n", asmbuf );
#ifdef ASM_END
	printf( "%s\n", ASM_END);
#endif
}
# endif


#define NSAVETAB	4096
char	*savetab;
unsigned int	saveleft;

char *
savestr( cp )			/* place string into permanent string storage */
	register char *cp;
{
	register int len = strlen( cp ) + 1;
	extern char * malloc();
	extern char * strncpy();

	if ( len > saveleft )
	{
		saveleft = NSAVETAB;
		if ( len > saveleft )
			saveleft = len;
		if ( ( savetab = (char *) malloc( saveleft ) ) == 0 )
			cerror( "out of memory [savestr()]" );
	}
	strncpy( savetab, cp, len );
	cp = savetab;
	savetab += len;
	saveleft -= len;
	return ( cp );
}

#ifdef LINT
#	define LNCHNAM	8	/* length of symbols to check if high warning level */
#endif

/*
* The segmented hash tables.
*/
#define MAXHASH		20
#define HASHINC		1013
struct ht
{
	char	**ht_low;
	char	**ht_high;
	int	ht_used;
} htab[MAXHASH];


char *
hash( s )	/* look for s in seg. hash tables.  Not found, make new entry */
	char *s;
{
	register char **h;
	register int i;
	register char *cp;
	struct ht *htp;
	int sh;
	extern char * calloc();
#ifdef LINT
	char *found = 0;	/* set once LNCHNAM chars. matched for name */
#endif

	/*
	* Hash on the correct number of characters.  Lint needs to be able
	* to limit this so that it can note length of names for portablility
	* concerns.
	*/
	cp = s;
	i = 0;
#ifdef LINT
	while ( *cp && ( ( cp - s ) < LNCHNAM ) )
#else
	while ( *cp )
#endif
	{
		i = ( i << 1 ) + *cp++;
	}
	sh = ( i & 077777 ) % HASHINC;
	cp = s;
	/*
	* Look through each table for name.  If not found in the current
	* table, skip to the next one.
	*/
	for ( htp = htab; htp < &htab[MAXHASH]; htp++ )
	{
		if ( htp->ht_low == 0 )
		{
			register char **hp = (char **) calloc(
				sizeof (char **), HASHINC );

			if ( hp == 0 )
				cerror( "out of memory [hash()]" );
			htp->ht_low = hp;
			htp->ht_high = hp + HASHINC;
		}
		h = htp->ht_low + sh;
		/*
		* Use quadratic re-hash
		*/
		i = 1;
		do
		{
			if ( *h == 0 )
			{
				if ( htp->ht_used > ( HASHINC * 3 ) / 4 )
					break;
				htp->ht_used++;
				*h = savestr( cp );
#ifdef LINT
				if ( found )
				{
					/*
					* If high warning level set, then warn of greater
					* than LNCHNAM character names which
					* differ past the LNCHNAM'th character.
					*/
					werror( W_LINT, "`%s' may be indistinguishable from `%s' due to internal name truncation",
					*h, found );
				}
#endif
				return ( *h );
			}
#ifdef LINT
			if ( warn_lvl >= W_LINT )
			{
				if ( **h == *cp &&
					strncmp( *h, cp, LNCHNAM ) == 0 )
				{
					/*
					* We have matched on LNCHNAM chars.
					* Now, look for the ``total'' name.
					*/
					found = *h;
					if ( strcmp( *h, cp ) == 0 )
					{
						/*
						* This entry really is
						* the name we want.
						*/
						return ( *h );
					}
				}
			}
			else	/* insufficient warning level - use entire name length */
			{
				if ( **h == *cp && strcmp( *h, cp ) == 0 )
					return ( *h );
			}
#else
			if ( **h == *cp && strcmp( *h, cp ) == 0 )
				return ( *h );
#endif
			h += i;
			i += 2;
			if ( h >= htp->ht_high )
				h -= HASHINC;
		} while ( i < HASHINC );
	}
	cerror( "out of hash tables" );
/*NOTREACHED*/
}

ppcontrol( c )
	int	c;
/* For processing if #ident <string> or #pragma <string> source file lines.
* For #ident, string passed into ident assember directive line in output code.
*/
{
	char	cstring[7];
	int	i;

	for ( i = 0; isalpha(c) && i < 6; c = getchar() )
		cstring[i++] = c;
	cstring[i] = '\0';

	if (strcmp( cstring, "ident" ) == 0)
	{
		while( c == ' ' || c == '\t' ) c = getchar();
		if (c != '"')
		{
			uerror( "string expected after #ident " );
			for ( ; c != '\n' && c != EOF; c = getchar() )
				;
			ungetc( c, stdin );
			return;
		}

		/* before doing any output, initialize the world */
		bg_file();

		printf( "\t" );
		printf( IDENTSTR );
		printf( "\t\"" );
		for (c = getchar(); c != '"' && c != '\n' && c != EOF; c = getchar())
			putchar( c );

		putchar('"');
		putchar('\n');
		for ( ; c != '\n' && c != EOF; c = getchar() )
			;
		ungetc( c, stdin );
	}
	else if (strcmp( cstring, "pragma" ) == 0)	/*bk*/
	{
		do_pragma();
	}
	else
		uerror( "unknown control statement" );
}

/* do beginning of file stuff.  once */
bg_file()
{
    static int done = 0;		/* non-zero if we've been here before */

    if (! done) {
	beg_file();
	done = 1;
    }
    return;
}

/* bk: Pragma Directives */
/* For now, we just have boolean keywords */
/* NOTE that values are initialized for MODE 1 */
/* Pragmas beginning with capital letter are non-documented. */
t_modes modes[] = {
	"strnull",		0,		/* ary of size N init'd by string of size N */
	"asm",			1,		/* asm is reserved, not _asm */
	"float",		1,		/* float is a real type (not promoted to double) */
	"xblock",		1,		/* extern declarations obey block scope rules */
	"tagblock",		1,		/* tag declarations obey block scope rules */
	"unsigned",		0,		/* unsigned preserving conversion rules */
	"topdown",		1,		/* multi-dim array initialization parsed top-down */
	"strwrite",		1,		/* writable string literals */
	"ansikeyw",		1,		/* signed/const/volatile are keywords */
			/* non-documented pragmas must be last before show/reset */
	"Benign",		0,		/* allow benign redeclaration of typedef's & tags */
	/* "show", "reset", "SYS" must be last (before sentinel) */
	"show",			-1,		/* show current state of pragma settings */
	"reset",		-2,		/* reset to original settings */
	"SYS",			-3,		/* for system headers */
	0,				0		/* sentinel */
};

mode_init(mode)
int mode;
{
	switch ( mode ) {
		case MODE_CUC:
			modes[ MODE_ASM ].lxmval = 1;
			modes[ MODE_STRNULL ].lxmval = 1;
			modes[ MODE_FLOAT ].lxmval = 0;
			modes[ MODE_XBLOCK ].lxmval = 0;
			modes[ MODE_TAGBLOCK ].lxmval = 0;
			modes[ MODE_UNSIGNED ].lxmval = 1;
			modes[ MODE_TOPDOWN ].lxmval = 0;
			modes[ MODE_STRWRITE ].lxmval = 1;
			modes[ MODE_BENIGN ].lxmval = 0;
			modes[ MODE_ANSIKEYW ].lxmval = 0;
			break;
		case MODE_ANSI:
			modes[ MODE_ASM ].lxmval = 1;
			modes[ MODE_STRNULL ].lxmval = 0;
			modes[ MODE_FLOAT ].lxmval = 1;
			modes[ MODE_XBLOCK ].lxmval = 1;
			modes[ MODE_TAGBLOCK ].lxmval = 1;
			modes[ MODE_UNSIGNED ].lxmval = 0;
			modes[ MODE_TOPDOWN ].lxmval = 1;
			modes[ MODE_STRWRITE ].lxmval = 0;
			modes[ MODE_BENIGN ].lxmval = 0;
			modes[ MODE_ANSIKEYW ].lxmval = 1;
			break;
		case MODE_STRICT_ANSI:
			modes[ MODE_ASM ].lxmval = 0;
			modes[ MODE_STRNULL ].lxmval = 0;
			modes[ MODE_FLOAT ].lxmval = 1;
			modes[ MODE_XBLOCK ].lxmval = 1;
			modes[ MODE_TAGBLOCK ].lxmval = 1;
			modes[ MODE_UNSIGNED ].lxmval = 0;
			modes[ MODE_TOPDOWN ].lxmval = 1;
			modes[ MODE_STRWRITE ].lxmval = 0;
			modes[ MODE_BENIGN ].lxmval = 0;
			modes[ MODE_ANSIKEYW ].lxmval = 1;
			break;
		default:
			cerror( "Invalid mode" );
	}
}

do_pragma()
{
	register int c;
	char	cstring[15];
	struct s_modes *p;				/* pointer to keyword info */
	char *keyp;							/* pointer to input keyword */
	int newval;							/* new boolean value */
	int	i;

	do {

		do 
			c = getchar();
		while( c == ' ' || c == '\t' );

		for ( i = 0; isalpha(c) && i < sizeof(cstring)-1; c = getchar() )
			cstring[i++] = c;
		cstring[i] = '\0';

		if ( strncmp( cstring, "no", 2 ) == 0 ) {
			keyp = cstring+2;
			newval = 0;
		} else {
			keyp = cstring;
			newval = 1;
		} 

		for ( p = modes; p->lxmch; p++ ) {
			if ( strcmp( p->lxmch, keyp ) == 0 ) {

				switch ( p->lxmval ) {

					case -1:				/* "show" current settings */
					{
						struct s_modes *s;
						int column = 0;
						int width;

						if ( newval == 0 ) 		/* #pragma noshow (!!) */
							goto unknown;

						width = where('c');	/* note, where() ignores its arg */
						width += fprintf( stderr, "Pragma Settings: " ); 
						for ( s = modes; !isupper(s->lxmch[0]); ) {

							fprintf( stderr, "%s%s", 
										s->lxmval > 0 ? "" : "no", s->lxmch );
												/* not last in list? */
							if ( !isupper((++s)->lxmch[0]) ) {
								fprintf( stderr, ", " );
								if ( ++column >= 4 ) {
									fprintf( stderr, "\n%*s", width, " " );
									column = 0;
								} 
							}
						}
						putc( '\n', stderr );
						break;
					}

					case -2:						/* "reset" */
						if ( newval == 0 ) 		/* #pragma noreset (!!) */
							goto unknown;
						mode_init(compiler_mode);
						break;
					
					case -3:				/* "SYS": inside System Header */
						modes[ MODE_BENIGN ].lxmval = 1;
						modes[ MODE_ANSIKEYW ].lxmval = 1;
						break;

					default:
						if ( p->lxmval == newval )	/* check vs existing value*/
							werror( W_LINT, 
							"redundant pragma directive %s (ignored)", cstring);
						else {
							p->lxmval = newval;
						}
						break;
				}
				break;					/* lookup found something */
			}
		}

		if ( p->lxmch == 0 ) { 				/* lookup returns 0 */
unknown:
			if ( strcmp( cstring, "noregsave" ) == 0 ||
				strcmp( cstring, "regsave" ) == 0 ) {	/* echo pragma */
				printf( "#pragma %s\n", cstring );
			} else
			werror( W_DFLT, "unknown pragma directive %s (ignored)", cstring );
		}

		while( c == ' ' || c == '\t' ) c = getchar();

	} while ( c == ',' );					/* comma separated list */

eat:
	{
		int garbled = 0;
		for ( ; c != '\n' && c != EOF; c = getchar() )
			if ( c != ' ' && c != '\t' ) 
				garbled = 1;

		if ( garbled )
				werror( W_DFLT, "garbled pragma directive" );
	}

	ungetc( c, stdin );
}
