/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mipsup.c: version 25.1 created on 12/2/91 at 17:40:56	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mipsup.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	MOT:mipsup.c	7.1		*/
/* mipsup.c */

#ident	"@(#)f77/f77optim/mach/m68k:mipsup.c	25.1"

/* MOT:mipsup.c:		mipsup.c	7.1	"*/
extern int quickpass;

/*	MC680X0 Optimizer	*/

/* Support for machine independent pieces */

#include "debug.h"
#include "optim.h"

/* Several routines here deal with LABEL nodes.  Such a node has
** the following structure.  For the normal parts:
**
**	p->op		LABEL
**	p->ops[0]	pointer to label string (note that this conflicts
**			with normal use of p->ops[0] as the text string
**			for a normal op)
**
** These items are non-standard:
**
**	p->ops[1]	pointer to label string, as above; done to keep
**			use of userdata.ud_rand[0] consistent
**	p->userdata.ud_op	contains pointer to LABEL OP structure
** 	p->userdata.ud_rand[0]	points at RAND structure for label
*/
/* ishlp -- test for hard label
**
** Ishlp tests for a hard label among a group of contiguous labels.
*/

boolean					/* true if hard label found */
ishlp(node)
register NODE * node;
{
	TRACE(ishlp);
    while ( islabel(node) )
	if (ishl(node))
	    return(true);
	else
	    node = node->forw;		/* forge ahead */
    
    return(false);			/* no hard label */
}
/* newlab -- create new label
**
** This routine returns a pointer to a label that should be new.
*/

static int labelnumber = 1;		/* initial label number */

char *
newlab()
{
    char labelbuf[10];
    int rlen;
    RAND * r;				/* pointer to resulting operand */
    if(quickpass)
         rlen = sprintf(labelbuf, "L%%%%%%%d", labelnumber++);
    else rlen = sprintf(labelbuf, "L%%%%%d", labelnumber++);
					/* label has form L%%n */

	TRACE(newlab);
    r = saverand(labelbuf, rlen, RT_LABEL, REG_NONE, true);
					/* okay to create new one */
    return (r->rname);			/* return pointer to name string */
}
/* getp -- get pointer to branch destination
**
** This routine returns a pointer to the operand character string
** corresponding to a branch's destination.
*/

char *
getp(p)
NODE * p;
{
    int nsrc;				/* number of source operands */

    /* check cases with null destination first */
    if(eflag)printf("#getp ()\n");

    switch ( p->op )			/* dispatch on type of operation */
    {
    case RTS:
    case RTE:
    case RTR:
    case SWITCH:
	return(NULL);			/* instructions with no dest. */
    
    default:
	if ((p->userdata.ud_flags & UD_NCBRA) != 0)
					/* conditional branch with null dest. */
	    return(NULL);

    /* for default case, return char string for highest numbered source
    ** operand.
    */

	nsrc = p->userdata.ud_op->op_nsrc; /* number of source operands, =
					   ** number of operand containing
					   ** branch destination
					   */
	return ( p->userdata.ud_rand[ nsrc-1 ]->rname );
					/* rands are numbered 0 - n-1 */
    }
/*NOTREACHED*/
}
/* setlab -- make node a label
**
** This routine is a bit of a crock.  It assumes that the node we're
** handed has an operand string as op[0], because that's how optim()
** uses setlab().
*/

void
setlab(node)
NODE * node;
{
    char * label = node->ops[0];	/* save the label string */
    RAND * lrand = findrand(label,true); /* locate the RAND corresponding to
					** the existing label; must exist
					*/
	TRACE(setlab);
    
    setop(node, getOP(LABEL));		/* make node a label
					** CROCK!!:  clobbers p->ops[0]
					*/
    
    node->ops[0] = label;		/* reset label string */
    setrand(node, 1, lrand);		/* set up operand 1 (non-standard) */
    setrand(node, 2, RNULL);		/* null out second operand */
    return;
}
/* setbr -- turn node into unconditional branch
**
** This routine makes a node an unconditional branch.  This requires
** clearing out the unused operands and setting the op field and
** first operand.
*/

