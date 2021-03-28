#ident	"@(#)rmframe.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	"@(#)  rmframe.c  1.2"	*/
/*																	*/
/*					Copyright (c) 1987 by Motorola Inc.				*/
/*																	*/

#include "optim.h"

extern int numfold, recurfold;		/* jcl: user control of ftn folding */
extern int keeplink;

void stash();
void fold();

				/* newproc() - processing for a new function */

/* 1)	Remove the context save and restore (f)movm from after/before
** 		LINK/UNLK instructions.  The link/unlk serve as prolog epilog
** 		indicators and we don't want these movm's to screw up live/dead
** 		information.
**
** 2)	If first pass then build call list for all functions called.
**
** 3)	If first pass and switch encountered mark function as unfoldable.
**
*/

void
newproc()
{
	register NODE	*node;			/* node scan pointer */
	register NODE	*link;			/* forw/back link */
	register CALL	*cptr;			/* call list pointer */

	for (ALLN(node))
	  switch(node->op)
		{	case LINK_L:		/* link.l */
			case LINK_W:		/* link.w */
				/* we expect fmovm and then movm after these */
				if ((link=node->forw)->op == MOVM_L)
					DELNODE(link);	/* remove movm */
#ifdef M68881
				if ((link=node->forw)->op == FMOVM)
					DELNODE(link);	/* remove fmovm */
#endif
				break;

			case UNLK:
				/* we expect fmovm and movm to preceed the unlk */
				if ((link=node->back)->op == MOVM_L)
					DELNODE(link);	/* remove movm */
#ifdef M68881
				if ((link=node->back)->op == FMOVM)
					DELNODE(link);	/* remove fmovm */
#endif
				break;

			case BSR_B:
			case BSR:
			case JSR:
				if (!Pass2)
				  {	/* search for name already in list */
					for (cptr=Curproc->calllist;
							cptr && strcmp(cptr->name, node->op1);
								cptr=cptr->next);
					if (cptr == NULL)
					  {	/* not in list so add it */
						cptr = (CALL *)xalloc(sizeof(CALL) + strlen(node->op1));
						(void) strcpy(cptr->name, node->op1);
						cptr->next = Curproc->calllist;
						Curproc->calllist = cptr;		/* chain in */
						}
					}
				break;

			case SWITCH:
				if (!Pass2)
					Curproc->foldable = 0;
				break;

				} /* end switch */

} /* newproc() */

			/* writefunc() - write out function and strip frame */

/*  This function prints all nodes sequentially while changing all frame
**  pointer references to the stack pointer.  We do this by recomputing
**  all frame variable offsets in terms of the current stack pointer
**  location.  Since the stack moves up
**  and down as arguments are pushed and popped we keep continous track
**  of the stack bias.  The bias is always included as part of the new
**  offset computation for frame variable references.
**
**  The compiler generates code to always free parameters from the stack
**  after a function call.  This routine delays any such adjustments 
**  (leaving a bias) until absolutely necessary for proper execution.
**  This enables us to sometimes fold two or more such adjustments
**  (normally add instructions) into a single instruction.  The times when
**  any stack bias must be undone are:
**
**		1 - any conditional branch statement
**		2 - any branch to a block which must start with no bias
**		3 - any switch statement
**		4 - anytime a zero displacement could have been used off of the
**		    stack but isn't because of a bias being in effect.
**
**  The last item isn't mandatory but is done for effeciency reasons since
**  the high-level optimizer attempts to locate oft-used frame items on
**  the frame bottom specifically for this fast access.
**
**  The picture of the calling environment is as follows:
**
**    Passed to us from the compiler        As created by this function
**
**        ---------------                        --------------
**            parms                                 parms
**        ---------------                        --------------
**           ret addr                              ret addr*
**        ---------------                        --------------
**          frame link                           reg save area
**   fp-> ---------------                        --------------
**           frame (save area and locals)          local vars
**        ---------------                        --------------
**           stack                                   stack
**   sp-> ---------------                   sp-> --------------
**
**  * no return address allocated for functions that are folded
** 
**  The output from the compiler bases parameters as +offset from the frame
**  and local variables as -offset from the frame.  As can be seen our new
**  scheme will always create +offset as based on the stack pointer.
**
**  LOGIC:
**
**  Before the function's statements can be sequentially printed out the 
**  starting bias for each block must be computed by following the logical 
**  flow of the function.  The flow graph does not connect for switches but
**  since switches can never occur during argument pushes on the stack we
**  know that the bias at a switch statement must be zero, and therefore
**  the bias of all blocks reached by a switch statement must also be zero.
**
**  First we mark the initial block and all blocks reached by switches as
**  requiring zero starting bias.  Then we follow the flow graph dynamically
**  from all such blocks computing and recording all block start biases as we
**  enter a new block.
**  
**  Bias folding (inhibiting stack increments after a function call) can only
**  occur within a block or a logically sequential chain of blocks that 
**  have no outside branches into them. For this reason a count of all 
**  references to blocks is done early on so such an isolated chain can be 
**  recognized.
**
**  After all starting block biases have been computed all function blocks
**  are sequentially processed and their associated statements printed (or
**	stashed in the fold buffer if the folding flag is true.)  Each
**  reference to a frame variable is altered to use the proper stack 
**  offset before being printed.
*/

