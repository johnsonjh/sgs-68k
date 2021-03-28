#ident	"@(#)parse.y	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
static	char	sccsid[] = "OLD MOT parse.y 7.1 5/30/85";
/* static	char	sccsid[] = "OLD MOT parse.y: 2.6 6/24/83"; */


/*			Motorola 68000 assembler			*/

#include	<stdio.h>
#include	<string.h>
#include	<scnhdr.h>
#include	"gendefs.h"
#include	"fpnum.h"		/* M68881 constant defination	*/

/* M68881 additions */
char	mantissa[80],exponent[80];
unsigned short	user_prec=UNSPEC;	/* user defined precision	*/
/* end M68881 additions */

extern int txtsec,txt2sec,txt3sec,dat2sec,datsec,bsssec;
extern unsigned short
		cline,		/* Line number from the .ln pseudo op.	*/
		line;		/* Current input file line number.	*/
extern short 	wflag;		/* generate word size for NAMEs		*/

extern FILE	*fdin;		/* Input source.			*/
extern short	opt;		/* Sdi optimization flag.		*/
extern char	file[],		/* Assembly input file name.		*/
		cfile[];	/* Original C file name.		*/

extern symbol	*dot;		/* Current pc pointer.			*/
extern long	newdot,		/* Up-to-date value of pc pointer.	*/
		dotbss;

struct arg	arglist[6];	/* 68020 instructions can have 6 arguments */
struct arg	*ap = arglist;
struct	exp	explist[100];
struct	exp	*xp = explist;

long		outword;	/* Bit field accumulator.		*/
int		bitsout;	/* Number of bits accumulated.		*/
BYTE		bytesout;	/* Output displacement.			*/
int		swbegct;	/* Number of switch locations.		*/

/*	M68020	*/
int		regseen;	/* count registers as we see them	*/
int		indexreg;	/* remember position of index register	*/
int		ssflag;		/* have we seen a size-scale		*/
int		lp_count;	/* used for matching parenthesis	*/
/*	M68020	*/


struct exp	*combine();	/* Handles expression arithmetic.	*/
extern upsymins	*lookup();	/* Symbol table lookup routine.		*/
extern		deflab();	/* Define label for sdi handler.	*/
extern		gen0op();	/* Handles no-operand instructions.	*/
extern		gen1op();	/* Handles one-operand instructions.	*/
extern		gen2op();	/* Handles two-operand instructions.	*/
extern		gen3op();	/* Handles three-operand instructions.	*/
extern		gen4op();	/* Handles four-operand instructions.	*/
extern		gen6op();	/* Handles six-operand instructions.	*/
extern		resolve();	/* resolves all SDI's to known length   */

char		err_buf[BUFSIZ];
char		yytext[BUFSIZ];
# define IFILE 257
# define ILN 258
# define IDEF 259
# define IENDEF 260
# define IVAL 261
# define ITYPE 262
# define ILINE 263
# define ISCL 264
# define ITAG 265
# define ISIZE 266
# define IDIM 267
# define ITYPEQ 268
# define ISET 269
# define IGLOBAL 270
# define IDATA 271
# define ITEXT 272
# define IBYTE 273
# define ISHORT 274
# define ILONG 275
# define ISECTION 276
# define ISPACE 277
# define IORG 278
# define ICOMM 279
# define ILCOMM 280
# define IEVEN 281
# define ISWBEG 282
# define IALIGN 283
# define IIDENT 284
# define IINIT 285
# define IVERSION 286
# define IROUND 287
# define IPREC 288
# define COLON 289
# define AMP 290
# define NOCHAR 291
# define SPC 292
# define ALPH 293
# define DIGIT 294
# define SQ 295
# define SH 296
# define CM 297
# define NL 298
# define LP 299
# define RP 300
# define SEMI 301
# define DQ 302
# define TWID 303
# define EXCLAM 304
# define LBRKT 305
# define RBRKT 306
# define LBRACE 307
# define RBRACE 308
# define PLUS 309
# define MINUS 310
# define MUL 311
# define DIV 312
# define UMINUS 313
# define NAME 314
# define NAMEW 315
# define INST0 316
# define INST1 317
# define INST2 318
# define INST3 319
# define INST4 320
# define INST6 321
# define REG 322
# define INST12 323
# define INST23 324
# define INST24 325
# define INST34 326
# define SIZE 327
# define INT 328
# define STRING 329
# define HEXSTR 330
# define DECSTR 331
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
# define YYERRCODE 256


int	type[] = {
	EOF,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	SPC,	NL,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	SPC,	EXCLAM,	DQ,	SH,	0,	REG,	AMP,	SQ,
	LP,	RP,	MUL,	PLUS,	CM,	MINUS,	SIZE,	DIV,
	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,
	DIGIT,	DIGIT,	COLON,	SEMI,	0,	0,	0,	0,
	0,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	LBRKT,	0,	RBRKT,	0,	ALPH,
	0,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	LBRACE,	0,	RBRACE,	TWID,	0,
};

yylex()
{
	upsymins	istp;
	static BOOL	mnemseen = FALSE; /* Mnemonic seen on line yet?	*/
	static BOOL SIZEseen = FALSE; /* a . is seen */
	register char	*yycharptr;
	register int	val;
	register int	ctype;
	register long	intval;

	if ( SIZEseen == TRUE ) 
	{	
		ctype = SIZE;
		SIZEseen = FALSE; 
	}
	else
	{
		while (type[(val = getc(fdin)) + 1] == SPC)
		;
		ctype = type[val+1];
	}

	switch (ctype)
	{

	case SEMI:	/* Reinitialize for next statement.		*/
		mnemseen = FALSE;
		return(SEMI);

	case SH:	/* Comment until the end of the line.		*/
	case EXCLAM:		/* bk */
		while ((val = getc(fdin)) != '\n' && val > 0)
			;
		/* Now same as in case NL.				*/

	case NL:	/* Reinitialize for next statement.		*/
		mnemseen = FALSE;
		return(NL);


	case TWID:
		val = '.';
	case ALPH:
		yycharptr = yytext;
		do
		{
#if FLEXNAMES
			if (yycharptr <= &yytext[BUFSIZ - 1])
#else
			if (yycharptr <= &yytext[NCHPS])
#endif
				*yycharptr++ = val;
		} while ((ctype = type[(val = getc(fdin)) + 1]) == ALPH || ctype == DIGIT || ctype == REG);
		*yycharptr = '\0';
		while (val=='\t' || val==' ')
			val = getc(fdin);
		ungetc(val,fdin);
		if (val == ':')
			istp = *lookup(yytext,INSTALL,USRNAME);
		else
			if (mnemseen){
				istp = *lookup(yytext,INSTALL,USRNAME);
				if( val == '.' ){
					val = getc(fdin); val = getc(fdin);
					{
						if( val == 'w' || val == 'W' ){
							yylval.yname = istp.stp;
							return(NAMEW);
						}
					val = getc(fdin); ungetc(val,fdin);
					}
				}
			}else
			{
				istp = *lookup(yytext,N_INSTALL,MNEMON);
				if (istp.stp == NULL)
					yyerror("invalid instruction name");
				yylval.yinst = istp.itp;
				mnemseen = TRUE;
				return(istp.itp->val + 256);
			}
		yylval.yname = istp.stp;
		return(NAME);

	case REG:
		yycharptr = yytext;
		do
		{
#if FLEXNAMES
			if (yycharptr <= &yytext[BUFSIZ - 1])
#else
			if (yycharptr <= &yytext[NCHPS])
#endif
				*yycharptr++ = val;
		} while ((ctype = type[(val = getc(fdin)) + 1]) == ALPH || ctype == DIGIT || ctype == REG);
		ungetc(val,fdin);
		*yycharptr = '\0';
		istp = *lookup(yytext,N_INSTALL,MNEMON);
		if (istp.stp == NULL || istp.itp->val != REG-256)
			yyerror("invalid register name");
		yylval.yinst = istp.itp;
		return(REG);

	case DIGIT:
		intval = val - '0';
		if (val == '0')
		{
			val = getc(fdin);
			if (val == 'x' || val == 'X')
			{	/* hex base	*/
				yylval.ystr = yycharptr = yytext;
				while ( hexdigit(val=getc(fdin)) )
				{
					intval <<= 4;
					if ('a' <= val && val <= 'f')
						intval += val - 'a' + 10;
					else if ('A' <= val && val <= 'F')
						intval += val - 'A' + 10;
					else intval += val - '0';
#ifdef M68881
					/* floating-point numbers can be specified explicitly as
	 				 * a series of hexadecimal digits preceded by 0x
	 				 */
					if ( yycharptr < &yytext[BUFSIZ-1] )
						*yycharptr ++ = val;
#endif /*M68881*/
				}
#ifdef M68881
				*yycharptr = '\0';
				if ( strlen(yylval.ystr) > 8 ) /* more than 32 bits */
				{
					ungetc(val,fdin);
					return(HEXSTR);
				}
#endif /*M68881*/
			}	/* hex base	*/
			else
			if (type[val + 1] == DIGIT)
					do
					{	/* octal base	*/
						intval <<= 3;
						intval += val - '0';
					} while (type[(val = getc(fdin)) + 1] == DIGIT);
#ifdef M68881
			else
			if (val == '.')
			{
				yylval.ystr = yycharptr =  yytext;
				goto parse_dig;
			}
#endif /* M68881 */
		}	/* hex or octal base	*/
		else
		{	/* decimal base	*/
			yylval.ystr = yycharptr =  yytext;
			*yycharptr ++ = val;

			while (type[(val = getc(fdin)) + 1] == DIGIT)
			{
				intval *= 10;
				intval += val - '0';
#ifdef M68881
				if ( yycharptr   <= &yytext[BUFSIZ - 1])
					*yycharptr  ++ = val;
#endif /* M68881 */
			}
#ifdef M68881
			if ( val == '.' )
	parse_dig:
			{	/* floating point fraction */

				if(type[(val = getc(fdin)) + 1] != DIGIT &&
					(val != 'e' && val != 'E'))
				{
					SIZEseen = TRUE;	/* indicate a . is seen */
					ungetc(val,fdin);
					yylval.yint = intval;
					return(INT);
				}
				/* fall through to parse_fp */
			}
			else
			{	/* decimal base */
				ungetc(val,fdin);
				yylval.yint = intval;
				return(INT);
			}
	parse_fp :
			/* a digit or E is seen after a decimal point */
			fp_p = 0;
			fp_man = mantissa;
			fp_exp = exponent;
			fp_expsign = POS;
			if ( val != 'e' && val != 'E' )
				do
				{	/* keep on packing digits to mantissa */
					/* count no. of digits after the dec. pt */
					*yycharptr ++ = val;
					fp_p ++;
				} while (type[(val = getc(fdin)) + 1] == DIGIT &&
					(yycharptr    <= &yytext[BUFSIZ - 1]) );

			*yycharptr = '\0';
			strcpy(fp_man,yylval.ystr);
			/* intialize pointer for exponent */
			yylval.ystr = yycharptr = yytext;
			if ( val == 'e' || val == 'E' )
			{
				val = getc(fdin);
				if( val  == '-' )
					fp_expsign = NEG;
				else
				if( type [val + 1] == DIGIT)
					*yycharptr ++ = val;

				while ( type[(val = getc(fdin)) + 1] == DIGIT &&
					(yycharptr    <= &yytext[BUFSIZ - 1]) )
					*yycharptr ++ = val;
			}	/* end E-real */
			*yycharptr = '\0';
			strcpy(fp_exp,yylval.ystr);
			if ( strlen(fp_man) > MANMAX )
				werror("Up to 17 significant digits are allowed");
			if ( strlen(fp_exp) > EXPMAX )
				werror("Up to 3 exponent digits are allowed");
			ungetc(val,fdin);
			return ( DECSTR );
#endif /* M68881 */
		}	/* decimal base	*/
		ungetc(val,fdin);
		yylval.yint = intval;
		return(INT);

	case SIZE:
		val = getc(fdin);
		yylval.yint = UNSPEC;
		if (val == 'b' || val == 'B')
			yylval.yint = B;
		else if (val == 'w' || val == 'W')
			yylval.yint = W;
		else if (val == 'l' || val == 'L')
			yylval.yint = L;
		else if (val == 's' || val == 'S')
			yylval.yint = S;
		else if (val == 'x' || val == 'X')
			yylval.yint = X;
		else if (val == 'p' || val == 'P')
			yylval.yint = P;
		else if (val == 'd' || val == 'D')
			yylval.yint = D;
#ifdef M68881
		/* check for round mode */
		else if (val == 'n' || val == 'N')
			yylval.yint = RN;
		else if (val == 'm' || val == 'M')
			yylval.yint = RM;
		else if (val == 'z' || val == 'Z')
			yylval.yint = RZ;
		else
		/* check for floationg point integer */
		if( type [val+1] == DIGIT)
		{
			yylval.ystr = yycharptr =  yytext;
			goto parse_fp;
		}
#endif /* M68881 */
		else
			yyerror("unknown size specification");
		return(SIZE);

	case SQ:
		if ((val = getc(fdin)) == '\n')
			line++;
		if (val == '\\')
			switch (val = getc(fdin))
			{
			/* hoss#21 */
			case 'a':
				val = '\007';
				break;
			case 'n':
				val = '\n';
				break;
			case 'r':
				val = '\r';
				break;
			case '\\':
			case '\'':
				break;
			case 'b':
				val = '\b';
				break;
			case 't':
				val = '\t';
				break;
			case 'v':
				val = '\013';
				break;
			case 'f':
				val = '\f';
				break;
			default:
				yyerror("illegal escaped character");
			}
		yylval.yint = val;
		return(INT);

	case DQ:
		yylval.ystr = yycharptr = yytext;
		while ((val = getc(fdin)) != '"' && val != '\n' && val != EOF)
		{
			if (yycharptr < &yytext[BUFSIZ-1])
				*yycharptr++ = val;
		}
		*yycharptr = '\0';
		if (val != '"')
			yyerror("nonterminated string");
		return(STRING);

	case 0:
		yyerror("illegal character");
		return(NOCHAR);


	default:
		return(ctype);
	}
}	/* yylex()	*/