void
setbr(p,s)
NODE * p;				/* node to change */
char * s;				/* branch string */
{
	TRACE(setbr);
    setrand(p, 2, RNULL);		/* clear out second operand */
    setop(p, getOP(BR));		/* make operation a branch */
    setrand(p, 1, findrand(s,true));	/* find the operand (it better
					** exist), and stick it in
					*/
    return;
}
/* revbr -- reverse a reversible conditional branch
**
** This routine changes a conditional branch to its "opposite"
*/

void
revbr(node)
NODE * node;				/* node whose op should be reversed */
{
    OPCODE newop = node->userdata.ud_op->op_opposite;
					/* get opposite code */
	TRACE(revbr);
    
    if (newop == NOOPP)
	oerr("Op has no opposite");
    
    setop(node, getOP(newop));		/* set up new op */
    return;
}
/* setop -- put op code into node
**
** This routine sets a node's opcode, as well as its opcode string
** and user data flags.
*/

void
setop(node, op)
NODE * node;				/* node to change */
OP * op;				/* OP pointer */
{
    /* in node, ->op is opcode, ->opcode is op string */

	TRACE(setop);
    node->op = op->op_code;		/* set code */
    node->opcode = op->op_name;		/* get string */
    node->userdata.ud_op = op;		/* save pointer to op structure */
    node->userdata.ud_flags = op->op_flags;
					/* copy flags */
    return;
}
/* setrand -- set operand into node
**
** This routine sets up an operand for a node.  It takes care of both
** the normal and userdata stuff.
*/

/*************************************************************************/
void
setrand(node, rnum, rand)
NODE * node;				/* node to change */
int rnum;				/* operand number */
RAND * rand;				/* pointer to operand structure */
{
    node->ops[rnum] = ( rand == RNULL ? NULL : rand->rname );
    /* copy operand string pointer */
    node->userdata.ud_rand[rnum-1] = rand;
    /* hanna Tue Aug  6 14:44:41 PDT 1985 */
    if(node->userdata.ud_op!=NULL
	&& rand != RNULL && node->userdata.ud_op->op_dsize < 5){
	if(node->userdata.ud_op->op_dsize)
	rand->dsize = node->userdata.ud_op->op_dsize;
	}
					/* save entire operand here */
    return;
}
/*************************************************************************/
#ifdef old
void
setrand(node, rnum, rand)
NODE * node;				/* node to change */
int rnum;				/* operand number */
RAND * rand;				/* pointer to operand structure */
{
	TRACE(setrand);
    node->ops[rnum] = ( rand == RNULL ? NULL : rand->rname );
					/* copy operand string pointer */
    node->userdata.ud_rand[rnum-1] = rand;
					/* save entire operand here */
    return;
}
#endif
/* prinst -- print instruction node
**
** This routine prints a single node in assembly format.
*/

void
prinst(node)
NODE * node;
{
    void outswitch();

	TRACE(prinst);
    switch (node->op)			/* dispatch on op type */
    {
    case LABEL:
	PRINTF("%s:\n", node->op1);	/* print label node */
	break;
    
    case SWITCH:
	outswitch(node);		/* output switch node */
	break;
    
    default:				/* everything else */
	if (node->uniqid != IDVAL)	/* print line number, if any */
	    PRINTF("	ln	%d\n", node->uniqid);

	if (node->op < OP_MIN || node->op > OP_MAX)
	    oerr("Bad op code in prinst");

	PRINTF("\t%s", node->opcode);	/* output op itself */
	if (node->op1 != NULL)		/* do first operand, if there is one */
	{
	    PRINTF("\t%s", node->op1);
	    if (node->op2 != NULL)	/* do second one if there is one, too */
		PRINTF(",%s", node->op2);
	}
	putchar('\n');			/* end line */
    }
    return;
}
/* uses -- produce register mask of registers used
**
** This routine returns a register mask corresponding to the
** registers used by an instruction.  The live/dead analysis
** routines use this information.
*/

