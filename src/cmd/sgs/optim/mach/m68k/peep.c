#ident	"@(#)peep.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* static char SCCSID2[] = "Module peep.c Version, 7.2 Creation date 85/08/26"; */
/*                          NOTE
**
**      This file is now included at the end of vtrace.c in order to
** gain access to the value cache and the routines which manipulate
** this cache.  2/9/84
*/

/*      MC680X0 family optimizer - File peep.c  */

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
**      mov.[bwl]       %R,%R
**
** We get called on a node-by-node basis.
*/

#ifndef LIVEDEAD
#define isdead(a,b) 0                   /* nothing looks dead */
#endif

extern	boolean	stkoffar[1024];

boolean                                 /* true if something deleted */
peep(node,pflag)
register NODE * node;                   /* node to examine now */
boolean pflag;                          /* print info as we go */
{
    register RAND ** rands;             /* operands for current node */
    unsigned int dnum;                  /* destination operand number */
    boolean noldchg = false;		/* true if delete doesn't change
					   live/dead info */
    boolean delete = false;             /* true to delete the node */
    RAND * trand;                       /* temporary operand pointer */
    long getimmnum();

    /* Assume live/dead information is correct, at least where we are.
    ** What we do depends on the current op.
    */
    /* do NOT bother if inline node */
    /* hoss #29 */
    if (isinline(node)) return (false);

    rands = node->userdata.ud_rand;     /* point at op's operands */
    dnum = node->userdata.ud_op->op_dnum; /* get destination operand number */

    switch(node->op)
    {
    case LINK_L:			/* link.l */
    case LINK_W:			/* link.w */
    case UNLK:				/* unlk */
	break;					/* never delete */

#ifdef m68k
    case FSCALE_S:                      /* Never delete this since it may be*/
	break;                          /* a boundary check to force sin/cos*/
					/* tan exceptions.  These appear to */
					/* never use their results.         */
#endif
#ifdef M68881
    case FMOVM:				/* fmovm */
#endif
    case MOVM_L:			/* movm.l */
    case MOVM_W:			/* movm.w */
	/* delete if null register list */
        if ( ( (trand = rands[0])->rtype == RT_IMMNUM ||
               (trand = rands[1])->rtype == RT_IMMNUM
             )
             && trand->rdisp == 0
           )
            delete = noldchg = true;       /* we should delete this */
        break;

    /* jsr followed by rts
    ** compiler generated code:
    **
    **  jsr	<ea>	 ------->	jmp	<ea>
    **  rts
    **
    ** This saves wasted return address save and restore
    */

    case RTS:
	{   register NODE * back = node->back;

	    if (back != NULL && back->op == JSR)
	    {	setop(back, getOP((OPCODE) JMP));	/* change JSR to JMP */
		delete = noldchg = true;
		}
	}
    break;

    /* clr.[wl] cases.  We often find the following idiom in
    ** compiler generated code:
    **
    **  clr.[wl]   %d0		or	clr.l	  %d0
    **  mov.b      x,%d0		mov.[bw]  %d0
    **
    ** Because of our (simplistic) live/dead analysis, %d0 looks
    ** like it's dead after the clr, whereas it's really used
    ** by the mov in the sense that the high-order bits are
    ** retained.  We could muck with the live/dead uses() routine,
    ** but it's simpler just to check the next instruction and
    ** assume we can delete the clr if it isn't one of the above cases.
    */

    case CLR_W:
    case CLR_L:
		{	register NODE * forw = node->forw;

			if (   (trand=rands[0])->rtype == RT_REG
				&& isdead(trand->ruses, node)
				&& ! (   (forw->op==MOV_B || forw->op==MOV_W)
				  	  && forw->userdata.ud_rand[1]==trand
				 	 )
		   	   )
				delete = true;
		}
	/* delete it if it clears a dead frame */
	if (     isdead(REG_FP, node)     /* frame pointer dead */ 
            	 && (((trand=rands[dnum])->ruses & REG_FP) != 0)
                                             /* it uses frame pointer */
		 && (   trand->rtype == RT_IREG
		     || trand->rtype == RT_OREG
			 || trand->rtype == RT_INDX
#ifdef m68k
			 || (   trand->rtype == RT_EXTINDX
				 && !(trand->rflags & rfmemind)
				)
#endif
		    ) 			     /* variable is on the frame */
            	 && isdead(REG_CC, node) /* condition codes are dead */
            )
            delete = true;              /* we can delete this one */
	break;

    /* 			cmp.[bwl]   cases
    **
    ** 1) If condition codes are dead then no reason to keep the compare.
    **
    ** 2) The value trace sometimes produces compare instructions of the
    ** form
    **          cmp.[bwl] X,X
    **
    ** followed by a conditional jump.  We can eliminate the cmp.[bwl]
    ** and possibly the jump if there's a condition based on zero.
    */
    
    case CMP_B:
    case CMP_W:
    case CMP_L:
	if (isdead(REG_CC, node))
	    {	delete = true;
		break;
	    }
        if (rands[0] == rands[1] && isbr(node->forw))
            switch(node->forw->op)      /* check for branches we handle */
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
                delete = true;          /* and we delete the cmp */
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
                NODE    *tnode;         /* temporary pointer */

                tnode = node->forw;
                DELNODE(tnode);         /* discard the branch */
                delete = true;          /* handle cmp this way */
                break;
                }

            default:                    /* for other branches, don't bother */
                break;
            }
        break;                          /* other cmp situations:  do nothing */