static int regsave;				/* size of register save area */
void compbias();				/* function to compute biases */
void issuefunc();				/* function to print statements */
static int stashit;				/* global flag for this module - stash flag */

void
writefunc(foldflg)
int		foldflg;				/* stash function as folded in memory */
{
	int		bias;				/* current stack usage byte count */
	NODE	*node;				/* scanning node pointer */
	int		i;					/* work variable */
	REF		*ref;				/* data label reference pointer */
	BLOCK	*b;					/* block pointer */
	BLOCK	*tb;				/* second block pointer */

/* block marked flags */
#define bmzerobias		1		/* start zero bias with this block */
#define bmhit			2		/* this block has been processed */
#define bmnoelbias		4		/* this block must end locally unbiased */
#define bmstacking		8		/* this block's initial stacking flag */

	stashit = foldflg;			/* set our internal global fold flag */
	if (stashit)
		Curproc->funclive = funclive;	/* remember function's hot regs */

	/* clear all work areas in the flow graph blocks */
	for (ALLB(b, b0.next))
	  {	b->indeg = 0;			/* count of blocks branching here */
		b->marked = 0;			/* flag for forced zero bias start */
		b->startbias = 0;		/* initial bias for block */
		b->localbias = 0;		/* initial local bias for block */
		}

	/* count times each block referenced by a branch */
	for (ALLB(b, b0.next))
	  {	if ((tb=b->nextl) != NULL)	/* left side block */
			tb->indeg++;		/* count block as referenced */
		if ((tb=b->nextr) != NULL)	/* right side block */
			tb->indeg++;		/* count block as referenced */
		}

	/* mark first block and all switch nodes as zero biased start */
	b0.next->marked |= bmzerobias;	/* first block always zero biased */
	for (ALLB(b, b0.next))
	  for (node=b->firstn; islabel(node); node=node->forw)
		if (!ishl(node))		/* check all soft labels in block */
		  {	/* see if label referenced by switch statement */
			for (ref=r0.nextref; 
					ref!=NULL && strcmp(ref->lab, node->ops[0]);
						ref=ref->nextref);
			if (ref != NULL)	/* referenced by switch ? */
			  {	b->marked |= bmzerobias;	/* mark this as zero bias start */
				b->indeg++;		/* mark reference to this block */
				}
			}

	/* flow from all zero biased starting blocks and calculate
	** the starting biases for all other blocks.
	*/
	regsave = regsavesz();			/* find register save area size */
	for (ALLB(b, b0.next))
	  if (b->marked & bmzerobias)	/* if zero bias required ... */
		/* compute bias starting at zero with no local bias and not stacking */
		compbias(b, 0, 0, false);

	/* now print (or stash) the function using the precomputed biases */
	for (ALLB(b, b0.next))
		issuefunc(b);				/* process each block's statements */

} /* writefunc() */

