#ident	"@(#)motopt.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*					Copyright (C) 1987 by Motorola Inc.				*/
/*																	*/

#include "optim.h"

/* operand definition for foropt() */
typedef struct
		{ enum	{notfound, immednum, regdisp} coptype;	/* operand type */
		  REG	copadreg;				/* 'regdisp' type register id */
		  int	copvalu;				/* imm. value or displacement to base */
		} COP;

/* Prototypes for static functions */
static unsigned short  floatcommute _PARMS((struct node *, struct node *));
static COP  resolveopr _PARMS((struct node *, int , struct node *, int , struct rand *, int *));
static int  setsto _PARMS((struct rand *, struct node *, COP *));
static int  oprvalu _PARMS((struct rand *, COP *));
static int  bcctst _PARMS((int , COP *, COP *));

					/* loopopt - optimize loop prologs */

/*	Programming situations like 'for (i=0; i<10; ...)' cause the following code:
**
**		clr  	i
**		br      looptest
** cont: ...
**	     ...
** looptest:
**		cmp		i,&10
**		blt		cont
**
**  which obviously need not have the initial branch.  This routine scans the
**  flow list and processes blocks which may represent the above situation.
**  
*/

int
loopopt()
{
	int		retv = 0;	/* function modified return value */
	BLOCK	*b;			/* next seq. block (checked for trailing branch) */
	BLOCK	*tb;		/* transfer block (checked for bcc back to next) */

	/* flow graph is valid upon entry */

	/* must restart everytime we delete a branch since flow graph changes */
 restart:

	/* scan all blocks looking for a possible pair of interest */
	for (ALLB(b,b0.next))
		if (   b->next != NULL
											/* not last block */
			&& (   (tb=b->nextr) != NULL
											/* conditional branch at end */
				|| (   (tb=b->nextl) != NULL
					&& tb != b->next
											/* or branch at end */
				   )
			   )
			&&
			   tb->nextr == b->next 
											/* target block conditionally */
											/* returns to our next block */

		   	   /* pass pair of interest to processing routine */
			&& foropt(b->lastn, b->length, tb->lastn, tb->length)
		   )
			{	retv = true;				/* flag optimization done */
				ldanal();					/* must rebuild FG and L/D */
				goto restart;				/* go for another loop */
			}

	return ( retv );
}

			/* foropt() - delete redundant initial loop condition checking */

/*	If the transfered to block is simply a condition check and we can
**	statically determine that the condition will always branch back then
**	remove the initial branch to the transfer block.
*/

/* conditional branch decision decode */
/* paired as eq/ne, hi/lo (unsigned), gt/lt (signed) */
#define	eq	0x01
#define	ne	0x02
#define	hi	0x04
#define	lo	0x08
#define	gt	0x10
#define lt	0x20

int
foropt(p, pcount, tp, tpcount)
NODE	*p;					/* trailing node of setup block */
int		pcount;				/* count of instruction in that block */
NODE	*tp;				/* trailing node of the transfer block */
int		tpcount;			/* count of it's instructions */
{
	NODE	*cmpp = tp->back;		/* anticipated 'cmp' instruction */
	int		istst = 0;				/* TST instruction flag */
	int		isfp = 0;				/* floating-point mode compare */
	COP		resolveopr();			/* resolve function */
	COP		lopr;					/* resolved left operand of the compare */
	COP		ropr;					/* resolved right oprand of the compare */
	int		usecnt;					/* target block instructions evaluated */
	int		cond;					/* condition codes used by target bcc */
	int		csize;					/* size used for target compare opreation */

	if (peepflag)
	  {	printf("foropt: processing ");
		prinst(p);
		printf(" targeting ");
		prinst(tp);
		}

	/* resolve the target block's branch conditions */
	cond = decodebcc(tp->op);
	if (cond == 0)
	  {	if (peepflag)
		  { printf("foropt: invalid condition ");
			prinst(tp);
			}
		return( 0 );	/* unsupported condition */
		}

	/* decode target 'cmp' size and check for fp or TST */
	csize = cmpsize(cmpp->op, &istst, &isfp);
	if (csize == 0)
	  {	if (peepflag)
		  {	printf("foropt: invalid compare ");
			prinst(cmpp);
			}
		return ( 0 );
		}

	usecnt = tpcount -2;	/* we must resolve all instructions in the */
							/* target block minus the compare and bcc  */

	/* decode the first operand of the compare by processing the target */
	/* block and the prolog block if need be 							*/
	lopr = resolveopr(p->back, pcount-1, cmpp, tpcount-2,
					  cmpp->userdata.ud_rand[0], &usecnt);

	/* if can't resolve it then return failed */
	if (lopr.coptype == notfound)
	  {	if (peepflag) printf("foropt: left operand unresolved\n");
		return ( 0 );
		}

	/* if TST instead of a CMP instruction we must fake a second oprand */
	if (istst)
	  {	ropr.coptype = immednum;		/* set type to immediate number */
		ropr.copvalu = 0;				/* and set it to a zero */
		if (peepflag) printf("foropt: TST faked\n");
		}
	/* otherwise try to resolve the second operand */
	else ropr = resolveopr(p->back, pcount-1, cmpp, tpcount-2,
						cmpp->userdata.ud_rand[1], &usecnt);

	/* we fail if it isn't resolved */
	if (ropr.coptype == notfound)
	  {	if (peepflag) printf("foropt: right operand unresolved\n");
		return ( 0 );
		}

	/* if we did not process all the instructions in the target we failed */
	if (usecnt > 0)
	  {	if (peepflag) printf("forop: failed to resolve %d target instructions\n",
							usecnt);
		return ( 0 );
		}

	/* both operands resolved.  determine if the target always returns back */
	if (bcctst(cond, &lopr, &ropr))
	  {	/* it does so lets delete the branch to the target */
		if (peepflag) printf("foropt: prolog branch deleted\n");
		DELNODE(p);
		return ( 1 );				/* return success */
		}

	/* return failure */
	return ( 0 );

}



		/* decodebcc() - break conditional branch down into it's primitives */

/*	Return three groups of decisions to make: eq/ne, hi/lo, gt/lt.  The last
**	two relate to unsigned versus signed comparisons.
*/