hexdigit(val)
register int val;
{
	if ( type[ val + 1] == DIGIT ||
		('a' <= val && val <= 'f') ||
		('A' <= val && val <= 'F') )
		return(1);
	else
		return(0);
}

#ifdef M68881
/*
 * binstr - convert a floating point hex string to binary value.
 *          The final result is stored in 3 long words left justified.
 * 
 *          input : char *fp_string containing legal hex characters.
 *
 *          output: long xp->xvalue
 *                       xp->xvalue2
 *                       xp->xvalue3
 *          assumptions: xp-> is the current pointer
 */
binstr(fp_string)
char *fp_string;
{
	register int val;
	register int intval;
	register int temp_fp;
	short int index;
	short int nchar;
	short int pos;	/* position (bit) of the most significant byte */
	long fp_bin[3];	/* temporary storage for fp binary */

	if ( strlen(fp_string) > 24 )
		yyerror("floating point hex format allows up to 24 hex digits");

	/* initialize fp_bin */
	for ( index = 0; index < 3 ; index++)
		fp_bin[index] = 0;

	val = *fp_string++;
	for ( index = 0; val != '\0' && index < 3; index++) 
	{
		temp_fp = 0;
		for (nchar = 1, intval = 0 , pos = 32; 
			val != '\0' && nchar <= 8 ; nchar ++, val = *fp_string++)
		{
			/* convert each character to hex value 
			 * and pack them into a long word left justified
			 */
			if ( 'a' <= val && val <= 'f' )
				intval = val - 'a' + 10 ;
			else
			if ( 'A' <= val && val <= 'F' )
				intval = val - 'A' + 10;
			else
				intval = val - '0';

			temp_fp |= (intval <<= (pos -= 4));
		}
		
		fp_bin[index] = temp_fp;
	}


	/* return expression value */
	xp->xvalue = fp_bin[0];
	xp->xvalue2 = fp_bin[1];
	xp->xvalue3 = fp_bin[2];
	
}	/* end binstr */
#endif /* M68881 */

struct exp *
combine(operator,left,right)
int		operator;
register struct exp	*left,
		*right;
{
	switch (operator)
	{
	case PLUS:
		if (left->xsymptr == NULL)
		{
			left->xsymptr = right->xsymptr;
			left->xtype = right->xtype;
		}
		else if (right->xsymptr != NULL)
			yyerror("illegal addition");
		left->xvalue += right->xvalue;
		return(left);
	case MINUS:
		if (swbegct > 0)
			return(left);
		if (left->xsymptr == NULL)
		{
			if (right->xsymptr != NULL)
				yyerror("illegal subtraction");
			left->xvalue -= right->xvalue;
		}
		else if (right->xsymptr == NULL)
			left->xvalue -= right->xvalue;
		else if (left->xtype == right->xtype && left->xtype != UNDEF)
		{
			left->xtype = ABS;
			left->xvalue = left->xvalue - right->xvalue +
				       left->xsymptr->value - right->xsymptr->value;
			left->xsymptr = NULL;		/* no symbol now */
		}
		else
		{
			yyerror("illegal subtraction");
			left->xsymptr = NULL;
			left->xtype = ABS;
			left->xvalue = 0;
		}
		return(left);
	case MUL:
		if (left->xsymptr != NULL || right->xsymptr != NULL)
			yyerror("illegal multiplication");
		left->xsymptr = NULL;
		left->xtype = ABS;
		left->xvalue *= right->xvalue;
		return(left);
	case DIV:
		if (left->xsymptr != NULL || right->xsymptr != NULL)
			yyerror("illegal division");
		left->xsymptr = NULL;
		left->xtype = ABS;
		left->xvalue /= right->xvalue;
		return(left);
	}
}	/* combine()	*/


extern short	readonly;	/* Put data into text space. 		*/

fill(nbytes)
register long	nbytes;
{
	register long	fillval;

	if (nbytes <= 0)
	    return;
	
	if (newdot & 1)			/* always use FILL to get to even
					** boundary
					*/
	{
	    nbytes--;
	    generate(BITSPBY,0,FILL,NULLSYM);
	}

	fillval = ((dot->styp & TYPE) == TXT && 
			/* check for SPECIAL CASES!!! */
				!readonly && 				/* could be readonly data */
				dot->sectnum!=2 ) ?			/* "const" data section */
				TXTFILL : FILL;

	while (nbytes >= 2)
	{
		generate(BITSPOW,0,fillval,NULLSYM);
		nbytes -= 2;
	}
	if (nbytes)			/* fill out odd byte with FILL */
		generate(BITSPBY,0,FILL,NULLSYM);
}	/* fill()	*/



ckalign(size)
register int	size;
{
	register int	mod;

	if ((mod = newdot % size) != 0)
		fill(size - mod);
}	/* ckalign()	*/

/*
 *		68020 SUPPORT FUNCTIONS
 *
 */

#define NULLREG	(instr *)0

/* mk_indirect:
 *
 *	This module is used for addressing syntax
 *	d(reg), (d,reg), and (reg,d)
 *	It validates the input register and generates
 *	efficient addressing mode as much as possible.
 */
mk_indirect(type,reg,disp)
short type;
instr	*reg;
struct	exp	*disp;
{
	/* d(An), d(pc)
	 * 1. if ssflag is off, register may be an base register,
	 *    then check for valid base register for indirect with
	 *    displacement.
	 *    if displacement is within 16 bits then generate register
	 *    indirect with displacement or program counter indirect
	 *    with displacement.
	 * 2. if ssflag is on, register may be an index register,
	 *    then check for valid index register.
	 */
	int		distance;	/* distance between label and currect pc */
		if ( !ssflag && ( reg->tag == PCREG || reg->tag == AAREG ) )
		{
			/* d(pc), d(An) */
			if ( disp->xsize == B || disp->xsize == W )
				mkdisp_indirect(INDIRECT,reg,disp);
			else
			if ( disp->xsymptr==NULL &&
				disp->xvalue <= 32767L && disp->xvalue >= -32768L )

				mkdisp_indirect(INDIRECT,reg,disp);
			else
			if ( disp->xsymptr!=NULL &&
				(disp->xsymptr->styp & TYPE) != UNDEF &&
			 	(disp->xsymptr->styp & TYPE) == dot->styp )
			{
				distance= disp->xsymptr->value - dot->value;
				if ( distance <= 32767L && distance >= -32768L )
					mkdisp_indirect(INDIRECT,reg,disp);
				else
					mkindex_indirect(INDIRECT,reg,0,disp);
			}
			else
				mkindex_indirect(INDIRECT,reg,0,disp);
		}
		else
		if ( reg->tag == AAREG || reg->tag == NULAREG 
			|| reg->tag == ADREG || reg->tag == NULDREG 
			|| reg->tag == PCREG  || reg->tag == NULPCREG )
			mkindex_indirect(INDIRECT,reg,0,disp);
		else
			yyerror ("illegal use of register in register indirect with displacement");
}