void
compbias(b, bias, lbias, stacking)
BLOCK		*b;						/* block to process */
int			bias;					/* starting bias for block */
int			lbias;					/* starting local bias for block */
boolean		stacking;				/* stack pushes in progress */
{
	NODE	*node;				/* scanning node pointer */
	RAND	*rand;				/* rand pointer */
	int		i;					/* work variable */
	int		j;					/* work variable */
	OP		*op;				/* instruction information pointer */
	RAND	**rands;			/* pointer to instruction operands */

#if 0
/*DEBUG*/printf("#! compbias blk %d, indeg %d, bias %d, lbias %d, stacking %d, marked %lx, startbias %d, localbias %d\n",
	 b->index,b->indeg,bias,lbias,stacking,b->marked,b->startbias,b->localbias);
#endif

	/* check for block already processed.  verify if so */
	if (b->marked & bmhit)
	  {	/* make sure biases are the same (sanity check) */
		if (   b->startbias != bias
								/* bias not same? */
			|| ((b->marked & bmstacking) != 0) != stacking
								/* stacking flag not identical */
			|| b->localbias != lbias
								/* local bias not same */
		   )
			oerr("compbias: inconsistent block start");
		return;
		}

	/* this is the data we need for this block's processing */
	b->marked |= bmhit;			/* flag this block processed */
	b->startbias = bias;		/* remember initial bias */
	b->localbias = lbias;		/* local bias */
	if (stacking)
		b->marked |= bmstacking; /* stacking flag */

	/* if this block does not uniquely chain to the next then we must
	** inhibit passing any local bias folding (adds to the stack)
	** to the next block.
	*/
	if (   b->nextr != NULL
								/* we have conditional branch at end */
		|| b->nextl == NULL
								/* this is the last block or a switch */
		|| (   b->nextl != NULL
			&& b->nextl->indeg != 1
								/* next block has >1 references */
		   )
	/* jcl: next block branches unconditionally back to this block */
		|| (b->nextl->nextr == NULL
			&& b->nextl->nextl != NULL
			&& b->nextl->nextl->index == b->index)
	   )
		b->marked |= bmnoelbias;	/* must remove local bias before end */
	
	/* scan instructions in this block tracking stack bias */
	for (node=b->firstn; ;node=node->forw)	/* scan block's instructions */
	  {	if ( node->userdata.ud_nattr & UDN_INLINE && 
		( !node->userdata.ud_nattr & UDN_INVOLATILE ) )
			if ( node == b->lastn )
				break;			/* done if this is last statement */
			else
				continue;
		op = node->userdata.ud_op;		/* instruction data */
		rands = node->userdata.ud_rand;	/* operands pointer */
		switch(node->op)
		  { case SWITCH:
			case LABEL:
			case LINK_L:
			case LINK_W:
			case UNLK:
					break;				/* ignore these */
		  	case LEA_L:
				if (rands[1]->ruses != REG_SP)	/* if not to stack pointer */
					goto dostmt;				/* not of interest here */
				if (   rands[0]->rtype != RT_OREG
											/* not disp16(%sp) */
					|| rands[0]->ruses != REG_SP
											/* not disp16(%sp) */
				   )
					oerr("compbias: illegal lea to %sp");
				j = rands[0]->rdisp;		/* get displacement */
				if (j > 0)	/* freeing parms? */
					goto addtostack;		/* yes */
				else
				if (j < 0)              	/* allocate parms */
					goto subtostack;		/* process stack subtract */
				break;						/* ?? */

			case ADD_W:
			case ADD_L:
				if (   rands[1]->ruses != REG_SP
											/* stack not destination */
					|| rands[1]->rtype != RT_REG
											/* not to a register */
				   )
					goto dostmt;			/* not of interest here */
				if (rands[0]->rtype != RT_IMMNUM)
					oerr("compbias: illegal add to %sp");
				j = rands[0]->rdisp;		/* get displacement */
				if (j < 0)					/* negative number? */
				  {	j = -j;					/* actually a subtract */
					goto subtostack;		/* consider as sub */
					}
		addtostack:
				bias -= j;              	/* absolute bias decreases */
				/* only fold the bias locally if the absolute bias
				** is zero (e.g. no nested function call stacking)
				*/
				if (bias == 0)				/* no nested stacking? */
				  {	lbias += j;				/* no, fold this pop */
					/* mark next statement as start of local bias */
					node->forw->userdata.ud_nattr |= UDN_LOCALBIASS;
					stacking = false;		/* not stacking mode */
					}
				break;						/* done with this statement */

			case SUB_W:
			case SUB_L:
				if (   rands[1]->ruses != REG_SP
											/* stack not destination */
					|| rands[1]->rtype != RT_REG
											/* not to a register */
				   )
					goto dostmt;			/* not of interest here */
				if (rands[0]->rtype != RT_IMMNUM)
					oerr("compbias: illegal sub to %sp");
				j = rands[0]->rdisp;		/* get valud */
				if ( j < 0)					/* negative number? */
				  {	j = -j;				/* invert */
					goto addtostack;		/* consider as sub */
					}
		subtostack:
				bias += j;              	/* absolute bias increases */
				stacking = true;			/* we are stacking */
				break;						/* we are through with this */

			default:
		dostmt:
				/* scan statement processing operands looking for
				** frame pointer basing or stack pointer adjustments.
				*/
				for (i=0;  i<MAXOPS && (rand=rands[i])!=RNULL; i++)
				  {
#ifdef M68881
				  	/* hack to insure that a floating-point operation
					** with two floating-point registers always has
					** a '.x' size.  This is to keep the assembler happy.
					*/
					if (   i == 1
								/* this is the second operand */
						&& isfpreg(rand)
								/* and it is floating-point register */
						&& isfpreg(rands[0])
								/* and the first oprand is too */
					   )
						/* the opposite field has the '.x' instruction */
						setop(node, getOP(node->userdata.ud_op->op_opposite));
#endif
				  	/* if operand alters stack then adjust bias */
 					if ((rand->ruses & REG_SP) &&	/* jcl: pass stack probe */
 					      !(rand->rtype == RT_OREG &&
 							node->op == TST_B))
					  if (rand->rtype != RT_DECR &&	/* must be -(%sp) */
					      rand->rtype != RT_INCR) 	/* or (%sp)+ */
							oerr("compbias: illegal use of %sp");
					  else
						{	if (i != op->op_dnum)	/* not destination? */
								j = op->op_ssize[i];	/* source size */
							else
								j = op->op_dsize;	/* destination size */
							if (j==1)
								j = 2;			/* never byte from stack */
							if (rand->rtype == RT_DECR) {
								bias += j;
								stacking = true;
							} else {
								bias -= j;
								/* jcl */
								stacking = bias != 0;
							}
						} /* end %sp in ea check */

					/* now see if frame involved in operand */
					if ((rand->ruses & REG_FP))
					  {	/* frame involved, compute change to stack */
						if (   rand->rtype != RT_OREG
											/* disp16(%a) */
							&& rand->rtype != RT_INDX
											/* disp8(%a,%d*scl) */
							&& (   rand->rtype != RT_EXTINDX
											/* disp>8(%a,%d*scl) */
								|| (rand->rflags & rfmemind)
											/* no memory indirect */
							   )
							&& rand->rtype != RT_IREG
											/* not (%fp) */
						   )
							oerr("compbias: illegal ea using %fp");

						/* Compute new stack offset.  
						** For parameters of unfolded functions it is
						**
						**	lbias + bias + framesz + regsave + olddisp - 4
						**
						** For parameters of folded functions it is
						**
						**	lbias + bias + framesz + regsave + olddisp - 8
						**
						** For local variables it is:
						**
						**	lbias + bias + framesz + olddisp
						*/
						j = rand->rdisp + lbias + bias + framesz;
						if (rand->rdisp > 3)		/* parameter? */
							j += regsave + (stashit ? -8 : -4);

						/* If this stack offset would have been zero
						** with no stack bias then get rid
						** of bias and rebuild this instruction.
						*/
						if (   lbias
											/* there is a local bias */
							&& bias == 0
											/* there would be no total bias */
							&& !stacking
											/* not stacking parms */
							&& rand->rindx == 0
											/* no index register used */
							&& (j == lbias) 
											/* bias and offset are same */
						   )
						  {	/* force zero biasing before this instruction */
						  	node->userdata.ud_nattr |= UDN_ZEROBIAS;
							lbias = 0;
							}
						
						} /* end frame involved */

					} /* end operand processing */

				/* special case for PEA_L since it alters stack implicitly */
				if (node->op == PEA_L)
				  {	bias += 4;		/* address added to stack */
					stacking = true;	/* stacking mode on */
					}

				break;

				} /* end switch on node->op */
#if 0
/*DEBUG*/printf("# lbias %d, bias %d, stacking %d ",lbias,bias,stacking);
/*DEBUG*/prinst(node);
#endif

			if (node == b->lastn)
				break;			/* done if this is last statement */

			} /* end for scan instructions */

	/* if we have local bias and are to end with none ... */
	if (lbias && (b->marked & bmnoelbias))
	  {	/* if stacking parms we must force local bias to clear before
		** the start of the parameter stacking.
		*/
		if (stacking) 				/* stacking parms */
	  	  {	/* search back till we find where local bias started */
	  		while (node != NULL && !(node->userdata.ud_nattr & UDN_LOCALBIASS))
		  		node = node->back;		/* go back */
			if (node == NULL)
				oerr("compbias: lbias clear failed");
			node->userdata.ud_nattr |= UDN_ZEROBIAS; /* force zero bias here */
#if 0
/*DEBUG*/printf("### UDN_ZEROBIAS set "); prinst(node);
#endif
			}
		/* if transfer is to a non-local block then stacking flag
		** on if there is non-local bias.
		*/
		if (b->marked & bmnoelbias)
			stacking = bias != 0;	/* flag on if parameters on stack */

		lbias = 0;					/* local bias always zero at this point */
		}
	else
		stacking = bias != 0;		/* for entry to non-local chain blocks */

	/* now invoke all descendant blocks */
	if (b->nextl != NULL)
			compbias(b->nextl, bias, lbias, stacking); /* to the left */
	if (b->nextr != NULL)
			compbias(b->nextr, bias, lbias, stacking); /* to the right */

} /* compbias() */

					/* issuefunc() - print/stash function statements */

