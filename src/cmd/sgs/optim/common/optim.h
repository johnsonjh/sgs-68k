#ident	"@(#)optim.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)optim.h	6.1		*/
/*	ID[] = "@(#) optim.h: 1.6 4/7/83";	*/

/*	machine independent include file for code improver */

#include <stdio.h>
#include <ctype.h>
#include "defs"
/** 		protos.h included as bottom of this file!
#include "protos.h"
**/

#ifndef MEMFCN	/* remove after 5.0 when memcpy is in all UNIX releases */
#define memcpy	strncpy
#endif

#ifndef MAXOPS
#define MAXOPS	1
#endif

/* booleans */

typedef int boolean;
#define false	0
#define true	1

#define FORWARD 0
#define BACKWARD 1

/* predefined "opcodes" for various nodes */

#define GHOST	0		/* temporary, to prevent linking node in */
#define TAIL	0		/* end of text list */
#define MISC	1		/* miscellaneous instruction */
#define FILTER	2		/* nodes to be filtered before optim */

#ifdef LIVEDEAD
extern void ldanal();
#if LIVEDEAD - 0 < 2
#undef LIVEDEAD
#define LIVEDEAD	16
#endif
#endif

/* structure of each text node */

typedef struct node {
	struct node *forw;	/* forward link */
	struct node *back;	/* backward link */
	char *ops[MAXOPS + 1];	/* opcode or label and operand field strings */
#ifdef IDVAL
	IDTYPE uniqid;		/* unique identification for this node */
#endif
	unsigned short op;	/* operation code */
#ifdef LIVEDEAD
	unsigned
#ifdef	M68040
	    nlive:32,
	    ndead:32;
#else
	    nlive:LIVEDEAD,	/* registers used by instruction */
	    ndead:LIVEDEAD;	/* registers set but not used by instruction */
#endif
#endif
#ifdef USERDATA
	USERTYPE userdata;	/* user-defined data for this node */
#endif
} NODE;

#define opcode	ops[0]
#define op1	ops[1]
#if MAXOPS > 1
#define op2	ops[2]
#if MAXOPS > 2
#define op3	ops[3]
#if MAXOPS > 3
#define op4	ops[4]
#if MAXOPS > 4
#define op5	ops[5]
#if MAXOPS > 5
#define op6	ops[6]
#endif
#endif
#endif
#endif
#endif

/* structure of non-branch text reference node */

typedef struct ref {
	char *lab;		/* label referenced */
	struct ref *nextref;	/* link to next ref */
} REF;

extern	REF	r0;

/* structure of static string length list element */

typedef struct sstr {
	char stname[20];	/* name of static string */
	short stlen;		/* length of the string */
	struct sstr *stnext; /* next list entry */
} SSTR;

/* The following are for function folding */

#define NUMFOLD	17			/* maximum size of function to fold */
#define RECURFOLD (NUMFOLD*2)	/* maximum recursive function folding size */
#define LABELMARK	0x1		/* internal text marker for unique label gen */

typedef struct call {
	struct call	*next;		/* next called function in list */
	char	name[1];		/* name of function called */
	} CALL;

typedef struct proc {
	struct proc *next;		/* pointer to next function */
	char	*name;			/* name of this function */
	int		usecnt;			/* recursive folding count */
	int		length;			/* instruction count in function */
	struct	call *calllist;	/* functions called from this function */
	REG		funclive;		/* live registers used by function */
	char	foldable;		/* foldable function */
	char	recurse;		/* only fold for self recursive use */
	char	*insert;		/* text storage insertion point */
	char	*text;			/* foldable function storage area */
	} PROC;

/* The following are for flow graph access */

typedef struct block {
    struct block *next; /* pointer to textually next block */
    struct block *nextl;    /* pointer to next executed block if no br */
	struct block *nextr;	/* pointer to next executed block if br */
	struct block *ltest;	/* for loop termination tests */
	NODE	*firstn;		/* first text node of block */
	NODE	*lastn;			/* last text node of block */
	short	index;	    /* block index for debugging purposes */
    short length;       /* number of instructions in block */
    short indeg;        /* number of text references */
    short marked;       /* marker for various things */
	int		startbias;	/* block start stack bias */
	int		localbias;	/* block start local stack bias */
} BLOCK;

extern BLOCK b0;      /* header for block list */

#define ALLB(b, s)  b = (s); b != NULL; b = b->next

/* externals */