mkdisp_indirect(type,reg,disp)
short type;
instr	*reg;
struct	exp	*disp;
{	/* make addressing register indirect with displacement or
	 * make program counter indirect with displacement
	 */
			/*  d(An) / d(PC) */
			if (reg->tag == PCREG)
				ap->atype = APCOFF;
			else
			if (reg->tag == AAREG)
				ap->atype = AOFF;
			ap->xp = disp;
			ap->areg1 = reg;
			ap->areg2 = 0;
}

/*
 *	mkindex_indirect()	- enters with four arguments:
 *
 *		type (short)
 *		disp (struct *exp)
 *		reg1 (instr *)
 *		reg2 (instr *)
 *
 *	we're not absolutely sure which register is the
 *	base and which is the index when no size or scale
 *	has been specified up to this point, so we need
 *	to determine which is which.
 *	possible base registers are:
 *
 *		%an, %pc, %zan
 *
 *	and the base register may be missing altogether.
 *
 *	possible index registers are:
 *
 *		%dn, %an, %zan, %zdn
 *
 */
mkindex_indirect(type,reg1,reg2,disp)
short type;
instr	*reg1;
instr	*reg2;
struct	exp	*disp;
{
	instr	*tmpreg;

	/*
	 *	determine index register (if any and not specified)
	 *	and flip registers if necessary so base is reg1 and
	 *	index is reg2.
	 */

	/*
	 *	first: if no index register was found explicitly
	 *	(had a ".s" or "*scale" attribute), check for a 
	 *	"%d" register in the first position, and flip.
	 *	NOTE: at this point it does not matter if there is
	 *	"%d" register in the second position, this error is checked
	 *	for in the next routine called.
	 *
	 *	else, do we have a null first register and a "%a" register
	 *	with no size or scale attributes?  then move it to the
	 *	first position where it assumes the position of index register.
	 *	so we force lone "%a" registers otherwise unspecified to be
	 *	an index.
	 */

	if( (indexreg == 0 && reg1 && (reg1->tag == NULDREG || reg1->tag == ADREG)) 
		|| indexreg == 1 )
	{
		tmpreg = reg1;
		reg1 = reg2;
		reg2 = tmpreg;
	}
	else if( indexreg == 0 && reg1 == NULLREG && reg2 && 
		(reg2->tag == ADREG || reg2->tag == NULAREG) )
	{
		tmpreg = reg1;
		reg1 = reg2;
		reg2 = tmpreg;
	}
	else if( reg1 && (reg1->tag == AAREG || reg1->tag == NULAREG) 
			&& reg2 && (reg2->tag == PCREG || reg2->tag == NULPCREG) )
	{
		tmpreg = reg1;
		reg1 = reg2;
		reg2 = tmpreg;
	}

	mk_full_ind(type,reg1,reg2,disp);
}


/*
 *	common routine to construct indexed indirect
 *	addressing argument.
 *
 *	we enter with 4 arguments:
 *
 *		type		- type of indirect addressing, 3 kinds
 *		base		- the base register
 *		index		- the index register
 *		base_disp	- the displacement (before indirection)
 *
 *	the regs are preordered before entering this routine so we
 *	need only to check that the regs are the proper types for
 *	the given argument.
 *
 *	NOTE: this routine does not handle the memory indirect case.
 */


mk_full_ind(type,base,index,base_disp)
short	type;
instr	*base;
instr	*index;
struct exp	*base_disp;
{
	int		distance;	/* distance between label and current pc */


	if( base && !(base->tag == AAREG || base->tag == PCREG 
		|| base->tag == NULAREG || base->tag == NULPCREG) )
	{
		yyerror("base register must be %pc, %an, %zpc or %zan");
		base = NULLREG;
	}


	if( index && !(index->tag == ADREG || index->tag == AAREG 
		|| index->tag == NULDREG || index->tag == NULAREG) )
	{
		yyerror("index register must be %dn, %an, %zdn or %zan");
		index = NULLREG;
	}

	/*
	 *	null out regs tagged NULxxx 
	 */

	if( base && ( base->tag == NULAREG || base->tag == NULPCREG ) )
		base = NULLREG;

	if( index && ( index->tag == NULDREG || index->tag == NULAREG ) )
		index = NULLREG;

	/*
	 *	set addressing mode, assign regs to argument and return
	 */

	if( base && base->tag == PCREG )
		ap->atype = APCNDX;
	else
		ap->atype = ANDX;

	ap->aflags = type;
	ap->xp1 = base_disp;

	/*
	 *	only set base and index members of ap
	 *	if set.  they are initialized to null.
	 */

	if( base )
		ap->areg1 = base;
	if( index )
		ap->areg2 = index;

	/* generate brief format:
	 * if no memory indirect, both base and index register exist
	 * (no suppressed register and displacement fixex into a byte
	 */
	if (ap->aflags == INDIRECT && base && index && ap->xp1)
	{
		if ( ap->xp1->xsize == B )
			ap->aflags = BFINDIRECT;
		else
		if (ap->xp1->xsymptr == NULL &&
			( ap->xp1->xvalue >= -128 && ap->xp1->xvalue <= 127))
			ap->aflags = BFINDIRECT;
		else
		if (ap->xp1->xsymptr != NULL &&
			(ap->xp1->xsymptr->styp!= UNDEF) &&
			(ap->xp1->xsymptr->styp & TYPE ) == dot->styp )
		{
				distance = ap->xp1->xsymptr->value - dot->value;
				if (distance >= -128 && distance <= 127)
					ap->aflags = BFINDIRECT;
		}
	} 
}


/*
 *	mk_post_ind():	add in index and displacement for 
 *			post index memory indirect.
 *
 *	This routine is called after the grunt work performed
 *	on the common sub-expression by mkindex_indirect().
 *	It will check for valid index register, and set up
 *	pointer for index and displacement.
 *
 */

mk_post_ind(index,disp)
instr	*index;
struct exp	*disp;
{
	if(ap->aflags != POSTNDXMI ){
		yyerror ("syntax error");
		fprintf(stderr," %s\n",yytext);
	}

	if( index && !(index->tag == NULDREG || index->tag == ADREG
		|| index->tag == NULAREG || index->tag == AAREG) )
	{
		yyerror("only %an,%zan,%dn,%zdn index in memory indirect");
		index = NULLREG;
	}

	if( index && ( index->tag == NULDREG || index->tag == NULAREG ) )
		index = NULLREG;

	ap->areg2 = index;
	ap->xp2 = disp;		/* displacement after memory indirection */
}

power(x,n)	/* raise x to n-th power ; n => 0 */
int x, n;
{
	int value;
	for ( value = 1; n > 0; --n)
		value = value * x;
	return (value);
}

/* Modules required for M68881 coprocessor */
get_fp_prec(prec)
short int prec;
{
	switch((int)(unsigned char)	prec )
	{
		case S:
			user_prec = FP_S;
			break;
		case D:
			user_prec = FP_D;
			break;
		case X:
			user_prec = FP_X;
			break;
		case P:
			user_prec = FP_P;
			break;
		default :
			aerror("Illegal floating point precision in parser");
	}	/* end switch */


}	/* end get_fp_prec */

get_fp_round(rd_mode)
short int rd_mode;
{
	switch((int)(unsigned char)	rd_mode )
	{
		case RN:
			fp_round = FP_RN;
			break;
		case P:
			fp_round = FP_RP;
			break;
		case RM:
			fp_round = FP_RM;
			break;
		case RZ:
			fp_round = FP_RZ;
			break;
		case UNSPEC:
			fp_round = FP_RN;
			break;
		default :
			aerror("Illegal floating point rounding mode in parser");
	}	/* end switch */
}	/* end get_fp_round */

/*
 *	GET_VALUE
 *		Returns floating point value in binary format for symbol.
 *	Requires Global variables:
 *	FP_STR:
 *		fp_man		floating point mantissa string
 *		fp_exp		floating point exponent string
 *		fp_mansign	sign of mantissa
 *		fp_expsign	sign of exponent
 *		fp_p		no. of decimal digits to the right of dec. pt.
 *
 *	user_prec:		if not defined, precision .X will be used.
 */
get_value(sym)
symbol *sym;
{
	int	fp_num[6];

	switch((int)(unsigned char)	user_prec )
	{
		case FP_S:
			fp_precision = FP_S;
			break;
		case FP_D:
			fp_precision = FP_D;
			break;
		case FP_X:
			fp_precision = FP_X;
			break;
		case FP_P:
			fp_precision = FP_P;
			break;
		default :
			fp_precision = FP_X;
	}	/* end switch */

	fp_extension = fp_precision;
	do_fp_num(fp_num);

	sym->value  = (fp_num[5] & 0xffff) ;
	sym->value  = (sym->value  << 16 | fp_num[4] & 0xffff);

	sym->value2  = (fp_num[3] & 0xffff) ;
	sym->value2  = (sym->value2  << 16 | fp_num[2] & 0xffff);

	sym->value3  = (fp_num[1] & 0xffff) ;
	sym->value3  = (sym->value3  << 16 | fp_num[0] & 0xffff);
}	/* end get_value */
yytabelem yyexca[] ={
-1, 1,
	0, -1,
	257, 6,
	258, 6,
	259, 6,
	260, 6,
	261, 6,
	262, 6,
	263, 6,
	264, 6,
	265, 6,
	266, 6,
	267, 6,
	268, 6,
	269, 6,
	270, 6,
	271, 6,
	272, 6,
	273, 6,
	274, 6,
	275, 6,
	276, 6,
	277, 6,
	278, 6,
	279, 6,
	280, 6,
	281, 6,
	282, 6,
	283, 6,
	284, 6,
	286, 6,
	287, 6,
	288, 6,
	298, 6,
	301, 6,
	314, 6,
	316, 6,
	317, 6,
	318, 6,
	319, 6,
	320, 6,
	321, 6,
	323, 6,
	324, 6,
	325, 6,
	326, 6,
	-2, 0,
-1, 87,
	299, 72,
	-2, 77,
	};