REG
uses(node)
NODE * node;				/* instruction to do calc. on */
{
    USERTYPE * ud = &(node->userdata);	/* point at user data for node */
    RAND ** rands = ud->ud_rand;	/* point at instruction's operands */
    REG retreg = REG_NONE;		/* current calculated return value */
    int temp;				/* temporary int, used variously */
    REG doreglist();

	TRACE(uses);
    if ((ud->ud_flags & UD_LD) != 0)	/* check for special case */
	switch(node->op)		/* do special cases first */
	{
#if m68k
	case BFCHG:
	case BFCLR:
	case BFSET:
	case BFTST:
	case BFEXTS:
	case BFEXTU:
	case BFFFO:
	    retreg |= rands[0]->owuses; /* add registers from {o:w} field */
	    break;
	case BFINS:
	    retreg |= rands[1]->owuses; /* add registers from {o:w} field */
	    break;
#endif
	case RTS:			/* return instruction */
	    return( RETREG );		/* "uses" returned registers */
	
	case EXG:			/* exchange uses both regs */
	    return( rands[0]->ruses | rands[1]->ruses );
	
	case BSR:
	case BSR_B:
	case JSR:			/* subroutines use whatever the
					** operand uses; also, if a special
					** subroutine, %d0 gets used
					*/
	    if (rands[0]->rtype == RT_RTSUB)
		retreg |= REG_D0;	/* this subroutine uses %d0 */	
	    if (rands[0]->rtype == RT_SUB01)
		retreg |= REG_D0 | REG_D1; /* this subroutine uses %d0 & %d1 */
	    break;			/* do normal processing now */

	case MOVM_W:
	case MOVM_L:			/* move multiple */
#ifdef M68881
	case FMOVM:
#endif
	    retreg |= doreglist(rands[0],rands[1], node->op);
					/* uses any registers in register list
					** if first operand
					*/
	    break;			/* fall through to do rest normally */
	
	case CLR_B:			/* the upper part of operands are */
	case CLR_W:			/* still useable */
		return(rands[0]->ruses);

	default:			/* special case not found */
	    oerr("Unrecognized special L/D in uses");
	}
/* Default "uses" processing:  the logical OR of any registers used
** by both operands and the destination, if the destination is not a
** register direct (RT_REG).
*/

    temp = ud->ud_op->op_nsrc;		/* get number of source operands */
    if (temp > 0)			/* if there's one */
    {
	retreg |= rands[0]->ruses;	/* include operand's register uses */
	if (temp > 1)			/* if two source operands... */
	    retreg |= rands[1]->ruses;	/* include second, as well */
    }

    temp = (unsigned) ud->ud_op->op_dnum; /* get destination operand number */
    if (temp != NODEST && rands[temp]->rtype != RT_REG)
	retreg |= rands[temp]->ruses;	/* include destination uses */
    
    /* conditional branches use condition codes */

    if ((ud->ud_flags & (UD_CBRA | UD_NCBRA)) != 0)
	retreg |= REG_CC;
    
    return(retreg);			/* return computed value */
}
/* sets -- produce register mask of registers used
**
** This routine returns a register mask of registers set (changed) by
** the specified instruction.  The information is used ultimately by
** live/dead analysis.
*/