int
decodebcc(theop)
int		theop;						/* opcode of the bcc in the target block */
{
	int		cond=0;					/* returned condition bits */

	switch (theop)
	{ case BHS_B: case BHS:
	  case BCC_B: case BCC:
		cond = hi + eq;
		break;
	  case BLO_B: case BLO:
	  case BCS_B: case BCS:
		cond = lo;
		break;
#ifdef M68881
	  case FBEQ: case FBSEQ:
#endif
	  case BEQ_B: case BEQ:
		cond = eq;
		break;
#ifdef M68881
	  case FBLE: case FBNGT: case FBOLE: case FBULE:
#endif
	  case BLE_B: case BLE:
		cond = lt + eq;
		break;
#ifdef M68881
	  case FBGE: case FBNLT: case FBOGE: case FBUGE:
#endif
	  case BPL_B: case BPL:
	  case BGE_B: case BGE:
		cond = gt + eq;
		break;
#ifdef M68881
	  case FBGT: case FBNLE: case FBOGT: case FBUGT:
#endif
	  case BGT_B: case BGT:
		cond = gt;
		break;
	  case BHI_B: case BHI:
		cond = hi;
		break;
#ifdef M68881
	  case FBLT: case FBNGE: case FBOLT: case FBULT:
#endif
	  case BMI_B: case BMI:
	  case BLT_B: case BLT:
		cond = lt;
		break;
#ifdef M68881
	  case FBNEQ: case FBSNEQ:
#endif
	  case BNE_B: case BNE:
		cond = ne;
		break;
	} /* end switch */

	return ( cond );

}



			/* cmpsize() - return compare size and some flags */

/*	return the size of the compare.  also indicate if this is a floating-point
**	compare and/or instead of a compare the TST instruction is being used.
*/

int
cmpsize(cmpop, tstflg, fpflg)
int		cmpop;				/* instruction opcode before the target's bcc */
int		*tstflg;			/* we set this if it is a TST instruction */
int		*fpflg;				/* we set this if it is floating-point */
{
	int		retsz = 0;		/* default to failure */

	switch (cmpop)
	{ case TST_B:
		++*tstflg;
	  case CMP_B:
		retsz = 1;
		break;

	  case TST_W:
		++*tstflg;
	  case CMP_W:
		retsz = 2;
		break;

	  case TST_L:
		++*tstflg;
	  case CMP_L:
		retsz = 4;
		break;
#ifdef M68881
	  case FCMP_B:
		++*fpflg;
		retsz = 1;
		break;

	  case FCMP_W:
		++*fpflg;
		retsz = 2;
		break;

	  case FTEST_S:
		++*tstflg;
	  case FCMP_S:
	  case FCMP_L:
		++*fpflg;
		retsz = 4;
		break;

	  case FTEST_D:
		++*tstflg;
	  case FCMP_D:
		++*fpflg;
		retsz = 8;
		break;
#endif
		} /* end switch */

	return ( retsz );

}



			/* resolveopr() - resolve compare operand */

/*	This function is given one of the compare operands and attempts to
**	establish just what it's value is for a later compare function.
**  We first search the target block going backwards but if we don't
**	find it there we start back at the prolog block.
**
**	We must count the target block instructions used and take care that
**	we don't come across an instruction which modifies the operand we are
**	searching for in an indirect way (i.e. change of a base pointer used in the
**	addressing mode we are trying to resolve.)
*/

static
COP
resolveopr(p, pcount, tp, tpcount, crand, usecnt)
NODE		*p;						/* prolog back tail */
int			pcount;					/* instructions in that block */
NODE		*tp;					/* target block tail (cmp instruction) */
int			tpcount;				/* instructions in that block */
RAND		*crand;					/* RAND to resolve */
int			*usecnt;				/* decrement by instructions processed */
{
	int		used;			/* count of instructions we've scanned */
	RAND	*scr;			/* work RAND pointer */
	COP		retop;			/* return opreand value structure */
	NODE	*snode;			/* work node pointer */

	/* default the operand to not found (unresolved) */
	retop.coptype = notfound;

	/* see if compare operand is itself immediate or pointer constant */
	if (oprvalu(crand, &retop))
		return ( retop );		/* it is so return the result */

	/* first scan the target block trying to resolve the operand */
	for (used=0,scr=crand,snode=tp->back; tpcount>0; --tpcount)
	  {
		/* if assignment from another operand then attempt to use that */
		if (asgto(scr, snode))
		  {	/* if the assignment is not to a register, or if it is
			** a register which is live after the compare, then the
			** assignment is necessary to execute and we've failed.
			*/
			if (   !isreg(scr)
												/* not to a register */
				|| (tp->nlive & scr->ruses)
												/* register is required! */
			   )
				return ( retop );				/* we Failed */
			scr = snode->userdata.ud_rand[0];	/* to new source */
		  	if (peepflag) printf("equated to rand: %s\n", scr->rname);
			++used;								/* indicate processed */
			/* see if new operand is resolvable */
			if (oprvalu(scr, &retop))
			  {	*usecnt -= used;				/* deduct target process cnt */
				return ( retop );
				}
			}
		/* see if this instructions is clr or lea */
		else if (setsto(scr, snode, &retop))
		  {	*usecnt -= used + 1;				/* deduct target process cnt */
			return ( retop );
			}
		/* see if this instruction side effects our address */
		else if (alters(scr->ruses, snode))
			return ( retop );				/* return failed */
		/* back up to next node */
		snode = snode->back;
		}  /* end scan of target block */

	/* not yet found so now scan back up prolog block as well */
	for (snode=p; pcount>0; --pcount)
	  {
		/* if assignment from another oprand then use it instead */
		if (asgto(scr, snode))
		  {	scr = snode->userdata.ud_rand[0];	/* to new source */
		  	if (peepflag) printf("equated to rand: %s\n", scr->rname);
			/* see if new operand is resolvable */
			if (oprvalu(scr, &retop))
			  {	*usecnt -= used;				/* deduct target process cnt */
				break;
				}
			}
		/* see if this instruction is clr or lea */
		else if (setsto(scr, snode, &retop))
		  {	*usecnt -= used;				/* deduct target process cnt */
			break;
			}
		/* see if this instruction side effects our address */
		else if (alters(scr->ruses, snode))
			break;							/* return failed */
		/* back up to next node */
		snode = snode->back;
		}  /* end scan of prolog block */

	/* return with the result of the resolve */
	return ( retop );

}


	/* setsreg() - indicate if register is completely set by instruction */

