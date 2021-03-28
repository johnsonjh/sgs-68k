#ident	"@(#)parse.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*      MC68000 Optimizer - File parse.c        */

/* This module contains the main driving routines for the code improver
** (aka optimizer).  The main machine-dependent program control takes
** place here.
*/

#include <stdio.h>
#include <string.h>

/* include compiler macdefs.h for ASM_COMMENT and ASM_END delimiters */
/* and for STACKSAFETY stack probe value */
#include "macdefs.h"

#include "optim.h"
/* #include "defs" is implicit */

/* Prototypes for static functions */
static void  setsection _PARMS((int , int ));
static void  outsection _PARMS((int , int ));
static void  prline _PARMS((void));
static void  begfunc _PARMS((void));
static void  endfunc _PARMS((void));
static void  prehash _PARMS((void));
static void  note _PARMS((long ));
static void  pstats _PARMS((void));
static void  doswbeg _PARMS((void));
static void  swittab _PARMS((int , int ));
static void  dodef _PARMS((void));

/* global items located in this module */
int		framesz;			/* optimized frame size from c1 */
boolean leafproc = false;	/* procedure doesn't call any other */
boolean adrframe = false;	/* address taken of frame object */
boolean ininline = false;   /* true if processing in-line ASM */
boolean involatile = false;   /* true if processing in-line VOL */
FLAGS	nattrmask = 0;		/* next node only inline optimize trigger flags */
SSTR	*sstrlist = 0;		/* static string length list */

/* global flags passed from compiler pass 2 */
boolean pass2optf;			/* this move stores floating-point data */
REG		frtnregs;			/* function return "hot" registers */

/* Line buffer for use while scanning input. */

static char line[BUFSIZ];

boolean	stkoffar[1024];
extern int keeplink;					/* jcl: don't optimize out link/unlk */

char * scanp;                           /* global scanning pointer -- points
                                        ** at current position in input
                                        */
static char * curstart;                 /* current line's virtual starting
                                        ** position
                                        */

/* State variables */

static boolean infunction;              /* true if currently within function
                                        ** bounds
                                        */
static void (* postline)();             /* pointer to post-line processor */
#define NOPOST  ((void (*)()) 0)        /* null pointer for same */

/* The parser keeps track of what section the input and output streams
** are in.  Redundant sections changes are eliminated.
*/

static OPCODE isection = TEXT;          /* current section being input,
                                        ** starts as TEXT 0
                                        */
static char isecnum = '0';              /* input section number */

static OPCODE osection = TEXT;          /* current section being output:
                                        ** starts as TEXT 0
                                        */
static char osecnum = '0';              /* output section number is zero */

static IDTYPE lineno = IDVAL;           /* current line number */

/* Debugging flags */

static boolean lineflag = false;        /* output input line:  -0 */
boolean labflag = false;         /* output label data:  -1 */
boolean opflag = false;          /* output op code data:  -2 */
boolean randflag = false;        /* output operand data:  -3 */
boolean switchflag = false;      /* output switch info:  -4 */
boolean statflag = false;        /* output register stats:  -5 */
boolean vtprintflag = false;     /* output value trace info -6 */
boolean peepflag = false;        /* output peephole info -7 */
boolean PCflag = false;          /* output PC values in blocks -8 */
boolean tempflag = false;	 	 /* tempreg logic, inline opt  -9 */
boolean frameflag = false;		 /* output frame removal info */
boolean shortflag = false;       /* suppress branch shortening
                                        ** if true -j */
boolean abortflag = false;       /* abort process on error:  -k */

/* yyinit -- initialize the world */

void
yyinit(flags)
char * flags;
{
	short	ind;
    infunction = false;                 /* not in function */

	for (ind = 0; ind != 1024; ind++)
		stkoffar[ind] = 0;

    while (*flags != 0)                 /* look for local flags */
    {
        switch( *flags++ )
        {
        case '0':       lineflag = true; break;
        case '1':       labflag = true; break;
        case '2':       opflag = true; break;
        case '3':       randflag = true; break;
        case '4':       switchflag = true; break;
        case '5':       statflag = true; break;
        case '6':       vtprintflag = true; break;
        case '7':       peepflag = true; break;
        case '8':       PCflag = true; break;
		case '9':		tempflag = true; break;
		case 'A':		frameflag = true; break;
        case 'j':       shortflag = true; break;
        case 'k':       abortflag = true; break;
        default:        break;          /* ignore unknowns */
        }
    }

    return;
}
/* getline -- read new input line, do appropriate set-up */

boolean                                 /* returns false on EOF, else true */
getline()
{
    if (fgets(line,BUFSIZ,stdin) == NULL)
    {
        if (feof(stdin))
            return(false);              /* hit EOF */
        else
            oerr("Error reading input");
    }

    if (lineflag)
        PRINTF("Line:  %s", line);
    curstart = scanp = line;            /* set up scan pointer */
    return(true);                       /* successful read */
}
/* yylex -- parse input, call optimization routines
**
** Yylex parses the input and calls the various optimization routines
** on a per/function basis.
*/