# define YYNPROD 139
# define YYLAST 696
yytabelem yyact[]={

    10,    12,    13,    14,    15,    16,    18,    19,    20,    21,
    51,    17,    23,    25,    27,    28,    29,    30,    31,    24,
    32,    33,    34,    35,    37,    38,    36,    11,   160,    26,
    40,    39,    59,   112,   113,   114,   115,   112,   113,   114,
   115,   183,   163,    86,    75,    54,    53,   245,    99,   136,
   164,    99,    84,   202,    83,    99,   137,     9,    74,    41,
    42,    43,    44,    45,    46,    61,    47,    48,    49,    50,
   136,    90,    61,   136,    61,   143,   140,    72,    71,    68,
    56,    57,    91,    60,    62,    60,   198,    56,    57,    56,
    57,    61,   114,   115,    58,   210,   141,   142,   320,   308,
    61,    58,    89,    58,   182,   305,    56,    57,   304,   189,
   166,    60,   111,   254,    88,    56,    57,   313,     5,   188,
    58,     6,   130,   136,   112,   113,   114,   115,   144,    58,
   112,   113,   114,   115,    90,    90,    90,    90,    90,    90,
    90,    90,    90,   312,   311,   145,   146,   147,   148,   149,
   150,   151,   152,   153,    61,   276,    61,   272,   256,   277,
   202,   328,   137,   324,   186,    60,   257,    60,   276,    56,
    57,    56,    57,    61,   187,   242,   258,   136,   160,   136,
   160,   160,   326,    58,    60,    58,   274,   321,    56,    57,
   112,   113,   114,   115,   273,   275,   169,   112,   113,   114,
   115,   266,    58,   272,   265,   213,   211,   314,   240,   237,
   214,   160,   160,   212,   214,   212,   322,    61,    90,    90,
    90,    90,    90,    90,    90,    90,    90,    90,     7,   217,
   218,   219,   220,   221,   222,   223,   224,   225,   226,   235,
   231,   229,   160,   160,   160,   227,   216,   175,   160,   160,
   160,   173,   171,   251,   160,   160,   310,   309,   260,   259,
   255,   250,   194,   193,   192,   191,   185,   172,   165,   128,
   127,   121,   120,   119,    70,   325,   307,   306,   253,   252,
   190,    52,     3,    90,    90,    90,    90,    90,    90,    90,
    90,    90,    90,    90,   279,   280,   281,   282,   283,   284,
   285,   286,   287,   288,   289,   134,   139,   159,   133,   123,
    97,   122,    96,    94,    93,    92,    80,   131,    79,    78,
    73,    22,     8,     4,     2,     1,   177,   209,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
    90,     0,    90,    90,     0,     0,     0,    90,     0,     0,
   315,   316,     0,   317,   318,    90,    87,     0,   319,    90,
     0,     0,     0,     0,     0,     0,   323,     0,     0,    55,
   327,     0,    63,    64,    65,    66,    67,     0,    69,     0,
     0,     0,     0,     0,    76,    77,     0,     0,     0,    81,
    82,   125,   126,    85,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   110,    95,
     0,     0,     0,     0,     0,     0,   116,   117,     0,     0,
     0,     0,     0,     0,     0,     0,   162,   118,     0,     0,
     0,     0,     0,     0,     0,   124,   124,   124,     0,   170,
     0,   174,   176,     0,     0,     0,   117,   135,   138,     0,
     0,     0,     0,     0,     0,   179,     0,     0,     0,     0,
     0,     0,   184,     0,     0,     0,     0,     0,   154,   155,
   156,   157,   158,     0,     0,   195,   161,   197,   205,     0,
   201,   204,     0,     0,   167,   168,     0,   116,     0,   200,
   203,   206,   215,     0,   181,     0,     0,   117,     0,     0,
   132,     0,     0,     0,     0,     0,     0,   228,   230,   232,
     0,   236,   238,   239,   241,   243,   244,     0,     0,     0,
   233,     0,   124,   196,     0,     0,     0,     0,   181,   247,
   181,   248,   181,     0,   249,     0,   262,     0,     0,     0,
     0,     0,     0,     0,   268,   261,     0,   180,     0,     0,
     0,     0,     0,   267,     0,   269,     0,   278,   129,   181,
   271,     0,     0,     0,     0,     0,     0,     0,   181,   290,
   291,   292,   293,     0,     0,   296,   297,   298,   299,   300,
     0,   199,     0,   207,   295,   208,   181,     0,   181,     0,
     0,   301,     0,     0,     0,   303,     0,   181,    98,   100,
     0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     0,   246,   181,     0,     0,     0,     0,     0,     0,     0,
     0,   181,     0,     0,     0,     0,     0,     0,     0,   263,
     0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
   270,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   294,     0,     0,     0,     0,
     0,     0,     0,     0,   302,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   178 };
yytabelem yypact[]={

 -1000,    26,  -180,   -70,  -257, -1000, -1000, -1000, -1000,    -8,
  -283,  -284,  -225,  -230, -1000,  -225,  -225,  -225,  -225,  -225,
  -235,  -225,   -23,  -236,  -237,  -256,  -285,  -225,  -225, -1000,
 -1000, -1000,  -225,  -225,  -260,  -262,  -225, -1000,  -208,  -279,
  -279, -1000,  -279,  -279,  -279,  -279,  -279,  -279,  -279,  -279,
  -279,  -225, -1000, -1000, -1000,  -185, -1000, -1000, -1000,  -225,
  -225, -1000, -1000,  -112,  -112,  -112,  -112,  -112, -1000,  -112,
  -225,   -24,   -25,   -26, -1000, -1000,  -112,  -112,  -225,  -225,
  -225,  -112,  -112,   -27,   -28,  -112, -1000,  -276, -1000,  -225,
  -143,  -234, -1000, -1000, -1000,   -82, -1000, -1000, -1000, -1000,
 -1000,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,
  -112,  -225,  -225,  -225,  -225,  -225,  -119, -1000,  -112,  -234,
  -287,  -264,   -29, -1000,  -179,   -29,   -29,  -225,  -225, -1000,
  -126,   -45,   -30,   -46,   -50,  -272,  -279,  -199,  -112, -1000,
  -227, -1000, -1000,  -290,  -273, -1000,   -31,  -133,  -188,    -9,
   -32,   -33,   -34,   -35,  -112,  -219,  -219, -1000, -1000, -1000,
 -1000,  -112, -1000, -1000, -1000,  -225,  -225,  -112,  -112,  -120,
  -223,  -145,  -249,  -199, -1000,  -225, -1000, -1000,  -216,   -91,
   -92,  -276, -1000, -1000,   -51,  -208,  -208,  -208,  -208,  -208,
  -208,  -208,  -208,  -208,  -208, -1000,  -112, -1000, -1000,   -52,
   -56,   -57,  -199,   -58,   -88,  -120,   -89,  -122,  -120, -1000,
  -281,  -199, -1000,  -273, -1000, -1000,  -273, -1000,   -36,   -10,
   -11,  -194,   -37, -1000,  -131,   -38,   -39,  -252, -1000,  -225,
 -1000,  -225, -1000,   -93,   -96,  -252, -1000,  -273, -1000, -1000,
  -225, -1000,  -273, -1000, -1000, -1000,  -103,  -111,  -138,  -120,
  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,
  -208,  -120,  -120,  -120,  -120,  -225,  -273,  -120,  -120,  -120,
  -120,  -120, -1000,  -273,  -225, -1000, -1000,  -273, -1000, -1000,
 -1000,  -200,  -203,   -12,   -13, -1000, -1000,  -209,   -40,   -41,
 -1000, -1000, -1000, -1000,  -149,  -151, -1000, -1000, -1000, -1000,
 -1000,  -162,  -163,  -189, -1000,   -90,  -208,  -208, -1000,  -208,
  -208, -1000, -1000, -1000,  -208,  -210,  -110, -1000, -1000, -1000,
 -1000,   -83,  -208,  -137,   -14,  -117,  -208,  -139, -1000 };
yytabelem yypgo[]={

     0,   317,   558,   327,   326,    32,   307,    43,   356,   409,
   306,   325,   324,   323,   322,   321,   320,   319,   311,   318,
   316,   309,   315,   314,   313,   312,   310,   308,   305 };
yytabelem yyr1[]={

     0,    11,    11,    11,    11,    12,    13,    13,    14,    14,
    14,    14,    14,    14,    14,    14,    14,    14,    14,    14,
    14,    14,    14,    14,    14,    14,    14,    14,    14,    14,
    14,    17,    14,    19,    14,    20,    14,    14,    14,    14,
    14,    14,    14,    14,    14,    14,    14,    14,    14,    14,
    14,    14,    14,    14,    14,    14,    14,    14,    14,    14,
    14,    14,    14,    14,    14,    14,    14,    15,    15,    16,
    16,     2,     2,    18,    18,    21,    21,     7,     7,     7,
     7,     7,     7,     7,    22,    22,    22,    22,    23,    23,
    23,    23,    23,    23,    23,    24,    24,    25,    25,    25,
    25,    25,    25,    25,    25,    25,    25,    25,    26,    26,
    26,    27,    27,    27,    27,    28,    28,    28,    28,     9,
     1,     4,     3,     3,     5,     6,     8,     8,     8,     8,
     8,     8,     8,     8,     8,    10,    10,    10,    10 };
yytabelem yyr2[]={

     0,     0,     7,     7,     7,     4,     0,     7,     5,     5,
     5,     9,     5,     3,     5,     5,     5,     5,     5,     5,
     5,     2,     9,     9,     9,     4,     4,     3,     5,     3,
     5,     1,     7,     1,     7,     1,     7,     5,     5,     9,
     9,     5,     3,     5,     5,     5,     3,     7,    11,    15,
    15,    17,    21,    21,    35,     7,    11,    11,    15,    15,
    17,    11,    15,    19,    15,    19,     0,     5,     7,     3,
     7,     3,     1,     2,     6,     3,     7,     3,     3,     9,
     9,     5,     5,     3,     3,     3,     9,    13,    15,    15,
    15,    11,    11,    11,     7,     3,     3,    15,    15,    15,
    15,    15,    15,    11,    11,    11,    11,     7,    11,    11,
     7,    11,    11,     7,     7,    15,    15,    15,    11,     5,
     5,     5,     5,     1,     3,     3,     3,     3,     3,     7,
     7,     7,     7,     7,     5,     3,     3,     5,     5 };