int
setsreg(reg, node)
REG		reg;						/* register we are trying to resolve */
NODE	*node;						/* current instruction being examined */
{
	int		dnum;					/* destination operand number */
	RAND	*rand;					/* destination RAND */

	/* we suceed if:
	**	Instruction has a destination
	**	Destination is a register (or registers)
	**	Our register is one of the registers
	**	Instruction does not use destination as a source (read-modify-write)
	**	Instruction is not a mov.b/w following a clr.w/l
	*/
	return (   (dnum=node->userdata.ud_op->op_dnum) != NODEST
									/* is a destination */
			&& (   (rand=node->userdata.ud_rand[dnum])->rtype == RT_REG
#ifdef m68k
				|| rand->rtype == RT_REG_PAIR
#endif
		   	   )					/* destination is register(s) direct */
			&& (reg & rand->ruses)
									/* register is destination */
			&& node->userdata.ud_op->op_nsrc-1 < dnum
									/* destination not also source */
			&& !(   node->op == MOV_B
				 && (   node->back->op == CLR_L
					 || node->back->op == CLR_W
				    )
				 && node->userdata.ud_rand[1] == node->back->userdata.ud_rand[0]
									/* mov.b follows clear */
				)
			&& !(   node->op == MOV_W
				 && node->back->op == CLR_L
				 && node->userdata.ud_rand[1] == node->back->userdata.ud_rand[0]
				)
		   );

} /* setsreg() */


			/* asgto() - determine if the operand is replaced by another */

int
asgto(r, p)
RAND	*r;				/* RAND we are trying to resolve */
NODE	*p;				/* current instruction being examined */
{
	int		retv=0;		/* default to not a change of operand */

	switch (p->op)
	{ case MOV_B:
	  case MOV_W:
		/*	We must catch the case of a clr %dn followed by a mov ,%dn
		**	of a smaller size to the same register since live/dead info 
		**	is incorrect for this case.
		*/
		if (   (   p->back->op == CLR_L
				|| p->back->op == CLR_W
						/* we are preceeded by a clr.l or clr.w */
			   )
			&& p->userdata.ud_rand[1] == p->back->userdata.ud_rand[0]
						/* which is clearing our register operand */
		   )
			break;		/* this is not a valid replacement */
	  case MOV_L:
#ifdef M68881
	  case FMOV_B:
	  case FMOV_W:
	  case FMOV_L:
	  case FMOV_S:
	  case FMOV_D:
	  case FMOV_X:
#endif
		if (p->userdata.ud_rand[1] == r)
			++retv;
		}

	return ( retv );

}



		/*  setsto() - resolve operand for sub, clr or lea */

static
int
setsto(r, p, oprnd)
RAND		*r;				/* the RAND in question */
NODE		*p;				/* the instruction being processed */
COP			*oprnd;			/* the definition structure */
{
	int		ret=0;			/* default process status */

	switch (p->op)
	{ case SUB_B:
	  case SUB_W:
	  case SUB_L:
		if (p->userdata.ud_rand[0] != p->userdata.ud_rand[1])
			break;			/* not a zero if not same registers */
		/* treat same as a clr */

	  case CLR_B:
	  case CLR_W:
	  case CLR_L:
		if (p->userdata.ud_rand[0] == r)
		  {	/* clearing our operand so we know it's a zero */
			if (peepflag) printf("operand resolves to a zero\n");
			oprnd->coptype = immednum;
			oprnd->copvalu = 0;
			ret++;
			break;
			}
	  case LEA_L:
		if (p->userdata.ud_rand[1] == r)
		  {	/* lea to our operand.  process it or fail */
			RAND	*sr=p->userdata.ud_rand[0];	/* lea source oprand */

			if (peepflag) printf("attempt to resolve lea\n");
			if (   sr->rtype == RT_OREG			/* disp(base) */
				|| sr->rtype == RT_IREG			/* (base) */
			   )
			  {	/* have base displacement. fill in type and register used */
				oprnd->coptype = regdisp;
				oprnd->copadreg = sr->ruses;
				oprnd->copvalu = sr->rdisp;		/* displacement computed */
				++ret;
				break;
				} /* lea accepted processing */
			} /* lea processing */
		} /* switch */
	
	return ( ret );

}



			/* oprvalu() - return value if immediate operand */

static
int
oprvalu(r, oprnd)
RAND	*r;				/* operand being processed */
COP		*oprnd;			/* value structure to set */
{
	int		ret=0;		/* default to not processed */

	if (r->rtype == RT_IMMNUM)
	  {	/* we have an immediate integer */
		ret++;
		oprnd->coptype = immednum;
		oprnd->copvalu = r->rdisp;
		}

	return ( ret );

}



			/* alters() - determine if instruction alters operand */

/*	This function insures no side effects occur on registers in the operand we 
**	are trying to resolve.  An example would be for the addressing mode
**	30(%a0) and the instruction we are at alters %a0.  This invalidates
**	our future processing.
*/

int
alters(r, p)
REG		r;				/* reg use from RAND we are trying to resolve */
NODE	*p;				/* instruction being processed */
{
	int		srcnum;						/* source operand scan number */
	OP		*top;						/* op structure for instruction */
	RAND	*pr;						/* destination RAND for it */

	/* GENERIC instructions always destroy optimization */
	if (p->op == GENERIC)
		return( true );					/* return destroys operand */
	/* if the destination alters a register in our RAND we are out of luck */
	top = p->userdata.ud_op;		
	if (   top->op_dnum != NODEST
  	  	&& (p->userdata.ud_rand[top->op_dnum]->ruses & r)
	   )
		return( true );			/* return indication that we can't continue */

	/* if a source side effects a register in our RAND we are out of luck */
	for (srcnum=0; srcnum < top->op_nsrc; srcnum++)
	  if (   (   (pr=p->userdata.ud_rand[srcnum])->rtype == RT_INCR
		  	  || pr->rtype == RT_DECR
		     )
		  && (pr->ruses & r)
	     )
		return( true );

	return ( false );

}


			/* bcctst() - return true if we know target bcc branches */

