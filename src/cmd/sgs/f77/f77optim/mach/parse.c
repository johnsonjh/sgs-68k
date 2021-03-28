/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) parse.c: version 25.1 created on 12/2/91 at 17:41:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)parse.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)f77/f77optim/mach:parse.c	25.1"

/* MOT:parse.c	7.1 Version, Creation date 9/18/85"*/

static int insub=0;
/*	MC68000 Optimizer - File parse.c	*/

/* This module contains the main driving routines for the code improver
** (aka optimizer).  The main machine-dependent program control takes
** place here.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "optim.h"
#include "debug.h"
/* #include "defs" is implicit */

/* Line buffer for use while scanning input. */

static char line[BUFSIZ];
char * scanp;				/* global scanning pointer -- points
					** at current position in input
					*/
static char * curstart;			/* current line's virtual starting
					** position
					*/

/* State variables */

static boolean infunction;		/* true if currently within function
					** bounds
					*/
static boolean ldcurrent;		/* live/dead information is correct */
static void (* postline)();		/* pointer to post-line processor */
#define	NOPOST	((void (*)()) 0)	/* null pointer for same */

/* The parser keeps track of what section the input and output streams
** are in.  Redundant sections changes are eliminated.
*/

static OPCODE isection = TEXT;		/* current section being input,
					** starts as TEXT 0
					*/
static char isecnum = '0';		/* input section number */

static OPCODE osection = TEXT;		/* current section being output:
					** starts as TEXT 0
					*/
static char osecnum = '0';		/* output section number is zero */

IDTYPE lineno = IDVAL;		/* current line number */

/* Debugging flags */

static boolean lineflag = false;	/* output input line:  -0 */
static boolean labflag = false;		/* output label data:  -1 */
static boolean opflag = false;		/* output op code data:  -2 */
static boolean randflag = false;	/* output operand data:  -3 */
static boolean switchflag = false;	/* output switch info:  -4 */
static boolean statflag = false;	/* output register stats:  -5 */
static boolean vtprintflag = false;	/* output value trace info -6 */
static boolean peepflag = false;	/* output peephole info -7 */
static boolean PCflag = false;		/* output PC values in blocks -8 */
static boolean shortflag = false;	/* suppress branch shortening
					** if true -j */
static boolean abortflag = false;	/* abort process on error:  -k */
static boolean vtraceflag = false;	/* suppress value trace if true -v */

/* yyinit -- initialize the world */

void
yyinit(flags)
char * flags;
{
    infunction = false;			/* not in function */

	TRACE(yyinit);
    while (*flags != 0)			/* look for local flags */
    {
	switch( *flags++ )
	{
	case '0':	lineflag = true; break;
	case '1':	labflag = true; break;
	case '2':	opflag = true; break;
	case '3':	randflag = true; break;
	case '4':	switchflag = true; break;
	case '5':	statflag = true; break;
	case '6':	vtprintflag = true; break;
	case '7':	peepflag = true; break;
	case '8':	PCflag = true; break;
	case 'j':	shortflag = true; break;
	case 'k':	abortflag = true; break;
	case 'v':	vtraceflag = true; break;
	default:	break;		/* ignore unknowns */
	}
    }

    return;
}
/* getline -- read new input line, do appropriate set-up */

