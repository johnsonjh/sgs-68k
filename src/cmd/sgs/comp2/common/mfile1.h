#ident	"@(#)mfile1.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.3	89/09/11 17:30:19 barryk
	Added SLAB symbol table flag for label namespace 

    1.2	89/06/26 15:33:40 hossein
	changes related to "const" type qualifier for POSIX.

    1.1	89/06/08 00:38:30 root
	date and time created 89/06/08 00:38:30 by root

*/
/*	@(#)mfile1.h	7.2		*/
/*
static	char	SCCSID[] = "@(#) mfile1.h:	2.1 83/07/08";
*/


# include "macdefs.h"
# include "manifest.h"
# include "sgs.h"
# include "c0protos.h"

/*	storage classes  */
# define SNULL 0
# define AUTO 1
# define EXTERN 2
# define STATIC 3
# define REGISTER 4
# define EXTDEF 5
# define LABEL 6
# define ULABEL 7
# define MOS 8
# define PARAM 9
# define STNAME 10
# define MOU 11
# define UNAME 12
# define TYPEDEF 13
# define FORTRAN 14
# define ENAME 15
# define MOE 16
# define UFORTRAN 17
# define USTATIC 18
#ifdef  IN_LINE
# define INLINE 19
#endif

	/* field size is ORed in */
# define FIELD 0100
# define FLDSIZ 077
# ifndef NODBG
extern char *scnames();
# endif

/*	location counters */
# define PROG 0
# define ADATA 1
# define DATA 2
# define ISTRNG 3
# define STRNG 4
# define TEXT2 5				/* bk: for global "const" data */
# define STRWRT 6				/* bk: for writable string literals */

/* symbol table flags */
# define SMOS 01
# define SHIDDEN 02
# define SHIDES 04
/* # define SSET 010 */			/* bk: not used */
# define SREG 010 	/*bk*/
# define SREF 020
# define SNONUNIQ 040
# define STAG 0100
# define SLAB 0200 	/*bk#8*/

typedef long OFFSZ;

struct symtab {
	char * sname;
	TWORD stype;  /* type word */

	char sclass;  /* storage class */
	char slevel;  /* scope level */
	char sflags;  /* flags for set, use, hidden, mos, etc. */
	TWORD stypeq; /* flags for const & volatile */
	int offset;  /* offset or value */
	short dimoff; /* offset into the dimension table */
	short sizoff; /* offset into the size table */
	short suse;  /* line number of last use of the variable */
	unsigned short sproto;	/* for function prototyping */
	struct symtab *scopelink;	/* chain of symbols at same level */
	int labels;
};


struct sw {
	CONSZ sval;
	int slab;
};

extern struct sw swtab[];
extern struct sw *swp;
extern int swx;

#ifdef m68k
extern int align_val;	/* Data alignment is variable on the M68020. */
			/* Environment var. DBLALIGN=YES/NO sets it. */
extern int align_struc;	/* Separate variable for alignment within
			** structures due to compatibility problems with
			** pre-existing file header blocks and object files;
			** environment var. STALIGN=YES/NO sets it.
			*/
# define FP_TYPE(x) ( ( x == FLOAT ) || ( x == DOUBLE ) )
#endif

extern int is_m68881;   /* TRUE if system has M68881 coporocessor */
extern int is_m68881u;  /* and returns fnct values in both D0/D1 & F0 */

extern int ftnno;
extern int blevel;
extern int instruct, stwart, strucdef;

extern TWORD typequal;
extern int constid;
extern int indcl;

extern int lineno, nerrors;
typedef union {
	int intval;
	NODE * nodep;
} YYSTYPE;
extern YYSTYPE yylval;

extern CONSZ lastcon;
extern CONSZ ccast();
extern double dcon;

extern char ftitle[];
extern struct symtab stab[];
extern int curftn;
extern int strftn;
#ifdef MC68K
extern TWORD ftntype;
#endif
extern char *locnames[];
extern int curloc;
extern int curdim;
extern int curclass;

extern int dimtab[];
extern int paramstk[];
extern int paramno;
extern int argstk[];
extern int argsoff[];
extern TWORD argty[];
extern int argno;
extern int autooff, argoff, strucoff;
extern int regvar;
extern int nextrvar;
extern int brkflag;
extern char yytext[];

extern int strflg;

extern OFFSZ inoff;

extern int reached;

/*	tunnel to buildtree for name id's */

extern int idname;

extern NODE node[];
extern NODE *lastfree;

extern int warn_level;

/* various labels */
extern int brklab;
extern int contlab;
extern int flostat;
extern int retlab;
extern int retstat;
extern int asavbc[], *psavbc;

/*	flags used in structures/unions */

# define SEENAME 01
# define INSTRUCT 02
# define INUNION 04
# define FUNNYNAME 010
# define TAGNAME 020

/*	flags used in the (elementary) flow analysis ... */

# define FBRK 02
# define FCONT 04
# define FDEF 010
# define FLOOP 020

/*	flags used for return status */

# define RETVAL 1
# define NRETVAL 2

/*	used to mark a constant with no name field */

# define NONAME 040000

	/* mark an offset which is undefined */

# define NOOFFSET (-10201)

/*	declarations of various functions */

extern NODE
	*aadjust(),
	*bcon(),
	*bdty(),
	*block(),
	*bpsize(),
	*buildtree(),
	*clocal(),
	*conval(),
	*convert(),
#ifdef m68k
	*cptree(),
#endif
	*dclstruct(),
	*doszof(),
	*getstr(),
	*makety(),
	*mkty(),
	*oconvert(),
	*offcon(),
	*optim(),
	*pconvert(),
	*ptmatch(),
	*pvconvert(),
	*rstruct(),
	*sconvert(),
	*strargs(),
	*stref(),
	*talloc(),
	*tymatch(),
	*tymerge(),
	*unconvert(),
	*xicolon();

OFFSZ	tsize(),
	psize();

TWORD	types(),
	ctype();

char *exname(), *exdcon();

# define checkst(x)

# ifdef SDB
# include "sdb.h"
# endif

/* type that is equivalent to pointers in size */
# ifndef PTRTYPE
# define PTRTYPE INT
# endif