yytabelem yychk[]={

 -1000,   -11,   -12,   256,   -13,   298,   301,   298,   -14,   314,
   257,   284,   258,   259,   260,   261,   262,   268,   263,   264,
   265,   266,   -15,   269,   276,   270,   286,   271,   272,   273,
   274,   275,   277,   278,   279,   280,   283,   281,   282,   288,
   287,   316,   317,   318,   319,   320,   321,   323,   324,   325,
   326,   267,   289,   329,   329,    -8,   314,   315,   328,    -5,
   310,   299,   314,    -8,    -8,    -8,    -8,    -8,   314,    -8,
   297,   314,   314,   -16,   314,   329,    -8,    -8,   -17,   -19,
   -20,    -8,    -8,   314,   314,    -8,    -7,    -8,   322,   310,
    -5,   290,   -22,   -23,   -24,    -9,   -25,   -26,    -2,   327,
    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
    -8,   297,   309,   310,   311,   312,    -8,    -8,    -8,   297,
   297,   297,   -18,   -21,    -8,   -18,   -18,   297,   297,    -2,
    -5,    -1,    -9,   -27,   -28,    -8,   322,   305,    -8,   -10,
   310,   330,   331,   309,    -5,    -7,    -7,    -7,    -7,    -7,
    -7,    -7,    -7,    -7,    -8,    -8,    -8,    -8,    -8,    -6,
   300,    -8,   -10,   329,   314,   297,   289,    -8,    -8,   322,
    -6,   297,   297,   297,    -6,   297,    -6,    -4,    -2,    -1,
    -9,    -8,   331,   331,    -1,   297,   297,   307,   307,   297,
   289,   297,   297,   297,   297,   -21,    -8,    -6,   309,    -9,
    -1,   -27,   305,    -1,   -27,   -28,    -1,    -9,    -9,    -3,
   311,   297,   306,   297,   306,    -6,   297,    -7,    -7,    -7,
    -7,    -7,    -7,    -7,    -7,    -7,    -7,   297,    -6,   297,
    -6,   297,    -6,    -1,    -9,   297,    -6,   297,    -6,    -6,
   297,    -6,   297,    -6,    -6,   328,    -9,    -1,    -1,    -1,
   297,   289,   289,   289,   307,   297,   289,   297,   307,   297,
   297,    -1,   -27,    -9,    -9,   297,   297,    -1,   -27,    -1,
    -9,    -1,   306,   297,   297,   306,   306,   297,    -6,    -7,
    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
    -6,    -6,    -6,    -6,    -9,    -1,    -6,    -6,    -6,    -6,
    -6,    -1,    -9,    -1,   308,   308,   289,   289,   308,   297,
   297,   306,   306,   306,   297,    -7,    -7,    -7,    -7,    -7,
   308,   297,   299,    -7,   300,   289,   299,    -7,   300 };
yytabelem yydef[]={

     1,    -2,     0,     0,    66,     2,     3,     4,     5,     0,
     0,     0,     0,     0,    13,     0,     0,     0,     0,     0,
     0,     0,    21,     0,     0,     0,     0,    27,    29,    31,
    33,    35,     0,     0,     0,     0,     0,    42,     0,    72,
    72,    46,    72,    72,    72,    72,    72,    72,    72,    72,
    72,     0,     7,     8,     9,    10,   126,   127,   128,     0,
     0,   124,    12,    14,    15,    16,    17,    18,    19,    20,
     0,     0,     0,    25,    69,    26,    28,    30,     0,     0,
     0,    37,    38,     0,     0,    41,    43,    -2,    78,     0,
     0,     0,    83,    84,    85,     0,    95,    96,    44,    71,
    45,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    67,     0,     0,     0,     0,     0,     0,   134,    68,     0,
     0,     0,    32,    73,    75,    34,    36,     0,     0,   119,
     0,     0,     0,     0,     0,    72,    72,     0,    81,    82,
     0,   135,   136,     0,     0,    47,     0,     0,     0,     0,
    55,     0,     0,     0,    11,   130,   131,   132,   133,   129,
   125,    22,    23,    24,    70,     0,     0,    39,    40,     0,
    94,     0,     0,     0,   107,     0,   110,   120,   123,     0,
     0,    72,   138,   137,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    74,    76,    79,    80,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     0,     0,   114,     0,   113,    86,     0,    48,     0,     0,
     0,     0,     0,    56,    57,    61,     0,     0,    92,     0,
    93,     0,   105,     0,     0,     0,    91,     0,   104,   109,
     0,   106,     0,   103,   108,   122,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   111,     0,     0,   118,   112,     0,    87,    49,
    50,     0,     0,     0,     0,    58,    59,     0,    62,    64,
    89,    99,    90,    98,     0,     0,    88,   100,   101,    97,
   102,     0,     0,     0,    51,     0,     0,     0,    60,     0,
     0,   116,   117,   115,     0,     0,     0,    63,    65,    52,
    53,     0,     0,     0,     0,     0,     0,     0,    54 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"IFILE",	257,
	"ILN",	258,
	"IDEF",	259,
	"IENDEF",	260,
	"IVAL",	261,
	"ITYPE",	262,
	"ILINE",	263,
	"ISCL",	264,
	"ITAG",	265,
	"ISIZE",	266,
	"IDIM",	267,
	"ITYPEQ",	268,
	"ISET",	269,
	"IGLOBAL",	270,
	"IDATA",	271,
	"ITEXT",	272,
	"IBYTE",	273,
	"ISHORT",	274,
	"ILONG",	275,
	"ISECTION",	276,
	"ISPACE",	277,
	"IORG",	278,
	"ICOMM",	279,
	"ILCOMM",	280,
	"IEVEN",	281,
	"ISWBEG",	282,
	"IALIGN",	283,
	"IIDENT",	284,
	"IINIT",	285,
	"IVERSION",	286,
	"IROUND",	287,
	"IPREC",	288,
	"COLON",	289,
	"AMP",	290,
	"NOCHAR",	291,
	"SPC",	292,
	"ALPH",	293,
	"DIGIT",	294,
	"SQ",	295,
	"SH",	296,
	"CM",	297,
	"NL",	298,
	"LP",	299,
	"RP",	300,
	"SEMI",	301,
	"DQ",	302,
	"TWID",	303,
	"EXCLAM",	304,
	"LBRKT",	305,
	"RBRKT",	306,
	"LBRACE",	307,
	"RBRACE",	308,
	"PLUS",	309,
	"MINUS",	310,
	"MUL",	311,
	"DIV",	312,
	"UMINUS",	313,
	"NAME",	314,
	"NAMEW",	315,
	"INST0",	316,
	"INST1",	317,
	"INST2",	318,
	"INST3",	319,
	"INST4",	320,
	"INST6",	321,
	"REG",	322,
	"INST12",	323,
	"INST23",	324,
	"INST24",	325,
	"INST34",	326,
	"SIZE",	327,
	"INT",	328,
	"STRING",	329,
	"HEXSTR",	330,
	"DECSTR",	331,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"file : /* empty */",
	"file : file linstruction NL",
	"file : file linstruction SEMI",
	"file : file error NL",
	"linstruction : labels instruction",
	"labels : /* empty */",
	"labels : labels NAME COLON",
	"instruction : IFILE STRING",
	"instruction : IIDENT STRING",
	"instruction : ILN expr",
	"instruction : ILN expr CM expr",
	"instruction : IDEF NAME",
	"instruction : IENDEF",
	"instruction : IVAL expr",
	"instruction : ITYPE expr",
	"instruction : ITYPEQ expr",
	"instruction : ILINE expr",
	"instruction : ISCL expr",
	"instruction : ITAG NAME",
	"instruction : ISIZE expr",
	"instruction : dotdim",
	"instruction : ISET NAME CM expr",
	"instruction : ISET NAME CM fp_expr",
	"instruction : ISECTION NAME CM STRING",
	"instruction : IGLOBAL names",
	"instruction : IVERSION STRING",
	"instruction : IDATA",
	"instruction : IDATA expr",
	"instruction : ITEXT",
	"instruction : ITEXT expr",
	"instruction : IBYTE",
	"instruction : IBYTE explist",
	"instruction : ISHORT",
	"instruction : ISHORT explist",
	"instruction : ILONG",
	"instruction : ILONG explist",
	"instruction : ISPACE expr",
	"instruction : IORG expr",
	"instruction : ICOMM NAME CM expr",
	"instruction : ILCOMM NAME CM expr",
	"instruction : IALIGN expr",
	"instruction : IEVEN",
	"instruction : ISWBEG arg",
	"instruction : IPREC optsize",
	"instruction : IROUND optsize",
	"instruction : INST0",
	"instruction : INST1 optsize arg",
	"instruction : INST2 optsize arg CM arg",
	"instruction : INST3 optsize arg CM arg CM arg",
	"instruction : INST3 optsize arg CM arg COLON arg",
	"instruction : INST3 optsize arg LBRACE arg COLON arg RBRACE",
	"instruction : INST4 optsize arg LBRACE arg COLON arg RBRACE CM arg",
	"instruction : INST4 optsize arg CM arg LBRACE arg COLON arg RBRACE",
	"instruction : INST6 optsize arg COLON arg CM arg COLON arg CM LP arg RP COLON LP arg RP",
	"instruction : INST12 optsize arg",
	"instruction : INST12 optsize arg CM arg",
	"instruction : INST23 optsize arg CM arg",
	"instruction : INST23 optsize arg CM arg COLON arg",
	"instruction : INST23 optsize arg CM arg CM arg",
	"instruction : INST23 optsize arg CM arg LBRACE arg RBRACE",
	"instruction : INST24 optsize arg CM arg",
	"instruction : INST24 optsize arg CM arg CM arg",
	"instruction : INST24 optsize arg CM arg CM arg CM arg",
	"instruction : INST34 optsize arg CM arg CM arg",
	"instruction : INST34 optsize arg CM arg CM arg CM arg",
	"instruction : /* empty */",
	"dotdim : IDIM expr",
	"dotdim : dotdim CM expr",
	"names : NAME",
	"names : names CM NAME",
	"optsize : SIZE",
	"optsize : /* empty */",
	"explist : outexpr",
	"explist : explist CM outexpr",
	"outexpr : expr",
	"outexpr : expr COLON expr",
	"arg : expr",
	"arg : REG",
	"arg : MINUS lp REG rp",
	"arg : lp reg.ss rp PLUS",
	"arg : AMP expr",
	"arg : AMP fp_expr",
	"arg : ind",
	"ind : nmi",
	"ind : memind",
	"ind : disp lp reg.ss rp",
	"ind : disp lp reg.ss CM reg.ss rp",
	"nmi : lp disp CM reg.ss CM reg.ss rp",
	"nmi : lp reg.ss CM disp CM reg.ss rp",
	"nmi : lp reg.ss CM reg.ss CM disp rp",
	"nmi : lp disp CM reg.ss rp",
	"nmi : lp reg.ss CM disp rp",
	"nmi : lp reg.ss CM reg.ss rp",
	"nmi : lp reg.ss rp",
	"memind : postind",
	"memind : preind",
	"postind : lp mbd CM reg.ss CM disp rp",
	"postind : lp reg.ss CM mbd CM disp rp",
	"postind : lp reg.ss CM disp CM mbd rp",
	"postind : lp disp CM reg.ss CM mbd rp",
	"postind : lp disp CM mbd CM reg.ss rp",
	"postind : lp mbd CM disp CM reg.ss rp",
	"postind : lp mbd CM disp rp",
	"postind : lp disp CM mbd rp",
	"postind : lp reg.ss CM mbd rp",
	"postind : lp mbd CM reg.ss rp",
	"postind : lp mbd rp",
	"preind : lp mbid CM disp rp",
	"preind : lp disp CM mbid rp",
	"preind : lp mbid rp",
	"mbd : LBRKT reg.ss CM disp RBRKT",
	"mbd : LBRKT disp CM reg.ss RBRKT",
	"mbd : LBRKT disp RBRKT",
	"mbd : LBRKT reg.ss RBRKT",
	"mbid : LBRKT disp CM reg.ss CM reg.ss RBRKT",
	"mbid : LBRKT reg.ss CM disp CM reg.ss RBRKT",
	"mbid : LBRKT reg.ss CM reg.ss CM disp RBRKT",
	"mbid : LBRKT reg.ss CM reg.ss RBRKT",
	"disp : expr optsize",
	"reg.ss : REG ss",
	"ss : optsize optscl",
	"optscl : MUL INT",
	"optscl : /* empty */",
	"lp : LP",
	"rp : RP",
	"expr : NAME",
	"expr : NAMEW",
	"expr : INT",
	"expr : lp expr rp",
	"expr : expr PLUS expr",
	"expr : expr MINUS expr",
	"expr : expr MUL expr",
	"expr : expr DIV expr",
	"expr : MINUS expr",
	"fp_expr : HEXSTR",
	"fp_expr : DECSTR",
	"fp_expr : PLUS DECSTR",
	"fp_expr : MINUS DECSTR",
};
#endif /* YYDEBUG */
#ident	"@(#)yaccpar	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	