static
int
bcctst(cond, lopr, ropr)
int		cond;				/* the condition to test for */
COP		*lopr;				/* left operand */
COP		*ropr;				/* right operand */
{
	int		tcond = 0;		/* known true condition states */

	/* do compare only if both of same type and found */
	if (   lopr->coptype == ropr->coptype
		&& lopr->coptype != notfound
	   )
	  switch ((int)lopr->coptype)
	  {
		case regdisp:
			/* we have disp(%rn).  if same register compare displacement */
			if (lopr->copadreg != ropr->copadreg)
				break;
			/* set true condition bits for eq/ne, hi/lo, gt/lt */
			/* Note: treat hi/lo as signed since in actuality 
			** the comparison will be with an address not just displacement
			*/
			if (lopr->copvalu == ropr->copvalu)
				tcond += eq;
			else
			  {	tcond += ne;
				if (lopr->copvalu > ropr->copvalu)
					 tcond += gt + hi;
				else tcond += lt + lo;
				}
			break;

		case immednum:
			/* set true condition bits for eq/ne, hi/lo, gt/lt */
			if (lopr->copvalu == ropr->copvalu)
				tcond += eq;
			else
			  {	tcond += ne;
				if (lopr->copvalu > ropr->copvalu)
					 tcond += gt;
				else tcond += lt;
				if ((unsigned)lopr->copvalu > (unsigned)ropr->copvalu)
					 tcond += hi;
				else tcond += lo;
				}
			break;
			} /* end switch */

	/* if any condition matches as true then branch will take */
	return ( cond & tcond );

}
				/* inline() - optimize inline functions */
 
/*  This function receives control the first time that the value trace
**  routine hits a trigger line set by a special inline code comment
**  statement.  Value tracing occurs as the first optimization which
**  means that the in-line function is still intact and unaltered by
**  loop rotations and other optimizations.
**
**  Processing for each function is unique.  Usually a check for specific
**  parameters is done with possible optimizations implemented.  Part or
**	all of the function may be replaced by the optimizations.
**
**	We return an indication of successful optimization.
*/

int
inline(p)
NODE	*p;							/* trigger node for inline checking */
{
	SSTR	*slist;					/* static string list */
	NODE	*scanp;					/* backscan node pointer */
	NODE	*endp;					/* end of in-line function pointer */
	RAND	*r;						/* operand rand pointer */
	int		funcid;					/* function represented */
	
	/* isolate function id and remove trigger so we don't get called again */
	funcid = p->userdata.ud_nattr & UDN_fmask; /* isolate function */
	p->userdata.ud_nattr &= ~UDN_fmask; /* delete function trigger */
	switch (funcid)					/* switch on function name */
	{ case UDN_strcpy:
		/*					 strcpy() 
		**			%a0->source  %a1->destination
		** Check for a "mov.l &L%xxxL,%a0" pointing to a static string 
		** for which we know the length passed to us from pass 1. Change the 
		** rest of the function to simple sequential mov instructions.
		*/

		/* first find the %a0 setup */
		for (scanp=p->back; scanp!=NULL; scanp=scanp->back)
			if (   scanp->op == MOV_L 
				&& (r=scanp->userdata.ud_rand[1])->rtype == RT_REG
				&& r->ruses == REG_A0
			   )
				break;
		if (   scanp == NULL
											/* ??? failed to find %a0 */
			|| (r=scanp->userdata.ud_rand[0])->rtype != RT_IMMED
											/* source not immediate */
		   )
			break;
		/* scan static string list to see if we find a match */
		for (slist=sstrlist; slist!=NULL; slist=slist->stnext)
		  if (strcmp(r->rname+1, slist->stname)==0)
			{	/* found it! delete rest of in-line and insert moves */
				if (tempflag)
					PRINTF("static string %s strxxx() optimized\n",
									slist->stname);
				p = p->back;				/* remember last inline setup */
				for (scanp=p->forw; isinline(scanp); scanp=p->forw)
				 { 	endp = scanp->forw;		/* in case end of inline function */
					if (scanp->userdata.ud_nattr & UDN_ENDINLINE)
					  {	DELNODE(scanp);		/* delete next inline stmt */
						break;				/* this was the end */
					  }
					else
						DELNODE(scanp);		/* delete next inline stmt */
				  }
				/* insert move instructions for the length of the string */
				{	int		len = slist->stlen;		/* length to move */
					RAND	*ra0 = saverand("(%a0)+", 6, RT_INCR, REG_A0, true);
					RAND	*ra1 = saverand("(%a1)+", 6, RT_INCR, REG_A1, true);
					NODE	*bldmov();

					while (len >= 4)
					  {	p = bldmov(endp, MOV_L, ra0, ra1); /* add mov.l */
						len -= 4;					/* count down */
					  }
					while (len >= 2)
					  {	p = bldmov(endp, MOV_W, ra0, ra1); /* add mov.w */
						len -= 2;					/* count down */
					  }
					while (len >= 1)
					  {	p = bldmov(endp, MOV_B, ra0, ra1); /* add mov.b */
						--len;						/* count down */
					  }
				return ( true );					/* return sucess */
				} /* end insert move */
			} /* end static match list */
		break;

	  } /* end function switch */

	return ( 0 );									/* return failure */

} /* inline() */
					/* bldmov() - insert a mov.[lwb] node */

NODE *
bldmov(endp, inst, a0, a1)
NODE	*endp;						/* node to insert before */
int		inst;						/* instruction to insert */
RAND	*a0;						/* first operand */
RAND	*a1;						/* second operand */
{
	NODE	*newp = GETSTR(NODE);	/* get a new node */
	int		i;						/* work integer */

	INSNODE(newp, endp);		/* insert it in chain */
	newp->nlive = 0;			/* clear stuff */
	newp->ndead = 0;
	newp->uniqid = IDVAL;
	newp->userdata.ud_nattr = 0;
	for (i=0; i < MAXOPS;)
		newp->ops[++i] = NULL;	/* clear ops */
	setop(newp, getOP((OPCODE) inst)); /* set opcode in */
	setrand(newp, 1, a0);		/* set (a0)++ */
	setrand(newp, 2, a1);		/* set (a1)++ */

}

		/* redmov() - redundant reg/reg moves and commutative operations */