boolean					/* returns false on EOF, else true */
getline()
{
	TRACE(getline);
    if (fgets(line,BUFSIZ,stdin) == NULL)
    {
	if (feof(stdin))
	    return(false);		/* hit EOF */
	else
	    oerr("Error reading input");
    }

    if (lineflag)
	PRINTF("Line:  %s", line);
    curstart = scanp = line;		/* set up scan pointer */
    return(true);			/* successful read */
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
    void addmemrefs();

    /* loop through input lines */

	TRACE(yylex);
    while (getline())
    {
	postline = NOPOST;		/* set line post-processor */
	sline();			/* process the line */
	if (postline != NOPOST)
	    postline();			/* do post-processor */
    }

    
    {
   	register NODE * node;
    	for( ALLN(node) ){
		addmemrefs(node);
    	}
    }
    if (infunction)			/* if still in function, optimize */
	endfunc();

    if (statflag)
	pstats();
    if (ferror(stdout))			/* check for output error */
	oerr("Error writing output");
    return;
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
    int templen;			/* local temporary length */
    char * cp;				/* temporary pointer */
    void doswitch();
    void doswbeg();
    void doset();
    void dodef();
    void setsection();
    int strlen();
    int sscanf();

    if(eflag)printf("#doop(ptr[ %s ],len[ %ul ])\n",ptr,len);
    if (opflag)
    {
	PRINTF("\t%s:  %s\n", (opptr->op_flags & OF_PSEUDO) ? "Special op" :
		"Normal op", opptr->op_name);
	PRINTF("\t------- Op code is %d\n",opptr->op_code );
    }

    switch( opptr->op_code )
    {
    case TEXT:
	if (*(cp = ptr+len) == '\t' || *cp == ' ')
	    setsection(TEXT, cp[1]);	/* set new section */
	else
	    setsection(TEXT, '0');	/* assume default TEXT section */
	return(true);
    
    case DATA:
	if (*(cp = ptr+len) == '\t' || *cp == ' ')
	    setsection(DATA, cp[1]);	/* set new section */
	else
	    setsection(DATA, '0');	/* assume default DATA section */
	return(true);

    case LN:				/* line number information;
					** get the actual number
					*/
	if (sscanf( scanp, " %d", &lineno) != 1)
	    serr("Bad ln");
	return(true);			/* done handling line */

    case DEF:				/* handle symbol info (def) */
	dodef();			/* handle the def */
	return(true);			/* all processing done by dodef */

    case SET:
	templen = strlen(curstart);	/* length of current line */
	appfl(curstart, templen);	/* append whole line as filter node */
	lastnode->userdata.ud_op=(OP *)0;/* hanna */
	lastnode->ops[0][templen-1] = '\0';
					/* clobber newline with null */
	postline = doset;		/* use this post-processor */
	return(false);			/* still want to process operands */

    case SWBEG:
	doswbeg();			/* handle special switch pseudo-op */
	return(true);			/* all done this line (and more) */

	/* otherwise, fall through on odd cases */

    default:
	if (   isection == DATA
	    || (opptr->op_flags & OF_PSEUDO) != 0
	    || (isection == TEXT && ! infunction)
	    )
	{
	    prline();
	    return(true);
	}
	break;				/* for everything else, get out */
    }
    saveop(0, opptr->op_name, 0, (int) (opptr->op_code) );
					/* add new node */
    setop( lastnode, opptr );		/* fill in the gory details */
    setrand( lastnode, 1, RNULL );	/* set null values for operands */
    setrand( lastnode, 2, RNULL );
					/* done processing if no source or
					** destination operands
					*/
    lastnode->uniqid = lineno;		/* save current line number */
    lineno = IDVAL;			/* set line number to "none" */
    return(opptr->op_nsrc == 0 && opptr->op_dnum == NODEST);
}
/* setsection -- set current program section
**
** This routine sets a new input program section.
*/

