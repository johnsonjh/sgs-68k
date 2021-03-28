/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scan.c: version 25.1 created on 12/2/91 at 17:41:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scan.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)f77/f77optim/mach/m68k:scan.c	25.1"

/* MOT:scan.c	7.2 Version,  Date of last Edit 9/18/85"*/

/*	MC680X0 optimizer - File scan.c 	*/

/* Routines to scan the input and find relevant pieces. */

/* All of these routines assume an input scan pointer, scanp.
** Scanp only moves over characters that have been successfully
** scanned.
*/

int shortmem =0;
#include <ctype.h>
#include "debug.h"
#include "optim.h"
/* #include "defs" is implicit */

extern	char * scanp;		/* scan pointer */

#define	swhite() while (*scanp == '\t' || *scanp == ' ') scanp++
/* sexpr -- scan across expression
**
** This routine scans across an expression, which can consist of
** terms (numbers or names) separated by + or -.  There may be a
** leading -.  Return true if we scan just a single number, false
** if we see a name or any non-leading + or -.
*/

static boolean
sexpr()
{
    boolean onenum = true;	/* true if scanned just one numeric value */
    short int sdisp();
    boolean snum();
    boolean sname();

	TRACE(sexpr);
    if (*scanp == '-')		/* leading - */
	scanp++;		/* skip it */
    
    while (true)		/* loop until we don't find +/- separator */
    {
	if (! snum()) 		/* check for number */
	    if (sname()){	/* see name? */
		/* hanna */
		if ( *scanp == '.'){
			shortmem++;
			scanp++;scanp++; /* just swallow the size for now */
		}
		onenum = false;	/* yes.  expression is not single number */
	}
	    else
		serr("Bad expression");
				/* no.  Neither number nor name */
	
	if (*scanp != '-' && *scanp != '+')
				/* check for operator */
	    return(onenum);	/* none there.  We're done. */
	
	scanp++;		/* skip over operator */
    }
}
/* sline -- scan one input line
**
** This routine scans an input line.  It takes care of labels,
** opcode, and operands.
*/

void
sline()
{
    char * first;		/* remembered beginning pointer */
    void dolab();
    boolean doop();
    boolean sname();
    void dorand();
    RAND * srand();
    RAND * r;

	TRACE(sline);
    swhite();			/* skip initial white space */

    /* check for label */

    first = scanp;		/* remember where we are */
    if (sname() && *scanp == ':')
    {
	dolab(first, scanp-first); /* take care of label */
	scanp++;		/* skip : */
	swhite();		/* skip to next non-white space */
    }
    else
	scanp = first;		/* back up in case we saw name */

    /* check for null operation */

    if (*scanp == '\n' || *scanp == CC)
	return;			/* no op code.  Done. */
    
    /* find op mnemonic */

    first = scanp;		/* remember starting point */
    while (islower(*scanp) || *scanp == '.')
	scanp++;		/* scan across likely candidate chars */
    
    if (scanp == first)		/* did we move? */
	serr("Bad op name");
    /* Do operation-specific things */

    if (! doop(first,scanp-first)) /* true if rest of line was processed */
    {
	/* do normal operand processing */
	int randno = 1;		/* current operand number */
	
	do
	{
	    r = srand();	/* scan next operand */
	    dorand(r, randno);	/* do other operand processing */
	    randno++;
	} while (*scanp++ == ','); /* until we don't see a , */
	scanp--;		/* back up over non-, */
	
	/* Must find newline or comment start */

	if (*scanp != '\n' && *scanp != CC)
	    serr("Bad end of instruction");
    }
    return;
}
/* sname -- scan across a "name"
**
** This routine scans across a "name".  In this context, a name is
** a leading ~, or it consists of a leading letter or _,
** followed by letters, digits, _, ~, or %.
** Return a boolean success/failure value.
*/

