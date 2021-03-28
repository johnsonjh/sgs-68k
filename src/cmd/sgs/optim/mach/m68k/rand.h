#ident	"@(#)rand.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* rand.h */
/*	@(#) rand.h:	2.1 83/07/08	*/

/*	MC68020 Optimizer	*/

/* Definitions for operand data */

/* Operand types */

#define	RT_REG		1	/* register direct:	%d0	*/
#define	RT_IREG		2	/* register indirect:	(%a0)	*/
#define	RT_INCR		3	/* register post-incr:	(%a0)+	*/
#define	RT_DECR		4	/* register pre-decr:	-(%a0)	*/
#define	RT_OREG		5	/* offset16 from reg:	4(%a0)	*/
#define	RT_INDX		6	/* indexed (short): (%a0,%d0.w) */
						/*          		4(%a0,%d0.w) */
						/*					((4).b,%a0,%d0.w*4) */
#ifdef m68k
#define RT_EXTINDX	7	/* indexed (extended): 0x2000000(%a0) */
						/*					((disp).w,%a0,%d0) */
						/* 					([4,%a0],%d0,8) */
#define RT_REG_PAIR	8	/* register pair Ri:Rj used in 64-bit mul,div */
#endif
#define	RT_MEM		9	/* memory direct:	xyz	*/
#define	RT_IMMNUM	10	/* immediate numeric:	&1	*/
#define	RT_IMMED	11	/* immediate other:	&xyz	*/

#define	RT_LABEL	12	/* label */
#define	RT_RTSUB	13	/* special run-time subroutine name returning %d0 */
#define RT_SUB01	14	/* special run-time subroutine returning %d0/%d1 */
#define RT_SUB02	15	/* special run-time subroutine requires %a0 */

typedef	unsigned char RTYPE;	/* operand type */
/* Register names and values.  The values are mutually exclusive bits. */

/* REG represents a register.  It must be a long to hold all the bits. */

typedef	long	REG;

#define	REG_NONE ((REG) 0)		/* no register */
#define	REG_D0	((REG) 1)		/* %d0 */
#define	REG_D1	((REG) 2)		/* %d1 */
#define	REG_D2	((REG) 4)		/* %d2 */
#define	REG_D3	((REG) 8)		/* %d3 */
#define	REG_D4	((REG) 0x10)		/* %d4 */
#define	REG_D5	((REG) 0x20)		/* %d5 */
#define	REG_D6	((REG) 0x40)		/* %d6 */
#define	REG_D7	((REG) 0x80)		/* %d7 */
#define	REG_A0	((REG) 0x100)		/* %a0 */
#define	REG_A1	((REG) 0x200)		/* %a1 */
#define	REG_A2	((REG) 0x400)		/* %a2 */
#define	REG_A3	((REG) 0x800)		/* %a3 */
#define	REG_A4	((REG) 0x1000)		/* %a4 */
#define	REG_A5	((REG) 0x2000)		/* %a5 */
#define	REG_A6	((REG) 0x4000)		/* %a6 */
#define	REG_A7	((REG) 0x8000)		/* %a7 */

#define REG_FP	((REG) 0x10000)		/* %fp - distinct from %a6! */
#define	REG_CC	((REG) 0x20000)		/* %cc */
/*efine         ((REG) 0x40000)     FREE   */
#define	REG_VOID	((REG) 0x80000)	/* unprocessable registers */
/* 				REG_VOID for MC68000:
**    usp, sr, ccr
**
** 				REG_VOID for MC68020 and MC68030:
**    usp, sr, ccr, vbr, dfc, sfc, caar, cacr, isp, msp
**
** 				REG_VOID for MC68040:
**    usp, sr, ccr, vbr, dfc, sfc, cacr, isp, msp, tc, itt0, itt1, dtt0,
**	  dtt1, mmusr, urp, srp
*/
#define	REG_SP	REG_A7			/* %sp is %a7 */
#ifdef M68881
#define	REG_FP0	((REG) 0x100000)	/* %fp0 */
#define	REG_FP1	((REG) 0x200000)	/* %fp1 */
#define	REG_FP2	((REG) 0x400000)	/* %fp2 */
#define	REG_FP3	((REG) 0x800000)	/* %fp3 */
#define	REG_FP4	((REG) 0x1000000)	/* %fp4 */
#define	REG_FP5	((REG) 0x2000000)	/* %fp5 */
#define	REG_FP6	((REG) 0x4000000)	/* %fp6 */
#define	REG_FP7	((REG) 0x8000000)	/* %fp7 */
#endif
#ifdef M68881
#define	NREGS	28		/* number of unique registers above */
#else
#define	NREGS	20		/* number of unique registers above */
#endif