/*					Redundant move logic:
**
**	This function attempts to find and correct instances of a redundant
**	register to register move.  The correction consists of substituting
**	the move destination for the move source in code earlier than the move,
**	or the move source for the move destination in code later
**	than the move.  An example of the former:
**
**		mov.l	%d2,%d1				mov.l	%d2,%d4
**		lsl.l	&2,%d1		to		lsl.l	&2,%d4
**		mov.l	%d1,%d4
**
**	and of the latter:
**
**		mov.l	%a0,%a1		to		mov.l	PtrGlbNext,(%a0)
**		mov.l	PtrGlbNext,(%a1)
**
**				Commutative operations logic:
**
**  Commutative operations such as add/or/and when preceeded by a move
**  are eligible for this improvement.  If the commutative operation uses
**  two registers and the source register is dead, then attempt to reverse
**  the roles of the two registers.  When this is possible the move
**  can be transformed directly into the communative operation and the
**  original communative instruction can be eliminated.
**
**  	mov.X   	A,B
**  	commop.X   	C,B             -->     commop.X   A,C
**									(and replace all later B with C)
**
**  where:
**          B and C are registers
**          C is dead after the commop
*/

int
redmov()
{
	register NODE	*node;			/* node search pointer */
	register NODE 	*next;			/* next node pointer */
	int				regsize;		/* byte size of move */
	RAND			*srcreg;		/* move's source RAND item */
	RAND			*dstreg;		/* move's destination RAND item */
	RAND			*trand;			/* temporary rand pointer */
	boolean			didsomething;	/* return flag if we optimized something */
	boolean			rerun;			/* did something so loop again */
	int				trysubmv();		/* function definition */
	static OPCODE	floatcommute();	/* function returning fmov replacement */

#define RMDEBUG
#undef RMDEBUG

	didsomething = false;			/* default to nothing optimized */

  /* live/dead and flow graph valid upon entry */

  /* loop restart point as long as we optimized something */
  restart:
	rerun = false;					/* default to nothing done this loop */
	/* scan text looking for (f)mov  X,R */
	for (ALLN(node))
		{
#ifdef RMDEBUG
		if (islabel(node)) prinst(node);
#endif
		/* skip anything that can't be a (f)mov */
		if (   node->userdata.ud_op->op_nsrc != 1
									/* not a single source operand */
			|| node->userdata.ud_op->op_dnum != 1
									/* second argument not a destination */
		   )
			continue;				/* skip this statement */
		switch (node->op)
		  {	case MOV_B:
			case MOV_W:
			case MOV_L:
#ifdef M68881
			case FMOV_B:
			case FMOV_W:
			case FMOV_L:
			case FMOV_S:
			case FMOV_D:
			case FMOV_X:
#endif
				break;				/* is a (f)mov */

			default:
				continue;			/* is not so continue search */
		  }

		srcreg = node->userdata.ud_rand[0];	/* source RAND */
		dstreg = node->userdata.ud_rand[1];	/* destination RAND */

#ifdef m68k
		/* don't let bit field instructions through */
		if ((srcreg->rflags | dstreg->rflags) & rfbitfld)
			continue;				/* skip bit field instructions */
#endif

		/* now look for commutative operation as next instruction */
		if (   isreg(dstreg)
									/* B is a register */
			&& ((next=node->forw)->userdata.ud_op->op_flags & OF_COMUOP)
									/* next instruction is commutative */
			&& next->userdata.ud_rand[1] == dstreg
									/* next has same 2nd operand */
			&& isreg((trand=next->userdata.ud_rand[0]))
									/* next first operand is a register */
			&& isdead(trand->ruses, next)
									/* next first operand dies */
			&& (isareg(trand) ^ isareg(dstreg)) == 0
			&& (isareg(srcreg) ^ isareg(dstreg)) == 0
									/* not mixing address and data registers */
#if M68881
			&& (isfpreg(trand) ^ isfpreg(dstreg)) == 0
									/* not mixing data and fp registers */
#endif
			/* jcl */
			&& (!(isfpreg(srcreg) && isdreg(dstreg)))	/* not mixing data and fp registers improperly */
			&& (   
#if M68881
					isfpreg(dstreg) ||
#endif
				node->userdata.ud_op->op_dsize == next->userdata.ud_op->op_dsize
			   )
									/* integers must be same size */
			&& trysubmv(next, trand, dstreg, 0, FORWARD)
									/* we have substituted B with C */
		   )
		  {	/* convert the mov to the commutative op */
			if (peepflag)
			  {	printf("  commute op was: ");
				prinst(node);
				printf("  is now: ");
				}
#if M68881
			if (isfpreg(dstreg))
			  	/* must handle floats uniquely since all R,R are ".x" */
				/* fmov.? to fcommop.x */
			  	setop(node, getOP(floatcommute(node, next)));
			else
#endif
				setop(node, next->userdata.ud_op);	/* change mov to the op */

			setrand(node, 2, trand);		/* 2nd operand is now C */
			if (peepflag)
				prinst(node);
			node = next;					/* setup to delete commuop */
			goto dodelete;					/* delete and continue */
			}

		/* make sure both operands are registers */
		if (   srcreg->rtype != RT_REG
			|| dstreg->rtype != RT_REG
		   )
			continue;				/* ignore this statement */

		regsize = 0;				/* default to illegal size */

		switch(node->op)
		{
#ifdef M68881
		   case FMOV_D:
		   case FMOV_S:
		   case FMOV_X:
				regsize = 10;		/* ten bytes for any floating register */
				break;
#endif
		   case MOV_B:
				/* if mov.b follows clr.w or clr.l we cannot alter */
				if (node->back->op == CLR_W || node->back->op == CLR_L)
					break;
				regsize = 1;		/* one byte size */
				break;

		   case MOV_W:
				/* if mov.w follows clr.l we cannot alter */
				if (node->back->op == CLR_L)
					break;
				regsize = 2;		/* word byte size */
				break;

		   case MOV_L:
				regsize = 4;		/* longword byte size */
				break;
		  }

		/* if not a move then skip this statement */
		if (!regsize)
			continue;

		/* insure both sides are of the same register type */
		if (   (isareg(srcreg) ^ isareg(dstreg))
#ifdef M68881
			|| (isfpreg(srcreg) ^ isfpreg(dstreg))
#endif
		   )
			continue;				/* non-identical types */

		/* We have (f)mov register to register of the same reg types.
		** Now try to optimize backwards then forwards.  We cannot
		** optimize backwards if the source is not dead since
		** something later requires that register. 
		*/
		if (   (   isdead(srcreg->ruses,node) 
				&& trysubmv(node, dstreg, srcreg, regsize, BACKWARD) != 0
									/* if source dead try backwards */
			   )
			|| trysubmv(node, srcreg, dstreg, regsize, FORWARD)
						/* if nothing there try forwards */
		   )
		  {	if (peepflag)
			  {	printf(" redmov() - removed redundant move:");
				prinst(node);
				}
	dodelete:
			next = node->forw;	/* continue at next node */
		  	didsomething = true;	/* flag we optimized something */
			DELNODE(node);			/* delete out the optimized move */

			/* Skip to the next optimization boundary to avoid encountering
			** invalid live/dead info due to our recent changes.
			*/
			for (node=next; 
					node != &ntail && !islabel(node) && !isbr(node);
							node = node->forw);
			rerun = true;			/* indicate something done this pass */

		  }; /* end optimized something */

		} /* end for ALLN */

	if (rerun == true)
	  {	ldanal();					/* recompute live/dead */
		goto restart;				/* and try all over again */
		}


	return( didsomething );			/* return optimized status */

} /* redmov() */
#ifdef M68881

			/* floatcommute() - return fmov.N to communativeop.N OPCODE */