#ifdef m68k
    /* extb.l case
**
**	logical comparison operator assignments create longword values.  But if
**	only the low byte is needed then we can delete the extb.l.  An example:
**
**		scc.b	%dn
**		neg.b	%dn
**		extb.l	%dn		-->		REMOVE
**		...
**		mov.b	%dn,<ea>
**		[ %dn dead here ]
**
*/

	case EXTB_L:
	{	register NODE *nextn = node;			/* forward scan node */
		register rused = rands[0]->ruses;		/* %dn used in extb.l */
		register RAND *rnd;						/* rand work pointer */

		/* skip if EXT is needed for condition codes */
		if (!isdead(REG_CC, node))
			break;

		/* scan till branch hit or register high end used or register dead */

		while ( (nextn=nextn->forw) != NULL 
				&& !isbr(nextn) 
				&& nextn->op != GENERIC
			  )
		  {	if (islabel(nextn)) continue; /* labels have no live/dead info */
			if ((rnd=nextn->userdata.ud_rand[0]) == rands[0])
										/* 1st operand is our register */
			  if (nextn->userdata.ud_op->op_ssize[0] > 1)
										/* and it's more than a byte */
					break;
				else; 					/* not more than a byte */
			  else 	 					/* not our register directly */
				if (   rnd != RNULL
										/* there is an operand */
					&& (rnd->ruses & rused) != 0 )
										/* addressing mode uses our register */
					break;
		   	if ((rnd=nextn->userdata.ud_rand[1]) == rands[0])
										/* 2nd operand is our register */
			  if (nextn->userdata.ud_op->op_ssize[1] > 1)
										/* and it's more than a byte */
					break;
				else; 					/* not more than a byte */
			  else 	 					/* not our register directly */
				if (   rnd != RNULL
										/* there is an operand */
					&& (rnd->ruses & rused) != 0 )
										/* addressing mode uses our register */
					break;
			if (isdead(rused,nextn))
			  {	delete = true;		/* extb.l not needed */
				break;
				}
			} /* end while */

		break;
		}	/* end case */