/*  Scan each block and print out the statements in it.  We scan all operands
**  changing the frame offsets to stack offsets.  The bias computations
**  track exactly the same as when compbias() scanned this block.
*/

void pbias();
void adjbias();
#define DIRUP	1
#define DIRDOWN 0
#define FREELBIAS()	{adjbias(lbias,DIRUP);lbias=0;}

void
issuefunc(b)
BLOCK		*b;					/* block to process */
{
	int		bias;				/* current stack usage byte count */
	int		lbias;				/* local block bias */
	NODE	*node;				/* scanning node pointer */
	RAND	*rand;				/* rand pointer */
	int		i;					/* work variable */
	int		j;					/* work variable */
	char	*instr;				/* input string pointer */
	char	*outstr;			/* output statement build pointer */
	OP		*op;				/* instruction information pointer */
	RAND	**rands;			/* pointer to instruction operands */
	char	stmt[80];			/* image for statement build */
	boolean	stacking;			/* indicates parameter stacking in progress */
	PROC	*pptr;				/* PROC work pointer */

	/* setup initial bias, local bias and stacking flag */
	stacking = (b->marked & bmstacking);	/* stacking in progress flag */
	bias = b->startbias;				/* initial bias */
	lbias = b->localbias;				/* local bias */

	/* now scan instructions in this block and print them out */
	for (node=b->firstn; ;node=node->forw)	/* scan block's instructions */
	  {	op = node->userdata.ud_op;		/* instruction data */
		rands = node->userdata.ud_rand;	/* operands pointer */

		/* if our first pass forces zero bias here handle that now */
		if (node->userdata.ud_nattr & UDN_ZEROBIAS)
			if ( node->userdata.ud_nattr & UDN_INLINE && 
			( !node->userdata.ud_nattr & UDN_INVOLATILE ) )
				lbias = 0;
			else
				FREELBIAS();			/* rid local bias */

		switch(node->op)
		{	case LINK_L:		/* link.l */
			case LINK_W:		/* link.w */
				/* if not folding and not main save register variables */
				if (   !stashit
					&& strcmp("main", Curproc->name)
				   )
					expanmovm(funclive, true); 		/* prolog call */

				/* create local variable space on the stack */
				if (framesz)
					adjbias(framesz, DIRDOWN);		/* adjust stack down */
				break;

			case UNLK:
				if (bias != 0)			/* we should have recovered bias */
					oerr("issuefunc: %sp not recovered");
				/* free up local variable space from stack */
				if ((framesz+lbias))		/* combine any local bias */
					adjbias(framesz+lbias, DIRUP);	/* bump stack back up */
				lbias = 0;				/* local bias now cleared (for rts) */

				/* restore registers if not folding and not main */
				if (   !stashit
					&& strcmp("main", Curproc->name)
				   )
					expanmovm(funclive, false);	/* epilog call */
				break;

			case SWITCH:	/* a switch */
				/* all bias must be gone at a switch */
				if (bias != 0)
					oerr("issuefunc: switch has bias");
				if (lbias)				/* any local bias? */
					FREELBIAS();		/* get rid of it */
				outswitch(node);		/* print switch statement */
				break;

			case LABEL:		/* a label */
				/* if folding don't stash entry point label of function */
				if (   stashit
					&& strcmp(node->op1, Curproc->name) == 0
				   )
					break;				/* ignore folded entry point label */
				(void) sprintf(stmt, "%s:", node->op1); /* colon after label */
				if (stashit)
					stash(stmt);		/* squirrel label if stashing */
				else
					puts(stmt);			/* print out with \n */
				break;

			case BSR_B:		/* bsr.b */
			case BSR:		/* bsr */
			case JSR:		/* jsr */
				/* if pass 2 check for function to be folded */
				if (Pass2)
				  {	for (pptr=Headproc;
							pptr && strcmp(pptr->name, node->op1);
								pptr=pptr->next);
					if (   pptr 
						&& pptr->foldable	/* if found and foldable ... */
						&& (   pptr->recurse == 0
											/* not recursive only */
							|| Curproc == pptr
											/* is self recursive call */
						   )
					   )
					  {	fold(pptr);	/* insert it in-line */
						/* if this is an empty insert and a label follows
						** then write out a NOP to avoid illegal bcc 
						** branching to the very next instruction.
						*/
						if (pptr->length <= 3 && islabel(node->forw))
							puts("\tnop");
						break;		/* done processing this statement */
						}
					}
				goto issuestmt;		/* process as normal statement */

			case RTS:		/* rts */
				/* issue only if not folding */
				if (!stashit)
					goto issuestmt;
				break;

			case LEA_L:
				if (rands[1]->ruses != REG_SP)	/* if not to stack pointer */
					goto issuestmt;			/* treat as normal statement */
				j = rands[0]->rdisp;		/* get the adjustment */
				if (j > 0)					/* freeing parms? */
					goto addtostack;		/* branch if so */
				else
					goto subtostack;		/* must be pushing parms */

			case ADD_W:
			case ADD_L:
				if (   rands[1]->ruses != REG_SP
											/* stack not destination */
					|| rands[1]->rtype != RT_REG
											/* not to a register */
				   )
					goto issuestmt;			/* treat as normal stmt */
				j = rands[0]->rdisp;		/* find value */
				if (j < 0)					/* really subtract? */
				  {	j = -j;					/* invert */
					goto subtostack;		/* yes */
					}
			addtostack:
				bias -= j;					/* absolute bias decreases */
				/*stacking = false;*/			/* not stacking now */
				stacking = bias != 0;
				/* if no abolute bias then fold this */
				if (bias == 0)				/* no nested function calls */
			  	  {	lbias += j;				/* remember local bias */
					break;					/* delete this statement! */
					}
				goto issuestmt;				/* print statement out */

			case SUB_W:
			case SUB_L:
				if (   rands[1]->ruses != REG_SP
											/* stack not destination */
					|| rands[1]->rtype != RT_REG
											/* not to a register */
				   )
					goto issuestmt;			/* treat as normal stmt */
				j = rands[0]->rdisp;		/* get value */
				if (j < 0)					/* really add? */
				  {	j = -j;					/* make positive */
					goto addtostack;		/* and do add */
					}
			subtostack:
				bias += j;              	/* adjust absolute bias */
				stacking = true;			/* stacking mode on */
				/* fall through to process statement */

			default:
		issuestmt:
				/* if this is a branch (the last statment in the block) and 
				** we are to end with no bias, we get rid of it here.
				*/
				if (   node == b->lastn
										/* last instruction */
					&&  lbias
										/* local bias still in effect */
					&& (b->marked & bmnoelbias)
										/* we are to end with none */
					&& isbr(node)
										/* this is a branch */
				   )
					if ( node->userdata.ud_nattr & UDN_INLINE && 
					( !node->userdata.ud_nattr & UDN_INVOLATILE ) )
						lbias = 0;
					else
						FREELBIAS();	/* get rid of local bias */

				/* format statement out while processing operands looking
				** for frame pointer basing or stack pointer adjustments.
				*/

				/* start instruction image with opcode copy */
				outstr = stmt;				/* point to build string area */
				*outstr++ = '\t';			/* set initial tab in */
				for (instr=node->opcode; *outstr=*instr++; outstr++);

				/* now process the operands */
				for (i=0;  i<MAXOPS && (rand=rands[i])!=RNULL; i++)
				  {	if (i==0)				/* first operand? */
						*outstr++ = '\t';	/* put out a tab */
					else
						*outstr++ = ',';	/* else put out a comma */

					/* if operand alters stack then adjust bias */
					if (   (rand->ruses & REG_SP)
						&& (rand->rtype == RT_DECR
						    || rand->rtype == RT_INCR)
					   )
					  { if (i != op->op_dnum)	/* not destination? */
							j = op->op_ssize[i];	/* source size */
						else
							j = op->op_dsize;	/* destination size */
						if (j==1)
							j = 2;			/* never byte from stack */
						if (rand->rtype == RT_DECR) {
							bias += j;
							stacking = true;
						} else {
							bias -= j;
							/* jcl */
							stacking = bias != 0;
						}
						} /* end ea modifies stack */

					/* now see if frame involved in operand */
					if (!(rand->ruses & REG_FP))
					  	/* no frame, simply copy as is */
						for (instr=node->ops[i+1]; 
									*outstr=*instr++; outstr++);
					else
					  {	/* frame involved, change to stack */
						int		newdisp;		/* new displacement */

						/*      Compute new stack offset.  
						** For parameters of unfolded functions it is
						**
						**	lbias + bias + framesz + regsave + olddisp - 4
						**
						** For parameters of folded functions it is
						**
						**	lbias + bias + framesz + regsave + olddisp - 8
						**
						** For local variables it is:
						**
						**	lbias + bias + framesz + olddisp
						*/
						newdisp = rand->rdisp + lbias + bias + framesz;
						if (rand->rdisp > 3)		/* parameter? */
							newdisp += regsave + (stashit ? -8 : -4);

						/* now print out the operand */
						if (newdisp == 0)		/* no displacement? */
						  if (rand->rindx == 0)	/* no index register? */
							(void) strcpy(outstr, "(%sp)");
						  else
							sprintf(outstr, "(%%sp,%s.%c*%d)",
									regchar(rand->rindx),
									(rand->rflags & rfdotw) ? 'w':'l',
									rand->rindxscl);
						else
						  if (rand->rindx == 0)	/* no index register? */
							sprintf(outstr, "(%d,%%sp)", newdisp);
						  else
							sprintf(outstr, "(%d,%%sp,%s.%c*%d)",
									newdisp,
									regchar(rand->rindx),
									(rand->rflags & rfdotw) ? 'w':'l',
									rand->rindxscl);
						/* move pointer to end of string */
						while (*outstr)
							outstr++;			/* to next free position */

						/* if a bit field is involved copy selection out */
						for (instr=rand->rname; /* scan for {o:w} bitfield */
									*instr && *instr != '{'; /* } */
											instr++);
						if (*instr == '{')			/* } copy if a bit field { */
							while ((*outstr++=*instr++) != '}');

						} /* end frame involved */

					} /* end operand processing */

				/* special case for PEA_L since it alters stack implicitly */
				if (node->op == PEA_L)
				  {	bias += 4;		/* address added to stack */
					stacking = true;	/* stacking mode on */
					}

				/* print the completed statement out now */
				*outstr = '\0';		/* terminate statement string */
				if (stashit)
					stash(stmt);	/* if stashing then store it */
				else
					puts(stmt);		/* else print it with \n */
				break;

				} /* end node->op switch */

			if (node == b->lastn)		/* last instruction in block? */
				break;					/* exit loop if so */

			} /* end scan all instructions */

	/* if zero bias required at end but no branch as last statement
	** then force rid the local bias.
	*/
	if (    lbias
									/* we still have a local bias */
		&& (b->marked & bmnoelbias)
									/* we must not have any */
	   )
		FREELBIAS();				/* free the local bias */

} /* issuefunc() */

				/* adjbias() - adjust stack up or down */

