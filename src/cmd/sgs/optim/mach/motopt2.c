#ident	"@(#)motopt2.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*												*/
/*		Copyright (C) 1987 by Motorola Inc.		*/
/*												*/

#include "optim.h"

			/* eaopt() - effective address optimization */

/*	This routine attempts to eliminate instructions by folding their
**	functionality into later addressing modes. Examine the following:
**
**		lea.l	-20(%fp),%a2
**		mov.l	%d2,%d0
**		sub.l	&1,%d0
**		mov.l	(%a2,%d0.l*4),...
**
**	The last mov.l uses extended indexing which allows an 8-bit
**	displacement.  If %d0 is dead after the final move the last two 
**	instructions can be replaced by
**
**		mov.l	(-4,%a2,%d0.l*4),...
**
**	with no increase in execution time for the last instruction.  Yet a 
**	further folding can be done to eliminate the lea:
**
**		mov.l	%d2,%d0
**		mov.l	(-24,%fp,%d0.l*4),...
**
**	then the redundant move optimization pass will get rid of the first move
**	leaving us with the much improved
**
**		mov.l	(-24,%fp,%d2.l*4),...
*/

int
eaopt()
{
	register NODE	*node;			/* node search pointer */
	RAND			*srand;			/* source RAND item */
	RAND			*drand;			/* destination RAND item */
	boolean			didsomething;	/* return flag if we optimized something */
	boolean			rerun;			/* flag anything done this pass */
	int				collapse();		/* sub-function definition */
	int				nsrc,ndes;		/* source and destination operand nos. */
	REG				reg;			/* register work mask */
	REG				deaddest;		/* destination register */
	REG				destregs;		/* non-reg destination ea registers used */
	REG				srcregs;		/* source ea registers used */

	/* live/dead and flowgraph are valid upon entry */

	didsomething = false;			/* default to nothing optimized */

  /* loop restart point as long as we optimized something */
  restart:
	rerun = false;					/* default nothing done this pass */
	/* scan text looking for potential addressing modes to optimize toward */
	for (ALLN(node))
		{
		ndes = node->userdata.ud_op->op_dnum;	/* destination operand */
		deaddest = 0;				/* default to no register destination */
		destregs = 0;				/* default to no destination ea registers */
		if (ndes != NODEST)
		  {	drand = node->userdata.ud_rand[ndes];	/* destination RAND */
			if (drand->rtype == RT_REG)
				deaddest = drand->ruses;	/* remember destination register */
			else
				destregs = drand->ruses;	/* remember regs used in dest ea */
			}
		srcregs = 0;				/* remember all source registers used */
		for (nsrc=0; nsrc < node->userdata.ud_op->op_nsrc; nsrc++) 
			srcregs |= node->userdata.ud_rand[nsrc]->ruses; /* or regs in */
#define DEBUGX 1
#undef DEBUGX
#ifdef DEBUGX
printf("#! test %s, deaddest %lx\n", node->ops[0], deaddest);
#endif
		/* process all source operands */
		for (nsrc=0; nsrc < node->userdata.ud_op->op_nsrc; nsrc++) 
		  if (   (srand=node->userdata.ud_rand[nsrc])->rtype == RT_OREG
									/* is disp16(%an) */
			  || srand->rtype == RT_INDX
									/* or (disp8,%an,%dn) */
		   	 )
		  {	/* check source operand out */
			reg = srand->ruses ^ srand->rindx;	/* isolate address register */
#ifdef DEBUGX
printf("#! %s reg %lx, rindx %lx\n", srand->rname, reg, srand->rindx);
#endif
			if (   (   srand->rindx
									/* there is an index register */
					&& (   isdead(srand->rindx, node)
									/* it is dead after this */
						|| srand->rindx == deaddest
									/* it is clobbered as the destination */
					   )
					&& (srand->rindx & destregs) == 0
									/* register not in both operands */
					&& collapse(node, srand, srand->rindx, 
								srand->rindxscl, nsrc)
									/* attempt to optimize index register */
				   )
				|| (   (   isdead(reg, node)
									/* address register dead */
						|| reg == deaddest
									/* it is clobbered as the destination */
					   )
					&& (reg & destregs) == 0
									/* register not in both operands */
					&& collapse(node, srand, reg, 1, nsrc)
				   )
			   )
				goto nextblock;		/* altered live/dead so skip */
			} /* end check source operands */
		if (   ndes != NODEST
									/* there is a destination */
			&& ndes >= nsrc
									/* and it's not also a source */
			&& (   drand->rtype == RT_OREG
									/* destination is disp16(%an) */
				|| drand->rtype == RT_INDX
									/* destination is (disp8,%an,%dn) */
			   )
		   )
		  {	/* process destination operand */
			reg = drand->ruses ^ drand->rindx;	/* isolate address register */
#ifdef DEBUGX
printf("#! %s reg %lx, rindx %lx\n", drand->rname, reg, drand->rindx);
#endif
			if (   (   drand->rindx
									/* is an index register */
					&& isdead(drand->rindx, node)
									/* it is dead after this */
					&& (drand->rindx & srcregs) == 0
									/* register not in both operands */
				   	&& collapse(node, drand, drand->rindx,
									drand->rindxscl, ndes)
									/* it's optimized */
				   )
				|| (   isdead(reg, node)
									/* address register is dead */
					&& (reg & srcregs) == 0
									/* register not in both operands */
					&& collapse(node, drand, reg, 1, ndes)
									/* it's optimized */
				   )
			   )
			  {	/* skip to next block since live/dead no longer valid here */
	  nextblock:
		  		didsomething = true;	/* flag we optimized something */
				rerun = true;			/* flag we opimized this pass */
				while (    node != &ntail 
						&& node->op != JSR 
						&& !islabel(node) 	/* find start of next block */
						&& !isbr(node)
					  )
					node = node->forw;

				} /* end process operand */

			  } /* end process destination */
	
		} /* end for ALLN */

	if (rerun)
	  {	ldanal();			/* redo live/dead information */
		goto restart;		/* loop once again */
		}

	return( didsomething );			/* return optimized status */

} /* eaopt() */