static void
setsection(op, sectnum)
OPCODE op;				/* section number:  TEXT or DATA */
char sectnum;				/* presumed section number char */
{
	TRACE(setsection);
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
OPCODE sect;				/* desired section */
char sectnum;				/* desired section number */
{
	TRACE(outsection);
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

    outsection( isection, isecnum );	/* make sure stuff goes in proper
					** section
					*/
    PRINTF("%s", curstart);		/* output entire current line from
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
    boolean makehard = false;		/* true if label should be hard */

    if(eflag)printf("# dolab( ptr[ %.*s ], len[ %d ]\n",len,ptr,len);
    if (labflag)
	PRINTF("\tSaw label:  %.*s\n", len, ptr);

    if (isection == DATA)		/* label in data */
    {
	outsection( isection, isecnum ); /* get it in proper section */
	PRINTF("%.*s:\n", len, ptr);	/* output label */
	curstart = ptr + len;		/* change apparent line's starting
					** point, removing label
					*/
	return;
    }

    /* check for start of function */

    if (   isection == TEXT
	&& (   len <= 3
	    || !( ptr[0] == 'L' && ptr[1] == '%' )
	    )
	)
    {
	if (labflag)
	    PRINTF("\t--------------- START OF FUNCTION ----------------\n");
	if (infunction && !insub)
	    endfunc();			/* do end of function processing */
	begfunc();			/* now start new one */
	makehard = true;		/* make this label hard */
    }
    if ( ptr[0] == 'L' && ptr[1] == '%' ){
	 if (!strncmp("%beg",&ptr[2],4)){
		makehard = true;
		insub++; 
	 }
	 else if (!strncmp("%end",&ptr[2],4)){
		makehard = true;
		insub=0; 
	 }
    }

    r = saverand(ptr, len, RT_LABEL, REG_NONE, true);
    rprint(r,1);
    applbl( r->rname, 0 );		/* append as label node */
    if (makehard)			/* if want to make a hard label */
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
	TRACE(dorand);
    setrand( lastnode, n, r);
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
	PRINTF("\t\t\tRegisters:  %#.8o, Operand type:  %d\n", regs, rtype);
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

    TRACE(begfunc);
    infunction = true;			/* say we're in a function */
    prehash();				/* initialize operand hash table */
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
    void optim();
    void vtrace();
    void bspan();
    void outsection();

	TRACE(endfunc);
    ldcurrent = false;			/* live/dead info not current */
    filter();				/* flush filter nodes */

/* value trace and peep-hole improvements are merged */

    if (! vtraceflag)			/* trace if not suppressed */
	vtrace(vtprintflag, peepflag);	/* print trace info if requested */

    optim();				/* optimize current function */
    ldcurrent = true;			/* optim() builds new live/dead info */
    if (! shortflag)
	bspan(PCflag);			/* shorten span dependent branches */

    outsection( TEXT, '0' );		/* get instructions in text */
    prtext();				/* print out everything */
    infunction = false;			/* no longer in function */
    init();				/* initialize mip for next function */
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

	TRACE(prehash);
    clrhash();				/* clear operand hash table */

    /* add common and special operands to table */

    (void) saverand("%d0", 3, RT_REG, REG_D0, true);
    (void) saverand("%a0", 3, RT_REG, REG_A0, true);
#ifdef M68881
    (void) saverand("%fp0", 4, RT_REG, REG_FP0, true);
#endif

#ifdef	LIVEDEAD
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

    return;
}
/* output optimizer scanning error */

void exit();

void
serr(s)
char * s;				/* error string to print */
{
    char * cp;
    fprintf(stderr,"Fatal Optimizer error:  %s\n%s",s,line);
    for (cp = line; cp < scanp; cp++)
	if (isgraph(*cp))
	    *cp = ' ';
    *scanp = '\0';
    FPRINTF(stderr,"%s^\n",line);

    if (abortflag)
	abort();			/* kill process */
    else
	exit(1);
/*NOTREACHED*/
}


/* output optimizer error in non-input-scanning situation */

void
oerr(s)
char * s;				/* error string */
{
    FPRINTF(stderr,"Fatal optimizer error:  %s\n", s);
    if (abortflag)
	abort();
    else
	exit(2);
/*NOTREACHED*/
}
/* Collect count of frequency of register reference (lexically) */

static int counts[NREGS];			/* word per register */

static void
note(regs)
REG regs;
{
    int i;
    long temp = regs;

	TRACE(note);
    for (i = 0; temp != 0; i++, temp >>= 1)	/* count each bit */
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
	    FPRINTF(stderr,"\t%d\t	%#.8o\n", counts[i], 1 << i);
}
/* doset -- handle set pseudo-op
**
** We process set's to replace symbol names by actual numeric values.
** For example, movm.l instructions look like
**
**	movm.l	&M%1,10(%fp)
**
** Later in the code there's a
**
**	set	M%1,<number>
**
** It's desirable to change the symbolic immediate to an absolute number.
** We do this in one fell swoop by
**	1) finding the RAND structure of the form &M%1, if it exists
**	2) creating a RAND structure of the form &<number>
**	3) replacing all instances of the first with the second
**
** This effectively changes all instances of &M%1 to &<number> in
** all instructions that use &M%1.
*/

void
doset()
{
    void addmemrefs();
    char buf[50];		/* temporary buffer in which to do our work */
    RAND * oldrand;		/* pointer to original operand */
    RAND * newrand;		/* pointer to new operand */
    int len;			/* length of new operand */
    NODE * node;		/* temporary node for scan */
    register RAND ** randptr;	/* pointer to RAND array in a node */

	TRACE(doset);
    (void) sprintf(buf, "&%s", lastnode->op1);
				/* make presumed immediate operand */
    if ((oldrand = findrand(buf, false)) == RNULL)
	return;			/* if operand doesn't exist, no point in
				** doing transformation
				*/
    len = sprintf(buf, "&%s", lastnode->op2);
				/* make string of form &<number> */
    
    /* make new RAND structure */

    newrand = saverand(buf, len, RT_IMMNUM, REG_NONE, true);
/* Find matching operands and replace them. */
    for( ALLN(node) )
    {
	randptr = node->userdata.ud_rand;

	/* check two operands */

	if (*randptr == oldrand)
	    setrand(node, 1, newrand);
				/* set up new operand */
	if (*++randptr == oldrand)
	    setrand(node, 2, newrand);
    }
    return;
}
/* Define these parameters for handling switches */

/* This structure describes a switch.  BEWARE!!  It assumes that the
** pointer to this structure is equivalent to a pointer to the text
** string which appears first!
*/

typedef struct swlabtype		/* Structure to hold a label of form */
{					/* L%?<number> where ?=_ or is nil.  */
    boolean us_found;			/* Does label have an underscore?    */
    int     lab_num;			/* Decimal part of label             */
} SW_LAB;

/* BEWARE if you change SW_LAB, since there are scanf/printf format strings
** that expect SW_LAB "lab_num" items to be sizeof(int).  If you change this 
** to long and sizeof(int) != sizeof(long), change the format strings to %ld.
*/

#define	SWLABSIZE	16		/* presumed number of chars in label */

struct switchnode
{
    char sw_label[SWLABSIZE];		/* dispatch table label (arbitrary
					** size
					*/
    int sw_ncases;			/* number of dispatch table entries */
    SW_LAB * sw_table;			/* pointer to label information for each
					** dispatch table entry
					*/
    boolean sw_swbeg;			/* true if switch came from swbeg
					** pseudo-op, else false
					*/
};




   /* Look for a label of the form "L%?<number>", where ? must be "_"
    ** or nothing at all.  <number> is decimal.
    */
boolean
read_lab(lab_ptr,lab_string,follow_char)
SW_LAB * lab_ptr;
char   * lab_string;
char     follow_char;
{
	int lablen;

	TRACE(read_lab);
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
	lablen = sprintf(lab_string,"L%%_%d", lab_ptr->lab_num);
    else
	lablen = sprintf(lab_string,"L%%%d", lab_ptr->lab_num);

    if (lablen >=SWLABSIZE)
	serr("Switch table label too long");
    return(true);		/* Valid label found */
}


/* doswbeg -- process "swbeg" pseudo-op
**
** This routine handles a switch derived from a "swbeg" pseudo-op.
** This is similar to the version above, but it has this form:
**
**	jmp	...
**	swbeg	&<ncases>
** L%n:
**	short <table entry>
**
** We grab the number of cases and call swittab to build the table.
*/

static void
doswbeg()
{
    int ncases;				/* number of switch cases */
    void swittab();
    int sscanf();
    
	TRACE(doswbeg);
    if (sscanf(scanp, "\t&%d", &ncases) != 1)
	serr("Missing number of cases following swbeg");
    
    swittab(ncases, true);		/* build switch table for swbeg */
    return;
}
/* swittab -- build switch table
**
** This routine builds a switch table and stuffs it away as part of the
** last instruction node.
*/

static void
swittab(ncases, flag)
int ncases;				/* number of switch table entries */
boolean flag;				/* true for swbeg table, false for
					** jmp table
					*/
{
    SW_LAB label;			/* integer part of table label */
    SW_LAB dummy;			/* dummy integer for scanning */
    SW_LAB * swittab;			/* allocated switch table */
    SW_LAB * curtab;			/* pointer to current table entry */
    char label_name[SWLABSIZE];
    char dummy_label[SWLABSIZE];
    struct switchnode * sn;		/* pointer to allocated switch node */
    int atoi();

	TRACE(swittab);
    if (switchflag)
	PRINTF("\t===== Switch with %d cases\n", ncases);

    /* Allocate switch node and large enough table. */

    sn = GETSTR(struct switchnode);
    swittab = (SW_LAB *) getspace((ncases) * sizeof(SW_LAB));
    sn->sw_ncases = ncases;		/* set number of cases */
    sn->sw_table = swittab;		/* point at switch table */
    sn->sw_swbeg = flag;		/* indicate table type */

    /* Now get label of table of offsets */

    if (! getline())
	serr("Premature end of file in switch (1)");
   
    if (!read_lab(&label,sn->sw_label,':'))
	serr("Missing switch label");
    
    if (switchflag)
	PRINTF("\t===== Offset table label:  %s\n", sn->sw_label);

/* Now loop through offset labels, storing each one in table. */

    curtab = swittab;			/* place to put real label */

    while (ncases-- > 0)
    {
	if (! getline())
	    serr("Premature EOF in switch (2)");

	if (strncmp(scanp,"\tshort\t",7) == 0)
	    scanp += 7;			/* Successfully read "\tshort\t" */
	else
	    serr("Bad switch offset table entry");

	if (!read_lab(curtab++,label_name,'-'))	/* Look for <label>-<label> */
	    serr("Bad switch offset table entry");
	while (*scanp++ != '-') ;		/* Set to start of second lab */
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

    lastnode->op2 = (char *) sn;	/* remember switch node in JMP node */
    setop(lastnode, getOP(SWITCH));	/* make it a SWITCH node */
    return;
}
/* outswitch -- reproduce switch code
**
** This routine regurgitates the information encoded in a SWITCH
** instruction node.
*/

void
outswitch(node)
NODE * node;				/* pointer to the SWITCH node */
{
    struct switchnode * sn = (struct switchnode *) node->op2;
					/* point at encoded switch info */
    SW_LAB * swittab = sn->sw_table;	/* point at encoded labels */
    int ncases = sn->sw_ncases;		/* get # of cases */
    char * tablelabel = sn->sw_label;	/* get table label */

	TRACE(outswitch);
    /* output jmp instruction */

    PRINTF("\tjmp\t%s\n", node->op1);

    /* output swbeg and following number of cases if it's that type of table */

    if (sn->sw_swbeg)
	PRINTF("\tswbeg\t&%d\n", ncases);

    PRINTF("%s:\n", tablelabel);	/* output table label */

    /* now produce table entries */

    while (ncases-- > 0)
	{
	PRINTF("\tshort\tL%%");
	if (swittab[0].us_found)
	    PRINTF("_");
        PRINTF("%d-%s\n", swittab[0].lab_num, tablelabel);
	swittab++;
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
NODE * node;				/* presumed switch node */
{
    struct switchnode * sn = (struct switchnode *) node->op2;
    int size = 4 + sn->sw_ncases * 2;	/* size of jump plus table entries */

	TRACE(switsize);
    if (sn->sw_swbeg)			/* if swbeg table, add for pseudo-op
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
**	valdot	(actually "val ~").  The def has a value of the current
**		location.  This gets changed into an explicit label
**		reference (a hard label), rather than leave the debug
**		information in the middle of the instruction stream.
**	vallab	is a "val" which is a local label ("val L%xxx").
**		This gets thrown away.
**		These get generated for user-defined C labels.
**		The problem is that the label could disappear as part
**		of our processing, but the debug information would
**		still be around and result, ultimately, in an
**		undefined symbol in the loader.
**	sclmin1	is "scl -1".  This denotes the lexical end of a function.
**		We must see this when other debug information is present.
**		Otherwise we would flush two "def's" declaring a function
**		past the code of the first.  The assembler dislikes that.
**		A typical line containing this looks like:
**			def name; val ~; scl -1; endef
**		which is the main reason we must scan the entire line.
**		(Otherwise we would declare this a "valdot".)
**	simple	is none of the above.  We just flush the line to output.
**
** An "Oh, by the way..." non-feature is that def's seem to have to be
** all in text, or at least ~bf and ~ef seem to need to be in the same
** section.  Otherwise the assembler gets unhappy.  To simplify matters
** we force all of them to be in text.
*/

static void
dodef()
{
    register char * cp = scanp;		/* start at current scan point */
    static char breaks[] = { ';', '\n', CC, '\0' };
					/* array of break characters */
    char * last;			/* pointer to end of an argument */
    char * casebeg;			/* beginning of interesting stuff */
    char * caseend;			/* end of interesting stuff */
    enum deftype {simple, valdot, vallab, sclmin1} class = simple;
					/* assume simple case at first */

    char * strpbrk();			/* find break character */


	TRACE(dodef);
    do					/* until logical end of line */
    {
	/* skip over character after "def" (first) or ; (later) and
	** spaces/tabs
	*/

	do
	    cp++;
	while ( *cp == '\t' || *cp == ' ' );

	/* now pointing at non-space/tab */

	last = strpbrk(cp, breaks);	/* find end of current arg. string */
	    
	if (strncmp(cp, "val", 3) == 0) /* if see "val" */
	{
	    cp += 4;			/* skip "val", plus presumed following
					** space/tab
					*/
	    while (*cp == '\t' || *cp == ' ')
		cp++;			/* skipping other spaces/tabs */
	    
	    /* check for the two special cases we care about */

	    if (*cp == '~' && last == (cp+1))
	    {
		class = valdot;		/* val ~ case */
		casebeg = cp;		/* remember useful pointers for later */
		caseend = last;
	    }
	    else if (cp[0] == 'L' && cp[1] == '%')
		class = vallab;		/* val L%xxx case */
	}
	else if (strncmp( cp, "scl", 3) == 0)
	{
	    cp += 4;			/* look past scl, following space/tab */

	    while (*cp == '\t' || *cp == ' ')
		cp++;			/* skip spaces/tabs */
	    
	    if (cp[0] == '-' && cp[1] == '1' && last == (cp+2))
	    {
		class = sclmin1;	/* found case we want */
		break;			/* takes precedence over others */
	    }
	}
	/* continue to next sub-part */
	cp = last;			/* point at ;, \n, or comment char */
    } while (*cp != '\n' && *cp != CC);
/* Now do something, based on the type of def we found */

    switch ( class )
    {
    case vallab:			/* val L%xxx */
	break;				/* ignore line */
    
    case sclmin1:			/* scl -1 */
	endfunc();			/* do end of function processing */
	/*FALLTHRU*/
    case simple:			/* ordinary case:  print line */
	outsection( TEXT, '0' );	/* force text section */
	prline();
	break;
    
    case valdot:			/* val ~ */
	{
	    char * lab = newlab();	/* get new label */
		
	    /* output line with replacement for ~, namely new label */

	    outsection( TEXT, '0' );	/* force text section first */
	    PRINTF("%.*s%s%s", casebeg-curstart, curstart, lab, caseend);
	    applbl( lab, 0 );		/* add new label to inst. stream */
	    lastnode->userdata.ud_flags |= UD_HLAB;
					/* make it a hard label */
	}
	break;
    }	/* end switch */
    return;
}
/* live/dead support routines */

#ifdef	LIVEDEAD

void
dold()					/* get up-to-date live/dead data */
{
    void ldanal();

	TRACE(dold);
    if (! ldcurrent)
	ldanal();			/* get current info */
    ldcurrent = true;
    return;
}

void
killld()				/* designate live/dead info as not
					** current
					*/
{
	TRACE(killld);
    ldcurrent = false;
    return;
}

#endif	/* def LIVEDEAD */
/* Miscellaneous stubs */

dstats(){}
wrapup(){}

/*
 * Sat Jun 21 15:44:18 PDT 1986
 * hanna
 * addmemrefs looks at the operands in a line and adds them to 
 * the reference list if not already there.
 * this is to prevent deletion of text labels that have been referenced
 * by something other than a branch.
 */
void
addmemrefs(node)
    register NODE * node;
    {		
	register RAND * rptr;
	register char *ptr ;
	register int i=0;
	/* don't add data section labels.
	 * and don't add labels in branch operations
	 * they seem to be taken care of elsewhere
	 */
	if (isection != TEXT || isbr(node) || islabel(node))
		return;

	for (i=1; i<= MAXOPS && (ptr=node->ops[i]) != NULL; i++){
		rptr = node->userdata.ud_rand[i-1];
		if (*ptr == 'L' && (rptr->rtype == RT_LABEL ||
		    rptr->rtype == RT_MEM || rptr->rtype == RT_SMEM)){
				addref(ptr,strlen(ptr)+1);
		}else {
			++ptr;
			if((rptr->rtype == RT_IMMED) && *ptr == 'L'){
				addref(ptr,strlen(ptr)+1);
			}
		}
		
	}
}