void
adjbias(lbias, dir)
int		lbias;						/* amount of local bias to undo */
int		dir;						/* direction (up or down) */
{
	char	stmt[80];				/* stmt image */

	/* format add to stack */
	(void) sprintf(stmt, "\t%s.%c\t&%d,%%sp",
						(dir == DIRUP) ? "add" : "sub",
						 (lbias > 32767) ? 'l' : 'w', lbias
					);
	if (stashit)
		stash(stmt);				/* stash if folding */
	else
		puts(stmt);					/* otherwise print it */

} /* pbias() */

				/* setfold() - set foldability of function */
void
setfold()
{
	/* if recursive function (e.g. calls only itself) than allow
	** higher size cutoff.
	*/
	if (  Curproc->calllist != NULL
										/* function calls another */
		&& Curproc->calllist->next == NULL
										/* only calls one other */
		&& strcmp(Curproc->name, Curproc->calllist->name) == 0
										/* calls itself only */
	   )
 	  {	if (Curproc->length > recurfold)
			Curproc->foldable = 0;		/* too big to ever fold */
		else
		if (Curproc->length > numfold)
			Curproc->recurse = true;	/* mark for recursive folding only */
		}
	else
	  {	if (Curproc->length > numfold)
			Curproc->foldable = 0;
		}

}

				/* foldstore() - squirrel away foldable functions */