void
yylex()
{
    void sline();
    void endfunc();
    void pstats();

    /* loop through input lines */

    while (getline())
    {
		/* process pass2 and ASM special optimization comments */
		if ( line[0] == COMMENTSTR[0] )
		  {	if ( strncmp(&line[1], "OPT", 3) == 0 )
				dopass2opt(true, line[4]);
			if ( strncmp(&line[1], "NOPT", 4) == 0 )
				dopass2opt(false, line[5]);
			if (strncmp(&line[1], "OPTVOL", 6) == 0)
			{
				ininline = 1;	/* volatile being processed */
				involatile = 1;	/* volatile being processed */
			}
			if (strncmp(&line[1], "NOPTVOL", 7) == 0)
			{
				ininline = 0;	/* exit volatile mode */
				involatile = 0;	/* exit volatile mode */
			}
			if (   strlen(line) >= strlen(ASM_COMMENT) 
	     		&&	strncmp(line,ASM_COMMENT,strlen(ASM_COMMENT)) == 0 
		   	)
				ininline = 1;           /* in-line ASM being processed */
			if (   strlen(line) >= strlen(ASM_END)
	     		&& strncmp(line,ASM_END,strlen(ASM_END)) == 0
	 	   	)
		  		ininline = 0;           /* exit in-line mode */
	  	  }
		
		/* process in-line special optimization flags */
		if ( line[1] == COMMENTSTR[0] )
	  	  {	/* inline.h function name check */
			if (strncmp(&line[2], "strcpy()", 8)==0)
				nattrmask = UDN_strcpy; /* strcpy() inline opt trigger */
			if (strncmp(&line[2], "endopt", 6)==0)
				lastnode->userdata.ud_nattr |= UDN_ENDINLINE;
				
	      }

    	postline = NOPOST;              /* set line post-processor */
    	sline();                        /* process the line */
    	if (postline != NOPOST)
        	postline();                 /* do post-processor */
      }

    if (infunction)                     /* if still in function, optimize */
        endfunc();
      else filter();                    /* catch possible psuedo ops at end */

    if (statflag)
        pstats();
    if (ferror(stdout))                 /* check for output error */
        oerr("Error writing output");
    return;
}
/* dopass2opt -- process option from compiler pass 2 
**
** Pass 2 (code generation) is sending us information to help us optimize.
** The proper flag is set on or off.
**
*/
void
dopass2opt(valu, c)
boolean valu;						/* true of false value to set */
char	c;							/* flag ID */
{
	switch (c)
	{        
	case 'D':
		frtnregs = RETREGINT;		/* integer live at exit */
		break;

	case 'P':
		frtnregs = RETREGPTR;		/* pointer live at exit */
		break;

#ifdef M68881
	case 'F':						/* "mov.l is floating-point" flag */
		pass2optf = valu;
		break;
#endif

	case 'X':
		frtnregs = RETREGFP;		/* floating live at exit */
		break;
#if 0
	default:
		oerr("Invalid flag from compiler pass 2");
#endif
	}
}
/* doop -- do operator processing
**
** This routine, called by the scanner, handles an operation or
** pseudo-op.  It returns true if the entire line has been scanned.
** Otherwise the scanner will continue scanning for operands.
*/

boolean
doop(ptr,len)
char * ptr;
int len;
{
    extern OP * findop();
    extern void prline();
    OP * opptr = findop(ptr,len);
    int templen;                        /* local temporary length */
    char * cp;                          /* temporary pointer */
    void doswitch();
    void doswbeg();
    int doset();
    void dodef();
    void setsection();

	/* If "long" or "short" op in in-line code text section 
	** convert it to GENERIC.
	*/
	if (	opptr != NULL				/* op was found */
		&&	isection != DATA			/* it's in text */
		&&	ininline					/* it's in inline code */
		&& (	opptr->op_code == LONG
			||	opptr->op_code == SHORT	/* it's 'long' or 'short' */
		   )
	   )
		opptr = findop("generic", 7);	/* must set to GENERIC */

	/* if undefined op then crash unless this is in-line text in which
	** case we treat it as a GENERIC.
	*/
	if (	opptr == NULL				/* op not recognized */
	  	&&	(	!ininline				/* not in-line code */
		  	 || isection == DATA		/* in data section */
		  	 || (opptr=findop("generic", 7)) == NULL /* no GENERIC operation */
		 	)
	   )
			serr("Op not found");		/* couldn't find op code */

    if (opflag)
    {
        PRINTF("\t%s:  %s\n", (opptr->op_flags & OF_PSEUDO) ? "Special op" :
                "Normal op", opptr->op_name);
        PRINTF("\t------- Op code is %d\n",opptr->op_code );
    }

    if (opptr->op_flags & OF_NOLEAF)
		leafproc = false;		/* not a leaf procedure */

    switch( opptr->op_code )
    {
    case TEXT:
        if (*(cp = ptr+len) == '\t' || *cp == ' ')
            setsection(TEXT, cp[1]);    /* set new section */
        else
            setsection(TEXT, '0');      /* assume default TEXT section */
        return(true);
    
    case DATA:
        if (*(cp = ptr+len) == '\t' || *cp == ' ')
            setsection(DATA, cp[1]);    /* set new section */
        else
            setsection(DATA, '0');      /* assume default DATA section */
        return(true);

    case LN:                            /* line number information;
                                        ** get the actual number
                                        */
        if (sscanf( scanp, " %d", &lineno) != 1)
            serr("Bad ln");
        return(true);                   /* done handling line */

    case DEF:                           /* handle symbol info (def) */
        dodef();                        /* handle the def */
        return(true);                   /* all processing done by dodef */

    case SET:
        templen = strlen(curstart);     /* length of current line */
        appfl(curstart, templen);       /* append whole line as filter node */
        lastnode->ops[0][templen-1] = '\0';
                                        /* clobber newline with null */
        postline = (void (*)())doset;           /* use this post-processor */
        return(false);                  /* still want to process operands */

    case SWBEG:
        doswbeg();                      /* handle special switch pseudo-op */
        return(true);                   /* all done this line (and more) */

	/* otherwise, treat as standard pseudo op */

    default:
        if (   isection == DATA
            || (opptr->op_flags & OF_PSEUDO) != 0
            || (isection == TEXT && ! infunction)
            )
        {
            prline();
            return(true);
        }
        break;                          /* for everything else, get out */
    }
   	saveop(0, opptr->op_name, 0, (int) (opptr->op_code) );
                                        /* add new node */
    setop( lastnode, opptr );           /* fill in the gory details */

	/* set as entire line for GENERIC statement */
	if (lastnode->op == GENERIC)
	  {	int templen = strlen(ptr);		/* find total line length */

		lastnode->opcode = COPY(ptr, templen); /* copy it for node */
		lastnode->ops[0][templen-1] = '\0';	/* remove trailing '\n' */
		}

    setrand( lastnode, 1, RNULL );      /* set null values for operands */
    setrand( lastnode, 2, RNULL );
                                        /* done processing if no source or
                                        ** destination operands
                                        */
    lastnode->uniqid = lineno;          /* save current line number */
    lineno = IDVAL;                     /* set line number to "none" */
    return(opptr->op_nsrc == 0 && opptr->op_dnum == NODEST);
}
/* setsection -- set current program section
**
** This routine sets a new input program section.
*/

