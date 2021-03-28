#ident	"@(#)manifest.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.4	89/08/25 11:41:44 barryk
	(bk#5) IS_NOT_SCALAR macro

    1.3	89/07/13 09:53:03 hossein
	Increased the PARAMSZ from 150 to 300 to handle larger enums & structs

    1.2	89/06/26 15:29:54 hossein
	changes related to "const" type qualifier for POSIX.

    1.1	89/06/08 00:38:28 root
	date and time created 89/06/08 00:38:28 by root

*/

/*	@(#)manifest.h	7.1		*/
/*
static	char	SCCSID[] = "@(#) manifest.h:	2.1 83/07/08";
*/
#ifndef _MANIFEST_H
#define _MANIFEST_H
# include <stdio.h>

	/* macro definitions for common cases of type collapsing */
# ifdef NOSHORT
# define SZSHORT SZINT
# define ALSHORT ALINT
# endif

# ifdef NOLONG
# define SZLONG SZINT
# define ALLONG ALINT
# endif

# ifdef NOFLOAT
# define SZFLOAT SZLONG
# define SZDOUBLE SZLONG
# define ALFLOAT ALLONG
# define ALDOUBLE ALLONG
# endif

# ifdef ONEFLOAT
# define SZFLOAT SZDOUBLE
# define ALFLOAT ALDOUBLE
# endif

/* define default assembly language comment starter */

# ifndef COMMENTSTR
# define COMMENTSTR	"#"
#endif


/*	manifest constant file for the lex/yacc interface */

# define ERROR 1
# define NAME 2
# define STRING 3
# define ICON 4
# define FCON 5
# define PLUS 6
# define PLUSASG 7
# define MINUS 9
# define MINUSASG 10
# define MUL 12
# define MULASG 13
# define STAR (UNARY MUL)
# define AND 15
# define ANDASG 16
# define OR 18
# define ORASG 19
# define ER 20
# define ERASG 21
# define QUEST 22
# define COLON 23
# define ANDAND 24
# define OROR 25

/*	special interfaces for yacc alone */
/*	These serve as abbreviations of 2 or more ops:
	ASOP	=, = ops
	RELOP	LE,LT,GE,GT
	EQUOP	EQ,NE
	DIVOP	DIV,MOD
	SHIFTOP	LS,RS
	ICOP	INCR,DECR
	UNOP	NOT,COMPL
	STROP	DOT,STREF

	*/
# define ASOP 26
# define RELOP 27
# define EQUOP 28
# define DIVOP 29
# define DIVOPASG 30
# define SHIFTOP 31
# define SHIFTOPASG 32
# define INCOP 33
# define UNOP 34
# define STROP 35

/*	reserved words, etc */
# define TYPE 36
# define TYPEQ 37
# define CLASS 38
# define STRUCT 39
# define RETURN 40
# define GOTO 41
# define IF 42
# define ELSE 43
# define SWITCH 44
# define BREAK 45
# define CONTINUE 46
# define WHILE 47
# define DO 48
# define FOR 49
# define DEFAULT 50
# define CASE 51
# define SIZEOF 52
# define ENUM 53

/*	little symbols, etc. */
/*	namely,

	LP	(
	RP	)

	LC	{
	RC	}

	LB	[
	RB	]

	CM	,
	SM	;

	*/

# define LP 54
# define RP 55
# define LC 56
# define RC 57
# define LB 58
# define RB 59
# define CM 60
# define SM 61
# define ASSIGN 62
# define ELLIPSIS 63		/* function prototype terminator */
	/* ASM returned only by yylex, and totally eaten by yyparse */
# define ASM 64
# define ARGTYPE 65

/*	END OF YACC */

/*	left over tree building operators */
# define COMOP 66
# define DIV 67
# define MOD 69
# define LS 71
# define RS 73
# define DOT 75
# define STREF 76
# define CALL 77
# define FORTCALL 80
# define NOT 83
# define COMPL 84
# define INCR 85
# define DECR 86
# define EQ 87
# define NE 88
# define LE 89
# define LT 90
# define GE 91
# define GT 92
# define MI 93          /* only appears in GENBR/GENUBR nodes */
# define PL 94          /* only appears in GENBR/GENUBR nodes */
# define ULE 95
# define ULT 96
# define UGE 97
# define UGT 98
# define SETBIT 99
# define TESTBIT 100
# define RESETBIT 101
# define ARS 102
# define REG 103
# define TEMP 104
# define CCODES 105
# define FREE 106
# define STASG 107
# define STARG 108
# define STCALL 109

/******* gap of one here, if you need it ****/

/*	some conversion operators */
# define FLD 113
# define CONV 114
# define PMUL 115
# define PDIV 116

/*	special node operators, used for special contexts */
/* # define FORCE 117 */
# define GENLAB 117
# define CBRANCH 118
# define GENBR 119
# define CMP 120
# define GENUBR 121
# define GENSCC 122
# define INIT 123
# define CAST 124
# define FUNARG 125
# define VAUTO 126
# define VPARAM 127
# define RNODE 128
# define SNODE 129
# define QNODE 130
/*	a whole bunch of ops, done with unary; I don't need to tackle prec */
# define UOP0  131
# define UOP1  132
# define UOP2  133
# define UOP3  134
# define UOP4  135
# define UOP5  136
# define UOP6  137
# define UOP7  138
# define UOP8  139
# define UOP9  140
#ifdef IN_LINE
# define INCALL 141             /* beware of UNARY INCALL == 143! */
/* define UNARY INCALL 143  */
#endif
#ifdef M68881
# define INTCALL1 144           /* MC68881 in-line */
# define INTCALL2 145           /* MC68881 in-line matherr support */
# define INTCALL3 146           /* MC68881 in-line matherr sin/cos/tan */
#endif
#ifdef ITREES
/*	pcc2 ops */
# define PROLOG 147
# define EPILOG 148
# define SWBEG  149
# define SWCASE 150
# define SWDEF  151
# define SWEND  152
#endif
/*	ops used in optimizer */
#ifdef HLOPTIM
# define EXPRNO 153   /* beginning of an expression */
# define SBLOCK 154   /* start of a block */
# define EBLOCK 155   /* end of a block */
#ifdef ASM_COMMENT
# define ASMNO  156   /* in-line asm stmt */
#endif
#endif
			/* hidden static allocator node, handles initialization */
			/* of local aggregates (first pass only) */
# define HIDASG 157
# define DEFTYP	158		/** "def" symbol table line to be passed from **/
						/**	coptim to c1 -- treat same as ASMNO **/
 
			/* prototype check nodes for first pass */
# define PROTOTYP 159
# define PROTOCHK 160

/* WARNING: COPTIM ASSUMES MANY IS HIGHEST OP NUMBER!!! */
# define MANY  161

	/* DSIZE is the size of the dope array:  highest OP # + 1 */
	/* WARNING: DON'T CHANGE THIS; SEE NOTE ABOVE #define MANY */
# define DSIZE MANY+1

/*	node types */
# define LTYPE 02
# define UTYPE 04
# define BITYPE 010

/*	type names, used in symbol table building */
# define TNULL 0
# define FARG 1
# define CHAR 2
# define SHORT 3
# define INT 4
# define LONG 5
# define FLOAT 6
# define DOUBLE 7
# define STRTY 8
# define UNIONTY 9
# define ENUMTY 10
# define MOETY 11
# define UCHAR 12
# define USHORT 13
# define UNSIGNED 14
# define ULONG 15
# define VOID 16
# define UNDEF 17

# define SIGNED 18	/* bk: practically speaking, this "type" is default */
			/* and we needn't bother to store it in symbol entry */

# define ASG 1+
# define UNARY 2+
# define NOASG (-1)+
# define NOUNARY (-2)+

/*	various flags */
# define NOLAB (-1)

/* type modifiers */

# define PTR  040
# define FTN  0100
# define ARY  0140

/* type qualifiers */

# define CONST		001
# define VOLATILE	002

/* type packing constants */

# define MTMASK 03
# define BTMASK 037
# define BTSHIFT 5 
# define TSHIFT 2
# define TMASK (MTMASK<<BTSHIFT)
# define TMASK1 (MTMASK<<(BTSHIFT+TSHIFT))
# define TMASK2  (TMASK||MTMASK)

/*	macros	*/

# ifndef BITMASK
	/* beware 1's complement */
# define BITMASK(n) (((n)==SZLONG)?-1L:((1L<<(n))-1))
# endif
# define ONEBIT(n) (1L<<(n))
# define MODTYPE(x,y) x = (x&(~BTMASK))|y  /* set basic type of x to y */
# define BTYPE(x)  (x&BTMASK)   /* basic type of x */
# define ISUNSIGNED(x) ((x)<=ULONG&&(x)>=UCHAR)
# define UNSIGNABLE(x) ((x)<=LONG&&(x)>=CHAR)
# define ENUNSIGN(x) ((x)+(UNSIGNED-INT))
# define DEUNSIGN(x) ((x)+(INT-UNSIGNED))
# define ISPTR(x) ((x&TMASK)==PTR)
# define ISFTN(x)  ((x&TMASK)==FTN)  /* is x a function type */
# define ISARY(x)   ((x&TMASK)==ARY)   /* is x an array type */
# define INCREF(x) (((x&~BTMASK)<<TSHIFT)|PTR|(x&BTMASK))
# define DECREF(x) (((x>>TSHIFT)&~BTMASK)|(x&BTMASK))

/* bk: type qualifier stuff */
# define DECQUAL(x) ( ((x>>TSHIFT)&~BTMASK) | ((x&TMASK)>>BTSHIFT) )
			/* Check if type category is const (see ANSI 3.1.2.5) */
# define CONST_QUALIFIED(type,qual) ( (qual) & (CONST << type_category(type)) )

# define SETOFF(x,y)   if( (x)%(y) != 0 ) x = ( ((x)/(y) + 1) * (y))
		/* advance x to a multiple of y */
# define NOFIT(x,y,z)   ( ((x)%(z) + (y)) > (z) )
	/* can y bits be added to x without overflowing z */
	/* pack and unpack field descriptors (size and offset) */
# define PKFIELD(s,o) (((o)<<6)|(s))
# define UPKFSZ(v)  ((v)&077)
# define UPKFOFF(v) ((v)>>6)

/*	operator information */

# define TYFLG 016
# define ASGFLG 01
# define LOGFLG 020

# define SIMPFLG 040
# define COMMFLG 0100
# define DIVFLG 0200
# define FLOFLG 0400
# define LTYFLG 01000
# define CALLFLG 02000
# define MULFLG 04000
# define SHFFLG 010000
# define ASGOPFLG 020000

# define SPFLG 040000

#define optype(o) (dope[o]&TYFLG)
#define asgop(o) (dope[o]&ASGFLG)
#define asgbinop(o) (dope[o]&ASGOPFLG)
#define logop(o) (dope[o]&LOGFLG)
#define callop(o) (dope[o]&CALLFLG)

/*	table sizes	*/

/* bk: bumped BCSZ from 100 to 200 */
# define BCSZ 200 /* size of the table to save break and continue labels */
# define MAXNEST BCSZ	/* maximum nesting depth for scopestack() */
# define DIMTABSZ 9000 /* size of the dimension/size table -- ken#6 && ken#7 */
# define PARAMSZ 300 /* size of the parameter stack */
# define ARGSZ 50 /* size of the argument stack */
# ifndef TREESZ
# ifndef HLOPTIM
# ifndef FORT
# define TREESZ 2000 /* space for building parse tree: bk, 700->2000 */
# else
# define TREESZ 1000
# endif
# else
# define TREESZ 20000 /* parse tree space for the optimizer */
# endif
# endif
# define SWITSZ 520 /* size of switch table */

	char		*hash();
	char		*savestr();
	char		*tstr();
	extern int	tstrused;
	extern char	*tstrbuf[];
	extern char	**curtstr;
#define freestr()	( curtstr = tstrbuf, tstrused = 0 )

#	define NCHNAM 8  /* number of characters in a truncated name */

/*	common defined variables */

extern int nerrors;  /* number of errors seen so far */
extern int Arix[];	/* Arix-specific flags */
enum {				/* indices into Arix[] */
	ARIX_PROBE, 	/* c1: generate tst.b stack probe */
	ARIX_RMFMOVM, 	/* c1: don't generate any fmovm instructions!! */
};

typedef union ndu NODE;
typedef unsigned int TWORD;
typedef long CONSZ;  /* size in which constants are converted */

	/* default is byte addressing */
	/* BITOOR(x) converts bit width x into address offset */
# ifndef BITOOR
# define BITOOR(x) ((x)/SZCHAR)
# endif

# ifdef SDB
# define STABS
# endif

# define NIL (NODE *)0

extern int dope[];  /* a vector containing operator information */
extern char *opst[];  /* a vector containing names for ops */

#define USE_AREG 16	/* This flag is placed in lval of REG nodes by coptim
					 * to indicate that the code generator is to use an 
					 * address register instead of a data register. This
					 * allows for ints and longs to be promoted even when
					 * all the data registers are full.
					 */


# define NCOSTS (NRGS+4)

/* define local frame variable address spread for coptim variable resolution */
#define  COPTPAD 0x40000

#ifdef HLOPTIM

typedef int VNA;

typedef struct Tbasicblock BB;
typedef struct Tbasicblock *BBPTR;
typedef struct Tbblink BBLINK;

struct Tbblink{		/* link between basic blocks */
   BBPTR bbptr;  	/* pointer to a basic block */
   BBLINK *next;	/* pointer to next element in list */
   };

struct Tbasicblock {	/* basic block structure */
   NODE *codetree;	/* pointer to code tree associated 
			** with basic block */
   BBLINK *succs;	/* pointer to the list of basic blocks
			** which are successors to the current 
			** basic block */
   BBLINK *preds;	/* pointer to the list of basic blocks 
			** which are predecessors to the current 
			** basic block */
   BBPTR next;		/* pointer to the next physical basic block */
   BBLINK *dom;		/* pointer to the list of basic blocks 
			** which this basic block dominates */
   VNA *vnaptr;		/* pointer to value number array 
			** associated with this basic block */
   BBPTR header;	/* pointer to header of containing 
			** loop, if any */
   int dfn;		/* depth first number of basic block */
   char flags;		/* attributes of basic block - header of 
			** a loop, member of a loop, exit from 
			** a loop, ... */
   char weight;		/* loop depth */
   int id;		/* identification number - order in which created
			** used for printing out the flow graph */
   };


struct Tlabel {		/* entry in list of basicblocks */
   int number;		/* number of the label */
   BBPTR bb;		/* pointer to basic block defined by 
			** this label.   NOTE - if NIL then 
			** block has not been seen */
   int references;	/* number of times it was referenced. 
			** If 0 then basic block is dead unless 
			** it has an immediate predecessor. */
   BBLINK list;		/* until basic block is defined this 
			** list will contain the basic blocks 
			** which reference it. */
   };

#endif

union flt_dbl
{
	float  f;
	double d;
	struct { long l,l2;	 } lg;
	struct { short slo, shi; } sh;
};

typedef union flt_dbl FLTDBL;

	/* in one-pass operation, define the tree nodes */

union ndu {

	struct {
		int op;
		int goal;
		TWORD type;
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int cst[NCOSTS];
		char * name;
		char pad[NCHNAM-sizeof(char *)];	/* padding hack! */
		NODE *left;
		NODE *right;
		int proto;					/* bk: index of ftn prototype list */
	}in;	/* interior node */
	
	struct {
		int op;
		int goal;
		TWORD type;
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int cst[NCOSTS];
		char * name;
		char pad[NCHNAM-sizeof(char *)];	/* padding hack! */
		CONSZ lval;
		int rval;
	}tn;	/* terminal node */
	
	struct {
		int op;
		int goal;
		TWORD type;
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int cst[NCOSTS];
		char * name;
		char pad[NCHNAM-sizeof(char *)];	/* padding hack! */
		double dval;
	}fp;	/* back end FCON node */

	struct {
		int op;
		int goal;
		TWORD type;
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int cst[NCOSTS];
		int label;  /* for use with branching */
		int lop;  /* the opcode being branched on */
	}bn;	/* branch node */

	struct {
		int op;
		int goal;
		TWORD type;
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int cst[NCOSTS];
		int stsize;  /* sizes of structure objects */
		short stalign;  /* alignment of structure objects */
		short argsize;  /* size of argument list for call */
	}stn;	/* structure node */

	struct {
		int op;
		int goal;
		TWORD type;
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int cdim;
		int csiz;
	}fn;	/* front node */
	
	struct {
		/* this structure is used when a floating point constant
		   is being computed */
		int op;
		int goal;
		TWORD type;
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int cdim;
		int csiz;
		double dval;
	}fpn;	/* floating point node */

#ifdef HLOPTIM

	struct {
		/* this structure is used by the optimizer */
		int op;		/* operator */	
		NODE *parent;	/* pointer to parent of current node */
		TWORD type;	/* type of node */
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int cdim;
		int csiz;
		double dval;	/* floating point constant */
		int sourceno;	/* source number for node */
		ENTRY *valueno;	/* value number for node */
		char *name;	/* name of external or label */
		int *udduptr;	/* pointer to UD/DU information - not used */
		NODE *left;	/* pointer to left child */
		NODE *right;	/* pointer to right child */
		char flags;	/* set of attributes */
	} opn;	/* optimizer node */ 

	struct {
		/* used by an EXPRNO, ASMNO, or SBLOCK node in the optimizer */
		int op;		/* operator */
		BBPTR bb; 	/* pointer to containing basic block */
		char *astring;  /* save the asm string */
		TWORD typeq;
#ifdef FLTDBL64
		int fltdummy;
#endif
		int filler1;    /* fillers keep the node fields aligned */
		int filler2;
		int newflag;    /* flag indicating new LCSE inserted */
		int filler4;
		int filler5;
		int filler6;
		NODE *pred;	/* pointer to predecessor */
		NODE *succ;	/* pointer to successor */
		NODE *left;	/* pointer to expression for EXPRNO */
		int rval;	/* line number or register mask */
	} stmt; /* this node heads a statement in the optimizer */ 

#endif

};

/*
 *	BEGIN: ported from VAX pcc to make two pass version
 */

# ifdef TWOPASS

# ifndef EXPR
# define EXPR '.'
# endif
# ifndef BBEG
# define BBEG '['
# endif
# ifndef BEND
# define BEND ']'
# endif

# endif

/*
 *	END: ported from VAX pcc to make two pass version
 */
#endif	/* _MANIFEST_H */