void
foldstore()
{
	/* return if function cannot be folded */
	if (!Curproc->foldable || keeplink)
		return;

	/* allocate memory block large enough to hold source text */
	Curproc->text = xalloc(recurfold*40); /* size big enough for function */
	Curproc->insert = Curproc->text;		/* init insertion point */
	*Curproc->text = '\n';					/* default to null function */

	/* now call writefunc() telling it to squirrel the function */
	writefunc(true);

} /* foldstore() */


				/* stash() - stash statement for foldable function */

void
stash(stmt)
register char	*stmt;				/* pointer to statement string */
{
	register char	*out;			/* output string pointer */
	register char	c;				/* work character */
	register char	c2;				/* work character */
	char	label[16];				/* label build area for verification */
	int		i;						/* work variable */
	char	*tempptr;				/* label build pointer */

	/* Append this complete statement to the function's text
	** buffer.  We identify all labels and those that lead instructions
	** have a special marker inserted inside which assists unique label
	** generation at fold-in time.
	** Labels have the following possible formats:
	**
	**	L%digits	-  first pass compiler label
	**  L%%digits	-  optimizer created labels
	**  L%_digits	-  second pass compiler label
	**  _ASMdigits	-  in-line generated labels
	**
	** We mark the end of each statement with '\0' and the entire function
	** with a '\n'.
	*/

	out = Curproc->insert;			/* setup insertion pointer */

	while (*out++ = c = *stmt++)	/* while character copied ... */
	  {	/* check for "L%..." label */
		if (   c == 'L'
			&& *stmt == '%'
			&& (   (c2=*(stmt+1)) == '%'
									/* have "L%%..." */
				|| c2 == '_'
									/* have "L%_..." */
				|| isdigit(c2)
									/* have "L%digit..." */
			   )
		   )
		  {	/* we must verify that this label is an instruction label */
			label[0] = c;
			label[1] = '%';			/* build first 3 chars */
			label[2] = c2;
			i = 3;					/* start digits scan */
			tempptr = stmt+2;
			do
			  	label[i++] = c = *tempptr++;
			  while (isdigit(c));
			label[i-1] = '\0';		/* terminate string */
			if (FINDLBL(label) != NULL)	/* if is instruction label ... */
		  	  {	/* put special marker as 3rd position */
				*out++ = *stmt++;		/* copy over 2nd label character */
				*out++ = LABELMARK;		/* insert special insertion marker */
				}
			} /* end "L%..." check */
		else
		/* check for "_ASM..." label */
		if (   c == '_'
			&& *(stmt)   == 'A'
			&& *(stmt+1) == 'S'
			&& *(stmt+2) == 'M'
		   )
		  {	/* put special marker as 3rd position */
			*out++ = *stmt++;		/* copy over 2nd label character */
			*out++ = LABELMARK;		/* insert special insertion marker */
			}
		} /* end instruction copy */

	/* default to end of function */
	*out = '\n';

	Curproc->insert = out;			/* new insertion point */

} /* stash() */

						/* fold() - inject foldable function */