#endif
    /* tst.[bwl] cases
**
**      xxx.[bwl]       {...,}destOP        -->     xxx.[bwl]   {...,}destOP
**      tst.[bwl]       destOP
**
** when the address mode for destOP has no side effects and is not an A
** register and instruction xxx sets the condition codes.
**
** also:
**
**	mov.[bwl]	OP1,OP2		-->	mov.[bwl]	OPT1,OP2
**	tst.[bwl]	OP1
**
** when the address mode for OP1 has no side effects and OP2 is not an
** A register.  The value trace doesn't catch this case for a register OP1.
*/
    case TST_B:
		if ((rands[0]->ruses & REG_SP)
				&& rands[0]->rtype == RT_OREG
				&& rands[0]->rdisp < 0)
			break;						/* jcl: keep stack probes */
    case TST_W:
    case TST_L:
		if (isdead(REG_CC,node))
		  {	delete = true;
			break;
			}
        switch( rands[0]->rtype )       /* make sure it's a valid type */
        {
        case RT_REG:
#ifndef m68k
            if (! isdreg( rands[0] ))   /* D register only */
                break;
#else
        case RT_EXTINDX:
#endif
        case RT_IREG:
        case RT_OREG:
        case RT_INDX:
        case RT_MEM:
        case RT_LABEL:
            {                           /* type is okay */
                NODE * back = node->back;       /* look at predecessor */
				int	dest;						/* destination operand index */
                
                delete =                /* delete tst if proper mov instruction
                                        ** precedes, and
                                        ** operands match
                                        */
                    (	node->op == TST_L && back->op == MOV_L
                     || node->op == TST_W && back->op == MOV_W
                     || node->op == TST_B && back->op == MOV_B
                    )
                && 
					( 	back->userdata.ud_rand[1] == rands[0]
		     		 || back->userdata.ud_rand[0] == rands[0]
					)
				&&
			  		! isareg(back->userdata.ud_rand[1]);

#if 0
/*DEBUG*/ printf("!! delete %d, isareg(rands[0]) %d, back->userdata.ud_flags&UD_CC %d\n",
					delete, isareg(rands[0]), back->userdata.ud_flags&(UD_CC|UD_CCA));
		printf("  back->userdata.ud_op->op_dnum %d, back->..->op_dsize %d\n",
				dest=back->userdata.ud_op->op_dnum, back->userdata.ud_op->op_dsize);
		printf("  node->userdata.ud_op->op_ssize[0] %d\n",
			     node->userdata.ud_op->op_ssize[0]);
		printf("  back->userdata.ud_rand[dest] %x, rands[0] %x\n",
				back->userdata.ud_rand[dest], rands[0]);
#endif
				/* now catch instruction 'xxx' setting condition codes */
				if (   !delete
												/* not already deleted */
					&& (   (   (back->userdata.ud_flags & UD_CCA)
							&& !isareg(rands[0])
						   )
												/* not A REG and non A CC */
						|| (back->userdata.ud_flags & UD_CC)
												/* previous sets CC */
					   )
					&& (dest=back->userdata.ud_op->op_dnum) != NODEST
												/* and has a destination */
					&& back->userdata.ud_op->op_dsize ==
							node->userdata.ud_op->op_ssize[0]
												/* and is same size */
					&& back->userdata.ud_rand[dest] == rands[0]
												/* destination same operand */
				   )
					delete = true;
            }
        }
        break;                          /* out of tst.[bwl] case */
#ifdef M68881

	/* Processing for FTEST %fpn */

/* If a previous instruction has set or tested %fpn then there is no need
** to redo the test.  Scan backwards for the last floating point destination.
*/
	case FTEST_D:
	case FTEST_S:
	case FTEST_X:
		if (isfpreg(rands[0]))
		  {	/* see if we can get rid of this ftest */
			NODE *nextn = node;				/* backward scan node */
			RAND	*rnd;					/* rand work pointer */

			while ( (nextn=nextn->back) != NULL 
					&& !islabel(nextn)
					&& nextn->op != GENERIC
				  )
			  {	/* look for second operand being our %fpn */
			 	if ((rnd=nextn->userdata.ud_rand[1]) != RNULL)
				  {	if (rnd == rands[0])
				  	 {	/* delete this ftest */
						delete = true;
						break;
						}
					if (isfpreg(rnd))
						break;		/* not our %fpn in fp CCR */
					}
				else /* not 2 operands */
				/* if first operand is %fpn we lose */
				if (   (rnd=nextn->userdata.ud_rand[0]) != RNULL
					&& isfpreg(rnd)
				   )
					break;

				} /* break while */

			} /* break if */
		break;