static void
setsection(op, sectnum)
OPCODE op;                              /* section number:  TEXT or DATA */
char sectnum;                           /* presumed section number char */
{
    isection = op;
    isecnum = sectnum;
    return;
}
/* outsection -- output new section name, if necessary
**
** This routine outputs a new section pseudo-op if the current
** output section differs from the one we want.
*/

static void
outsection(sect, sectnum)
OPCODE sect;                            /* desired section */
char sectnum;                           /* desired section number */
{
    if (sect != osection || sectnum != osecnum)
    {
        if (sect == TEXT)
            PRINTF("\ttext");
        else
            PRINTF("\tdata");
        
        if (sectnum != '0')
            PRINTF("\t%c\n", isecnum);
        else
            PUTCHAR('\n');
        
        /* remember new section, number */
        osection = sect; osecnum = sectnum;
    }
    return;
}
/* prline -- print current line */

static void
prline()
{

    outsection( isection, isecnum );    /* make sure stuff goes in proper
                                        ** section
                                        */
    PRINTF("%s", curstart);             /* output entire current line from
                                        ** current virtual starting point
                                        */
    return;
}
/* dolab -- process a label
**
** This routine handles a label.  A non-compiler-generated label in
** text space marks the beginning of a function.
*/

void
dolab(ptr,len)
char * ptr;
int len;
{
    void endfunc();
    void begfunc();
    RAND * r;
    boolean makehard = false;           /* true if label should be hard */

    if (labflag)
        PRINTF("\tSaw label:  %.*s\n", len, ptr);

    if ((isection == DATA) ||              /* label in data */
		( isection == TEXT && (isecnum == '2' || isecnum == '3') ) )	/* bk */
    {
        outsection( isection, isecnum ); /* get it in proper section */
        PRINTF("%.*s:\n", len, ptr);    /* output label */
        curstart = ptr + len;           /* change apparent line's starting
                                        ** point, removing label
                                        */
        return;
    }

    /* check for start of function */

    if (   isection == TEXT
        && (   len <= 3
            || !( ptr[0] == 'L' && ptr[1] == '%' )
            )
		&& !ininline
        )
    {	int	i;				/* work variable */

        if (labflag)
            PRINTF("\t--------------- START OF FUNCTION ----------------\n");
        if (infunction)
            endfunc();                  /* do end of function processing */
        begfunc();                      /* now start new one */
		if (!Pass2)
		  {	/* if first pass create PROC entry */
			Curproc->name = xalloc(len+1);	/* space for function name */
			(void) strncpy(Curproc->name, ptr, len);	/* and stash it away */
			(Curproc->name)[len] = '\0';	/* null terminate name */
			}
		else
		  {	/* if second pass find PROC entry */
			for (Curproc=Headproc; Curproc; Curproc=Curproc->next)
				if (   strlen(Curproc->name) == len
					&& strncmp(Curproc->name, ptr, len) == 0
				   )
					break;					/* found entry */
			if (Curproc == NULL)
				oerr("Can't Find PROC");	/* something wrong */
			}
        makehard = true;                /* make this label hard */
    }

    r = saverand(ptr, len, RT_LABEL, REG_NONE, true);
    rprint(r,1);
    applbl( r->rname, 0 );              /* append as label node */
    if (makehard ||  					/* if want to make a hard label */
		ininline)          				/* or inline asm (jcl) */
        lastnode->userdata.ud_flags |= UD_HLAB;
    return;
}
/* dorand -- process operand
**
** The scanner calls this routine to dispose of an operand.  The
** RAND structure has already been built at this point.  We simply
** squirrel the relevant parts away in the current node.
*/