static int funcnum = 0;				/* unique function count for labels */

void
fold(pptr)
PROC	*pptr;						/* PROC pointer of function to told */
{
	char	*scan;					/* input scan pointer */
	char	*fill;					/* output scan pointer */
	char	stmt[80];				/* output statement build area */
	char	c;						/* work character */
	int		t;						/* label generation work */
	char	newlab[5];				/* new label segment for this function */

	if (fflag)						/* only if debugging ... */
		fprintf(stderr,"#Folding %s, len %d, live %x\n",pptr->name,
				pptr->length,pptr->funclive);
	/* generate a unique label segment for this function */
	t = funcnum++;					/* unique number */
	fill = newlab;					/* label start pos */
	do
	  *fill++ = "abcdefghijklmnopqrstuvwxyz"[t%26]; /* next character */
	  while (t /= 26);
	*fill = '\0';					/* end of label */

	/* issue register save instructions */
	expanmovm(pptr->funclive, true); 		/* prolog call */

	scan = pptr->text;				/* point to start of text */
	while (*scan != '\n')			/* while not to end of text ... */
	  {	fill = stmt;				/* start new statement buld */
		while (*fill++ = c = *scan++)	/* while more chars in stmt ... */
		  if (c == LABELMARK)
		  	/* hit unique label mark ... */
			for (--fill,t=0; newlab[t]; t++)
				*fill++ = newlab[t];	/* add unique segment */
		puts(stmt);					/* write out statement with '\n' */
		} /* end text scan */

	/* issue register restore instructions */
	expanmovm(pptr->funclive, false); 	/* epilog call */

} /* fold() */

	/* regsavesz() -- return total size of register stack save area */

/* Called by writefunc() to ascertain who much area is reguired on the
** stack to save registers.  The size of the instructions to save/restore
** is returned in a global so instsize() can obtain the size of UNLK.
*/

int	unlksize;			/* instructions size to save/restore */