int
collapse(inode, irand, reg, scale, opnum)
NODE	*inode;						/* instruction with operand */
RAND	*irand;						/* rand with complex addressing mode */
REG		reg;						/* register to optimize for */
int		scale;						/* scale factor (1 if none) */
int		opnum;						/* operand number */
{
	boolean		changed;			/* flag if altered RAND */
	int			newdisp;			/* potential new displacement */
	NODE		*node;				/* scan node pointer */
	RAND		*srand;				/* source rand pointer */
	RAND		*drand;				/* destination rand pointer */
	RAND		rand;				/* copy of original RAND */

	rand = *irand;					/* copy in case modified */
	changed = 0;					/* no change yet */
	/* scan backwards */
#ifdef DEBUGX
printf("#!! collapse %s, rand %s, reg %lx, scl %d, opnum %d\n",
				inode->ops[0], rand.rname, reg, scale, opnum);
#endif
	for (node=inode->back; !islabel(node) 
						   && !isbr(node)
						   && node->op != GENERIC;
											node=node->back)
	  { if (   node->op == JSR
									/* this is a subroutine call */
			&& (reg & SCRATCHREGS)
									/* our register is altered */
		   )
			break;					/* cannot optimize further */
#ifdef DEBUGX
printf("#!!  backscan %s\n", node->ops[0]);
#endif
	  	/* look for add/sub &imm,reg */
		if (   (node->op == ADD_L || node->op == SUB_L)
									/* add or sub */
			&& (srand=node->userdata.ud_rand[0])->rtype == RT_IMMNUM
									/* it is immediate */
			&& (drand=node->userdata.ud_rand[1])->rtype == RT_REG
									/* to a register */
			&& drand->ruses == reg
									/* it's to our register */
		   )
		  {	/* see if we can fold this add/sub */
			if (node->op == ADD_L)
				newdisp = rand.rdisp + srand->rdisp*scale; /* add.l */
			else
				newdisp = rand.rdisp - srand->rdisp*scale; /* sub.l */
#ifdef DEBUGX
printf("#!!   newdisp %d\n", newdisp);
#endif
			if (   (   rand.rtype == RT_OREG
					&& newdisp >= -32768	/* disp fits in disp16 */
					&& newdisp < 32768
				   )
				|| (   newdisp >= -128
					&& newdisp < 128		/* disp fits in disp8 */
				   )
			   )
			  {	/* ok to fold displacement in */
				NODE	*nextnode;	 /* remember link */

		common:
				nextnode = node->forw;	/* remember link */
				changed = true;		/* indicate need new RAND built */
				rand.rdisp = newdisp; /* new displacement */
#ifdef DEBUGX
printf("#!!! newdisp %d, newuses %lx, newrindx %lx\n", rand.rdisp,
				rand.ruses, rand.rindx);
#else
				if (peepflag)
#endif
					printf("collapse() - removed %s %s,%s\n",node->ops[0],
									srand->rname, drand->rname);
				DELNODE(node);		/* remove instruction */
				node = nextnode;	/* ready for next back step */
				continue;			/* to next previous */
				}
			} /* end add/sub handling */

		/* catch lea.l {ea},%an  folds */
		if (   node->op == LEA_L
								/* is lea.l */
			&& (drand=node->userdata.ud_rand[1])->ruses == reg
								/* lea is setting our reg */
			&& (   (   srand=node->userdata.ud_rand[0])->rtype == RT_IREG
					|| srand->rtype == RT_OREG
				   )
								/* mode is valid */
			&& (   (   (newdisp=(rand.rdisp+srand->rdisp)) >=-128
					&& newdisp < 128
				   )
								/* fits in disp8 */
				|| (   rand.rtype == RT_OREG
					&& newdisp >= -32768
					&& newdisp < 32768
				   )
								/* fits in disp16 and disp(%an) mode */
			   )
		   )
		  {	/* fold both displacement and register here */
#ifdef DEBUGX
printf("#!!! oldruses %lx, oldrindx %lx, oldreg %lx\n", rand.ruses,
				rand.rindx, reg);
#endif
			rand.ruses ^= reg;			/* remove old register */
			rand.ruses |= srand->ruses; /* add replacement */
			if (reg == rand.rindx)		/* index register? */
			  {	rand.rindx = srand->ruses; /* new index register */
				reg = srand->ruses;		/* !! now switch to this reg */
				}
			goto common;				/* enter common processing */

			} /* end lea processing */

			/* if our register is used we cannot optimize further */
			if (isused(reg, node))
			   {
#ifdef DEBUGX
printf("#!!!  isused() = true  reg %lx, node %s\n", reg, node->ops[0]);
#endif
				break;						/* can't do more if altered */
				};

		} /* end backward scan */

	if (changed)
	  {	/* we changed RAND so replace it with a correct one */
		RAND		*newrand;		/* new rand pointer */
		char	newstr[40];			/* new rand image */
		char	*instr, *outstr;	/* string copy pointers */

		instr = rand.rname;		/* point to start of rand string */
		outstr = newstr;			/* point to start of new rand string */
		if (rand.rindx == 0)		/* no index register */
			sprintf(outstr,"(%d,%s)", rand.rdisp, regchar(rand.ruses));
		else						/* index register */
			sprintf(outstr, "(%d,%s,%s.%c*%d)", rand.rdisp,
						regchar(rand.ruses ^ rand.rindx),
							regchar(rand.rindx),
								(rand.rflags & rfdotw) ? 'w' : 'l',
									rand.rindxscl);
		/* move pointer to end of string */
		while (*outstr)
			outstr++;				/* to next free position */
		/* if a bit field is involved copy selection out */
		for (instr=rand.rname;
				*instr && *instr != '{';
						instr++);
		if (*instr == '{')			/* copy if a bit field */
		  {	while((*outstr++=*instr++) != '}');
			*outstr = '\0';			/* terminate string */
			}
		newrand = saverand(newstr, strlen(newstr), RT_OREG, rand.ruses, true);
#ifdef m68k
		newrand->owuses = rand.owuses;
#endif
		setrand( inode, opnum+1, newrand);
#ifndef DEBUGX
		if (peepflag)
#endif
			printf(" eaopt() %s oldrand %s to %s\n", inode->ops[0],
							rand.rname, newrand->rname);
		}

	return (changed);				/* return success flag */

} /* collapse */

    /* usetemps() -- if any compiler scratch registers are not being used
    ** try allocating a regular register variable to them.  Doing this omits
    ** the save/restore overhead which would otherwise be required for
    ** such variables.
    **
    ** 		***********   WARNING WARNING WARNING   *************
    **
    ** This code will only work if complete dead instruction removal is
    ** performed by earlier optimization efforts.  We assume that a scratch
    ** register is re-usable if this procedure is a leaf procedure (doesn't
    ** call another procedure) and it has never been used in the function, or 
    ** has only been used as a destination.  The latter assumption is included
    ** so that we can re-use scratch registers which are also used to return
    ** function results.  If a register has never been used as a source we
    ** presume that it is safe for us to use since returned values should
    ** only be set as the very last assignment by a function before it
    ** returns.
    */

