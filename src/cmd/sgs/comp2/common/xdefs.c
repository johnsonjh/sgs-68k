#ident	"@(#)xdefs.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
     1.2	89/06/26 15:41:36 hossein
 	changes related to "const" type qualifier for POSIX.
 
     1.1	89/06/08 00:38:41 root
 	date and time created 89/06/08 00:38:41 by root
 
*/
/*	@(#)xdefs.c	7.1		*/
static	char	SCCSID[] = "@(#) xdefs.c:	2.1 83/07/08";

# include "mfile1.h"

/*	communication between lexical routines	*/

int	lineno;		/* line number of the input file */

CONSZ lastcon;  /* the last constant read by the lexical analyzer */
double dcon;   /* the last double read by the lexical analyzer */


/*	symbol table maintainence */

struct symtab stab[SYMTSZ+1];  /* one extra slot for scratch */

int	curftn;  /* "current" function */
int	strftn;  /* 1 if current function returns struct or union */
#ifdef MC68K
unsigned int	ftntype; /* type of function. Has had FTN indicator DECREF'd */
#endif
int	ftnno;  /* "current" function number */
int	curloc;		  /* current location counter value */

TWORD typequal;	/** type qualifier **/
int	curclass,	  /* current storage class */

	constid,	/* HOSS */
	indcl,		/* HOSS */

instruct,	/* "in structure" flag */
strucdef,       /* has level number >0 for nested structures */
stwart,		/* for accessing names which are structure members or names */
blevel,		/* block level: 0 for extern, 1 for ftn args, >=2 inside function */
curdim;		/* current offset into the dimension table */

int	dimtab[ DIMTABSZ ];

int	paramstk[ PARAMSZ ];  /* holds symtab indices of function parameters */
int	paramno;	  /* the number of parameters */
int	argsoff[ ARGSZ ];  /* the offsets for the arguments */
int	argno;		/* the number of arguments */
int	argstk[ ARGSZ ];	/* the symtab indices  of arguments */
TWORD	argty[ ARGSZ ];		/* the types of the arguments */
int	autooff,	/* the next unused automatic offset */
argoff,	/* the next unused argument offset */
strucoff;	/*  the next structure offset position */
int	regvar;		/* the next free register for register variables */
int	nextrvar;	/* the next allocated reg (set by cisreg) */
OFFSZ	inoff;		/* offset of external element being initialized */
int	brkflag = 0;	/* complain about break statements not reached */

struct sw swtab[SWITSZ];  /* table for cases within a switch */
struct sw *swp;  /* pointer to next free entry in swtab */
int swx;  /* index of beginning of cases for current switch */

/* debugging flag */
int xdebug = 0;

int strflg;  /* if on, strings are to be treated as lists */

int reached;	/* true if statement can be reached... */

int idname;	/* tunnel to buildtree for name id's */


extern NODE node[];

int warn_level = 0;		/* warning level */

int brklab;
int contlab;
int flostat;
int retlab = NOLAB;
int retstat;

/* save array for break, continue labels, and flostat */

int asavbc[BCSZ];
int *psavbc = asavbc ;

/* stack of scope chains */

struct symtab *scopestack[MAXNEST];

int Arix[2];		/* Arix-specific flags */

static char *ccnames[] = 
{
	 /* names of storage classes */
	"SNULL",
	"AUTO",
	"EXTERN",
	"STATIC",
	"REGISTER",
	"EXTDEF",
	"LABEL",
	"ULABEL",
	"MOS",
	"PARAM",
	"STNAME",
	"MOU",
	"UNAME",
	"TYPEDEF",
	"FORTRAN",
	"ENAME",
	"MOE",
	"UFORTRAN",
	"USTATIC",
#ifdef IN_LINE
	"INLINE",
#endif
};

char *
scnames( c )
register c; 
{
	/* return the name for storage class c */
	static char buf[12];
	if( c&FIELD )
	{
		sprintf( buf, "FIELD[%d]", c&FLDSIZ );
		return( buf );
	}
	return( ccnames[c] );
}