int
regsavesz()
{
    REG	testlist;		/* regs which can be assigned to variables */
    unsigned long mask;	/* shift testing mask */
	int	returnval;		/* returned value */

	unlksize = 0;		/* start at instruction size of zero */
	/* if this is "main" we don't have to save/restore registers */
	if (strcmp("main", Curproc->name) == 0)
		return ( 0 );		/* no registers to save/restore */

	returnval = 0;			/* default to no registers saved */

	/* first the A/D registers */
	if (testlist=(funclive & ADVAR_REGS))	/* any A/D registers? */
      	/* count how many registers */
    	for(mask=1; mask; mask<<=1)
			if (mask & testlist)
				returnval += 4;	/* add space for this register */
	if (returnval > (4*4))
		unlksize +=4;			/* if movm.l used */
	else
		unlksize += returnval/2;	/* else mov.l for each register */

#ifdef M68881
	if (testlist=(funclive & FPVAR_REGS))	/* any float registers? */
      	for (mask=1; mask; mask<<=1)
		  if (mask & testlist)
			returnval += 12;	/* add space for this register */
	if (testlist & (testlist-1))
		unlksize += 6;			/* fmovm if more than one register */
	else
		unlksize += 4;			/* single fmov.l only */
#endif

	return ( returnval );			/* return size of save area */

} /* regsavesz() */

	/* expanmovm -- produce save/restore register save instructions */

/* This routine is called when link/unlk are hit during final function
** write out time.  We produce either (f)movm or individual mov.l
** instructions (depending on which is faster) to save registers used for 
** the variables in this function.  We use the latest live/dead data
** which tells us which registers have really been used in this function.
*/

extern REG dlist[];             /* register lists in mipsup.c */
extern char * nlist[];
extern REG flist[];
extern char * fnlist[];

void
expanmovm(liveregs, prologflag)
REG			liveregs;			/* registers live for function */
boolean		prologflag;			/* prolog or epilog processing */
{
    int	imm;		/* immediate build value for movm mask */
    int	i,j;		/* work values */
    REG	testlist;	/* regs which can be assigned to variables */
    REG	* reglist;	/* index into register mask list */
    unsigned long mask;	/* shift mask used to create imm */

    /* now check for saving registers at procedure prolog */
	if (prologflag)
	  {  /* first save A/D registers */
		if (testlist=(liveregs & ADVAR_REGS))	/* any A/D registers? */
	      {	/* build pre-decrement list and count how many registers */
	    	for(i=0,imm=0,reglist=&dlist[16],mask=1;
				*reglist != REG_NONE;
					--reglist,mask<<=1)
			  if (*reglist & testlist)
			    { i++;			/* count registers */
		    	  imm += mask;		/* include reg mask value */
		    	  };
	    	if (i > 4)			/* movm is faster */
	    	  	PRINTF("\tmovm.l\t&%#.4x,-(%%sp)\n", imm);
	    	else			/* individual moves are faster */
	    	  	for (reglist=&dlist[16]; *reglist != REG_NONE; --reglist)
		    	  if (*reglist & testlist)
					PRINTF("\tmov.l\t%s,-(%%sp)\n", nlist[reglist-dlist]);
			} /* end A/D save */
#ifdef M68881
		if (testlist=(liveregs & FPVAR_REGS))	/* only what needs saving */
	      {	for (i=0,imm=0,reglist=&flist[1],mask=1; 
				*reglist!=REG_NONE; 
					reglist++,mask<<=1)
			  if (*reglist & testlist)
			    {	i++;
					j = reglist - flist;			/* used if only one found */
		    		imm += mask;
					}
			if (i==1)
				/* issue single fmov.x %fpn,-(sp) */
				PRINTF("\tfmov.x\t%s,-(%%sp)\n", fnlist[j]);
	    	else /* issue "fmovm &imm,-(%sp)" */
	    		PRINTF("\tfmovm\t&%#.2x,-(%%sp)\n", imm);
			} /* end FP save */
#endif
		} /* end prolog save */
	else
    	/* restore registers at procedure epilog */
    {
#ifdef M68881
		/* first restore floating point variable registers */
	    if (testlist=(liveregs & FPVAR_REGS))	/* any floating regs? */
	      {	for (i=0,imm=0,reglist=&flist[8],mask=1; 
				*reglist!=REG_NONE; 
					--reglist,mask<<=1)
			  if (*reglist & testlist)
			    { i++;
				  j = reglist - flist;			/* used if only one found */
		    	  imm += mask;
				  }
			if (i==1)
				/* issued single fmov.x (%sp)+,%fpn */
				PRINTF("\tfmov.x\t(%%sp)+,%s\n", fnlist[j]);
	    	else /* issue "fmovm (%sp)+,&imm" */
	    		PRINTF("\tfmovm\t(%%sp)+,&%#.2x\n", imm);
	    	} /* end float restores */
#endif
		if (testlist=(liveregs & ADVAR_REGS))	/* any A/D variables? */
	    /* build pre-decrement list and count how many registers */
	      {	for(i=0,imm=0,reglist=&dlist[1],mask=1;
				*reglist != REG_NONE;
					reglist++,mask<<=1)
			  if (*reglist & testlist)
			    { i++;			/* count registers */
		    	  imm += mask;		/* include reg mask value */
		    	  };
	    	if (i > 6)			/* movm is faster */
	    	  	PRINTF("\tmovm.l\t(%%sp)+,&%#.4x\n", imm);
	    	else			/* individual moves are faster */
	    	  	for (reglist=&dlist[1]; *reglist != REG_NONE; reglist++)
		    	  if (*reglist & testlist)
					PRINTF("\tmov.l\t(%%sp)+,%s\n", nlist[reglist-dlist]);
	    	} /* end restore A/D registers */

		} /* end restore sequence */

} /* expanmovm() */