REG
sets(node)
NODE * node;				/* instruction to do calc. on */
{
    REG retreg = REG_NONE;		/* initial value to return */
    USERTYPE * ud = &(node->userdata);	/* pointer to user data */
    RAND ** rands = ud->ud_rand;	/* point at operands */
    FLAGS flags = ud->ud_flags;		/* flags for this instruction */
    RAND * drand = RNULL;		/* pointer to destination operand */
    RAND * r;				/* temporary operand pointer */
    REG doreglist();

	TRACE(sets);
    /* do special cases first, based on instruction */

    if ((flags & UD_LD) != 0)		/* is this a special case? */
	switch (node->op)
	{
	case RTS:			/* return instruction */
	    return(REG_NONE);		/* no register changes */
	
	case EXG:			/* exchange sets both */
	    return( rands[0]->ruses | rands[1]->ruses );
	
	case BSR:
	case BSR_B:			/* pretend that subroutines "set"
					** their scratch registers
					*/
	case JSR:
	    return (SCRATCHREGS);
	
	case MOVM_W:			/* move multiple */
	case MOVM_L:
#ifdef M68881
	case FMOVM:
#endif
	    retreg |= doreglist(rands[1],rands[0],node->op);
					/* if second operand is register list,
					** get mask
					*/
	    break;			/* then do normal processing */

	case CLR_B:			/* handle these in the normal way */
	case CLR_W:
	case BFCHG:
	case BFCLR:
	case BFSET:
	case BFTST:
	case BFEXTS:
	case BFEXTU:
	case BFFFO:
	case BFINS:
		break;

	}
/* now do normal processing */

    if (ud->ud_op->op_dnum != NODEST)	/* if the op has a destination */
	drand = rands[ud->ud_op->op_dnum]; /* get pointer to it */

    if ((flags & UD_MODSP) != 0)	/* if modifies %sp implicitly */
	retreg |= REG_SP;
    
    /* check for setting condition codes; some instructions set them only
    ** if the destination is not an address register
    */

    if (   (flags & UD_CC) != 0
	|| ((flags & UD_CCA) != 0 && ! isareg(drand))
	)
	retreg |= REG_CC;
    
    /* Go through each operand, checking for registers modified.
    ** Only auto-increment and -decrement change register, except
    ** for the special cases above, and for a destination whose
    ** operand type is register direct.
    */

    if ((r = rands[0]) != RNULL && (r->rtype == RT_INCR || r->rtype == RT_DECR))
	retreg |= r->ruses;		/* here's one */

    if ((r = rands[1]) != RNULL && (r->rtype == RT_INCR || r->rtype == RT_DECR))
	retreg |= r->ruses;

    if (   drand != RNULL
	&& (drand->rtype == RT_REG || drand->rtype == RT_INCR ||
#if m68k
		drand->rtype == RT_REG_PAIR || /* Ri:Rj for '020 mul,div */
#endif
		drand->rtype == RT_DECR)
	)
	retreg |= drand->ruses;
    
    return(retreg);
}
/* regmask -- register bit array */
#ifdef M68881
/*
** This array contains the REG_ bits defined in the order in which
** a fmovm.[wl] instruction would have them for the auto-decrement mode.
** All other modes require scanning the list backward.
*/

static REG
fprmask[] =
{
    REG_NONE,				/* sentinel for backward search */
    REG_FP0,
    REG_FP1,
    REG_FP2,
    REG_FP3,
    REG_FP4,
    REG_FP5,
    REG_FP6,
    REG_FP7,
    REG_NONE
};
#endif

/*
** This array contains the REG_ bits defined in the order in which
** a movm.[wl] instruction would have them for most address modes.
** Auto-decrement requires scanning the list backward, however.
*/
static REG
regmask[] =
{
    REG_NONE,				/* sentinel for backward search */
    REG_D0,
    REG_D1,
    REG_D2,
    REG_D3,
    REG_D4,
    REG_D5,
    REG_D6,
    REG_D7,
    REG_A0,
    REG_A1,
    REG_A2,
    REG_A3,
    REG_A4,
    REG_A5,
    REG_A6,
    REG_A7,
    REG_NONE				/* sentinel for forward search */
};
/* doreglist -- compute register mask for register list
**
** This routine calculates the register mask corresponding to a
** register list for a movm.[wl] instruction.  Such a list must be
** presented as an immediate operand.  If the operand is an immediate
** operand whose value we don't know, we assume the worst.  We need
** the other operand from the movm.[wl] to tell us which direction
** the register mask applies to:  auto-increment and auto-decrement
** have different masks.
*/