extern REG dlist[];				/* register lists in mipsup.c */
extern char * nlist[];
extern REG flist[];
extern char * fnlist[];

boolean
usetemps()
{
	boolean changed = false;			/* changed flag for caller */
	REG		rnew;						/* new register mask */
	REG		rold;						/* old register mask */
	int		i;	
	int		j;
	register NODE *node;
	RAND	* randnew;					/* new operand */
	RAND	* randold;					/* old operand */
	char	* snew;						/* new reg string */
	char	* sold;						/* old reg string */

	if (   !leafproc			/* if this function calls another ... */
		|| genericpass > 0		/* or it has generic instructions ... */
	   ) 
		return(false);		/* can't use temps */

	/* scan register mask for temporary registers */
	for (rnew=1; rnew>0; rnew<<=1)
		/* if scratch register which has not been used as a source */
		if (   (rnew & SCRATCHREGS)!=0
			&& (rnew & funcsrc)==0
		   )
		{	/* we have a scratch register to use */
			if (tempflag) printf("usetemps() scratch available %#.8x\n", rnew);
			/* see if we can match it with a register variable of same type */
			for (rold=1; rold>0; rold<<=1)
			  if (	 (rold & funclive)!=0
				  && (   ( (rold & DVAR_REGS)!=0 && (rnew & D_SCRATCH)!=0 )
					  || ( (rold & AVAR_REGS)!=0 && (rnew & A_SCRATCH)!=0 )
#ifdef M68881
					  || ( (rold & FPVAR_REGS)!=0 && (rnew & FP_SCRATCH)!=0 )
#endif
					 )
				 )
			  {	/* all conditions met - now change it to the scratch */
				changed = true;					/* indicate change */
				funclive &= ~rold;				/* remove old from live list */
				funcsrc |= rnew;				/* add new to source list */
				/* find the character representation of both registers */
#ifdef M68881
				if (rnew & FP_SCRATCH)
				{	for (i=1; flist[i]!=rnew && flist[i]!=0; i++);
					snew = fnlist[i];			/* new reg id */
					for (i=1; flist[i]!=rold && flist[i]!=0; i++);
					sold = fnlist[i];			/* old reg id */
					}
				else
#endif
				{	for (i=1; dlist[i]!=rnew && dlist[i]!=0; i++);
					snew = nlist[i];			/* new reg id */
					for (i=1; dlist[i]!=rold && dlist[i]!=0; i++);
					sold = nlist[i];			/* old reg id */
					}
				if (*sold==0 || *snew==0)
					oerr("Bad register search in usetemps()\n");
				if (tempflag) printf("Replacing %s with %s\n", sold, snew);
				/* find the RAND structure for both old and new */
				randold = findrand(sold, false);
				randnew = saverand(snew, strlen(snew), RT_REG, rnew, true);
#ifdef m68k
				randnew->owuses = 0;
#endif
				/* scan function replacing old register operand with the new */
				for (ALLN(node))
				{	j = node->userdata.ud_op->op_nsrc;	/* source operands */
					i = node->userdata.ud_op->op_dnum;  /* destination number */
					if ( i!= NODEST && i>= j) j = i+1;	/* process dest also */
					while (j>0)
					{	if (node->userdata.ud_rand[j-1]==randold)
						{	if (tempflag) printf("newrand #%d for %s\n", j, node->ops[0]);
							setrand(node, j, randnew);
							}
						--j;
						}
					}
				/* replace all occurances of the old register and it's mask */
				/* with the new in all operands in the hash operand table */
				chgregrand(snew, rnew, sold, rold);

				break;						/* to next scratch regiser */

				} /* end matched var with scratch register type */

			} /* end found scratch register to use */

	return (changed);				/* indicate if we've changed anything */
}
#ifdef M68881

			/* preprocfp -- preprocess floating-point instructions */