static
OPCODE
floatcommute(node, nextnode)
NODE	*node;							/* the fmov node */
NODE	*nextnode;						/* the commutative operation node */
{
	OPCODE	nextop;						/* commutative fxxx operation */
	OPCODE	noper;						/* replacement operation */
	
	/* find the commutative operation in the "opposite" element of the opcode */
	nextop = nextnode->userdata.ud_op->op_opposite;

	switch(node->op)
	{	case FMOV_B:	/* fmov.b */
			switch(nextop)
			  {	case FADD_X:
					noper = FADD_B;
					break;

				case FMUL_X:
					noper = FMUL_B;
					break;
				
				case FSGLMUL_X:
					noper = FSGLMUL_B;
					break;
				} 
			break;

		case FMOV_W:	/* fmov.w */
			switch(nextop)
			  {	case FADD_X:
					noper = FADD_W;
					break;

				case FMUL_X:
					noper = FMUL_W;
					break;

				case FSGLMUL_X:
					noper = FSGLMUL_W;
					break;
				} 
			break;

		case FMOV_L:	/* fmov.l */
			switch(nextop)
			{	case FADD_X:
					noper = FADD_L;
					break;

				case FMUL_X:
					noper = FMUL_L;
					break;

				case FSGLMUL_X:
					noper = FSGLMUL_L;
					break;
				} 
			break;

		case FMOV_S:	/* fmov.s */
			switch(nextop)
			{	case FADD_X:
					noper = FADD_S;
					break;

				case FMUL_X:
					noper = FMUL_S;
					break;

				case FSGLMUL_X:
					noper = FSGLMUL_S;
					break;
				} 
			break;

		case FMOV_D:	/* fmov.d */
			switch(nextop)
			{	case FADD_X:
					noper = FADD_D;
					break;

				case FMUL_X:
					noper = FMUL_D;
					break;

				case FSGLMUL_X:
					noper = FSGLMUL_D;
					break;
				} 
			break;

		case FMOV_X:	/* fmov.l */
			noper = nextop;
			break;

		default:
			oerr("redmov: invalid fmov");	/* this is not a fmov */
			/*NOT REACHED*/

			} /* end switch node->op */

	return ( noper );			/* return replacement instruction */

} /* floatcommute() */
#endif

				/* trysubmv() - attempt to substitute the move out */

/*	We are passed a (f)mov instruction with register operands of identical
**	types and will try to substitute one for the other so that the move
**	can be deleted.
**
**	For backwards replacement a backward scan is done to test for the 
**	validity of the substitution.  It succeeds if it eventually reaches an 
**	assignment to the source register without hitting a label or branch or 
**	seeing the destination register being used in any way.  If the condition
**	code is not dead after the move we must insure it will still be accurate.
**
**	Forward scans are verified by insuring that neither the source or 
**	destination registers are altered and then used while the other is
**	still active.  (In this case they cannot be the same register.)
**	Labels or branches fail forward test scans also.
*/