void
dorand(r,n)
RAND * r;
int n;
{
    setrand( lastnode, n, r);
	if (isfpreg(r))
		postline = (void (*)()) preprocfp;		/* need to preprocess */
    rprint(r,n);
    return;
}
rprint(r,n)
RAND * r;
int n;
{
    RTYPE rtype = r->rtype;
    REG regs = r->ruses;
    char * ptr = r->rname;
    void note();

    if (randflag)
    {
        PRINTF("\t\tOperand %d:  %s\n", n, ptr);
        PRINTF("\t\t\tRegisters:  0x%.8x, Operand type:  %d\n", regs, rtype);
    }
    if (statflag)
        note(regs);
    return;
}
/* begfunc -- do beginning of function processing */

static void
begfunc()
{
    void prehash();
    void init();

    /* If this is pass 1 then setup new PROC for this function */
    if (!Pass2)
	  {	PROC	*pptr;				/* proc pointer */

    	pptr = (PROC *) xalloc(sizeof(PROC));	/* space for PROC structure */
		if (Headproc == NULL)
		  	Headproc = pptr;		/* start PROC list */
		else
		  	Curproc->next = pptr;	/* chain in this one */
		Curproc = pptr;			/* this is now Curproc */
		pptr->next = NULL;			/* clear next link */
		pptr->calllist = NULL;		/* clear out structure */
		pptr->foldable = 1;			/* default to foldable */
		pptr->recurse = 0;			/* default not recursive function */
		pptr->usecnt = 0;			/* no usecount */
		pptr->length = 0;			/* no instruction count yet */
		pptr->insert = NULL;		/* no insert point yet */
		pptr->text = NULL;			/* no text yet */
		pptr->name = NULL;			/* no name yet */
		}

    infunction = true;                  /* say we're in a function */
	adrframe = false;		/* default to no address taken of frame item */
	frtnregs = REG_NONE;	/* default to no value returned in regs */
    leafproc = true;			/* default to leafproc type */
    prehash();                          /* initialize operand hash table */
    return;
}
/* endfunc -- do end of function processing
**
** End of function processing is triggered either by a global label
** in text space or a special "def" pseudo-op with a storage class
** of -1.  See dolab() and dodef().
*/

static void
endfunc()
{
    void bspan();
    void outsection();
    boolean didsomething;		/* flag if optimized somewhere */

	newproc();					/* new function prolog processing */

    filter();                       /* flush filter nodes */

reoptimize:

	/* Master optimization loop */

	/* The requirements for a valid flow graph and live/dead information
	** are noted for each function.  Generally each function will guarantee
	** that both are valid upon return.
	*/
	do
	{   didsomething = false;

		/* neither requires nor returns FG and L/D info */
		rmconlbl();					/* remove consecutive labels */

	    /* do combined value trace and peephole optimizations */
		/* Note: before optim() so loop rotation doesn't foil value trace */
		/* requires: FG and L/D,  leaves: valid FG and L/D */
		ldanal();					/* furnish good live/dead info */
	    if (vtrace())
			didsomething = true;

		if (vtprintflag | peepflag )
			printf("funclive=%#.8x, funcsrc=%#.8x\n", funclive, funcsrc);

	    /* optimize blocks in current function */
		/* requires: nothing,  leaves: valid FG and L/D */
        if (optim())
			didsomething = true;

		if (vtprintflag | peepflag )
			printf("funclive=%#.8x, funcsrc=%#.8x\n", funclive, funcsrc);

		/* optimize loop prologs */
		/* requires: FG and L/D,  leaves: valid FG and L/D */
		if (loopopt())
			didsomething = true;

		/* remove redundant mov r,r */
		/* requires: FG and L/D,  leaves: valid FG and L/D */
		if (redmov())
			didsomething = true;

		/* fold instructions into addressing modes */
		/* requires: FG and L/D,  leaves: valid FG and L/D */
		if (eaopt())
			didsomething = true;

#ifdef m68k
		/* convert potential if/else to Scc */
		/* requires: FB,  leaves: valid FG (L/D unchanged) */
		if (sccopt())
			didsomething = true;
#endif

	}
        while ( didsomething );

	/* requires: funclive/funcsrc */
    if (usetemps()) goto reoptimize;	/* if scratch used for reg variable*/

	/* Now set foldable status using final function image */
	if (!Pass2)
		setfold();

	/* do not optimize branches to RTS if this function is foldable */
	if (Pass2 || !Curproc->foldable)
		/* requires: FG, funclive/funcsrc */
		brarts();					/* optimize branches to RTS */

    if (! shortflag)
		/* requires: FG */
        bspan(PCflag);              /* shorten span dependent branches */

	if (!Pass2)
		foldstore();				/* save small functions for folding */

    outsection( TEXT, '0' );        /* get instructions in text */

	/* requires: FG, funclive */
	if (keeplink)					/* jcl */
		prtext();		    		/* write out function keeping link/unlk */
	else
		writefunc(false);           /* write out the function */

    infunction = false;             /* no longer in function */
    clrhash();                      /* fix machine dependent bug */
                                    /* Hashtab[] invalid after init() */
    init();                         /* initialize mip for next function */
    return;
}
/* prehash -- initialize operand hash table
**
** This routine presets the operand hash table after first clearing it.
** Certain operand strings must be added to the hash table if we're
** doing live/dead analysis so we'll recognize special subroutine
** calls to run-time support routines.
*/

