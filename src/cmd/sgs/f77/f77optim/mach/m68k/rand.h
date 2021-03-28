/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rand.h: version 25.1 created on 12/2/91 at 17:41:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rand.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	MOT:rand.h	7.1		*/

#ident	"@(#)f77/f77optim/mach/m68k:rand.h	25.1"

/* rand.h */
/*	rand.h:	2.1 83/07/08	*/

/*	MC680X0 Optimizer	*/

/* Definitions for operand data */

/* Operand types */

#define	RT_REG		1	/* register direct:	%d0	*/
#define	RT_IMMNUM	2	/* immediate numeric:	&1	*/
#define	RT_IMMED	3	/* immediate other:	&xyz	*/
#define	RT_IREG		4	/* register indirect:	(%a0)	*/
#define	RT_INCR		5	/* register post-incr:	(%a0)+	*/
#define	RT_DECR		6	/* register pre-decr:	-(%a0)	*/
#define	RT_OREG		7	/* offset from reg:	4(%a0)	*/
#define	RT_DBLIND	8	/* reg. double ind.:	4(%a0,%d0.w) */
#define	RT_MEM		9	/* memory direct:	xyz	*/
#define	RT_SMEM		16	/* short memory direct:	xyz	*/

#define	RT_RTSUB	10	/* name of a special run-time support
				** subroutine which expects %d0
				*/
#define	RT_LABEL	11	/* label */
#define	RT_OTHER	12	/* other:  for pseudo-ops, etc. */

#define RT_SUB01	13	/* name of a special run-time subroutine
				** which expects arguments in %d0 and %d1
				*/
#if m68k
#define RT_020          14      /* new 680X0 modes :    (bd,An,Ri,od)
				**                      ([bd,An,Ri],od)
				**                      ([bd,An],Ri,od)
				*/
#define RT_REG_PAIR	15	/* register pair Ri:Rj used in 64-bit mul,div */
#endif

typedef	unsigned char RTYPE;	/* operand type */
/* Register names and values.  The values are mutually exclusive bits. */

/* REG represents a register.  It must be a long to hold all the bits. */

typedef	long	REG;

#define	REG_NONE ((REG) 0)		/* no register */
#define	REG_D0	((REG) 01)		/* %d0 */
#define	REG_D1	((REG) 02)		/* %d1 */
#define	REG_D2	((REG) 04)		/* %d2 */
#define	REG_D3	((REG) 010)		/* %d3 */
#define	REG_D4	((REG) 020)		/* %d4 */
#define	REG_D5	((REG) 040)		/* %d5 */
#define	REG_D6	((REG) 0100)		/* %d6 */
#define	REG_D7	((REG) 0200)		/* %d7 */
#define	REG_A0	((REG) 0400)		/* %a0 */
#define	REG_A1	((REG) 01000)		/* %a1 */
#define	REG_A2	((REG) 02000)		/* %a2 */
#define	REG_A3	((REG) 04000)		/* %a3 */
#define	REG_A4	((REG) 010000)		/* %a4 */
#define	REG_A5	((REG) 020000)		/* %a5 */
#define	REG_A6	((REG) 040000)		/* %a6 */
#define	REG_A7	((REG) 0100000)		/* %a7 */
#define	REG_PC	((REG) 0200000)		/* %pc */
#define	REG_CC	((REG) 0400000)		/* %cc */
#define	REG_SR	((REG) 01000000)	/* %sr */
#define	REG_USP	((REG) 02000000)	/* %usp */
#define	REG_FP	REG_A6			/* %fp is %a6 */
#define	REG_SP	REG_A7			/* %sp is %a7 */
#ifdef M68881
#define	REG_FP0	((REG) 04000000)	/* %fp0 */
#define	REG_FP1	((REG) 010000000)	/* %fp1 */
#define	REG_FP2	((REG) 020000000)	/* %fp2 */
#define	REG_FP3	((REG) 040000000)	/* %fp3 */
#define	REG_FP4	((REG) 0100000000)	/* %fp4 */
#define	REG_FP5	((REG) 0200000000)	/* %fp5 */
#define	REG_FP6	((REG) 0400000000)	/* %fp6 */
#define	REG_FP7	((REG) 01000000000)	/* %fp7 */
#endif

#ifdef M68881
#define	NREGS	28		/* number of unique registers above */
#else
#define	NREGS	20		/* number of unique registers above */
#endif

/* Declare register classes */

#define	REG_AREG \
	(REG_A0 | REG_A1 | REG_A2 | REG_A3 | REG_A4 | REG_A5 | REG_A6 | REG_A7)

#define	REG_DREG \
	(REG_D0 | REG_D1 | REG_D2 | REG_D3 | REG_D4 | REG_D5 | REG_D6 | REG_D7)

#ifdef M68881
#define REG_FPREG \
	(REG_FP0 | REG_FP1 | REG_FP2 | REG_FP3 | REG_FP4 | REG_FP5 | REG_FP6 | \
		REG_FP7)
#endif

/* These definitions are for live/dead analysis */
/* hanna add REG_SR to live regs */

#define	LIVEREGS	( REG_PC | REG_SP | REG_FP | REG_SR )
#ifdef M68881
#define	REGS \
	( REG_AREG | REG_DREG | REG_FPREG | REG_CC | REG_SR | REG_PC | REG_USP )
#define	RETREG	( REG_D0 | REG_A0 | REG_FP0 )
/* Data are returned in %d0, pointers are returned in %a0, and float in FP0 */
#else
#define	REGS ( REG_AREG | REG_DREG | REG_CC | REG_SR | REG_PC | REG_USP )
/* Data are returned in %d0, but pointers are returned in %a0 */
#define	RETREG	( REG_D0 | REG_A0 )
#endif
#define	LIVEDEAD	NREGS

/* This next definition is highly dependent on the compiler.  It states
** which registers the compiler thinks it can use for scratch registers.
*/

#ifdef M68881
#define	SCRATCHREGS	(REG_D0 | REG_D1 | REG_A0 | REG_A1 | REG_FP0 | REG_FP1 )
#else
#define	SCRATCHREGS	(REG_D0 | REG_D1 | REG_A0 | REG_A1 )
#endif
/* RAND contains all of the information that the optimizer requires
** for each operand.  Two operands with the same operand text string
** share the same RAND structure.  Thus two operands are the same if
** the pointer to their RAND structures is the same.
*/

struct rand
{
    char * rname;		/* operand char string */
    RTYPE rtype;		/* operand type (one of the types above) */
    REG ruses;			/* registers used by operand */
#if m68k
    REG owuses;                 /* registers used in the {o:w} offset:width
				** construct in M680X0 bit field instrucs.
				*/
    short int disp1,            /* disp. sizes for base and outer displace-
				** ments.  Legal values are (0,2,4)
				*/
	      disp2;
#endif
    unsigned char dsize;	/* hanna Tue Aug  6 14:42 PDT 1985 */
};

typedef struct rand	RAND;

#define	RNULL	((RAND *) 0)	/* a null operand pointer */

RAND * saverand();
RAND * findrand();
