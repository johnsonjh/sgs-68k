#ident	"@(#)mfile2.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)mfile2.h	7.3		*/
/*
static	char	SCCSID[] = "@(#) mfile2.h:	2.1 83/07/08";
*/

# include "macdefs.h"
# include "manifest.h"
# include "sgs.h"
/** # include "c1proto.h"	** included at bottom of this file */

/*  code generation goals, used in table entries */

# define FOREFF 01 /* compute for effects only */
# define INREG 02 /* compute into a register */
# define FORCC 04 /* compute for condition codes only */

	/* types of operators and shapes; optab.tyop, optab.ltype optab.rtype */
# define TCHAR		01
# define TUCHAR		0100
# define TSHORT		02
# define TUSHORT	0200
# define TLONG		010
# define TULONG		01000

/* make INT match SHORT, LONG, or both depending on NOSHORT, NOLONG */
# define urTINT		04
# define urTUNSIGNED	0400

# ifdef NOSHORT
	/* shorts == ints */
#	ifdef	NOLONG
		/* longs == ints */
#		define TINT		(urTINT|TSHORT|TLONG)
#		define TUNSIGNED	(urTUNSIGNED|TUSHORT|TULONG)
#	else
		/* longs != ints, and shorts == ints */
#		define TINT		(urTINT|TSHORT)
#		define TUNSIGNED	(urTUNSIGNED|TUSHORT)
#	endif
# else
	/* shorts != ints */
#	ifdef NOLONG
		/* longs == ints */
#		define TINT		(urTINT|TLONG)
#		define TUNSIGNED	(urTUNSIGNED|TULONG)
#	else
		/* longs, shorts, and ints all distinct */
#		define TINT		urTINT
#		define TUNSIGNED	urTUNSIGNED
#	endif
# endif

# define TDOUBLE	040
/* handle floats: match doubles if ONEFLOAT */
# ifdef ONEFLOAT
	/* floats are the same as doubles in this case */
#	define TFLOAT		(TDOUBLE|020)
# else
#	define TFLOAT		020
# endif
# define FP_TYPE(x) ( ( x == TFLOAT ) || ( x == TDOUBLE ) )

# define TSTRUCT      02000   /* structure or union */
# define TPOINT	      04000
# define TPOINT2      010000
# define TVOID	      020000
# define TANY	      077777  /* matches anything within reason */
	/* 0100000 (SPTYPE) is reserved for matches */

	/* reclamation cookies */

# define RLEFT 01
# define RRIGHT 02
# define RESC1 04
# define RESC2 010
# define RESC3 020
# define RESCC 040
# define RNOP 0100   /* DANGER: can cause loops.. */
# define RNULL 0200    /* clobber result */

	/* needs */

# define NREG 01
# define NCOUNT 017
# define NMASK 01777
# define LSHARE 020 /* share left register */
# define RSHARE 040 /* share right register */
# define NPAIR 0100 /* allocate registers in pairs */
# define LPREF 0200 /* prefer left register, if any */
# define RPREF 0400 /* prefer right register, if any */
# define ALLREGS 01000 /* all registers used, none available */
# define NEEDNUM 12 /* shift count to user defined need number */
# define USERNEEDS 0170000 /* user defined needs mask */
/* NOTE: FP881 no longer used for dynamic fp instruction generation.
**       ifdef FORT no longer occurs.
**       Sty takes only one digit for userneed and or's them together.
**       For this reason only 4 distinct needs can be defined.
*/
# ifdef M68881
# define FP881  1	/* M68881 required user need */
# define R_TEMP 2	/* left side of assign must be a register temp */
# ifdef FORT
# define INTR_881 4	/* f77 intrinsic function computed by M68881 user need*/
# endif
# endif
 
# define CC_AREG 8	/* This flag is used to indicate that a template
			 * will correctly set the condition codes when an
			 * address register the destination. Its stricktly
			 * used to allow condition code setting for cases 
			 * where an address register is used to hold an INT
			 */
# define EMU_FP 4       /* This flag is used to indicate taht we
                         * want to generate calls to a library
                         * subroutine instead of generating the
                         * instructions fintrz or fscale
                         * WARNING: this option conflicts with
                         * ifdef FORT... so you can't have both
			 */


	/* register allocation */

extern struct busyregs
{
	int areg,
	    dreg;
#if M68881
	int fpreg;
#endif
} busy[];


# define INFINITY 10000

extern NODE node[];

typedef struct shape SHAPE;

	/* special shapes (enforced by making costs high) */
# define SPECIAL	0170000
# define SPTYPE		0100000
# define STMASK 	 070000
# define SVAL		 010000	/* positive constant value */
# define SNVAL 		 020000	/* negative constant value */
# define SRANGE0 	 030000 /* positive range [0, 2**mm - 1] */
# define SSRANGE 	 040000	/* signed range [-2**mm, 2**mm] */
# define NACON		 050000	/* nameless contant */
# define SUSER		 070000	/* user's cost function */
# define SVMASK		  07777	/* shape value mask */