extern int genericpass;		/* used for GENERIC live/dead processing */
extern int Pass2;			/* folding pass in progress */
extern PROC *Curproc;		/* function in progress */
extern PROC *Headproc;		/* function list */
extern framesz;				/* optimized frame size from c1 */
extern SSTR *sstrlist;		/* list of static string lengths */
extern FLAGS nattrmask;		/* next node optimizer mask */
extern boolean leafproc;	/* this procedure calls no other */
extern REG funcsrc;		/* all registers sourced in a procedure */
extern REG funclive;		/* all live registers seen in a procedure */
extern boolean adrframe;	/* address of object on frame taken */
extern NODE n0;			/* header node of text list */
extern NODE ntail;		/* trailer node of text list */
extern NODE *lastnode;		/* pointer to last node on text list */
extern REF *lastref;		/* pointer to last label reference */
extern boolean ininline;        /* processing inserted user assembly code */
extern boolean involatile;      /* processing inserted user volatile code */

/* global flags from compiler pass 2 */
extern boolean pass2optf;		/* "mov.l is floating-point move" indicator */
extern REG frtnregs;		/* function return live registers */

/* global debug and control flags */
extern int fflag;		/* display function folding info */
extern int dflag;		/* display live-dead info */
extern int sflag;		/* statistics (on stderr) */
extern int ndisc;		/* # of instructions discarded */
extern int ninst;		/* total # of instructions */
extern int npass;		/* # of times through this function */
extern int fnum;		/* # of this function */
extern boolean lineflag;	/* -0 output input line */
extern boolean labflag;		/* -1 output label data */
extern boolean opflag;		/* -2 output opcode data */
extern boolean randflag;	/* -3 output operand data */
extern boolean switchflag;	/* -4 output switch info */
extern boolean statflag;	/* -5 output register stats */
extern boolean vtprintflag;	/* -6 output value trace data */
extern boolean peepflag;	/* -7 output peephole info */
extern boolean PCflag;		/* -8 output PC values in blocks */
extern boolean tempflag;	/* -9 output temporary reg assign logic */
extern boolean frameflag;	/* -A output frame removal info */
extern boolean shortflag;	/* -j supress branch shortening */
extern boolean abortflag;	/* -k abort process on error */

extern BLOCK *lblhash();
extern NODE *Saveop();
extern boolean same(), sameaddr(), optim(), usetemps();
extern char *getspace(), *strcpy(), *xalloc(), *regchar();

#if __STDC__ || _POSIX_SOURCE
extern void *memcpy(), *memset();
#else
extern char *memcpy(), *memset();
#endif

extern void addref(), fatal(), filter(), init(), prtext(), xfree();
extern void chgregrand(), rmframe(), dopass2opt(), preprocfp(), fillrand();
extern void clrhash(), writefunc(), expanmovm();
extern int redmov(), eaopt(), regsavesz();

/* user-supplied functions or macros */

#ifndef getp
extern char *getp();
#endif
#ifndef newlab
extern char *newlab();
#endif

#define FINDLBL(l)	lblhash((l), (BLOCK *)NULL)
#define saveop(opn, str, len, op) \
	    (void) Saveop((opn), (str), (unsigned)(len), (op))
#define addtail(p)		/* superfluous */
#define appinst()		/* superfluous */
#define appmisc(str, len)	saveop(0, (str), (len), MISC)
#define appfl(str, len)		saveop(0, (str), (len), FILTER)
#define applbl(str, len) \
	(setlab(Saveop(0, (str), (unsigned)(len), MISC)), --ninst)
#define ALLN(p)			p = n0.forw; p != &ntail; p = p->forw
#define PRINTF			(void) printf
#define FPRINTF			(void) fprintf
#define SPRINTF			(void) sprintf
#define PUTCHAR(c)		(void) putchar(c)
#define DELNODE(p)		((p)->back->forw = (p)->forw, \
				    (p)->forw->back = (p)->back)
#define APPNODE(p, q)		PUTNODE((p), (q), back, forw)
#define INSNODE(p, q)		PUTNODE((p), (q), forw, back)
#define PUTNODE(p, q, f, b)	((p)->f = (q), (p)->b = (q)->b, \
				    (q)->b = (q)->b->f = (p))
#define GETSTR(type)		(type *) getspace(sizeof(type))
#define COPY(str, len)	((len) != 0 ? memcpy(getspace(len), str, (int)(len)) : str)

/* function prototypes */
#include "protos.h"