#endif
  /* Processing for the mov family
    **  for mov.[bwl]
    **
    **  lea.l   P,B             -->     lea.X   P,A
    **  mov.X   B,A
    **
    **  where:
    **          A and B are A registers.
    */
    case MOV_B:
    case MOV_W:
    case MOV_L:

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

                setrand( back, 2, rands[1] );   /* modify lea instruction */
                vset(rands[1], backrands[0], -1);	/* add "new" LEA to cache */

                delete = true;          /* delete current node */
                break;                  /* out of switch */
            }
        }

  
    /* A mov case utilizing the commutative nature of add/mult/or/and:
    **
    **  commop.X   	A,B             -->     commop.X   B,A
    **  mov.X   	B,A
    **
    **  where:
    **          A and B are registers
    **          B is dead after the move.
	**			X is identical size
    **
    ** For purposes of this improvement, "node" points to the second
    ** instruction (which may get deleted).
    */
#ifdef M68881
    /* Now some of the fmov family joins the processing */
    case FMOV_S:
    case FMOV_D:
    case FMOV_X:
#endif

        {
            register NODE * back = node->back;
                                        /* previous instruction */

            if (   isreg(rands[0])
				&& isreg(rands[1])	
										 /* both operands registers */
				&& isdead(rands[0]->ruses, node)
										/* first operand no longer needed */
				&& (back->userdata.ud_op->op_flags & OF_COMUOP)
										/* operation is communative */
#ifdef M68881
				&& (isfpreg(rands[0]) ^ isfpreg(rands[1])) == 0
										/* both registers of same type */
#endif
 				&& node->userdata.ud_op->op_dsize == 
								back->userdata.ud_op->op_dsize
										/* both instructions of same size */
				&& rands[0] == back->userdata.ud_rand[1]
				&& rands[1] == back->userdata.ud_rand[0]
										/* operands are swapped */
				)
			  { /* swap operands of first instruction */
				setrand(back, 1, rands[0]);	/* second to first */
				setrand(back, 2, rands[1]);	/* first to second */
				delete = true;			/* delete the move */
				break;					/* exit switch */
				}
		}
  
    /* Another mov.[bwl] case:
    **
    **  mov.X   P,D             -->     mov.X   P,Q
    **  mov.X   D,Q
    **
    **  where:
    **          D is a d register which is dead after the second move
    **          Q is NOT an a register or condition codes are dead after
    **                  the second move, and Q does not use D
    **
    ** For purposes of this improvement, "node" points to the second
    ** instruction (which may get deleted).
    */
#ifdef M68881
    /* Now the rest of the fmov family joins the processing */
    case FMOV_B:
    case FMOV_W:
    case FMOV_L:
