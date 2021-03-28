#ident	"@(#)scan.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	MC68020 optimizer - File scan.c 	*/

/* Routines to scan the input and find relevant pieces. */

/* All of these routines assume an input scan pointer, scanp.
** Scanp only moves over characters that have been successfully
** scanned.
*/

#include <ctype.h>
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
**
** As a secondary action this routine returns a pointer to the first
** numeric constant found in the expression (with any leading '-') or NULL.
*/

static char * begnum;		/* pointer to first numeric constant */
static REG sreg();

/* Prototypes for static functions */
static int  sexpr _PARMS((void));
static int  sname _PARMS((void));
static int  snum _PARMS((void));
static struct rand * srand _PARMS((void));
static long  sreg _PARMS((void));


static boolean
sexpr()
{
    boolean onenum = true;	/* true if scanned just one numeric value */
	boolean leadingminus = false; /* default no leading minus found */
    boolean snum();
    boolean sname();
	char * holdscan;		/* hold scanning position */

	begnum = NULL;			/* clear numeric constant pointer */

    if (*scanp == '-')		/* leading - */
	  {	scanp++;		/* skip it */
		leadingminus = true; /* flag found leading minus */
	  }
    
    while (true)		/* loop until we don't find +/- separator */
	  { holdscan = scanp;
		if (snum())			/* number scanned? */
		  {	if (begnum == NULL)
			  {	begnum = holdscan;	/* point to first number scanned */
				if (leadingminus && *(begnum-1) == '-')
					/* back up to leading minus sign */
					--begnum;
			  }
		  }
		else
		if (sname())	/* see name? */
			onenum = false;	/* yes.  expression is not single number */
	    else
			serr("Bad expression");	/* SHOULD NOT OCCUR */
	
		/* quit if we don't see another +/- operator */
		if (*scanp != '-' && *scanp != '+')
			/* none there - we're done. */
	    	return(onenum);
	
		scanp++;		/* skip over operator */

	  } /* end while */

} /* sexpr() */
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

    if (islower(*scanp))
	while (islower(*scanp) || isdigit(*scanp) || *scanp == '.')
	    scanp++;            /* scan across likely candidate chars */
    
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

	/* If any operand has a void register then change this to GENERIC */
	if (	lastnode->op != FILTER			/* not filter node */
		&&	((uses(lastnode) | sets(lastnode)) & REG_VOID) != 0
	   )
	  {	int templen = strlen(first);		/* length of line */

		setop( lastnode, getOP(GENERIC));	/* fill in the gory details */
        lastnode->opcode = COPY(first, templen); /* copy text for node */
		lastnode->ops[0][templen-1] = '\0'; /* clear '\n' at line end */
    	setrand( lastnode, 1, RNULL );      /* set null values for operands */
    	setrand( lastnode, 2, RNULL );
		}
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