static REG
doreglist(r1,r2,op)
RAND * r1;				/* pointer to register list operand */
RAND * r2;				/* pointer to other operand */
unsigned short op;			/* op code, needed to identify 'fmov'*/
{
    REG retreg = REG_NONE;		/* default return value */
    unsigned long mask;			/* instruction's register mask value */
    int incr;				/* index increment for list search */
    REG * reglist;			/* pointer to list of register values */
    long atol();

	TRACE(doreglist);
    switch(r1->rtype)			/* see what to do */
    {
    case RT_IMMED:			/* non-resolved immediate */
#ifdef M68881
	return( REG_AREG | REG_DREG | REG_FPREG);	/* assume the worst */
#else
	return( REG_AREG | REG_DREG );	/* assume the worst */
#endif
    
    default:				/* most modes */
	return( REG_NONE );		/* no register list */
    
    case RT_IMMNUM:			/* immediate with value */
	break;				/* do the interesting stuff */
    }
/* Interest case:  the operand is a register mask whose value we can
** determine.  We'll examine the mask bits as we step through a table
** of corresponding REG_ bits, setting return bits for 1 bits in the mask.
*/

    mask = atol(r1->rname+1);		/* interpret string following '&' */
    if (mask == 0)
	return(REG_NONE);		/* no registers affected */
    
    /* Determine direction to scan list of register bits */

#ifdef M68881
    if( op == FMOVM )
	if( r2->rtype == RT_DECR )	/* for fmovm and auto decrement */
	{
	     reglist = &fprmask[1];
	     incr = 1;				/* scan forward */
	}
	else				/* fmovm and not auto decrement */
	{
	     reglist = &fprmask[8];
	     incr = -1;				/* scan backward */
	}
    else 				/* movm */
#endif
    if (r2->rtype == RT_DECR)		/* for auto-decrement */
    {
	reglist = &regmask[16];
	incr = -1;			/* scan backwards from bottom */
    }
    else				/* all other modes */
    {
	reglist = &regmask[1];
	incr = 1;			/* scan forward from top */
    }

    /* search list while more mask bits and more list bits */

    while (*reglist != REG_NONE && mask != 0)
    {
	if ((mask & 1) != 0)		/* check low order bit */
	    retreg |= *reglist;		/* include this register bit */
	
	mask >>= 1;			/* next mask bit */
	reglist += incr;		/* next list entry */
    }

    return(retreg);			/* return result */
}
/* instsize -- calculate size of instruction
**
** This routine calculates the size of an instruction to support
** branch shortening.  In the interests of speed, we sometimes
** make a conservative estimate of instruction size.
** The calculations are in bytes.
*/