#endif

        {
            register NODE * back = node->back;
                                        /* previous instruction */
            register RAND ** backrands = back->userdata.ud_rand;
                                        /* previous nodes operands */

            if (   node->op == back->op
                && rands[0] == back->userdata.ud_rand[1]
#ifdef M68881
		&& ( isfpreg(rands[0]) || isdreg(rands[0]) )
#else
                && isdreg( rands[0] )
#endif
                && isdead( rands[0]->ruses, node )
                && (   isdead( REG_CC, node )  || !isareg( rands[1] ) )
                && !( rands[0]->ruses&backrands[1]->ruses )
               )
            {
                unsigned int size;              /* operand size */

                size = back->userdata.ud_op->op_ssize[0];

                
                /* remove operands from cache */

                vkill(backrands[1], dest); /* operand is dead */
                vkill(rands[1],dest); /* new cache value added below */

                setrand( back, 2, rands[1] );   /* move operand */

                /* set new cache elements, as appropriate */

                if (backrands[1]->rtype == RT_REG)
                        vset(backrands[1], backrands[0], size);

                /* Add first operand to cache if it's a register, but
                ** only if the destination isn't.  Otherwise we get
                ** duplicate entries.
                */

                else if (backrands[0]->rtype == RT_REG)
                        vset(backrands[0], backrands[1], size);

                delete = true;          /* delete current node */
                break;                  /* out of switch */
            }
        }

    /* For the mov.[bwl] family, we can delete all moves where the
    ** source and destination is the same, provided:
    **
    **  1) condition codes are dead thereafter
    **
    ** Remember that moves to A registers don't affect condition codes.
    */

        if (   rands[0] == rands[1]
            && (isareg(rands[0]) || isdead(REG_CC, node))
            )
        {
            delete = true;              /* we can delete this one */
            break;
        }
    /* Another mov.[bwl] case:
    **
    **  mov.X   D,P             -->     mov.X   D,P
    **  mov.X   P,D
    **
    **  where:
    **          D,P are registers
    **             for P to be an EA,
    **                  (Must be careful of P being an EA using D)
    **                  (Must be careful of P having side effects )
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
                delete = true;          /* delete current node */
                break;                  /* out of switch */
            }
        }

        				/* end of mov.[bwl] tests */

	/* Fall into the general case for moves */
    /* General case:  we can delete instructions for which there is a
    ** destination if the destination is a register whose contents are
    ** dead after the instruction, provided:
    **
	**  1) The destination is not the stack pointer 
    **  2) Condition codes are dead after the instruction, or
    **  3) The instruction doesn't set condition codes, or
    **  4) The instruction sets condition codes except when the
    **          destination is an A register, and the destination
    **          IS an A register.
    **
    ** We can also delete instructions for which the destination is the
    ** frame pointer if the frame pointer is dead after the instruction
    ** and cases 1) or 2) above hold and there have been no addresses
	** taken of items on the frame.
    */

    default:                            /* default case */
	/* first try pruning actions to dead registers */
        if (   dnum != NODEST           /* there is a destination */
            && isreg(trand = rands[dnum])
                                        /* it's a register */
            && isdead(trand->ruses, node)
                                        /* it's dead after this inst. */
	    && (trand->ruses & REG_SP) == 0
					/* it's not the stack register */
            && (   isdead(REG_CC, node) /* condition codes are dead */
                || (node->userdata.ud_flags & (UD_CC | UD_CCA)) == 0
                                        /* condition codes are never set */
                || isareg(trand) && (node->userdata.ud_flags & UD_CCA) != 0
                                        /* we have an A register, and CC
                                        ** aren't set for them
                                        */
               )
           )
	  { delete = true;		/* we can delete this one */
	    switch (node->op)
	    { case MOV_B:
	      case MOV_W:
	      case MOV_L:
	      case FMOV_B:
	      case FMOV_W:		/* kill value cache if is a move */
	      case FMOV_L:
	      case FMOV_S:
	      case FMOV_D:
	      case FMOV_X:
	      case LEA_L:
		vkill(trand, dest);
		break;
	    }
	  }

	/* now try pruning actions to a dead frame pointer */
	else if ( !adrframe		/* no addresses taken of frame item */
		 && isdead(REG_FP, node)     /* frame pointer dead */ 
            	 && dnum != NODEST           /* there is a destination */
            	 && (((trand=rands[dnum])->ruses & REG_FP) != 0)
                                             /* it uses frame pointer */
		 && (   trand->rtype == RT_IREG
		     || trand->rtype == RT_OREG
		     || trand->rtype == RT_INDX
#ifdef m68k
			 || (   trand->rtype == RT_EXTINDX
				 && !(trand->rflags & rfmemind)
				)
#endif
		    ) 			     /* it is a safe addresing mode */
            	 && (   isdead(REG_CC, node) /* condition codes are dead */
                     || (node->userdata.ud_flags & (UD_CC | UD_CCA)) == 0
                                             /* condition codes are never set */
                    )
                )
            	 delete = true;              /* we can delete this one */
        	 break;
    }   /* end switch */

    if (delete)                         /* if we should delete node */
    {
        rmnode( node, pflag );
    }
    return (delete && !noldchg);        /* flag if live/dead info changed */
}

/* rmnode -- remove an instruction node; called by peep and vtrace */

rmnode( node, pflag )
NODE *node;
boolean pflag;
{
	if (node->forw != NULL)		/* if not last node */
    	DELNODE(node);                  /* delete the node */
	  else node->back->forw = NULL;	/* delete last node */
    ndisc++;                    	/* discarded an instruction */
    if (pflag)                  	/* if printing */
        (void) puts("~~~~~~ deleted");
}

/* getimmnum -- get immediate number
**
** This routine returns the actual numeric value from an immediate
** operand.
*/

long
getimmnum(rand)
RAND * rand;                            /* operand to get it from */
{
    long strtol();

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

    for (ALLN(p))                       /* for all instruction nodes... */
    {
        PUTCHAR(CC);                    /* write comment char */

        PRINTF("(live: %#.8x)", p->nlive); /* print live/dead data */
        prinst(p);                      /* print instruction */
    }
    return;
}

#endif