#ifdef m68k
/* sdisp -- scan a displacement in a new m68k addressing mode 
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
	serr("Bad sized displacement in m68k mode");

if (*scanp == '.')
    {
    scanp++;		/* Pass by "." */
	switch(*scanp++)
	{
	case 'l' : return(4);
	case 'b' : return(0);
	case 'w' : return(2);
	default  : serr("Bad size code in m68k mode displacement");
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
/* scan_020_mode -- scan across a new m68k addressing mode
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
scan_020_mode(regs_020,d1_size,d2_size) /* scan a new 68020 addr mode */
REG         * regs_020;   /* regs used in this addressing mode */
short int   * d1_size,    /* first  disp. size in bytes (0,2,4) */
            * d2_size;    /* second disp. size in bytes (0,2,4) */
{
    boolean   rpar_found  = false,     /* Have we seen a right paren yet ? */
	      disp1_found = false;
    short int cur_dsize;

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

#ifdef m68k
    REG ow_regs = REG_NONE;     /* regs used in the {o:w} field for BFxxx inst*/
    short int displ1,           /* size in bytes of '020 mode displs. */
	      displ2;
    char    * tempp;

    void scan_020_mode();       /* scan a new 68020 addr mode */
#endif

    swhite();			/* skip leading white space */
    first = scanp;		/* remember this point */

/* Begin processing based on first non-space character. */

    switch(*scanp)
    {
    case '(':			/* register indirect and auto-increment */
	scanp++;		/* skip ( */

#ifdef m68k 
	/* detect new m68k addressing modes */
	tempp = scanp;
        if (   *tempp++ != '%'     /* if not A-indirect or A-postinc  mode */
	    ||(   *tempp   != 'a'	   /* not %ai */
	       && *tempp   != 'f'	   /* not %fp */
	       && *tempp   != 's'	   /* not %sp */
	      )
	    || *(tempp+2) != ')')
	{
	    rtype = RT_OREG;        /* Default to OREG, fillrand() will complete */
  	    scan_020_mode(&regs,&displ1,&displ2); /* scan a new 68020 mode */
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
#ifdef m68k
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
	    rtype = RT_MEM;	/* no, straight memory reference */
	    break;
	}

	scanp++;		/* skip ( */
	regs = sreg();		/* get register information */
	if (*scanp == ')')	/* ) means register ind., ',' means DBLIND */
	{
	    scanp++;		/* skip ) */
	    rtype = RT_OREG;	/* register offset */

	    break;
	}
	if (*scanp++ == ',')	/* double-check double indexed */
	{
	    regs |= sreg();	/* get second register information */
	    rtype = RT_INDX;	/* double indexing operand */

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

#ifdef m68k

#define scan_past(char)    \
while (*scanp != char)     \
    scanp++;               \
scanp++;

    /* check for an appended {o:w} used by m68k BFxxx instrucs */
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
#ifdef m68k
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
	PACK('s','p'),		/* %sp */
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
	PACK(PACK(PACK('c','o'),'n'),'t'),/* %control */
	PACK(PACK(PACK('s','t'),'a'),'t'),/* %status */
	PACK(PACK(PACK('i','a'),'d'),'d'),/* %iaddr */
#endif
#ifdef m68k
    PACK(PACK('v','b'),'r'),/* %vbr */
    PACK(PACK('d','f'),'c'),/* %dfc */
    PACK(PACK('s','f'),'c'),/* %sfc */
/* #ifndef	M68040 */
    PACK(PACK(PACK('c','a'),'a'),'r'),/* %caar */
/* #endif */
    PACK(PACK(PACK('c','a'),'c'),'r'),/* %cacr */
    PACK(PACK('i','s'),'p'),/* %isp */
    PACK(PACK('m','s'),'p'),/* %msp */
#endif
/* #ifdef	M68040 */
    PACK('t','c'),			/* %tc */
    PACK(PACK(PACK('i','t'),'t'),'0'),/* %itt0 */
    PACK(PACK(PACK('i','t'),'t'),'1'),/* %itt1 */
    PACK(PACK(PACK('d','t'),'t'),'0'),/* %dtt0 */
    PACK(PACK(PACK('d','t'),'t'),'1'),/* %dtt1 */
    PACK(PACK(PACK(PACK('m','m'),'u'),'s'),'r'),/* %mmusr */
    PACK(PACK('u','r'),'p'),/* %urp */
    PACK(PACK('s','r'),'p'),/* %srp */
/* #endif */
	0			/* end of table */
};


/* Table of encode register values.  These must correspond, one-to-one
** with the encoded names above.
*/

static REG rcode[] =
{
	REG_D0,		/* %d0 */
	REG_FP,		/* %fp */
	REG_A0,		/* %a0 */
	REG_A2,		/* %a2 */
	REG_A3,		/* %a3 */
	REG_D1,		/* %d1 */
	REG_D2,		/* %d2 */
	REG_A1,		/* %a1 */
	REG_A4,		/* %a4 */
	REG_D3,		/* %d3 */
	REG_A5,		/* %a5 */
	REG_NONE,	/* %pc */
	REG_SP,		/* %sp */
	REG_D4,		/* %d4 */
	REG_D6,		/* %d6 */
	REG_D5,		/* %d5 */
	REG_D7,		/* %d7 */
	REG_VOID,	/* %cc */
	REG_VOID,	/* %sr */
	REG_A6,		/* %a6 */
	REG_A7,		/* %a7 */
	REG_VOID,	/* %usp */
#ifdef M68881
	REG_FP0,	/* %fp0 */
	REG_FP1,	/* %fp1 */
	REG_FP2,	/* %fp2 */
	REG_FP3,	/* %fp3 */
	REG_FP4,	/* %fp4 */
	REG_FP5,	/* %fp5 */
	REG_FP6,	/* %fp6 */
	REG_FP7,	/* %fp7 */
	REG_VOID,	/* %control */
	REG_VOID,	/* %status */
	REG_VOID,	/* %iaddr */
#endif
#ifdef m68k
    REG_VOID,       /* %vbr */
    REG_VOID,       /* %dfc */
    REG_VOID,       /* %sfc */
/* #ifndef	M68040 */
    REG_VOID,       /* %caar */
/* #endif */
    REG_VOID,       /* %cacr */
    REG_VOID,       /* %isp */
    REG_VOID,       /* %msp */
# endif
/* #ifdef	M68040 */
    REG_VOID,       /* %tc */
    REG_VOID,       /* %itt0 */
    REG_VOID,       /* %itt1 */
    REG_VOID,       /* %dtt0 */
    REG_VOID,       /* %dtt1 */
    REG_VOID,       /* %mmusr */
    REG_VOID,       /* %urp */
    REG_VOID,       /* %srp */
/* # endif */
	REG_NONE	/* no register */
};
/* sreg -- scan register name, return encoded value. */

static REG
sreg()
{
    unsigned long name = 0;	/* encoded name */
    unsigned long * lp;		/* pointer into table */

    if (*scanp++ == '%')	/* check for start of register */
    {
	name = PACK(scanp[0],scanp[1]);
	scanp += 2;		/* encode first two chars */

	/* allow 3 character register names and digits for M68881 */
    if (islower(*scanp) || isdigit(*scanp))
#if m68k
	  /* allow 4 character register names and numbers MC68020/30/851 */
      if (islower(*(scanp+1)) || isdigit(*(scanp+1)))
        { name = PACK(PACK(name,*scanp++),*scanp++);
		  while (islower(*scanp)) scanp++;	/* skip anything over 4 chars */
		}
	  else
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

				/* fillrand() - complete rand structure */

/* Everytime a new rand structure is built this routine is called to
** finish initializing some fields.  The cases are:
**
**	RT_REG
**	RT_IREG			Fill in 'rflags' for bit field
**
**	RT_IMMNUM:		Fill in 'rdisp' with the value of the immediate
**					Set 'rflags' to indicate size of the value (b, w, or l).
**
**	RT_OREG:
**	RT_INDX:
**	RT_EXTINDX:		Fill in 'rdisp' with any numeric constant appearing
**					in a displacement field.  For RT_OREG and RT_INDX
**					this represents the displacement value of the mode.
**
**					Fill in 'rindx' and 'rindxscl' representing the
**					index register and scale factor.
**
**					Fill in appropriate 'rflags'.
*/

void
fillrand(rand)
RAND *	rand;		/* rand to be completed */
{
	char * holdscan;			/* remember current scan pointer */
	boolean	gotdisp;			/* displacement seen */
	boolean gotreg;				/* register seen */
	boolean force16disp;		/* displacement is 16 bits */
	boolean force32disp;		/* displacement is 32 bits */
#define FILLDEBUG
#undef FILLDEBUG

#ifdef FILLDEBUG
	printf("# !!fillrand %s, rtype %d, uses %lx\n", rand->rname,
				rand->rtype, rand->ruses);
#endif
	holdscan = scanp;			/* remember scan pointer */
	rand->rdisp = 0;			/* initialize elements we set */
	rand->rindx = 0;
#ifdef m68k
	rand->owuses = 0;			/* clear bit field regs used */
	rand->rindxscl = 1;			/* default to scale of *1 */
#endif
	rand->rflags = 0;

	switch(rand->rtype)			/* break down rand type */
	{ case RT_REG:
	  case RT_IREG:
		/* if bit field found then flag this as one */
		for (scanp=rand->rname; *scanp && *scanp!='{'; scanp++);
		if (*scanp == '{')
			rand->rflags |= rfbitfld;	/* this is a bit field */
		break;

	  case RT_IMMNUM:
		rand->rdisp = getimmnum(rand);	/* get the immediate value */
		break;

	  case RT_OREG:
	  case RT_INDX:
#ifdef m68k
	  case RT_EXTINDX:
#endif
			/* Process fields in the order of
			**		  disp(disp,reg,index,disp) 
			** with optional brackets indicating memory indirection.
			** Note that this parsing covers all possible syntax.
			*/
			gotdisp = false;			/* no displacement yet */
			gotreg = false;				/* no register scanned yet */
			force16disp = false;		/* displacement > 8 bits */
			force32disp = false;		/* displacement > 16 bits */
			scanp = rand->rname;		/* start scanning operand */
			while (*scanp)				/* while more operand ... */
			  switch (*scanp)			/* check character */
			  {  case '[':
				 case ']':
					rand->rflags |= rfmemind;	/* indicate memory indirect */

			     case '(':
				 case ')':
				 case ',':
					scanp++;			/* skip this character */
					break;

				 case '{':
					rand->rflags |= rfbitfld;	/* indicate bit field */
					scan_past('}');		/* skip bit field {o:w} */
					break;

				 case '%':
					if (gotreg == false)	/* first register? */
					  {	gotreg = true;		/* flag first */
						(void) sreg();		/* scan and ignore register */
					  }
					else
					  	rand->rindx = sreg(); /* note index register */
					if (*scanp == '.')		/* size ? */
					  {	if (*++scanp == 'w') /* to size specification */
							rand->rflags |= rfdotw;	/* show .w index reg */
						scanp++;			/* skip over size  indicator */
					  }
#ifdef m68k
					if (*scanp == '*')		/* scale factor? */
					  {	rand->rindxscl = (*++scanp) - '0'; /* get scale */
						scanp++;			/* skip scale number */
					  }
#endif
					break;					/* done with register scan */


				 default:
					/* must be a displacement so determine size and value */
					{	char * prescan = scanp;	/* hold for prescan */

						(void) sexpr();		/* scan for numeric value */
						scanp = prescan;	/* restore pointer */
					}

					if (begnum != NULL)		/* have a numeric constant? */
						/* remember its value */
						rand->rdisp = strtol(begnum, (char **) NULL, 0);

					switch (sdisp())		/* determine displacement type */
					 { case 2:
							/* sdisp() doesn't return 1 so we test for this */
							if (rand->rdisp < -128 || rand->rdisp >= 128)
								force16disp = true;	/* force 16 bit size */
							break;

					   case 4:				/* external name or big offset */
							force32disp = true; /* force 32 bit size */
							break;
					 }
					} /* end operand scan */
				/* now insure addressing modes are accuratly stated */
				if (   rand->rtype == RT_OREG
											/* disp16(%an) specified */
					&& rand->rindx
											/* there is an index register */
				   )
					rand->rtype = RT_INDX;	/* force indexing mode */
				if (   force32disp
											/* displacement is 32 bits */
					|| (   force16disp
											/* displacement is 16 bits */
						&& rand->rtype == RT_INDX
											/* there is an index register */
					   )
					|| (rand->rflags & rfmemind) != 0
											/* memory indirect specified */
					|| (   rand->rtype == RT_OREG
											/* disp16(%Rn) */
						&& isdreg(rand)
											/* the register is a D reg */
					   )
				   )
				rand->rtype = RT_EXTINDX; /* force extended indexed */
				if (    rand->rtype == RT_OREG
					&& rand->rdisp == 0		/* test if no displacement */
				   )
					rand->rtype = RT_IREG;	/* force register indirect */
#ifdef FILLDEBUG
				printf("# !!! rtype %d, rindx %lx, rindxscl %d, flags %d, disp %d\n",
					rand->rtype, rand->rindx, rand->rindxscl,
					rand->rflags, rand->rdisp);
#endif
				break;
			} /* end rtype switch */

	scanp = holdscan;				/* restore scan pointer */

} /* fillrand() */