static boolean
sname()
{
    register char c;		/* current character */
    boolean first = true;	/* whether we're working on the first char */

	TRACE(sname);
    shortmem=0;
    if ( *scanp == '~' )	/* ~ as first character is single name */
    {
	scanp++;		/* skip ~ */
	return(true);		/* saw a name */
    }

    while ( (c = *scanp),
	       isalpha(c)	/* letter */
	    || c == '_'		/* underline */
	    || ((! first) && (isdigit(c) || c == '%' || c == '~'))
				/* non-first digit or % or ~ */
	    )
    {
	scanp++;		/* bump scan pointer */
	first = false;		/* no longer on first char */
    }

    return (! first);		/* saw something if not doing first char */
}
/* snum -- scan over number
**
** This routine scans over a number, which can be decimal, octal
** (with preceding 0), or hexadecimal (with preceding 0x or 0X).
** Allow a leading - sign for numbers.
** A success/failure code gets returned.
*/

static boolean
snum()
{
    boolean hex = false;	/* not doing hex value initially */
    register char * p = scanp;	/* use this for scanning */

	TRACE(snum);
    if (*p == '-')
	p++;			/* skip over leading sign */

    if (! isdigit(*p))		/* check for digit */
	return(false);		/* return with scanp unaffected */
    
    if (*p++ == '0')		/* check for 0x, 0X */
	if (*p == 'x' || *p == 'X')
	{
	    p++;		/* skip x/X */
	    hex = true;		/* have a hex number */
	}
    
    /* scan digits, being lazy about octal */

    while (isdigit(*p) || (hex && isxdigit(*p)))
	p++;			/* move to next char */
    
    scanp = p;			/* reset scan pointer */
    return(true);		/* say we saw a number */
}



#if m68k
/* sdisp -- scan a displacement in a new M680X0 addressing mode 
**
** This routine scans from the beginning of a displacement value
** to the first character afterward.  It returns an estimate of
** the size of the displacement.
*/

short int
sdisp()
{
long d_value;
long strtol();
char *tmpp = scanp;
boolean parfound = false;
boolean singlenum;

	TRACE(sdisp);
if (*scanp == '(')
    {
    parfound = true;
    scanp++;         /* Scan past "(" */
    }

 /* Expression may have a forced size code */
 /* such as (expr).[bwl] or <number>.[bwl] */

singlenum = sexpr();  /* read the displacement */

if (parfound)
    if (*scanp++ != ')' || *scanp != '.')
	serr("Bad sized displacement in M680X0 mode");

if (*scanp == '.')
    {
    scanp++;		/* Pass by "." */
	switch(*scanp++)
	{
	case 'l' : return(4);
	case 'b' : return(0);
	case 'w' : return(2);
	default  : serr("Bad size code in M680X0 mode displacement");
	}
    }
else /* Estimate the size of the expr. explicitly    */
{
    if (!singlenum)    /* not a single numeric value */
        return(4);     /* allocate 32 bits, then     */
    else
    {
	d_value = strtol(tmpp,(char **)NULL,0);
	if (d_value < -32768 || d_value > 32767)
	    return(4); /* number too big for 16 bits */
	else
	    if (d_value == 0)
		return(0); /* no extension bytes needed */
	    else
	        return(2); /* number fits in 16 bits    */
    }
}
}
/* scan_020_mode -- scan across a new M680X0 addressing mode
**
** This routine scans from the character past the opening '('
** to the final balanced closing ')'.  The operand is classified
** by type, and the RAND structure is built and returned.
**
** The most general form of operand allowed without double indirection is
**
** (base_disp_expr,Ai,Rj,outer_disp_expr)
**
** Note that any of these four terms may be absent, and that 
** the index register Rj may contain a size code and/or scaling factor.
**
** The forms allowing double indirection are similar to the ones above,
** except that a single [] pair encloses some of the above terms.
*/