/*	This routine first turns off the "sets condition codes" bit in the user
**	data if this floating-point instruction stores into memory. Such
**	instructions don't set the floating-point condition code register.
**
**	If the instruction has a constant immediate operand of type double
**	then we see if the value will fit within the range of a single.  
**	If so we change the operand which saves space and execution time.
**	The floating-point formats of interest are:
**
**		double
**
**		seee eeee eeee mmmm mmmm mmmm mmmm mmmm		exponent 11 bits
**		mmmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm		significand 52 bits
**
**		single
**
**		seee eeee emmm mmmm mmmm mmmm mmmm mmmm		exponent 8 bits
**													significand 23 bits
**
**	where s is the sign, e the exponent biased 1023 for double and 127 for
**	single, and m is the significand.  We can replace the double with a
**	single if the unbiased exponent is within a +-126 range and the 
**	excess 29 right-most significant bits are zero.
*/

void
preprocfp()
{
	RAND **rands = lastnode->userdata.ud_rand;		/* point to operands */
	OPCODE	newop;							/* possible new .S opcode */
	int		isfcmp = 0;						/* floating-point compare */

	/* must flag floating compare since it's operands are reversed */
	switch(lastnode->op)
	  {	case FCMP_B:
		case FCMP_W:
		case FCMP_L:
		case FCMP_S:
		case FCMP_D:
		case FCMP_X:
			isfcmp = true;
	  }
	/* if store to memory then turn off CC setting bit */
	if (rands[1] != RNULL && !isfpreg(rands[1]) && !isfcmp)
		lastnode->userdata.ud_flags &= ~(UD_CC | UD_CCA);

	/* else if source is immediate and the right length for a double */
	else if (  (   rands[0]->rtype == RT_IMMNUM 
				&& strlen(rands[0]->rname) == 19 
			   )
			|| (   rands[1] != RNULL
				&& rands[1]->rtype == RT_IMMNUM
				&& strlen(rands[1]->rname) == 19
			   )
		    )

	  {	/* find the correct new opcode assumming this immediate is double */
		newop = OP_MAX + 1;					/* set to no op found */
		switch (lastnode->op)
		{ case FABS_D:		newop = FABS_S; 	break;
		  case FACOS_D:		newop = FACOS_S; 	break;
		  case FADD_D:		newop = FADD_S;		break;
		  case FASIN_D:		newop = FASIN_S;	break;
		  case FATAN_D:		newop = FATAN_S;	break;
		  case FATANH_D:	newop = FATANH_S;	break;
		  case FCMP_D:		newop = FCMP_S;		break;
		  case FCOS_D:		newop = FCOS_S;		break;
		  case FCOSH_D:		newop = FCOSH_S;	break;
		  case FDIV_D:		newop = FDIV_S;		break;
		  case FETOX_D:		newop = FETOX_S;	break;
		  case FETOXM1_D:	newop = FETOXM1_S;	break;
		  case FGETEXP_D:	newop = FGETEXP_S;	break;
		  case FGETMAN_D:	newop = FGETMAN_S;	break;
		  case FINT_D:		newop = FINT_S;		break;
		  case FINTRZ_D:	newop = FINTRZ_S;	break;
		  case FLOG10_D:	newop = FLOG10_S;	break;
		  case FLOG2_D:		newop = FLOG2_S;	break;
		  case FLOGN_D:		newop = FLOGN_S;	break;
		  case FLOGNP1_D:	newop = FLOGNP1_S;	break;
		  case FMOD_D:		newop = FMOD_S;		break;
		  case FMOV_D:		newop = FMOV_S;		break;
		  case FMUL_D:		newop = FMUL_S;		break;
		  case FNEG_D:		newop = FNEG_S;		break;
		  case FREM_D:		newop = FREM_S;		break;
		  case FSCALE_D:	newop = FSCALE_S;	break;
		  case FSGLDIV_D:	newop = FSGLDIV_S;	break;
		  case FSGLMUL_D:	newop = FSGLMUL_S;	break;
		  case FSIN_D:		newop = FSIN_S;		break;
		  case FSINCOS_D:	newop = FSINCOS_S;	break;
		  case FSINH_D:		newop = FSINH_S;	break;
		  case FSQRT_D:		newop = FSQRT_S;	break;
		  case FSUB_D:		newop = FSUB_S;		break;
		  case FTAN_D:		newop = FTAN_S;		break;
		  case FTANH_D:		newop = FTANH_S;	break;
		  case FTENTOX_D:	newop = FTENTOX_S;	break;
		  case FTEST_D:		newop = FTEST_S;	break;
		  case FTWOTOX_D:	newop = FTWOTOX_S;	break;
		  }

		if (newop <= OP_MAX)
		  {	/* we've got out opcode. now parse the constant */
			char	c;							/* work character */
			int		sign;						/* sign of float */
			int		exp;						/* exponent of float */
			unsigned long sig1 = 0;				/* first half of double */
			unsigned long sig2 = 0;				/* second half of double */
			char	*scan;						/* immediate scan pointer */
			char	newrand[40];				/* new immediate string */
			int		i;							/* work integer */
			
			/* FCMP has immediates on the right side */
			scan = isfcmp ? rands[1]->rname : rands[0]->rname;
			scan +=3;							/* past '&0x' */
			for (i=1; i<=16; i++)
			  {	c = *scan++;
				if (c > '9') c = c - 'a' + 10;	/* hex */
				  else 		 c = c - '0';		/* dec */
				sig1 = (sig1<<4) + (sig2>>28);
				sig2 = (sig2<<4) + c;
				}
			sign = (sig1 & 0x80000000) != 0;		/* sign */
			exp = ( (sig1>>20) & 0x7ff ) - 1023;	/* unbiased exponent */
			sig1 = sig1 & 0x000fffff;				/* strip both */

			/* now see if it will fit in a single */
			if (   (exp > -127 && exp < 127 && (sig2 & 0x1fffffff) == 0)
											/* fits in a single */
				|| (exp == -1023 && sig1 == 0 && sig2 == 0)
											/* it's zero */
			   )
			  {	/* change the instruction and operand */
				setop(lastnode, getOP(newop));
				if (exp == -1023) sig1 = 0;
				else sig1 = (sign<<31) | (exp+127)<<23 | (sig1<<3) | (sig2>>29);
				i = sprintf(newrand, "&0x%.8x", sig1);
				setrand(lastnode, isfcmp ? 2 : 1, 
							saverand(newrand, i, RT_IMMNUM, REG_NONE, true) );
				/* if fcmp %fpn,&zero change to a ftst */
				if (isfcmp && sig1 == 0)
				  {	setop(lastnode, getOP(FTEST_X));
					lastnode->ops[2] = NULL;		          	/* clear 2nd operand */
					lastnode->userdata.ud_rand[1] = RNULL;	/* strip 2nd rand */
					}
				if (vtprintflag) 
					printf("float constant compressed: %s\n", rands[0]->rname);
				} /* end replace operand */

			} /* end opcode and replacement found */

		} /* end immediate source and of right string length */

	return;
}
#endif
#ifdef m68k

				/* sccopt() - convert if/else to Scc */