static void
prehash()
{
    void clrhash();

    clrhash();                          /* clear operand hash table */

    /* add common and special operands to table */

    (void) saverand("%d0", 3, RT_REG, REG_D0, true);
    (void) saverand("%a0", 3, RT_REG, REG_A0, true);
#ifdef M68881
    (void) saverand("%fp0", 4, RT_REG, REG_FP0, true);
#endif

#ifdef  LIVEDEAD
#ifndef M68881
    (void) saverand("ldiv%%", 6, RT_RTSUB, REG_NONE, true);
    (void) saverand("lrem%%", 6, RT_RTSUB, REG_NONE, true);
    (void) saverand("lmul%%", 6, RT_RTSUB, REG_NONE, true);
    (void) saverand("uldiv%%", 7, RT_RTSUB, REG_NONE, true);
    (void) saverand("ulrem%%", 7, RT_RTSUB, REG_NONE, true);
    (void) saverand("ltodb%%", 7, RT_RTSUB, REG_NONE, true);
    (void) saverand("ultodb%%", 8, RT_RTSUB, REG_NONE, true);
    (void) saverand("fltodb%%", 8, RT_RTSUB, REG_NONE, true);

    (void) saverand("dbadd%%", 7, RT_SUB01, REG_NONE, true);
    (void) saverand("dbcmp%%", 7, RT_SUB01, REG_NONE, true);
    (void) saverand("dbdiv%%", 7, RT_SUB01, REG_NONE, true);
    (void) saverand("dbmul%%", 7, RT_SUB01, REG_NONE, true);
    (void) saverand("dbneg%%", 7, RT_SUB01, REG_NONE, true);
    (void) saverand("dbsub%%", 7, RT_SUB01, REG_NONE, true);
    (void) saverand("dbtol%%", 7, RT_SUB01, REG_NONE, true);
    (void) saverand("dbtst%%", 7, RT_SUB01, REG_NONE, true);
    (void) saverand("dbtofl%%", 8, RT_SUB01, REG_NONE, true);
#endif
	(void) saverand("mcount%", 7, RT_SUB02, REG_NONE, true);
#endif

    return;
}
/* output optimizer scanning error */

void exit();

void
serr(s)
char * s;                               /* error string to print */
{
    char * cp;
    fprintf(stderr,"Fatal Optimizer error:  %s\n%s",s,line);
    for (cp = line; cp < scanp; cp++)
        if(isprint(*cp))        /* used to be isgraph()         E.J.R. */
            *cp = ' ';
    *scanp = '\0';
    FPRINTF(stderr,"%s^\n",line);

    if (abortflag)
        abort();                        /* kill process */
    else
        exit(1);
/*NOTREACHED*/
}


/* output optimizer error in non-input-scanning situation */

void
oerr(s)
char * s;                               /* error string */
{
    FPRINTF(stderr,"Fatal optimizer error:  %s\n", s);
    if (abortflag)
        abort();
    else
        exit(2);
/*NOTREACHED*/
}
/* Collect count of frequency of register reference (lexically) */

static int counts[NREGS];                       /* word per register */

static void
note(regs)
REG regs;
{
    int i;
    long temp = regs;

    for (i = 0; temp != 0; i++, temp >>= 1)     /* count each bit */
        if ((temp & 1) != 0)
            counts[i]++;
}

/* output lexical register use statistics */

static void
pstats()
{
    int i;

    FPRINTF(stderr, "\n\nRegister statistics:\n");

    for (i = 0; i < 20; i++)
        if (counts[i] != 0)
            FPRINTF(stderr,"\t%d\t      %#.8x\n", counts[i], 1 << i);
}
/* doset -- handle set pseudo-op
**
** There are two types of set processing.  The first has to do with setting
** values for items of name "L&xxxL" which represent the length of static
** strings processed by pass1.  For these we add an entry to the static
** string list.
**
** For other set's we try to replace symbol names by actual numeric values.
** For example, movm.l instructions look like
**
**      movm.l  &M%1,10(%fp)
**
** Later in the code there's a
**
**      set     M%1,<number>
**
** It's desirable to change the symbolic immediate to an absolute number.
** We do this in one fell swoop by
**      1) finding the RAND structure of the form &M%1, if it exists
**      2) creating a RAND structure of the form &<number>
**      3) replacing all instances of the first with the second
**
** This effectively changes all instances of &M%1 to &<number> in
** all instructions that use &M%1.
**
** We delete sets of the following types since they are no longer needed:
**
**	set L%...		pass1 static string lengths for in-line optimization
**	set	M%xxx...	movm mask which we compute ourselves now
**	set	FPM%...		fmovm mask which we compute ourselves
**	set	FPO%...		fmovm offset on stack frame no longer needed
*/