void
scan_020_mode(regs_020,d1_size,d2_size) /* scan a new 680X0 addr mode */
REG         * regs_020;   /* regs used in this addressing mode */
short int   * d1_size,    /* first  disp. size in bytes (0,2,4) */
            * d2_size;    /* second disp. size in bytes (0,2,4) */
{
    boolean   rpar_found  = false,     /* Have we seen a right paren yet ? */
	      disp1_found = false;
    short int cur_dsize;

	TRACE(scan_020_mode);
    *regs_020 = REG_NONE;
    *d1_size = *d2_size = 0;

    while (rpar_found != true)
    {
	switch(*scanp)
	{
	case ')': rpar_found = true;
	case '[':
	case ']':
	case ',': scanp++; /* next character, please */
		  break;
	case '%': *regs_020 |= sreg(); /* save the register value */
		  if (*scanp == '.')   /* read optional size code */
		      scanp += 2;
		  if (*scanp == '*')   /* read optional scale factor */
		      scanp += 2;
		  break;
	default : /* It must be a displacement expression, then.  */
		  cur_dsize = sdisp(); /* estimate the disp. size */
		  if (disp1_found == true)
		      *d2_size = cur_dsize;
		  else
		  {
		      *d1_size = cur_dsize;
		      disp1_found = true;
		  }
		  break;
	}
    }
}
#endif

/* srand -- scan across complete operand
**
** This routine scans across a complete normal operand.
** The operand is classified by type, and the RAND structure
** is built and returned.  The basic operand types we look for
** are keyed on the first character or two:
**
**	%	reg				RT_REG
**	&	number				RT_IMMNUM
**	&	expression			RT_IMMED
**	(	%reg)				RT_IREG
**	(	%reg)+				RT_INCR
**	-	(%reg)				RT_DECR
**	[-]	expression			RT_MEM
**	[-]	expression(%reg)		RT_OREG
**	[-]	expression(%reg,%reg.wl)	RT_DBLIND
*/