extern struct shape {
	int	op;	/* operator */
	SHAPE	*sl;	/* pointers to left- and right-hand shape */
	SHAPE	*sr;
	int	sh;	/* flags for special shape and type matches */
	int	sc;	/* cost of applying this shape */
} shapes[];
extern SHAPE	*pshape[];

typedef struct optab OPTAB;

struct optab {
	int	op;	/* operator */
	int	tyop;	/* type of the operator node itself */
	OPTAB	*nextop;
	SHAPE	**lshape;	/* pointer to pointer to left shape */
	int	ltype;		/* type of left shape */
	SHAPE	**rshape;	/* likewise for the right shape */
	int	rtype;
	int	needs;
	int	rewrite;
	char	*cstring;
	int	cost;
	int	lcount;		/* number of times left used */
	int	rcount;		/* number of times right used */
	int	stinline;	/* line number in stin file */
};

extern OPTAB
	*match(),
	*ophead[],
	table[];

extern NODE resc[];

extern int tmpoff;
extern int maxboff;
extern int maxtemp;
extern int maxarg;
extern int ftnno;
extern int sideff;
#ifdef m68k
extern int align_val;	/* Data alignment is variable on the m68k. */
			/* Environment var. DBLALIGN=YES/NO sets it. */
extern int align_struc;	/* Separate variable for alignment within
			** structures due to compatibility problems with
			** pre-existing file header blocks and object files;
			** environment var. STALIGN=YES/NO sets it.
			*/
#endif

extern int is_m68881;   /* TRUE if system has M68881 coporocessor */
extern int is_m68881u;  /* set if D0/D1 and F0 get function results */

extern NODE
	*eread(),
	*talloc(),
	*ind2type(),
# ifdef m68k
	*cptree(),
# endif
	*tcopy(),
	*getadr(),
	*getlr();

# define getlt(p,t) ((t)==LTYPE?p:p->in.left)
# define getlo(p,o) (getlt(p,optype(o)))
# define getl(p) (getlo(p,p->tn.op))
# define getrt(p,t) ((t)==BITYPE?p->in.right:p)
# define getro(p,o) (getrt(p,optype(o)))
# define getr(p) (getro(p,p->tn.op))

extern CONSZ rdin();

extern char *rnames[];

extern int lineno;
extern char ftitle[];
extern int fldshf, fldsz;
extern int lflag, udebug, e2debug, odebug, rdebug, radebug, sdebug;
extern int fast;  /* try to make the compiler run faster */

#ifndef callchk
#define callchk(x) allchk()
#endif
#ifndef callreg
	/* try to number so results returned in 0 */
#define callreg(x) 0
#endif
#ifndef szty
	/* it would be nice if number of registers to hold type t were 1 */
	/* on most machines, this will be overridden in macdefs.h */
# define szty(t) 1
#endif


#ifndef PUTCHAR
# define PUTCHAR(x) putchar(x)
#endif

# define CEFF (NRGS+1)
# define CTEMP (NRGS+2)
# define CCC (NRGS+3)
# define SCC (NRGS+4)
# define ISCC (NRGS+5)
	/* the assumption is that registers 0 through NRGS-1 are scratch */
	/* the others are real */
	/* some are used for args and temps, etc... */

# define istreg(r) ((r)<NRGS)

typedef struct inst INST;

struct inst {
	NODE	*p;
	OPTAB	*q;
	int	goal;
};

# define NINS 1200	/* ken#6-to 400; bk-to 1200 */
extern INST inst[NINS];
extern nins;

	/* definitions of strategies legal for ops */

# define STORE 01
# define LTOR 02
# define RTOL 04

	/* shape matching */
# ifndef NSHP
# define NSHP 25
# endif

typedef SHAPE	*SHTABLE[2][NSHP];

extern SHTABLE	sha;

#ifdef m68k
/* Effective address descriptor, used when building an */
/* indirect operand address			       */
typedef struct
{
	int	basereg,	/* base register */
		inxreg,		/* index register */
		inxlong,	/* 1=>index register is long */
		scale,		/* index scale */
		dblindir,	/* double indirect flag */
		inxpost,	/* index reg is post mem indirect */
		basepost,	/* base reg is post mem indirect */
		autoincr,	/* not zero ==> post increment */
		autodecr;	/* not zero ==> predecrement */
	NODE    *idisp,		/* inner displacement */
		*odisp;		/* outer displacement */
} EADESC;

#define IS_BYTE(i)	( ( i > -129 ) && ( i < 128 ) )
#define IS_WORD(i)	( ( i > -32769 ) && ( i < 32768 ) )

#define TRUE 1
#define NONE -1		/* indicates no register specified */
#define AREGOFF 8	/* address register number offset */
#if m68k
#define AREGMSK 0xff00	/* address register mask */
#endif

#ifdef M68881
#define FPREGOFF 16	/* offset in word of fp reg number */
#define FPREGMSK 0xff0000	/* fp register mask */
#endif

#define IN_INDIR 0x01
#define IN_SCL   0x02
#define IN_UAND  0x04
#endif


/* function prototypes */
# include "c1protos.h"
