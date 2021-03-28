/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) peep.c: version 25.1 created on 12/2/91 at 17:41:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)peep.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* MOT:peep.c Version, 7.3 Creation date 85/09/18"*/

#ident	"@(#)f77/f77optim/mach/m68k:peep.c	25.1"

/*                          NOTE
**
**	This file is now included at the end of vtrace.c in order to
** gain access to the value cache and the routines which manipulate
** this cache.  2/9/84
*/

/*	MC680X0 family optimizer - File peep.c 	*/

/* This module contains the peephole improver.  Because of the high
** quality code that the compiler generates, this module primarily
** cleans up the debris left by the value trace routine.
*/

/* peep -- do peephole improvements
**
** This routine serves two main functions:  to delete dead code (from
** live/dead analysis) and to remove redundant moves as revealed by
** the value trace.  The redundant moves appear as
**
**	mov.[bwl]	%R,%R
**
** We get called on a node-by-node basis.
*/

#include "debug.h"
#ifndef LIVEDEAD
#define	isdead(a,b) 0			/* nothing looks dead */
#endif


boolean					/* true if something deleted */
peep(node,pflag)
register NODE * node;			/* node to examine now */
boolean pflag;				/* print info as we go */
{
    register RAND ** rands;		/* operands for current node */
    unsigned int dnum;			/* destination operand number */
    boolean delete = false;		/* true to delete the node (the only
					** basic option)
					*/
    RAND * trand;			/* temporary operand pointer */
    long getimmnum();

    /* Assume live/dead information is correct, at least where we are.
    ** What we do depends on the current op.
    */

	TRACE(peep);
	if(eflag)pflag++;
    rands = node->userdata.ud_rand;	/* point at op's operands */
    dnum = node->userdata.ud_op->op_dnum; /* get destination operand number */

    switch(node->op)
    {
    case MOVM_L:			/* delete these if immediate number
					** is zero
					*/
    case MOVM_W:
#if m68k
    case FMOVM:
#endif
	if (   (   (trand = rands[0])->rtype == RT_IMMNUM
		|| (trand = rands[1])->rtype == RT_IMMNUM
		)
	    && getimmnum(trand) == 0
	    )
	    delete = true;		/* we should delete this */
	break;

    /* For the mov.[bwl] family, we can delete all moves where the
    ** destinations are the same, provided:
    **
    **	1) condition codes are dead thereafter, and
    **	2) if the operands are registers, the register is dead
    **
    ** Remember that moves to A registers don't affect condition codes.
    */

    case MOV_B:
    case MOV_W:
    case MOV_L:
	if (   rands[0] == rands[1]
	    && (isareg(rands[0]) || isdead(REG_CC, node))
	    )
	{
	    delete = true;		/* we can delete this one */
	    break;
	}


#if m68k
   case FMOV_X:
    /* The following case for the MC68881: 
    **
    **	fmov.x	%fpi,%fpj	-->	fmov.x	%fpi,%fpj
    **	fmov.x	%fpj,%fpi
    **
    ** is also handled by the case just below for mov.X ....
    */
#endif


    /* Another mov.[bwl] case:
    **
    **	mov.X	D,P		-->	mov.X	D,P
    **	mov.X	P,D
    **
    **	where:
    **		D,P are registers
    **		   for P to be an EA,
    **			(Must be careful of P being an EA using D)
    **			(Must be careful of P having side effects )
    **
    ** For purposes of this improvement, "node" points to the second
    ** instruction (which may get deleted).
    */

	{
	    register NODE * back = node->back;
					/* previous instruction */

	    if (   node->op == back->op
		&& rands[0] == back->userdata.ud_rand[1]
		&& rands[1] == back->userdata.ud_rand[0]
		&& isreg( rands[0] )
		&& isreg( rands[1] )    /* P a reg for now */
	       )
	    {
		delete = true;		/* delete current node */
		break;			/* out of switch */
	    }
#if m68k
	    if (node->op == FMOV_X)
		break; /* don't subject fmov.x to the following
		       ** mov.X peephole optimizations */
#endif
	}


    /* Another mov.[bwl] case:
    **
    **	lea.l	P,B		-->	mov.X	P,A
    **	mov.X	B,A
    **
    **	where:
    **		A and B are A registers.
    */

	{
	    register NODE * back = node->back;
					/* previous instruction */
	    register RAND ** backrands = back->userdata.ud_rand;
					/* previous nodes operands */

	    if (   back->op == LEA_L
		&& rands[0] == back->userdata.ud_rand[1]
		&& isareg( rands[0] )
		&& isareg( rands[1] )
		&& isdead( rands[0]->ruses, node )
	       )
	    {
		
		/* remove operands from cache */

	        vkill(backrands[1], dest); /* operand is dead */
		vkill(rands[1],dest); /* LEA doesn't add to cache */

		setrand( back, 2, rands[1] );	/* modify lea instruction */

		delete = true;		/* delete current node */
		break;			/* out of switch */
	    }
	}

    /* Another mov.[bwl] case:
    **
    **	mov.X	P,D		-->	mov.X	P,Q
    **	mov.X	D,Q
    **
    **	where:
    **		D is a d register which is dead after the second move
    **		P is NOT a small numeric literal (we want to keep uses
    **			of move-quick here)
    **		Q is NOT an a register or condition codes are dead after
    **			the second move, and Q does not use D
    **
    ** For purposes of this improvement, "node" points to the second
    ** instruction (which may get deleted).
    */

	{
	    register NODE * back = node->back;
					/* previous instruction */
	    register RAND ** backrands = back->userdata.ud_rand;
					/* previous nodes operands */

	    if (   node->op == back->op
		&& rands[0] == back->userdata.ud_rand[1]
		&& back->userdata.ud_rand[0]->rtype != RT_IMMNUM
		&& isdreg( rands[0] )
		&& isdead( rands[0]->ruses, node )
		&& (   isdead( REG_CC, node )  || !isareg( rands[1] ) )
		&& !( rands[0]->ruses&backrands[1]->ruses )
	       )
	    {
		unsigned int size;		/* operand size */

		size = back->userdata.ud_op->op_ssize[0];

		
		/* remove operands from cache */

	        vkill(backrands[1], dest); /* operand is dead */
		vkill(rands[1],dest); /* new cache value added below */

		setrand( back, 2, rands[1] );	/* move operand */

		/* set new cache elements, as appropriate */

		if (backrands[1]->rtype == RT_REG)
			vset(backrands[1], backrands[0], size);

		/* Add first operand to cache if it's a register, but
		** only if the destination isn't.  Otherwise we get
		** duplicate entries.
		*/

		else if (backrands[0]->rtype == RT_REG)
			vset(backrands[0], backrands[1], size);

		delete = true;		/* delete current node */
		break;			/* out of switch */
	    }
	}

	break;				/* end of mov.[bwl] tests */
    /* clr.[bwl] cases.  We often find the following idiom in
    ** compiler generated code:
    **
    **	clr.l	%d0
    **	mov.w	x,%d0
    **
    ** Because of our (simplistic) live/dead analysis, %d0 looks
    ** like it's dead after the clr.l, whereas it's really used
    ** by the mov.w, in the sense that the high-order bits are
    ** retained.  We could muck with the live/dead uses() routine,
    ** but it's simpler just to retain the clr.[bwl] always.
    */

    case CLR_B:
    case CLR_W:
    case CLR_L:
	break;			/* don't delete */
    /* The value trace sometimes produces compare instructions of the
    ** form
    **		cmp.[bwl] X,X
    **
    ** followed by a conditional jump.  We can eliminate the cmp.[bwl]
    ** and possibly the jump if there's a condition based on zero.
    */
    
    case CMP_B:
    case CMP_W:
    case CMP_L:
	if (rands[0] == rands[1] && isbr(node->forw))
	    switch(node->forw->op)	/* check for branches we handle */
	    {
	    case BEQ:
	    case BEQ_B:
	    case BGE:
	    case BGE_B:
	    case BHS:
	    case BHS_B:
	    case BLE:
	    case BLE_B:
	    case BLS:
	    case BLS_B:
	    case BPL:
	    case BPL_B:
		/* for these cases, branch becomes unconditional */
		setop(node->forw, getOP(BR));
		delete = true;		/* and we delete the cmp */
		break;
	    
	    case BGT:
	    case BGT_B:
	    case BHI:
	    case BHI_B:
	    case BLT:
	    case BLT_B:
	    case BLO:
	    case BLO_B:
	    case BMI:
	    case BMI_B:
	    case BNE:
	    case BNE_B:
		{
		/* for these cases, branch vanishes */
		NODE	*tnode;		/* temporary pointer */

		tnode = node->forw;
		DELNODE(tnode);		/* discard the branch */
		delete = true;		/* handle cmp this way */
		break;
		}

	    default:			/* for other branches, don't bother */
		break;
	    }
	break;				/* other cmp situations:  do nothing */
    /* tst.[bwl] cases
**
**	mov.[bwl]	OP1,OP2		-->	mov.[bwl]	OP1,OP2
**	tst.[bwl]	OP2
**
** when the address mode for OP2 has no side effects and is not an A
** register.
*/
    case TST_B:
    case TST_W:
    case TST_L:
	switch( rands[0]->rtype )	/* make sure it's a valid type */
	{
	case RT_REG:
	    if (! isdreg( rands[0] ))	/* D register only */
		break;
#if m68k
	case RT_020:
#endif
	case RT_IREG:
	case RT_OREG:
	case RT_DBLIND:
	case RT_MEM:
	case RT_SMEM:
	case RT_LABEL:
	    {				/* type is okay */
		NODE * back = node->back;	/* look at predecessor */
		
		delete =		/* delete tst if proper mov instruction
					** precedes, and
					** operands match
					*/
		    (
			( node->op == TST_L && back->op == MOV_L )
		     || ( node->op == TST_W && back->op == MOV_W )
		     || ( node->op == TST_B && back->op == MOV_B )
		    )
		    && back->userdata.ud_rand[1] == rands[0];
	    }
	}
	break;				/* out of tst.[bwl] case */
    /* General case:  we can delete instructions for which there is a
    ** destination if the destination is a register whose contents are
    ** dead after the instruction, provided:
    **
    **	1) Condition codes are dead after the instruction, or
    **	2) The instruction doesn't set condition codes, or
    **	3) The instruction sets condition codes except when the
    **		destination is an A register, and the destination
    **		IS an A register.
    */

    default:				/* default case */
	if (   dnum != NODEST		/* there is a destination */
	    && isreg(trand = rands[dnum])
			 		/* it's a register */
	    && isdead(trand->ruses, node)
					/* it's dead after this inst. */
	    && (   isdead(REG_CC, node) /* condition codes are dead */
		|| (node->userdata.ud_flags & (UD_CC | UD_CCA)) == 0
					/* condition codes are never set */
		    || isareg(trand) &&
			    (node->userdata.ud_flags & UD_CCA) != 0
					/* we have an A register, and CC
					** aren't set for them
					*/
		)
	    )
	    delete = true;		/* we can delete this one */
	break;
    }	/* end switch */

    if (delete)				/* if we should delete node */
    {
	rmnode( node, pflag );
    }
    return (delete);			/* say whether we deleted something */
}

/* rmnode -- remove an instruction node; called by peep and vtrace */

rmnode( node, pflag )
NODE *node;
boolean pflag;
{
	TRACE(rmnode);
    DELNODE(node);			/* delete the node */
    ndisc++;			/* discarded an instruction */
    if (pflag)			/* if printing */
        (void) puts("~~~~~~ deleted");
}

/* getimmnum -- get immediate number
**
** This routine returns the actual numeric value from an immediate
** operand.
*/

static long
getimmnum(rand)
RAND * rand;				/* operand to get it from */
{
    long strtol();

	TRACE(getimmnum);
    if (rand->rtype != RT_IMMNUM)
	oerr("Not immediate number in getimmnum()");
    
    return( strtol( rand->rname+1, (char **) NULL, 0 ) );
   					 /* assume of the form "&nnnn" */
}
/* Print live/dead data for all instruction nodes */

#ifdef LIVEDEAD


void
prld()
{
    register NODE * p;
    void prinst();

    for (ALLN(p))			/* for all instruction nodes... */
    {
	PUTCHAR(CC);			/* write comment char */

	PRINTF("(live: 0x%.6X)", p->nlive); /* print live/dead data */
	prinst(p);			/* print instruction */
    }
    return;
}

#endif