static RAND *
srand()
{
    REG regs = REG_NONE;	/* registers used */
    RTYPE rtype;		/* operand type */
    char * first;		/* starting point for operand */
    RAND * operand;		/* pointer to result structure */

    boolean sexpr();
    REG sreg();

#if m68k
    REG ow_regs = REG_NONE;     /* regs used in the {o:w} field for BFxxx inst*/
    short int displ1,           /* size in bytes of '020 mode displs. */
	      displ2;
    char    * tempp;

    void scan_020_mode();       /* scan a new 680X0 addr mode */
#endif

	TRACE(srand);
    swhite();			/* skip leading white space */
    first = scanp;		/* remember this point */

/* Begin processing based on first non-space character. */

    switch(*scanp)
    {
    case '(':			/* register indirect and auto-increment */
	scanp++;		/* skip ( */

#if m68k 
	/* detect new M680X0 addressing modes */
	tempp = scanp;
        if (   *tempp++ != '%'     /* if not A-indirect or A-postinc  mode */
	    ||(   *tempp   != 'a'	   /* not %ai */
	       && *tempp   != 'f'	   /* not %fp */
	       && *tempp   != 's'	   /* not %sp */
	      )
	    || *(tempp+2) != ')')
	{
	    rtype = RT_020;        /* flag addr mode type        */
  	    scan_020_mode(&regs,&displ1,&displ2); /* scan a new 680X0 mode */
	}
	else
#endif
	{   /* Handle A-indirect or A-postincrement mode    */
	    regs = sreg();		/* get register information */
	    if (*scanp++ != ')')	/* must see trailing ) */
	    serr("Bad register indirect");
	
	    rtype = RT_IREG;	/* assume register indirect */
	    if (*scanp == '+')	/* check for auto-increment */
	    {
	         scanp++;		/* have it.  Skip + */
	         rtype = RT_INCR;
	    }
	}

	break;

    case '%':			/* register */
	regs = sreg();		/* get register code */
#if m68k
	if (*scanp == ':')	/* read Ri:Rj construct for '020 mul,div */
	{
	    scanp++;
	    regs |= sreg();	/* save Rj register value */
	    rtype = RT_REG_PAIR;
	}
	else
#endif
	    rtype = RT_REG;	/* set operand type */
	break;

    case '&':			/* immediate */
	scanp++;		/* skip & */
	rtype = RT_IMMNUM;	/* assume single number */
	if (! sexpr())		/* single numeric value? */
	    rtype = RT_IMMED;	/* no. */
	break;
    
    case '-':			/* predecrement and others */
	scanp++;		/* skip - */
	if (*scanp == '(')	/* start predecrement */
	{
	    scanp++;		/* skip ( */
	    regs = sreg();	/* get register information */
	    rtype = RT_DECR;	/* set operand type */
	    if (*scanp++ != ')') /* check for closing ) */
		serr("Bad predecrement");
	    break;		/* done */
	}
    
	/* fall through to default case */
/* Handle other cases of operand type */

    default:
	(void) sexpr();		/* scan across expression */
	if (*scanp != '(')	/* register offset or double index? */
	{
	    if(shortmem){
		rtype = RT_SMEM;
		shortmem = 0;
	    }else rtype = RT_MEM;	/* no, straight memory reference */
	    break;
	}

	scanp++;		/* skip ( */
	regs = sreg();		/* get register information */
	if (*scanp == ')')	/* ) means register ind., ',' means DBLIND */
	{
	    scanp++;		/* skip ) */
	    rtype = RT_OREG;	/* register offset */

	    /* If the register is %fp or %sp, this is really the same as
	    ** a memory reference.
	    */

	    if ((regs & (REG_FP | REG_SP)) != 0)
		rtype = RT_MEM;	/* call this memory */
	    break;
	}
	if (*scanp++ == ',')	/* double-check double indexed */
	{
	    regs |= sreg();	/* get second register information */
	    rtype = RT_DBLIND;	/* double indexing operand */

	    /* check for trailing .[wl] ) */

	    if (*scanp++ == '.')
	    {
		if (*scanp == 'w' || *scanp == 'l')
		    if (*++scanp == ')')
		    {
			scanp++; /* skip ) */
			break;
		    }
	    }
	    serr("Bad double index syntax");
	}
	/* Reach here on some error */

	serr("Bad operand");
    } /* end switch */

#if m68k

#define scan_past(char)    \
while (*scanp != char)     \
    scanp++;               \
scanp++;

    /* check for an appended {o:w} used by M680X0 BFxxx instrucs */
    if (*scanp == '{')
    {
	if (*(scanp++) == '%')    /* up to {o    */
	    ow_regs = sreg();     /* save register used */
	scan_past(':');           /* up to {o:   */
	if (*scanp == '%')
	    ow_regs |= sreg();    /* up to {o:w  */
	scan_past('}');           /* up to {o:w} */
    }
#endif

    operand = saverand(first, scanp-first, rtype, regs, true);
				/* build RAND structure */
#if m68k
    operand->disp1 = displ1;   /* 0 if not appropriate */
    operand->disp2 = displ2;   /* 0 if not appropriate */
    operand->owuses = ow_regs; /* REG_NONE if not appropriate */
#endif
    
    swhite();			/* skip trailing white space */
    return(operand);		/* return new operand */
}
/* Tables for sreg.  They give the correspondence between register
** names and REG value.  There are two parallel tables, one with the
** packed representation for the name, the other with the value.
** Since sreg() uses a linear search, the names are ordered by
** frequency of use, according to several large code samples.
*/

/* Table of packed names */

#define	PACK(a,b)	(((a) << 8 ) + (b))