/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** global variables used by the parser
*/
YYSTYPE yyv[ YYMAXDEPTH ];	/* value stack */
int yys[ YYMAXDEPTH ];		/* state stack */

YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ YYMAXDEPTH ] )	/* room on stack? */
		{
			yyerror( "yacc stack overflow" );
			YYABORT;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 2:{
		line++;
		dot->value = newdot;
		generate(0,NEWSTMT,(long) line,NULLSYM);
		ap = arglist;
		xp = explist;
	} break;
case 3:{
		dot->value = newdot;
		generate(0,NEWSTMT,(long) line,NULLSYM);
		ap = arglist;
		xp = explist;
	} break;
case 4:{
		line++;
		yyerrok;
		dot->value = newdot;
		ap = arglist;
		xp = explist;
	} break;
case 7:{
		if ((yypvt[-1].yname->styp & TYPE) != UNDEF)
		{
			strcpy(err_buf,"multiply defined label--");
			strcat(err_buf,yytext);
			yyerror(err_buf);
		}
		yypvt[-1].yname->value = newdot;
		yypvt[-1].yname->styp |= dot->styp;
		yypvt[-1].yname->sectnum = dot->sectnum;
		if (opt && ((dot->styp & TYPE) == TXT) && dot->sectnum == 1) /* bk */
			deflab(yypvt[-1].yname);
	} break;
case 8:{
		if (cfile[0] != '\0')
			yyerror("only one .file allowed");
		strcpy(cfile,yypvt[-0].ystr);
		generate(0,SETFILE,0,NULLSYM);
	} break;
case 9:{
		comment(yypvt[-0].ystr);
	} break;
case 10:{
		cline = (short) yypvt[-0].yexp->xvalue;
		generate(0,LINENBR,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 11:{
		cline = (short) yypvt[-2].yexp->xvalue;
		generate(0,LINENUM,yypvt[-2].yexp->xvalue,yypvt[-2].yexp->xsymptr);
		generate(0,LINEVAL,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 12:{
/**		if ( $2->value == 0 ) {			/bk: prevent redefinition /
			generate(0,DEFINE,0L,$2);
			$2->value = 1;
		} **/
		generate(0,DEFINE,0L,yypvt[-0].yname);
	} break;
case 13:{
		generate(0,ENDEF,0,NULLSYM);
	} break;
case 14:{
		generate(0,SETVAL,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 15:{
		generate(0,SETTYP,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 16:{
		/** do NOT generate for now. when the linker and debuggers
		 ** support "const" and "volatile" we will need to "generate".
		 **/ 
		/** generate(0,SETTYPQ,$2->xvalue,$2->xsymptr);**/
		;
	} break;
case 17:{
		generate(0,SETLNO,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 18:{
		generate(0,SETSCL,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 19:{
		generate(0,SETTAG,0,yypvt[-0].yname);
	} break;
case 20:{
		generate(0,SETSIZ,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 22:{
		if ((yypvt[-0].yexp->xtype & TYPE) == UNDEF)
			yyerror("illegal expression in .set");
		if (yypvt[-2].yname == NULL)
			yyerror("unable to define symbol in .set");
		else
		{
			yypvt[-2].yname->value = yypvt[-0].yexp->xvalue;
			yypvt[-2].yname->styp = yypvt[-0].yexp->xtype;
		}
	} break;
case 23:{
		if ((yypvt[-0].yexp->xtype & TYPE) == UNDEF)
			yyerror("illegal expression in .set");
		if (yypvt[-2].yname == NULL)
			yyerror("unable to define symbol in .set");
		else
		{	if ( yypvt[-0].yexp->xvaltype == FP_DEC )
				get_value(yypvt[-2].yname);
			else
			{
				yypvt[-2].yname->value = yypvt[-0].yexp->xvalue;
				yypvt[-2].yname->value2 = yypvt[-0].yexp->xvalue2;
				yypvt[-2].yname->value3 = yypvt[-0].yexp->xvalue3;
			}
			yypvt[-2].yname->styp = yypvt[-0].yexp->xtype;
		}
	} break;
case 24:{ 
		register int att=0;
		char *ptr;
		while (*yypvt[-0].ystr)	switch (*yypvt[-0].ystr++){
			case 'b': /* zero initialized block */ 
				att |= STYP_BSS; break;
			case 'c': /* copy */ 
				att |= STYP_COPY; break;
			case 'i': /* info */ 
				att |= STYP_INFO; break;
			case 'd': /* dummy */ 
				att |= STYP_DSECT; break;
			case 'x': /* executable */ 
				att |= STYP_TEXT; break;
			case 'n': /* noload */ 
				att |= STYP_NOLOAD; break;
			case 'o': /* overlay */ 
				att |= STYP_OVER; break;
			case 'l': /* lib */ 
				att |= STYP_LIB; break;
			case 'w': /* writable */ 
				att |= STYP_DATA; break;
			default:
				yyerror("Invalid section attribute");
				break;
			}
		/* bk: temp fix, until we fully incorporate 4.1 improvements */
		if (yypvt[-2].yname->_name.name[7] != 0 ) yyerror( "section name too long" );
		ptr=(char *)malloc(strlen((char *)(yypvt[-2].yname->_name.name) )+2);
		*ptr='.';
		strcpy(ptr+1,(char *)(yypvt[-2].yname->_name.name) );
		cgsect(mksect(lookup(ptr,INSTALL,USRNAME)->stp,att),0);
		free(ptr);
	} break;
case 27:{
			cgsect((datsec),0);
	} break;
case 28:{
			cgsect(( ( yypvt[-0].yexp->xvalue==2 ? dat2sec : datsec)), yypvt[-0].yexp->xvalue);
	} break;
case 29:{
		cgsect((txtsec),0);
	} break;
case 30:{
		switch ( yypvt[-0].yexp->xvalue ) {		/* bk */
			case 2:
				cgsect( txt2sec, yypvt[-0].yexp->xvalue );
				break;
			case 3:
				cgsect( txt3sec, yypvt[-0].yexp->xvalue );
				break;
			default:
				cgsect( txtsec, yypvt[-0].yexp->xvalue );
				break;
		}
	} break;
case 31:{bytesout = 1;} break;
case 32:{
		if (bitsout)
		{
			generate(BITSPBY*bytesout,0,outword,NULLSYM);
			outword = 0;
			bitsout = 0;
		}
	} break;
case 33:{bytesout = 2;} break;
case 34:{
		if (bitsout)
		{
			generate(BITSPBY*bytesout,0,outword,NULLSYM);
			outword = 0;
			bitsout = 0;
		}
	} break;
case 35:{bytesout = 4;} break;
case 36:{
		if (bitsout)
		{
			generate(BITSPBY*bytesout,0,outword,NULLSYM);
			outword = 0;
			bitsout = 0;
		}
	} break;
case 37:{
		if (yypvt[-0].yexp->xtype != ABS)
			yyerror("space size not absolute");
		fill(yypvt[-0].yexp->xvalue);
	} break;
case 38:{
		if (yypvt[-0].yexp->xtype != ABS)
			yyerror("org requires absolute operand");
		if ((yypvt[-0].yexp->xvalue - dot->value) < 0)
			yyerror("backwards org");
		else
		{
			if (opt && ((dot->styp & TYPE) == TXT) 
			    && dot->sectnum == 0)
			   resolve();	/* force PC to known value */
			fill(yypvt[-0].yexp->xvalue - dot->value);
		}
	} break;
case 39:{
		if (yypvt[-0].yexp->xtype != ABS)
			yyerror("comm size not absolute");
		if ((yypvt[-2].yname->styp & TYPE) != UNDEF)
			yyerror("illegal attempt to redefine symbol");
		else
		{
			if ( yypvt[-2].yname->styp == (EXTERN|UNDEF) ) {	/* bk: merge multiple comm's */
				if ( yypvt[-0].yexp->xvalue > yypvt[-2].yname->value )	/* take larger of two symbols */
					yypvt[-2].yname->value = yypvt[-0].yexp->xvalue;
			} else {
				yypvt[-2].yname->styp = (EXTERN | UNDEF);
				yypvt[-2].yname->value = yypvt[-0].yexp->xvalue;
			}
		}
	} break;
case 40:{
		if (yypvt[-0].yexp->xtype != ABS)
			yyerror("lcomm size not absolute");
		if ((yypvt[-2].yname->styp & TYPE) != UNDEF)
			yyerror("illegal attempt to redefine symbol");
		else
		{
		/* even align all items > 1 byte;
		** long align all items > 3 bytes.
		*/
		extern int previous;
		register int prevsec=previous;
		symbol *bfk = yypvt[-2].yname;
		cgsect(bsssec,0);
#define dotbss dot->value
			if (yypvt[-0].yexp->xvalue > 3 && (dotbss & 03) != 0)
			    dotbss += (4 - (dotbss & 03));
			else if (yypvt[-0].yexp->xvalue > 1 && (dotbss & 01) != 0)
			    dotbss++;
			yypvt[-2].yname->value = dotbss;
			dotbss += yypvt[-0].yexp->xvalue;
			yypvt[-2].yname->styp |= BSS;
			yypvt[-2].yname->sectnum = dot->sectnum;/* so it picks up size */
			/* align newdot */
			newdot=dotbss;
		cgsect(previous,0); /* return dot to previous section */
		previous=prevsec; /* reset previous - as if this never happnd */
		}
	} break;
case 41: { if ((yypvt[-0].yexp->xtype!=ABS) ||
				     (yypvt[-0].yexp->xvalue < 1) ||
				     (yypvt[-0].yexp->xvalue > 1024))
					yyerror("illegal align expression");
				  else
				     ckalign(yypvt[-0].yexp->xvalue);
			       } break;
case 42:{
		ckalign(2);
	} break;
case 43:{
		generate(BITSPOW,SWBEG,0x4afc,NULLSYM);
		generate(BITSPOW,SWBEG,yypvt[-0].yarg->xp->xvalue,NULLSYM);
		swbegct = yypvt[-0].yarg->xp->xvalue;
	} break;
case 44:{
		get_fp_prec(yypvt[-0].yint);
	} break;
case 45:{
		get_fp_round(yypvt[-0].yint);
	} break;
case 46:{
		/* No operands, and no size attribute.			*/
		gen0op(yypvt[-0].yinst);
	} break;
case 47:{
		gen1op(yypvt[-2].yinst,yypvt[-1].yint,yypvt[-0].yarg);
	} break;
case 48:{
		gen2op(yypvt[-4].yinst,yypvt[-3].yint,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 49:{
		gen3op(yypvt[-6].yinst,yypvt[-5].yint,yypvt[-4].yarg,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 50:{	/* 68881 instruction syntax */
		gen3op(yypvt[-6].yinst,yypvt[-5].yint,yypvt[-4].yarg,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 51:{
		gen3op(yypvt[-7].yinst,yypvt[-6].yint,yypvt[-5].yarg,yypvt[-3].yarg,yypvt[-1].yarg);
	} break;
case 52:{
		gen4op(yypvt[-9].yinst,yypvt[-8].yint,yypvt[-7].yarg,yypvt[-5].yarg,yypvt[-3].yarg,yypvt[-0].yarg);
	} break;
case 53:{
		gen4op(yypvt[-9].yinst,yypvt[-8].yint,yypvt[-7].yarg,yypvt[-5].yarg,yypvt[-3].yarg,yypvt[-1].yarg);
	} break;
case 54:{
		gen6op(yypvt[-16].yinst,yypvt[-15].yint,yypvt[-14].yarg,yypvt[-12].yarg,yypvt[-10].yarg,yypvt[-8].yarg,yypvt[-5].yarg,yypvt[-1].yarg);
	} break;
case 55:{
		gen1op(yypvt[-2].yinst,yypvt[-1].yint,yypvt[-0].yarg);
	} break;
case 56:{
		gen2op(yypvt[-4].yinst,yypvt[-3].yint,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 57:{
		gen2op(yypvt[-4].yinst,yypvt[-3].yint,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 58:{
		gen3op(yypvt[-6].yinst,yypvt[-5].yint,yypvt[-4].yarg,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 59:{
		gen3op(yypvt[-6].yinst,yypvt[-5].yint,yypvt[-4].yarg,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 60:{	/* 68881 instruction syntax */
		gen3op(yypvt[-7].yinst,yypvt[-6].yint,yypvt[-5].yarg,yypvt[-3].yarg,yypvt[-1].yarg);
	} break;
case 61:{
		gen2op(yypvt[-4].yinst,yypvt[-3].yint,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 62:{
		gen3op(yypvt[-6].yinst,yypvt[-5].yint,yypvt[-4].yarg,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 63:{
		gen4op(yypvt[-8].yinst,yypvt[-7].yint,yypvt[-6].yarg,yypvt[-4].yarg,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 64:{
		gen3op(yypvt[-6].yinst,yypvt[-5].yint,yypvt[-4].yarg,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 65:{
		gen4op(yypvt[-8].yinst,yypvt[-7].yint,yypvt[-6].yarg,yypvt[-4].yarg,yypvt[-2].yarg,yypvt[-0].yarg);
	} break;
case 67:{
		generate(0,SETDIM1,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 68:{
		generate(0,SETDIM2,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 69:{
		yypvt[-0].yname->styp |= EXTERN;
	} break;
case 70:{
		yypvt[-0].yname->styp |= EXTERN;
	} break;
case 71:{
		yyval.yint = yypvt[-0].yint;
	} break;
case 72:{
		yyval.yint = UNSPEC;
	} break;
case 75:{
		if (bitsout)
		{
			generate(BITSPBY*bytesout,0,outword,NULLSYM);
			outword = 0;
			bitsout = 0;
		}
		if (swbegct-- > 0)
			generate(BITSPBY*bytesout,SWBEG,0,yypvt[-0].yexp->xsymptr);
		else
			generate(BITSPBY*bytesout,GENRELOC,yypvt[-0].yexp->xvalue,yypvt[-0].yexp->xsymptr);
	} break;
case 76:{
		if (yypvt[-2].yexp->xtype != ABS)
			yyerror("width expression not absolute");
		if (yypvt[-2].yexp->xvalue > (BITSPBY * bytesout))
			yyerror("expression crosses field boundary");
		if ((yypvt[-2].yexp->xvalue + bitsout) > (BITSPBY * bytesout))
		{
			generate(BITSPBY*bytesout,0,outword,NULLSYM);
			bitsout = 0;
			outword = 0;
		}
		yypvt[-0].yexp->xvalue &= (power(2,yypvt[-2].yexp->xvalue)-1);	/* mask out n bits */
		yypvt[-0].yexp->xvalue <<= (bytesout*BITSPBY - bitsout - yypvt[-2].yexp->xvalue);
		outword |= yypvt[-0].yexp->xvalue;
		bitsout += yypvt[-2].yexp->xvalue;
	} break;
case 77:{
		if(yypvt[-0].yexp->xtype != ABSW){
			ap->atype = ABSL;
		}else{
			ap->atype = ABSW;
		}
		ap->xp = yypvt[-0].yexp;
		ap->areg1 = 0;
		ap->areg2 = 0;
		yyval.yarg = ap++;
	} break;
case 78:{
		
		ap->atype = yypvt[-0].yinst->tag;
		ap->areg1 = yypvt[-0].yinst;
		ap->areg2 = 0;
		yyval.yarg = ap++;
	} break;
case 79:{
		ap->atype = ADEC;
		if (yypvt[-1].yinst->tag != AAREG)
			yyerror("predecrement requires address register");
		ap->areg1 = yypvt[-1].yinst;
		ap->areg2 = 0;
		yyval.yarg = ap++;
	} break;
case 80:{
		/*
		 *	simple disambiguating rule:
		 *
		 *	here we only accept REG, but this conflicted
		 *	with new "ind" rules.  it is sufficient to check
		 *	that "ssflag" is not set, if it is - we've seen
		 *	a "size-scale" modifier which is illegal!
		 */

		if ( ssflag )
			yyerror("no size/scale modifier on post increment");
		ap->atype = AINC;
		if (yypvt[-2].yinst->tag != AAREG)
			yyerror("postincrement requires address register");
		ap->areg1 = yypvt[-2].yinst;
		ap->areg2 = 0;
		yyval.yarg = ap++;
	} break;
case 81:{
		ap->atype = AIMM;
		ap->xp = yypvt[-0].yexp;
		ap->areg1 = 0;
		ap->areg2 = 0;
		yyval.yarg = ap++;
	} break;
case 82:{
		ap->atype = AIMM;
		ap->xp = yypvt[-0].yexp;
		ap->areg1 = 0;
		ap->areg2 = 0;
		yyval.yarg = ap++;
	} break;
case 83:{
		yyval.yarg = ap++;
	} break;
case 84:{
		;
	} break;
case 85:{
		;
	} break;
case 86:{
		mk_indirect(INDIRECT,yypvt[-1].yinst,yypvt[-3].yexp);
	} break;
case 87:{	
		mkindex_indirect(INDIRECT,yypvt[-3].yinst,yypvt[-1].yinst,yypvt[-5].yexp);
	} break;
case 88:{	
		mkindex_indirect(INDIRECT,yypvt[-3].yinst,yypvt[-1].yinst,yypvt[-5].yexp);
	} break;
case 89:{
		mkindex_indirect(INDIRECT,yypvt[-5].yinst,yypvt[-1].yinst,yypvt[-3].yexp);
	} break;
case 90:{
		mkindex_indirect(INDIRECT,yypvt[-5].yinst,yypvt[-3].yinst,yypvt[-1].yexp);
	} break;
case 91:{
		mk_indirect(INDIRECT,yypvt[-1].yinst,yypvt[-3].yexp);
	} break;
case 92:{
		mk_indirect(INDIRECT,yypvt[-3].yinst,yypvt[-1].yexp);
	} break;
case 93:{
		mkindex_indirect(INDIRECT,yypvt[-3].yinst,yypvt[-1].yinst,0);
	} break;
case 94:{
		if ( !ssflag && yypvt[-1].yinst->tag == AAREG)
		{	/* simple indirect (An) */
			ap->atype = AIREG;
			ap->areg1=yypvt[-1].yinst;
			ap->areg2=0;
		}
		else
		if ( yypvt[-1].yinst->tag == AAREG || yypvt[-1].yinst->tag == NULAREG 
			|| yypvt[-1].yinst->tag == ADREG || yypvt[-1].yinst->tag == NULDREG 
			|| yypvt[-1].yinst->tag == PCREG  || yypvt[-1].yinst->tag == NULPCREG )
			mkindex_indirect(INDIRECT,yypvt[-1].yinst,0,0);
		else
			yyerror("indirect required address register");
	} break;
case 95:{
		;
	} break;
case 96:{
		;
	} break;
case 97:{
		mk_post_ind(yypvt[-3].yinst,yypvt[-1].yexp);
	} break;
case 98:{
		mk_post_ind(yypvt[-5].yinst,yypvt[-1].yexp);
	} break;
case 99:{
		mk_post_ind(yypvt[-5].yinst,yypvt[-3].yexp);
	} break;
case 100:{
		mk_post_ind(yypvt[-3].yinst,yypvt[-5].yexp);
	} break;
case 101:{
		mk_post_ind(yypvt[-1].yinst,yypvt[-5].yexp);
	} break;
case 102:{
		mk_post_ind(yypvt[-1].yinst,yypvt[-3].yexp);
	} break;
case 103:{
		/* this syntax can be pre-index or post-index */
		if (ap->aflags == PRENDXMI)
			ap->xp2 = yypvt[-1].yexp;
		else
			mk_post_ind(0,yypvt[-1].yexp);
	} break;
case 104:{
		/* this syntax can be pre-index or post-index */
		if (ap->aflags == PRENDXMI)
			ap->xp2 = yypvt[-3].yexp;
		else
			mk_post_ind(0,yypvt[-3].yexp);
	} break;
case 105:{
		mk_post_ind(yypvt[-3].yinst,0);
	} break;
case 106:{
		mk_post_ind(yypvt[-1].yinst,0);
	} break;
case 107:{
		/* this syntax can be pre-index or post-index */
		if (ap->aflags == PRENDXMI)
			ap->xp2 = 0;
		else
			mk_post_ind(0,0);
	} break;
case 108:{
		ap->xp2 = yypvt[-1].yexp;
	} break;
case 109:{	
		ap->xp2 = yypvt[-3].yexp;
	} break;
case 110:{
		ap->xp2 = 0;
	} break;
case 111:{
		/* If register is a data or null data register, it may 
		 * be an index register with base suppressed, then assume preindex.
		 * If the size and scale flag is on, and the register is legal
		 * index register, then assume preindex with base suppressed.
		 */
		if ( yypvt[-3].yinst->tag == ADREG || yypvt[-3].yinst->tag == NULDREG )
			mk_full_ind(PRENDXMI,0,yypvt[-3].yinst,yypvt[-1].yexp);
		else
		if ( ssflag && (yypvt[-3].yinst->tag == AAREG || yypvt[-3].yinst->tag == NULAREG) )
			mk_full_ind(PRENDXMI,0,yypvt[-3].yinst,yypvt[-1].yexp);
		else
		{
			if (ssflag)
				werror("base register should have no size/scale");
			mk_full_ind(POSTNDXMI,yypvt[-3].yinst,0,yypvt[-1].yexp);
		}
	} break;
case 112:{
		if ( yypvt[-1].yinst->tag == ADREG || yypvt[-1].yinst->tag == NULDREG )
			mk_full_ind(PRENDXMI,0,yypvt[-1].yinst,yypvt[-3].yexp);
		else
		if ( ssflag && (yypvt[-1].yinst->tag == AAREG || yypvt[-1].yinst->tag == NULAREG) )
			mk_full_ind(PRENDXMI,0,yypvt[-1].yinst,yypvt[-3].yexp);
		else
		{
			if (ssflag)
				werror("base register should have no size/scale");
			mk_full_ind(POSTNDXMI,yypvt[-1].yinst,0,yypvt[-3].yexp);
		}
	} break;
case 113:{
		mk_full_ind(POSTNDXMI,0,0,yypvt[-1].yexp);
	} break;
case 114:{
		/* if register is data or null data register, it may
		 * be an index register with base suppressed. Then assume preindex.
		 */
		if ( yypvt[-1].yinst->tag == ADREG || yypvt[-1].yinst->tag == NULDREG )
			mk_full_ind(PRENDXMI,0,yypvt[-1].yinst,0);
		else
		if ( ssflag && (yypvt[-1].yinst->tag == AAREG || yypvt[-1].yinst->tag == NULAREG) )
			mk_full_ind(PRENDXMI,0,yypvt[-1].yinst,0);
		else
		{
			if (ssflag)
				werror("base register should have no size/scale");
			mk_full_ind(POSTNDXMI,yypvt[-1].yinst,0,0);
		}
	} break;
case 115:{	
		mkindex_indirect(PRENDXMI,yypvt[-3].yinst,yypvt[-1].yinst,yypvt[-5].yexp);
	} break;
case 116:{ 
		mkindex_indirect(PRENDXMI,yypvt[-5].yinst,yypvt[-1].yinst,yypvt[-3].yexp);
	} break;
case 117:{
		mkindex_indirect(PRENDXMI,yypvt[-5].yinst,yypvt[-3].yinst,yypvt[-1].yexp);
	} break;
case 118:{
		mkindex_indirect(PRENDXMI,yypvt[-3].yinst,yypvt[-1].yinst,0);
	} break;
case 119:{
		if((yypvt[-1].yexp->xtype) == ABSW)
			yypvt[-1].yexp->xsize = W;
		else	yypvt[-1].yexp->xsize = yypvt[-0].yint;
		yyval.yexp = yypvt[-1].yexp;
	} break;
case 120:{
		regseen++;
		if( yypvt[-0].yint == SET )		/* mark index register if seen */
			indexreg = regseen;
		yyval.yinst = yypvt[-1].yinst;
	} break;
case 121:{ 
		if( ssflag && (yypvt[-1].yint != UNSPEC || yypvt[-0].yint == SET) )
		{
			yyerror("only one index register allowed");
			if ( yypvt[-0].yint != SET ) 
				ap->ascale = X1;	/* set back to default value */
			ap->asize = yypvt[-1].yint;	/* set size of index */
			yyval.yint = SET;
		}
		else if( yypvt[-1].yint != UNSPEC || yypvt[-0].yint == SET )
		{
			ssflag++;
			ap->asize = yypvt[-1].yint;	/* set size of index */
			yyval.yint = SET;
		}
		else
			yyval.yint = NOTSET;
	} break;
case 122:{
		switch(yypvt[-0].ylong)
		{
			case 1:
				ap->ascale = X1;
				break;
			case 2:
				ap->ascale = X2;
				break;
			case 4:
				ap->ascale = X4;
				break;
			case 8:
				ap->ascale = X8;
				break;
			default:
				yyerror("illegal scale for index register");
				ap->ascale = X1;
		}
		yyval.yint = SET;
	} break;
case 123:{
		yyval.yint = NOTSET;
	} break;
case 124:{
		if ( lp_count == 0)
		{
			ssflag = 0;
			regseen = 0;
			indexreg = 0;
			ap->areg1 = NULLREG;
			ap->areg2 = NULLREG;
			ap->asize = W;	/* default */
			ap->ascale = X1;/* default */
		}
		lp_count++;
		yyval.yint = yypvt[-0].yint;
	} break;
case 125:{
		lp_count--;
		yyval.yint = yypvt[-0].yint;
	} break;
case 126:{
		if ((xp->xtype = yypvt[-0].yname->styp & TYPE) == ABS)
		{
			xp->xvalue = yypvt[-0].yname->value;
			xp->xsymptr = NULL;
			/* M68881 addition */
			xp->xvalue2 = yypvt[-0].yname->value2;
			xp->xvalue3 = yypvt[-0].yname->value3;
			/* value already in binary format */
			xp->xvaltype = FP_BIN;
			/* end M68881 addition */
		}
		else
		{
			if(wflag)
				xp->xtype = ABSW;
			xp->xvalue = 0;
			xp->xsymptr = yypvt[-0].yname;
		}
		yyval.yexp = xp++;
	} break;
case 127:{
			xp->xtype = ABSW;
			xp->xvalue = 0;
			xp->xsymptr = yypvt[-0].yname;
			yyval.yexp = xp++;
	} break;
case 128:{
		xp->xtype = ABS;
		xp->xvalue = yypvt[-0].ylong;
		xp->xsymptr = NULL;
		xp->xvaltype = FP_INT;
		yyval.yexp = xp++;
	} break;
case 129:{
		yyval.yexp = yypvt[-1].yexp;
	} break;
case 130:{
		yyval.yexp = combine(PLUS,yypvt[-2].yexp,yypvt[-0].yexp);
	} break;
case 131:{
		yyval.yexp = combine(MINUS,yypvt[-2].yexp,yypvt[-0].yexp);
	} break;
case 132:{
		yyval.yexp = combine(MUL,yypvt[-2].yexp,yypvt[-0].yexp);
	} break;
case 133:{
		yyval.yexp = combine(DIV,yypvt[-2].yexp,yypvt[-0].yexp);
	} break;
case 134:{
		xp->xtype = ABS;
		xp->xsymptr = NULL;
		xp->xvalue = 0;
		yyval.yexp = combine(MINUS,xp++,yypvt[-0].yexp);
	} break;
case 135:{
		xp->xtype = ABS;
		xp->xsymptr = NULL;
		xp->xvaltype = FP_BIN;
		binstr(yypvt[-0].ystr);
		yyval.yexp = xp++;
	} break;
case 136:{
		xp->xtype = ABS;
		xp->xsymptr = NULL;
		xp->xvaltype = FP_DEC;
		fp_mansign = POS;
		yyval.yexp = xp++;
	} break;
case 137:{
		xp->xtype = ABS;
		xp->xsymptr = NULL;
		xp->xvaltype = FP_DEC;
		fp_mansign = POS;
		yyval.yexp = xp++;
	} break;
case 138:{
		xp->xtype = ABS;
		xp->xsymptr = NULL;
		xp->xvaltype = FP_DEC;
		fp_mansign = NEG;
		yyval.yexp = xp++;
	} break;
	}
	goto yystack;		/* reset registers in driver code */
}