/*	Detect C constructs of the following form:
**
**			if (<exp>) i = 1;
**			else       i = 2;
**
**	and when the constants differ by a value of one (1) and range from
**	-9 to +9 then alter the code to use the Scc instruction.
**
**	The compiler input and our alteration looks like:
**
**		bcc      yyy                   scc.b     %dn
**		mov.l    &1,%dn                add/sub.b &con,%dn  <IF REQUIRED>
**		bra      zzz            to     extb.l    %dn
** yyy:                                bra       zzz
**		mov.l    &2,%dn
** zzz:
**
** We do not asume that the block beginning 'yyy' is dead so it and the branch
** around it remain. In most cases it will be deleted by dead code removal.
*/

int
sccopt()
{
	BLOCK	*b1;					/* block ending with initial bcc */
	BLOCK	*b2;					/* block containing first mov.l */
	BLOCK	*b3;					/* block containing second mov.l */
	int		imm1;					/* first immediate number */
	int		imm2;					/* second immediate number */
	RAND	*rand;					/* RAND work pointer */
	RAND	*regrand;				/* %dn RAND pointer */
	NODE	*node;					/* node pointer */
	NODE	*mov1;					/* first mov pointer */
	NODE	*mov2;					/* second mov pointer */
	int		returnv;				/* returned value */
	OPCODE	newop;					/* scc equivalent for bcc */
	char	immedstr[3];			/* string for add/sub immediate */

#if 0
	/* NOT NEEDED */
	bldgr(false);
#endif
	returnv = false;				/* default nothing optimized */

	/* recursive entry as long as we optimize something */
 reexecute:
	for (ALLB(b1, b0.next))
		if (   (b2=b1->nextl) != NULL
									/* there is a next block */
			&& (b3=b1->nextr) != NULL
									/* we have a bcc after first block */
			&& (mov1=b2->firstn)->op == MOV_L
			&& (   b2->length == 1
				|| (   b2->length == 2
					&& b2->lastn->op == BRA
				   )
			   )
									/* second block only does a mov.l */
			&& (   (   b3->length == 1
					&& (mov2=b3->lastn)->op == MOV_L
				   )
				|| (   b3->length == 2
					&& b3->lastn->op == BRA
					&& (mov2=b3->lastn->back)->op == MOV_L
				   )
			   )
									/* third block only does a mov.l */
			&& b2->nextl == b3->nextl
									/* both blocks merge */
			&& isdreg((regrand=mov1->userdata.ud_rand[1]))
									/* first mov.l 2nd operand is %dn */
			&& regrand == mov2->userdata.ud_rand[1]
									/* and second move has same operand */
			&& isdead(REG_CC, mov1)
									/* make sure condition code is dead */
			&& (rand=mov1->userdata.ud_rand[0])->rtype == RT_IMMNUM
									/* first move is immediate */
			&& (imm1=rand->rdisp) < 10
			&& imm1 > -10
									/* and within range */
			&& (rand=mov2->userdata.ud_rand[0])->rtype == RT_IMMNUM
									/* second move is immediate */
			&& (imm2=rand->rdisp) < 10
			&& imm2 > -10
									/* and within range */
			&& (   imm1 == (imm2-1)
				|| imm2 == (imm1-1)
			   )
									/* one number is one less than other */
		   ) /* whew! */
		  {	/* We can replace this construct with Scc */
			node = b1->lastn;		/* point to initial bcc */
			/* find Scc equivalant and it's opposite for the bcc */
			switch(node->op)
			{ case BCC:	newop = SCC_B;	break;
			  case BCS: newop = SCS_B;	break;
			  case BEQ: newop = SEQ_B;	break;
			  case BGE: newop = SGE_B;	break;
			  case BGT: newop = SGT_B;	break;
			  case BHI:	newop = SHI_B;	break;
			  case BHS: newop = SHS_B;	break;
			  case BLE: newop = SLE_B;	break;
			  case BLO: newop = SLO_B;	break;
			  case BLS: newop = SLS_B;	break;
			  case BLT: newop = SLT_B;	break;
			  case BMI: newop = SMI_B;	break;
			  case BNE: newop = SNE_B;	break;
			  case BPL: newop = SPL_B;	break;
			  case BVC: newop = SVC_B;	break;
			  case BVS: newop = SVS_B;	break;
#ifdef M68881
			  case FBEQ: newop = FSEQ;	break;
			  case FBGE: newop = FSGE;	break;
			  case FBGT: newop = FSGT;	break;
			  case FBLE: newop = FSLE;	break;
			  case FBLT: newop = FSLT;	break;
			  case FBNEQ: newop = FSNEQ;	break;
			  case FBNGE: newop = FSLT;	break;		/* jcl: added following 4 */
			  case FBNGT: newop = FSLE;	break;
			  case FBNLE: newop = FSGT;	break;
			  case FBNLT: newop = FSGE;	break;
#endif
			  default:
					oerr("sccopt: invalid bcc");
			  } /* end switch */
			setop(node, getOP(newop));		/* set in new instruction */
			setrand(node, 1, regrand);		/* set %dn as 1st operand */
			/* reverse the condition if we should gen lower number */
			if (imm2 > imm1)
				setop(node, getOP(node->userdata.ud_op->op_opposite));
			else
				imm1 = imm2;				/* imm1 is smaller */
			/* if result is not -1/0 then we need to add/sub immediate */
			if (imm1 != -1)
			  {	NODE	*newnode = GETSTR(NODE);	/* new node */

				newnode->uniqid = IDVAL;
				newnode->userdata.ud_nattr = 0;	/* clear new fields */
				setop(newnode, getOP((imm1 > -1) ? ADD_B : SUB_B));
				/* build the immediate value */
				immedstr[0] = '&';
				if (imm1 > -1)
					imm1 += 1;
				else
					imm1 = -imm1 - 1;		/* proper displacement */
				immedstr[1] = imm1 + '0';	/* to character */
				immedstr[2] = '\0';			/* end of string */
				/* set rands in */
				setrand(newnode, 1, saverand(immedstr, 2, RT_IMMNUM, 0, true));
				setrand(newnode, 2, regrand);
				INSNODE(newnode, mov1);		/* insert behind the new scc */
				}
			/* change the first mov.l to extb.l */
			setop(mov1, getOP((OPCODE) EXTB_L));	/* set to extb.l */
			setrand(mov1, 1, regrand);		/* %dn as operand */
			mov1->ops[2] = NULL;			/* clear 2nd operand */
			mov1->userdata.ud_rand[1] = RNULL;
			returnv = true;					/* indicate we optimized */
			bldgr(false);					/* rebuild the flow graph */
			goto reexecute;					/* and try this all again */
			}

	return ( returnv );					/* indicate success */

} /* sccopt() */
#endif