int
doset()
{
    char buf[50];               /* temporary buffer in which to do our work */
    RAND * oldrand;             /* pointer to original operand */
    RAND * newrand;             /* pointer to new operand */
    int len;                    /* length of new operand */
    NODE * node;                /* temporary node for scan */
    register RAND ** randptr;   /* pointer to RAND array in a node */
    void delsets();				/* delete uneeded sets routine */
	char buf2[50];              /* temporary buffer in which to do our work */
	RAND * oldrand2;            /* pointer to original operand */
	RAND * newrand2;            /* pointer to new operand */
	int len2;                   /* length of new operand */

	/* process static string lengths from pass 0 */
	if (strncmp("L%", lastnode->op1, 2)==0)
	  {	
		SSTR	*snode = (SSTR *) xalloc(sizeof(SSTR)); /* node to build */
	
		snode->stnext = sstrlist;	/* link list to node */
		sstrlist = snode;		/* point to new list */
		/* copy name without last "L" in name */
		(void) strcpy(snode->stname, lastnode->op1);
		snode->stname[strlen(snode->stname)-1] = 0;
		/* obtain the length */
		snode->stlen = strtol(lastnode->op2, (char **) NULL, 0);
		if (tempflag)
			PRINTF("static string %s, len %d, entry %lx\n", snode->stname,
							snode->stlen, snode);
		delsets(lastnode);		/* delete the node */
		return;					/* through with this set */
	  }

	/* process true frame size from pass 2 */
	if (strncmp("O%", lastnode->op1, 2)==0)
	  {	framesz = strtol(lastnode->op2, (char **) NULL, 0);
		if (peepflag)
			printf("frame size is: %d\n", framesz);
		delsets(lastnode);		/* remove this statement */
		return;
		}

    (void) sprintf(buf, "&%s", lastnode->op1);
                                /* make presumed immediate operand */
    (void) sprintf(buf2, "%s-%d(%%sp)", lastnode->op1, STACKSAFETY );
				/* make stack probe operand */
    oldrand = findrand(buf, false);
    oldrand2 = findrand(buf2, false);
    if (oldrand == RNULL && oldrand2 == RNULL)
    {  delsets(lastnode);      /* remove sets no longer needed */
       return;                 /* if operand doesn't exist, no point in this */
    }
 
    sprintf(buf, "&%s", lastnode->op2);
    len = strlen(buf);
                        /* make string of form &<number> */
	sprintf(buf2, "%d(%%sp)", atoi(lastnode->op2)-256);
	len2 = strlen(buf2);
						/* make string of form <number>(%sp) */
    
    /* make new RAND structure */

    newrand = saverand(buf, len, RT_IMMNUM, REG_NONE, true);
	newrand2 = saverand(buf2, len2, RT_OREG, REG_A7, true);
/* Find matching operands and replace them. */
    for( ALLN(node) )
    {
        randptr = node->userdata.ud_rand;

        /* check two operands */
		if (*randptr == RNULL)
			continue;

        if (*randptr == oldrand)
            setrand(node, 1, newrand);
		else if (*randptr == oldrand2)
			setrand(node, 1, newrand2);
                                /* set up new operand */
		if (*++randptr == RNULL)
			continue;
		if (*randptr == oldrand)
            setrand(node, 2, newrand);
    }
	    
    delsets(lastnode);		/* delete set no longer needed */
    return;
}


/* delsets -- delete sets no longer needed for assembly
**
** if set begings "L%", "M%", "FPM%", "FPO%", "O%", "F%" then delete it
**
*/

void
delsets(node)
NODE * node;
{
    if (   strncmp( "M%",   lastnode->op1, 2) == 0
	 	|| strncmp( "L%",	lastnode->op1, 2) == 0
	 	|| strncmp( "F%",	lastnode->op1, 2) == 0
	 	|| strncmp( "O%",	lastnode->op1, 2) == 0
	 	|| strncmp( "FPM%", lastnode->op1, 4) == 0
	 	|| strncmp( "FPO%", lastnode->op1, 4) == 0
       )
	{   NODE * t = lastnode;
	    
	    lastnode = lastnode->back;		/* backup lastnode ptr */
	    DELNODE(t);				/* delete this last node */
	    }
}
/* Define these parameters for handling switches */

/* This structure describes a switch.  BEWARE!!  It assumes that the
** pointer to this structure is equivalent to a pointer to the text
** string which appears first!
*/

typedef struct swlabtype                /* Structure to hold a label of form */
{                                       /* L%?<number> where ?=_ or is nil.  */
    boolean us_found;                   /* Does label have an underscore?    */
    int     lab_num;                    /* Decimal part of label             */
} SW_LAB;

static int  read_lab _PARMS((struct swlabtype *, char *, int ));

/* BEWARE if you change SW_LAB, since there are scanf/printf format strings
** that expect SW_LAB "lab_num" items to be sizeof(int).  If you change this 
** to long and sizeof(int) != sizeof(long), change the format strings to %ld.
*/

#define SWLABSIZE       16              /* presumed number of chars in label */

struct switchnode
{
    char sw_label[SWLABSIZE];           /* dispatch table label (arbitrary
                                        ** size
                                        */
    int sw_ncases;                      /* number of dispatch table entries */
    SW_LAB * sw_table;                  /* pointer to label information for each
                                        ** dispatch table entry
                                        */
    boolean sw_swbeg;                   /* true if switch came from swbeg
                                        ** pseudo-op, else false
                                        */
};




   /* Look for a label of the form "L%?<number>", where ? must be "_"
    ** or nothing at all.  <number> is decimal.
    */
static
boolean
read_lab(lab_ptr,lab_string,follow_char)
SW_LAB * lab_ptr;
char   * lab_string;
char     follow_char;
{
        int lablen;

    if (*scanp++ != 'L' || *scanp++ != '%')
        return(false);

    if (*scanp == '_')
        {
        lab_ptr->us_found = true;
        scanp++;
        }
    else
        lab_ptr->us_found = false;

    if (follow_char == ':')
        {
        if (sscanf(scanp, "%d:", &(lab_ptr->lab_num)) != 1)
            return(false);
        } 
    else if (follow_char == '-')
        {
        if (sscanf(scanp, "%d-", &(lab_ptr->lab_num)) != 1)
            return(false);
        }
    else
        if (sscanf(scanp, "%d", &(lab_ptr->lab_num)) != 1)
            return(false);
     
    if (lab_ptr->us_found)
        {       sprintf(lab_string,"L%%_%d", lab_ptr->lab_num);
                lablen = strlen(lab_string);
        }
    else
        {       sprintf(lab_string,"L%%%d", lab_ptr->lab_num);
                lablen = strlen(lab_string);
        }

    if (lablen >=SWLABSIZE)
        serr("Switch table label too long");
    return(true);               /* Valid label found */
}