static unsigned long rname[] =
{
	PACK('d','0'),		/* %d0 */
	PACK('f','p'),		/* %fp */
	PACK('s','p'),		/* %sp */
	PACK('a','0'),		/* %a0 */
	PACK('a','2'),		/* %a2 */
	PACK('a','3'),		/* %a3 */
	PACK('d','1'),		/* %d1 */
	PACK('d','2'),		/* %d2 */
	PACK('a','1'),		/* %a1 */
	PACK('a','4'),		/* %a4 */
	PACK('d','3'),		/* %d3 */
	PACK('a','5'),		/* %a5 */
	PACK('p','c'),		/* %pc */
	PACK('d','4'),		/* %d4 */
	PACK('d','6'),		/* %d6 */
	PACK('d','5'),		/* %d5 */
	PACK('d','7'),		/* %d7 */
	PACK('c','c'),		/* %cc */
	PACK('s','r'),		/* %sr */
	PACK('a','6'),		/* %a6 */
	PACK('a','7'),		/* %a7 */
	PACK(PACK('u','s'),'p'),/* %usp */
#ifdef M68881
	PACK(PACK('f','p'),'0'),/* %fp0 */
	PACK(PACK('f','p'),'1'),/* %fp1 */
	PACK(PACK('f','p'),'2'),/* %fp2 */
	PACK(PACK('f','p'),'3'),/* %fp3 */
	PACK(PACK('f','p'),'4'),/* %fp4 */
	PACK(PACK('f','p'),'5'),/* %fp5 */
	PACK(PACK('f','p'),'6'),/* %fp6 */
	PACK(PACK('f','p'),'7'),/* %fp7 */
#endif
	0			/* end of table */
};


/* Table of encode register values.  These must correspond, one-to-one
** with the encoded names above.
*/

static REG rcode[] =
{
	REG_D0,		/* %d0 */
	REG_FP,		/* %fp */
	REG_SP,		/* %sp */
	REG_A0,		/* %a0 */
	REG_A2,		/* %a2 */
	REG_A3,		/* %a3 */
	REG_D1,		/* %d1 */
	REG_D2,		/* %d2 */
	REG_A1,		/* %a1 */
	REG_A4,		/* %a4 */
	REG_D3,		/* %d3 */
	REG_A5,		/* %a5 */
	REG_PC,		/* %pc */
	REG_D4,		/* %d4 */
	REG_D6,		/* %d6 */
	REG_D5,		/* %d5 */
	REG_D7,		/* %d7 */
	REG_CC,		/* %cc */
	REG_SR,		/* %sr */
	REG_A6,		/* %a6 */
	REG_A7,		/* %a7 */
	REG_USP,	/* %usp */
#ifdef M68881
	REG_FP0,	/* %fp0 */
	REG_FP1,	/* %fp1 */
	REG_FP2,	/* %fp2 */
	REG_FP3,	/* %fp3 */
	REG_FP4,	/* %fp4 */
	REG_FP5,	/* %fp5 */
	REG_FP6,	/* %fp6 */
	REG_FP7,	/* %fp7 */
#endif
	REG_NONE	/* no register */
};
/* sreg -- scan register name, return encoded value. */

static REG
sreg()
{
    unsigned long name = 0;	/* encoded name */
    unsigned long * lp;		/* pointer into table */
	TRACE(sreg);

    if (*scanp++ == '%')	/* check for start of register */
    {
	name = PACK(scanp[0],scanp[1]);
	scanp += 2;		/* encode first two chars */

#ifdef M68881
	if (islower(*scanp) || isdigit(*scanp))	/* check for %fp[0-9] or %usp */
#else
	if (islower(*scanp))	/* check for third letter */
#endif
	    name = PACK(name,*scanp++);
	
	/* search for register name in list */

	for (lp = rname; *lp != 0; lp++)
	    if (*lp == name)
		return(rcode[lp-rname]);
				/* return encoded value on match */
    }

    /* On any failure, produce error message. */

    serr("Bad register name");
/*NOTREACHED*/
}