int
instsize(node)
register NODE * node;			/* instruction whose size we calc. */
{
    int size;				/* current instruction size */
    register RAND ** rands;		/* instruction's operands */
    int switsize();

	TRACE(instsize);
    if (node->op == LABEL)
	return(0);			/* label has no size */
    
    if (node->op == SWITCH)
	return(switsize(node));		/* we need help on this one */
    
    /* For branches, assume the assembler will do the worst:  change
    **
    **		br	x
    ** into
    **		br.b	.+6
    **		jmp	x		# with 4-byte absolute
    **
    ** Thus, it's the optimizer's responsibility to find possibilities
    ** for very short branches, and the assembler's to find very long
    ** ones.  However, since very long branches (bottom case) happen
    ** only rarely, assume the upper case.  If we shorten it, we get
    ** two bytes of slop free, which will help cover for instances of
    ** the bottom case.  Also, in "defs", the distance a short branch can
    ** span is estimated conservatively to allow for the possibility that
    ** the assembler might have to stick in a very long branch, thus
    ** invalidating our calculations.  THIS IS A HEURISTIC which could
    ** conceivable fail under unusual circumstances.
    */

    if (is4bytes(node))              	/* base size of instruc is 4 bytes */
	size = 4;
    else
	size = 2;

    if (isbr(node))
	return(size+2);			/* size of br (not br.b) */
    
    /* calculate operand sizes */

    rands = node->userdata.ud_rand;	/* point at operands */
    if (rands[0] != RNULL)
    {
	switch(rands[0]->rtype)		/* size depends on type */
	{
	case RT_OREG:
	case RT_DBLIND:
	case RT_SMEM:		
	    size += 2; break;		/* these take 2-bytes extra */
	
	case RT_IMMED:
	case RT_IMMNUM:
	case RT_MEM:			/* assume 4-byte extra for these */
	case RT_LABEL:
	case RT_RTSUB:
	case RT_SUB01:
	    size += 4; break;
#if m68k	
	case RT_REG_PAIR:		/* Ri:Rj for '020 mul,div */
#endif
	case RT_REG:
	case RT_IREG:
	case RT_INCR:
	case RT_DECR:			/* assume zero cost for the these */
	    break;
#if m68k
	case RT_020:
	    /* Add in extension word size and disp. word sizes */
	    size += 2 + rands[0]->disp1 + rands[0]->disp2; break;
#endif
	default:
	    oerr("Unexpected operand type in instsize()");
	}
	if (rands[1] != RNULL)		/* now check second operand */
	{
	    switch(rands[1]->rtype)	/* size depends on type */
	    {
	    case RT_OREG:
	    case RT_DBLIND:
	    case RT_SMEM:		
		size += 2; break;	/* these take 2-bytes extra */
	    
	    case RT_IMMED:
	    case RT_IMMNUM:
	    case RT_MEM:		/* assume 4-byte extra for these */
	    case RT_LABEL:
	    case RT_RTSUB:
	    case RT_SUB01:
		size += 4; break;
	    
#if m68k	
	case RT_REG_PAIR:		/* Ri:Rj for '020 mul,div */
#endif
	    case RT_REG:
	    case RT_IREG:
	    case RT_INCR:
	    case RT_DECR:		/* assume zero cost for the these */
		break;
#if m68k
	case RT_020:
	    /* Add in extension word size and disp. word sizes */
	    size += 2 + rands[1]->disp1 + rands[1]->disp2; break;
#endif
	    default:
		oerr("Unexpected operand type in instsize()");
	    }
	}
    }
	if (Sflag){			
    		PRINTF("%#.6x\t",size);
    		prinst(node);		/* print instruction */
	}
    return(size);			/* return calculated result */
}
/* bshortsub -- shorten a branch
**
** This routine shortens a branch.  We can only shorten conventional
** branches, not dbCC types.
*/

void
bshortsub(node)
NODE * node;				/* pointer to node whose branch we
					** shorten
					*/
{
    register OPCODE newop;		/* op code for new branch */

    TRACE(bshortsub);
    /* if you will be doing another pass, then don't optimize
       this time round
      */
    if(quickpass)
	return;
    switch (node->op)			/* choose new op code */
    {
    case BCC:	newop = BCC_B; break;
    case BCS:	newop = BCS_B; break;
    case BEQ:	newop = BEQ_B; break;
    case BGE:	newop = BGE_B; break;
    case BGT:	newop = BGT_B; break;
    case BHI:	newop = BHI_B; break;
    case BHS:	newop = BHS_B; break;
    case BLE:	newop = BLE_B; break;
    case BLO:	newop = BLO_B; break;
    case BLS:	newop = BLS_B; break;
    case BLT:	newop = BLT_B; break;
    case BMI:	newop = BMI_B; break;
    case BNE:	newop = BNE_B; break;
    case BPL:	newop = BPL_B; break;
    case BR:	newop = BR_B; break;
    case BRA:	newop = BRA_B; break;
    case BVC:	newop = BVC_B; break;
    case BVS:	newop = BVS_B; break;
    default:				/* can't shorten others */
	return;
    }

    setop(node, getOP(newop));		/* set the new instruction */
    return;
}