int
trysubmv(node, usereg, unusereg, regsize, direction)
NODE *		node;				/* (f)mov node to start from */
RAND *		usereg;				/* register to be substituted */
RAND *		unusereg;			/* register to be replaced */
int			regsize;			/* byte size of move to optimize out */
int			direction;			/* forward or backward scanning direction */
{
	enum	{cont, fail, success} ind; /* scanning indicator */
	NODE *	scan;				/* scanning node pointer */
	OP *	op;					/* pointer to node instruction structure */
	int		len;				/* work variable */
	int		ccrequired;			/* if CC required during backward scan */
	REG		deadreg;			/* set for moves into a register */

	ind = cont;					/* start scanning mode */
	scan = node;				/* start at (f)mov node */

	ccrequired = !isdead(REG_CC, node);	/* set condition code required */

	/* process forward direction */
	if (direction == FORWARD)
	  while (ind == cont)
	  {	scan = scan->forw;		/* to next statement */
		/* make exception for moves which actually kill register */
		if (  (   scan->op == MOV_L
#ifdef M68881
			   || scan->op == FMOV_X
#endif
			  )
			&& scan->userdata.ud_rand[1]->rtype == RT_REG
		   )
			deadreg = scan->userdata.ud_rand[1]->ruses;
		else
			deadreg = 0;
#ifdef RMDEBUG
printf("# ?? FOR %s dead(use) %d, alters(unuse) %d, dead(unuse) %d, unuse->ruses %lx, deadreg %lx, alters(use) %d\n",
			scan->ops[0], isdead(usereg->ruses, scan), 
			alters(unusereg->ruses, scan),
			isdead(unusereg->ruses, scan), deadreg, 
			unusereg->ruses, alters(usereg->ruses,scan));
#endif

		/* if label or branch or JSR with scratch regsiters
		** or generic instructions hit we failed.
		*/
		if (   islabel(scan) 
			|| isbr(scan)
			|| scan->op == GENERIC
			|| (   (   scan->op == JSR	
					|| scan->op == BSR
					|| scan->op == BSR_B
				   )
								/* JSR blows scratch registers */
				&& ((usereg->ruses|unusereg->ruses) & SCRATCHREGS)
								/* if we have any scratch registers */
			   )
				/* jcl */
			|| scan->userdata.ud_op->op_dsize != node->userdata.ud_op->op_dsize
		   )
			ind = fail;
		else
		/* if destination is destroyed then we can substitute */
		if (unusereg->ruses == deadreg)
			ind = success;
		else
		/* if mutual use of source and destination we fail */
		if (   (   !isdead(usereg->ruses, scan)
								/* source is still hot */
				&& alters(unusereg->ruses, scan)
								/* and destination is altered */
			   )
			|| (   !isdead(unusereg->ruses, scan)
								/* destination is still hot */
				&& alters(usereg->ruses, scan)
								/* and source is altered */
			   )
		   )
			ind = fail;
		/* if destination is dead after this we can substitute */
		else
		if (isdead(unusereg->ruses, scan))
			ind = success;

		} /* end forward scan */

	else
	/* backward scan */
	while (ind == cont)
	  {	scan = scan->back;		/* previous statement */
#ifdef RMDEBUG
printf("# ?? BAC %s dead(use) %d, alters(unuse) %d, dead(unuse) %d, unuse->ruses %lx, deadreg %lx, alters(use) %d\n",
			scan->ops[0], isdead(usereg->ruses, scan), 
			alters(unusereg->ruses, scan),
			isdead(unusereg->ruses, scan), deadreg, 
			unusereg->ruses, alters(usereg->ruses,scan));
#endif
		/* if label or branch or GENERIC hit we failed */
		if (   islabel(scan) 
			|| isbr(scan)
			|| scan->op == GENERIC
			|| (   (   scan->op == JSR	
					|| scan->op == BSR
					|| scan->op == BSR_B
				   )
								/* JSR blows scratch registers */
				&& ((usereg->ruses|unusereg->ruses) & SCRATCHREGS)
								/* if we have any scratch registers */
			   )
				/* jcl */
			|| scan->userdata.ud_op->op_dsize != node->userdata.ud_op->op_dsize
		   )
			ind = fail;
		else
		/* if source is set by this instruction we have succeeded */
		if (setsreg(unusereg->ruses, scan))
			ind = success;
		else
		/* if source appears in any operand we fail */
		if (isused(usereg->ruses, scan))
			ind = fail;

		/* if condition code is required make sure it will still be proper */
		if (   ccrequired
									/* CC required after the move */
			&& (scan->userdata.ud_flags & (UD_CC | UD_CCA))
									/* this instruction sets CC */
		   )
		  if (   (len=scan->userdata.ud_op->op_dnum) != NODEST
									/* instruction has a destination */
			  && node->userdata.ud_rand[len] == unusereg
									/* destination is our replacement reg */
			 )
			ccrequired = 0;			/* CC requirement is met */
		  else
			ind = fail;				/* deleted move will fail CC setting */

#ifdef M68881
		/* A final check: fmov cannot be the top move if condition code 
		** is required and destination is %dn. 
		*/
		if (   ind == success
									/* this is final replacement (f)mov */
			&& ccrequired
									/* condition code is required */
			&& isfpreg(scan->userdata.ud_rand[0])
									/* source is floating point register */
			&& !isfpreg(scan->userdata.ud_rand[1])
									/* destination is data register */
		   )
			ind = fail;				/* fmov does not set condition code! */
#endif

		}  /* end backward scan */

	/* if we failed then exit with a failed indication */
	if (ind == fail)
		return ( 0 );			/* return no optimization had */

	/* Now go through replacing the old register with the new */

	len = strlen(usereg->rname);	/* find register operand length */
								/* Note: assume replacement is same len */

	while (node != scan)		/* loop until we reach the destination */
	  {	int		ii,jj;			/* work variables */
	
		node = (direction == FORWARD) ? node->forw : node->back; /* next node */
		op = node->userdata.ud_op;	/* find the instruction definition */
		/* process each operand */
		for (ii=0; ii < MAXOPS; ii++)
		  if (   ii < op->op_nsrc
								/* if operand is a source operand */
		      || (   op->op_dnum != NODEST
								/* or instruction has a destination */
				  && ii == op->op_dnum
								/* and this is the destination */
				 )
			 )
		  {	boolean	changed;	/* flag if operand contains old register */
			char	newstr[40];	/* new string with replaced register */
			RAND *	wrand;		/* work RAND pointer */

			/* Do not process the source of a backward scan if this
			** is the last one to do since this is the assignment statement
			** and the source does not belong in the substitution scheme.
			*/
			if (   direction == BACKWARD
								/* we are going backwards */
				&& node == scan
								/* this is the assignment to the unusereg */
				&& ii < op->op_nsrc
								/* this is the source operand */
			   )
				continue;

			wrand = node->userdata.ud_rand[ii];	/* this operand */

			/* Do not process the destination of a forward scan if this
			** is the last statement to do and it's a (f)mov to our
			** unusedreg.  (This is a new assignment to the unusedreg.)
			*/
			if (   direction == FORWARD
								/* scanning foward */
				&& node == scan
								/* this is the final statement */
				&& ii == op->op_dnum
								/* this is the destination operand */
				&& wrand->rtype == RT_REG
								/* this is a register destination */
				&& wrand->ruses == unusereg->ruses
								/* this is the register we are 'unuse'ing */
				&& (node->op == MOV_L || node->op == FMOV_X)
								/* this is a move */
			   )
				continue;

			/* copy over old operand */
			(void) strcpy(newstr, wrand->rname);
			/* now replace old register anywhere in operand */
			changed = 0;		/* default to not found */
			for (jj = strlen(newstr) - len;
					jj >=0;
						--jj)
			  /* if found old */
			  if (strncmp(newstr+jj, unusereg->rname, len) == 0) 
				/* copy new over old */
				{	(void) strncpy(newstr+jj, usereg->rname, len);
					changed = 1;					/* indicate old found */
				};

			/* if operand changed then replace it's RAND structure */
			if (changed)
			  {	if (peepflag)
				  {	prinst(node);
					printf(" -> to -> ");
					}
				setrand(node,
					ii+1,
					saverand(newstr, strlen(newstr), wrand->rtype, 
							(wrand->ruses ^ unusereg->ruses) | usereg->ruses,
							true)
					);
				if (peepflag)
				  {	prinst(node);
					printf("\n");
					}
				}
			} /* end operand processing */

		} /* end next node processing */

	return ( 1 );		/* return optimization succeeded */

} /* trysubmv() */


				/* isused() - return if registers altered in any operand */