/* Declare register classes */

#define	REG_AREG \
	(REG_A0 | REG_A1 | REG_A2 | REG_A3 | REG_A4 | REG_A5 | REG_A6 | REG_A7)

#define AVAR_REGS \
	(REG_A2 | REG_A3 | REG_A4 | REG_A5 | REG_A6)

#define A_SCRATCH \
	(REG_A0 | REG_A1)

#define	REG_DREG \
	(REG_D0 | REG_D1 | REG_D2 | REG_D3 | REG_D4 | REG_D5 | REG_D6 | REG_D7)

#define DVAR_REGS \
	(REG_D2 | REG_D3 | REG_D4 | REG_D5 | REG_D6 | REG_D7)

#define D_SCRATCH \
	(REG_D0 | REG_D1)

#ifdef M68881
#define REG_FPREG \
	(REG_FP0 | REG_FP1 | REG_FP2 | REG_FP3 | REG_FP4 | REG_FP5 | REG_FP6 | \
		REG_FP7)

#define FPVAR_REGS \
	(REG_FP2 | REG_FP3 | REG_FP4 | REG_FP5 | REG_FP6 | REG_FP7)

#define FP_SCRATCH \
	(REG_FP0 | REG_FP1)
#endif

/* These definitions are for live/dead analysis */

/* integers always returned in %d0 */
#define RETREGINT	REG_D0
/* pointers always returned in %a0 AND %d0 */
#define RETREGPTR	(REG_A0+REG_D0)
#ifdef M68881
#define	REGS \
	( REG_AREG | REG_DREG | REG_FPREG | REG_CC | REG_FP | REG_VOID )
/* floats always returned in %fp0 */
#define	RETREGFP	REG_FP0
#else
#define	REGS ( REG_AREG | REG_DREG | REG_CC | REG_FP | REG_VOID )
/* floats always returned in %d0/%d1 */
#define	RETREGFP	( REG_D0 | REG_D1 )
#endif
#define	LIVEDEAD	NREGS

#ifdef M68881
#define	SCRATCHREGS	(D_SCRATCH | A_SCRATCH | FP_SCRATCH)
#define VAR_REGS	(DVAR_REGS | AVAR_REGS | FPVAR_REGS)
#else 
#define	SCRATCHREGS	(D_SCRATCH | A_SCRATCH)
#define VAR_REGS	(DVAR_REGS | AVAR_REGS)
#endif
#define ADVAR_REGS	(DVAR_REGS | AVAR_REGS)
/* RAND contains all of the information that the optimizer requires
** for each operand.  Two operands with the same operand text string
** share the same RAND structure.  Thus two operands are the same if
** the pointer to their RAND structures is the same.
*/

struct rand
{
    char * rname;		/* operand char string */
    REG ruses;			/* registers used by operand */
	REG	rindx;			/* index register */
	long	rdisp;		/* displacement constant (or partial) or immediate */
#ifdef m68k
	REG	owuses;			/* bit field {o:w} registers */
	RTYPE	rindxscl;	/* scale factor for index register */
#endif
    RTYPE	rtype;		/* operand type (one of the types above) */
	char	rflags;		/* operand flags */
};
/* rflags definition */

#define rfmemind	0x01	/* memory indirect ([]) used in indexed mode */
#define rfbitfld	0x02	/* this rand is a bit field */
#define rfdotw		0x04	/* index register is '.w' */

typedef struct rand	RAND;

#define	RNULL	((RAND *) 0)	/* a null operand pointer */

RAND * saverand();
RAND * findrand();