/* doswbeg -- process "swbeg" pseudo-op
**
** This routine handles a switch derived from a "swbeg" pseudo-op.
** This is similar to the version above, but it has this form:
**
**      jmp     ...
**      swbeg   &<ncases>
** L%n:
**      short <table entry>
**
** We grab the number of cases and call swittab to build the table.
*/

static void
doswbeg()
{
    int ncases;                         /* number of switch cases */
    void swittab();
    
    if (sscanf(scanp, "\t&%d", &ncases) != 1)
        serr("Missing number of cases following swbeg");
    
    swittab(ncases, true);              /* build switch table for swbeg */
    return;
}
/* swittab -- build switch table
**
** This routine builds a switch table and stuffs it away as part of the
** last instruction node.
*/

static void
swittab(ncases, flag)
int ncases;                             /* number of switch table entries */
boolean flag;                           /* true for swbeg table, false for
                                        ** jmp table
                                        */
{
    SW_LAB label;                       /* integer part of table label */
    SW_LAB dummy;                       /* dummy integer for scanning */
    SW_LAB * swittab_a;                   /* allocated switch table */
    SW_LAB * curtab;                    /* pointer to current table entry */
    char label_name[SWLABSIZE];
    char dummy_label[SWLABSIZE];
    struct switchnode * sn;             /* pointer to allocated switch node */
    int atoi();

    if (switchflag)
        PRINTF("\t===== Switch with %d cases\n", ncases);

    /* Allocate switch node and large enough table. */

    sn = GETSTR(struct switchnode);
    swittab_a = (SW_LAB *) getspace((ncases) * sizeof(SW_LAB));
    sn->sw_ncases = ncases;             /* set number of cases */
    sn->sw_table = swittab_a;             /* point at switch table */
    sn->sw_swbeg = flag;                /* indicate table type */

    /* Now get label of table of offsets */

    if (! getline())
        serr("Premature end of file in switch (1)");
   
    if (!read_lab(&label,sn->sw_label,':'))
        serr("Missing switch label");
    
    if (switchflag)
        PRINTF("\t===== Offset table label:  %s\n", sn->sw_label);

/* Now loop through offset labels, storing each one in table. */

    curtab = swittab_a;                   /* place to put real label */

    while (ncases-- > 0)
    {
        if (! getline())
            serr("Premature EOF in switch (2)");

        if (strncmp(scanp,"\tshort\t",7) == 0)
            scanp += 7;                 /* Successfully read "\tshort\t" */
        else
            serr("Bad switch offset table entry");

        if (!read_lab(curtab++,label_name,'-')) /* Look for <label>-<label> */
            serr("Bad switch offset table entry");
        while (*scanp++ != '-') ;               /* Set to start of second lab */
        if (!read_lab(&dummy,dummy_label,'\t'))
            serr("Bad switch offset table entry");

        if (strcmp (sn->sw_label, dummy_label) != 0)
            serr("Second label not table label in switch");
        
        /* Add label to referenced labels
        ** table so the associated code will stay around.
        */

        addref(label_name, (unsigned) (strlen(label_name)+1));

        if (switchflag)
            PRINTF("\t===== Switch table entry %s\n", label_name);
    }

    lastnode->op2 = (char *) sn;        /* remember switch node in JMP node */
    setop(lastnode, getOP(SWITCH));     /* make it a SWITCH node */
    return;
}
/* outswitch -- reproduce switch code
**
** This routine regurgitates the information encoded in a SWITCH
** instruction node.
*/

void
outswitch(node)
NODE * node;                            /* pointer to the SWITCH node */
{
    struct switchnode * sn = (struct switchnode *) node->op2;
                                        /* point at encoded switch info */
    SW_LAB * swittab_e = sn->sw_table;    /* point at encoded labels */
    int ncases = sn->sw_ncases;         /* get # of cases */
    char * tablelabel = sn->sw_label;   /* get table label */

    /* output jmp instruction */

    PRINTF("\tjmp\t%s\n", node->op1);

    /* output swbeg and following number of cases if it's that type of table */

    if (sn->sw_swbeg)
        PRINTF("\tswbeg\t&%d\n", ncases);

    PRINTF("%s:\n", tablelabel);        /* output table label */

    /* now produce table entries */

    while (ncases-- > 0)
        {
        PRINTF("\tshort\tL%%");
        if (swittab_e[0].us_found)
            PRINTF("_");
        PRINTF("%d-%s\n", swittab_e[0].lab_num, tablelabel);
        swittab_e++;
        }
    return;
}
/* switsize -- get size of switch node
**
** This routine calculates the size of a switch node for the purpose
** of branch shortening.  The size is the size of the jump instruction
** (4 bytes) plus 2 * number of table entries.  If it's a swbeg table,
** add 4 for "swbeg" and "short n".
*/