int
isused(r, p)
REG 	r;				/* ruses with register we care about */
NODE *	p;				/* node pointer */
{
	OP *	top;		/* statement structure pointer */
	int		i;			/* work integer */

	top = p->userdata.ud_op;	/* operation structure */

	/* first check for destination using the register RAND */
	if (   top->op_dnum != NODEST
								/* statement has a destination */
		&& (p->userdata.ud_rand[top->op_dnum]->ruses & r) != 0
								/* destination uses register RAND */
	   )
		return ( 1 );			/* return true */

	/* now scan source operands looking for the register */
	for (i=0; i<top->op_nsrc; i++)
	  if ((p->userdata.ud_rand[i]->ruses & r) !=0)
		return ( 1 );			/* return true if so */

	return ( 0 );				/* reg RAND not found in instruction */

} /* isused() */

		/* brarts - optimize branches to UNLK/RTS instructions */

/*	Occasionally we have a branch to the end of the function (unlk/rts.)
**	This code replaces such a branch with a direct unlk/rts if there
**	are no more than 3 registers to restore.
*/

void
brarts()
{
	BLOCK	*b;						/* next sequential block scanned */
	BLOCK	*tb;					/* transfer target block */
	int		i;						/* work variable */
	int		j;						/* work variable */
	boolean	changed;				/* dirty flowgraph flag */
	NODE	*newnode;				/* new RTS node allocation pointer */
	NODE	*branode;				/* branch node pointer */

	/* count amount of variable registers to restore */
	for (j=0,i=(funclive & VAR_REGS); i; i>>=1)
	  if (i & 1)
		j++;
	if (j > 3)
		return;						/* return if too much code required */

	changed = false;

#if 0
	/* NOT NEEDED SINCE WILL ALREADY BE VALID */
	bldgr(false);					/* build flow graph */
#endif

	/* scan all blocks looking for one ending in a branch to UNLK/RTS */
	for (ALLB(b, b0.next))
	  if (   (tb=b->nextl) != b->next
									/* block doesn't fall into next */
	  	  && tb != NULL
									/* block ends in bcc */
		  && b->nextr == NULL
									/* it is unconditional branch */
		  && tb->nextl == NULL
									/* destination ends function */
		  && tb->length == 2
									/* it has two instructions */
		  && tb->lastn->op == RTS
									/* it ends in RTS */
		 )
		  {	newnode = GETSTR(NODE);	/* new node for RTS */
			branode = b->lastn;		/* branch node ptr */

			/* clear operands out of branch and new node */
			for (i=0; i<MAXOPS; i++)
			  {	branode->ops[i+1] = NULL; /* remove UNLK operands */
				branode->userdata.ud_rand[i] = RNULL;
				newnode->ops[i+1] = NULL;	/* remove RTS operands */
				newnode->userdata.ud_rand[i] = RNULL;
				}
			/* change branch to UNLK */
			setop(branode, getOP((OPCODE) UNLK)); /* to UNLK */
			setrand(branode, 1 , 
					saverand("%fp", 3, RT_REG, REG_FP, true)
				   );
			/* initialize new RTS node */
			newnode->nlive = 0;			/* clear live/dead */
			newnode->ndead = 0;
			newnode->uniqid = IDVAL;	/* input stmt no. (?) */
			newnode->userdata.ud_nattr = 0; /* clear attribute bits */
			setop(newnode, getOP((OPCODE) RTS)); /* set opcode */
			branode = branode->forw;	/* INSNODE() inserts before */
			INSNODE(newnode, branode);	/* insert new node in */
			changed = true;				/* flag flow graph changed */

			} /* end insert unlk/rts */

	/* rebuild flow graph if we changed blocks */
	if (changed)
		bldgr(false);			/* rebuild it */

}

			/* rmconlbl() - remove consecutive labels */

/*	Consecutive labels impede optimization.  This routine purges all but the
**	last label in a sequence.
*/

extern REF r0;						/* head of data section label list */

void
rmconlbl()
{
	register NODE *bnode;				/* branch node in inner search */
	register NODE *anode;				/* primary node pointer for search */
	register REF *ref;					/* data section reference */
	register NODE *lnode;				/* last label node of the group */
	RAND	*lrand;						/* new label RAND structure */
	int		nsrc;						/* branch source operand count */

	for (ALLN(anode))
	  if (islabel(anode) && !ishl(anode))
		{	/* make sure isn't referenced from data statement */
			for (ref=r0.nextref; 
					ref!=NULL && strcmp(ref->lab, anode->ops[0]);
						ref=ref->nextref);
			if (ref != NULL)
				continue;			/* found data reference */
	    	for (lnode=anode; 
					lnode->forw != NULL && islabel(lnode->forw);
							lnode = lnode->forw);
			if (lnode != anode)
			  { /* consecutive labels, find branch references */
				lrand = findrand(lnode->ops[0],true);	/* new label RAND */
				for (ALLN(bnode))
				  if (isbr(bnode) && getp(bnode) == anode->ops[0])
					{	/* found reference to leading label, now replace it */
						if (labflag)
							printf("rmconlbl: br label %s -> %s\n",
										anode->ops[0], lnode->ops[0]);
						nsrc = bnode->userdata.ud_op->op_nsrc;
						setrand(bnode, nsrc, lrand);	/* new label RAND */
					}
				DELNODE(anode);				/* delete original label */
			  }
		} 

} /* rmconlbl() */