int
switsize(node)
NODE * node;                            /* presumed switch node */
{
    struct switchnode * sn = (struct switchnode *) node->op2;
    int size = 4 + sn->sw_ncases * 2;   /* size of jump plus table entries */

    if (sn->sw_swbeg)                   /* if swbeg table, add for pseudo-op
                                        ** and number of cases
                                        */
        size += 4;
    return(size);
}
/* dodef -- process def line
**
** This routine processes a line containing symbol table information.
** We assume that "def" has been recognized at the beginning of a line,
** and that successive parts of the symbol table information are
** separated by ';'.
**
** We look for three kinds of things.  Since more than one can be
** present on a line, we must scan the whole line first before deciding
** what to do.  The cases are:
**
**      valdot  (actually "val ~").  The def has a value of the current
**              location.  This gets changed into an explicit label
**              reference (a hard label), rather than leave the debug
**              information in the middle of the instruction stream.
**      vallab  is a "val" which is a local label ("val L%xxx").
**              This gets thrown away.
**              These get generated for user-defined C labels.
**              The problem is that the label could disappear as part
**              of our processing, but the debug information would
**              still be around and result, ultimately, in an
**              undefined symbol in the loader.
**      sclmin1 is "scl -1".  This denotes the lexical end of a function.
**              We must see this when other debug information is present.
**              Otherwise we would flush two "def's" declaring a function
**              past the code of the first.  The assembler dislikes that.
**              A typical line containing this looks like:
**                      def name; val ~; scl -1; endef
**              which is the main reason we must scan the entire line.
**              (Otherwise we would declare this a "valdot".)
**      simple  is none of the above.  We just flush the line to output.
**
** An "Oh, by the way..." non-feature is that def's seem to have to be
** all in text, or at least ~bf and ~ef seem to need to be in the same
** section.  Otherwise the assembler gets unhappy.  To simplify matters
** we force all of them to be in text.
*/

static void
dodef()
{
    register char * cp = scanp;         /* start at current scan point */
    static char breaks[] = { ';', '\n', CC, '\0' };
                                        /* array of break characters */
    char * last;                        /* pointer to end of an argument */
    char * casebeg;                     /* beginning of interesting stuff */
    char * caseend;                     /* end of interesting stuff */
	char offset[20];
	char * dummy;
	long index = 0;
    enum deftype {simple, valdot, vallab, valoff, sclmin1} class = simple;
                                        /* assume simple case at first */

    char * strpbrk();                   /* find break character */


    do                                  /* until logical end of line */
    {
        /* skip over character after "def" (first) or ; (later) and
        ** spaces/tabs
        */

        do
            cp++;
        while ( *cp == '\t' || *cp == ' ' );

        /* now pointing at non-space/tab */

        last = strpbrk(cp, breaks);     /* find end of current arg. string */
            
        if (strncmp(cp, "val", 3) == 0) /* if see "val" */
        {
            cp += 4;                    /* skip "val", plus presumed following
                                        ** space/tab
                                        */
            while (*cp == '\t' || *cp == ' ')
                cp++;                   /* skipping other spaces/tabs */
            
            /* check for the two special cases we care about */

            if (*cp == '~' && last == (cp+1))
            {
                class = valdot;         /* val ~ case */
                casebeg = cp;           /* remember useful pointers for later */
                caseend = last;
            }
            else if (cp[0] == 'L' && cp[1] == '%')
                class = vallab;         /* val L%xxx case */
			else if (cp[0] == '-')
			{
				dummy = cp+1;
 				strncpy (offset, dummy, (strchr(dummy,'+') - (dummy)));
 				index = (digits (offset) / 4 ) - 1;
				class = valoff;
			}
        }
        else if (strncmp( cp, "scl", 3) == 0)
        {
            cp += 4;                    /* look past scl, following space/tab */

            while (*cp == '\t' || *cp == ' ')
                cp++;                   /* skip spaces/tabs */
            
            if (cp[0] == '-' && cp[1] == '1' && last == (cp+2))
            {
                class = sclmin1;        /* found case we want */
                break;                  /* takes precedence over others */
            }
        }
        else if (strncmp( cp, "typeq", 5) == 0)
		{
            cp += 6;           /* look past typeq, following space/tab */
            while (*cp == '\t' || *cp == ' ')
                cp++;                   /* skip spaces/tabs */
/***			if (cp[0] != '0')
				stkoffar [index] = 1;	*** bk: disabled: M4.1 breaks this **/
		}
        /* continue to next sub-part */
        cp = last;                      /* point at ;, \n, or comment char */
    } while (*cp != '\n' && *cp != CC);
/* Now do something, based on the type of def we found */

    switch ( (int)class )
    {
    case vallab:                        /* val L%xxx */
    case valoff:
        break;                          /* ignore line */
    
    case sclmin1:                       /* scl -1 */

/*      endfunc();  NEED FINAL SETS! d.w.t. /* do end of function processing */
        /*FALLTHRU*/

    case simple:                        /* ordinary case:  print line */
        outsection( TEXT, '0' );        /* force text section */
        prline();
        break;
    
    case valdot:                        /* val ~ */
        {
            char * lab = newlab();      /* get new label */

            /* output line with replacement for ~, namely new label */

            outsection( TEXT, '0' );    /* force text section first */
            PRINTF("%.*s%s%s", casebeg-curstart, curstart, lab, caseend);
            applbl( lab, 0 );           /* add new label to inst. stream */
            lastnode->userdata.ud_flags |= UD_HLAB;
                                        /* make it a hard label */
        }
        break;
    }   /* end switch */
    return;
}
/* Miscellaneous stubs */

dstats(){}
wrapup(){}

int digits (string)
char * string;
{
int length, result = 0, mult = 1;

	for (length = strlen(string); length != 0; length --)
	{
		result = result + (mult * (string[length - 1] - '0'));
		mult *= 10;
	}
	return (result);
}
