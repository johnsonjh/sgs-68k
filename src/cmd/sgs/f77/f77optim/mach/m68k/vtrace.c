/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) vtrace.c: version 25.1 created on 12/2/91 at 17:41:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)vtrace.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	MOT:vtrace.c	7.2		*/

#ident	"@(#)f77/f77optim/mach/m68k:vtrace.c	25.1"

/* vtrace.c */
/*MOT:vtrace.c	7.2		85/09/18"*/

/*	MC680X0 Optimizer	*/

/* This module contains routines that do a value trace over the program
** nodes in a single lexical sweep.  The idea is to identify places
** where a memory reference can be replaced by a register reference.
** We also do limited peephole improvements at each step.  The value
** trace often facilitates these.
**
** The algorithm works roughly like this:
**
** We have a value cache, which is a representation of data in registers.
** As we examine each instruction node, we:
**
**	1) Try to replace a memory reference by a use of an equivalent
**		operand in the cache whose value is in a register.
**	2) Update the cache by eliminating values which may be destroyed
**		by the instruction, and by adding values which are
**		newly present.
**
** The mov.[bwl] family is treated as the only instructions which can
** add things to the cache.  Many address modes destroy the cache.  For
** example, add.w &1,(%a3) must be assumed to destroy all cached values,
** because it's nearly impossible to tell just what %a3 was pointing at
** (except for immediate values).
** Therefore, as a conservative measure we assume the worst.
*/

extern   char *strrchr();
#include "optim.h"
#include "debug.h"
/* Define the value cache and related items. */

/* This is the basic structure. */

struct val
{
    REG inreg;			/* REGister bit mask for the register in which
				** the cached value lives
				*/
    RAND * regrand;		/* the RAND structure for that register */
    RAND * rand;		/* the RAND structure for the register's
				** contents
				*/
    unsigned short size;	/* size of the operand in bytes */
};

typedef struct val VAL;		/* a shorthand */

/* The values are held in an array which is searched associatively.
** There can be more than one value associated with a register.
*/

/* Size of array.  Note that increasing the size of the array does
** not necessarily mean that a better value trace will occur.  The
** lifetime of items in the value cache is relatively short anyway,
** and clearing it each time is costly.
*/

#define	NUMVALS	20		/* maximum number of values in array */

static VAL valarray[NUMVALS];	/* the cache array */
static int numcache = 0;	/* number of real values currently in cache */

enum sdflag {src, dest};	/* source/destination flag:  gets used in a
				** couple of places
				*/
/* These macros simplify the routines the are defined below. */

/* This first one scans the value array for non-empty entries, and is used as:
**	for ALLVAL(ptr,n) {...}
** where
**	ptr	is VAL *, and points to a non-empty value entry
**	n	is a bookkeeping counter
*/

#define	ALLVAL(ptr,n) \
	(ptr=valarray, n=numcache; n > 0; ptr++) \
	    if ((ptr->regrand != RNULL) ? n-- : 0)


/* This macro kills an entry in the table.  It is used as
**	clearval(ptr)
**
** where
**	ptr	is VAL *
*/

#define	clearval(ptr)	ptr->regrand = RNULL
/* The value trace mechanism depends on five primitives.  They
** are defined below.
*/


/* vclear -- clear the cache
**
** This routine clears the cache of all entries.
*/

static void
vclear()
{
    register VAL * valptr;		/* pointer to cache array item */
    register int count;

	TRACE(vclear);
    for ALLVAL(valptr,count)		/* clear all non-empty entries */
	clearval(valptr);
    
    numcache = 0;			/* no cached values */
    return;
}
/* vreset -- reset cache contents
**
** This routine is similar to vclear(), except it discards only those
** elements affected by memory writes.  In particular it retains
** immediate operands that are in registers.
*/

static void
vreset()
{
    register VAL * valptr;		/* pointer to cache array item */
    register int count;
    register RTYPE rtype;		/* operand type of register contents */

	TRACE(vreset);
    for ALLVAL(valptr,count)		/* clear anything not an immediate */
	if (   (rtype = valptr->rand->rtype) != RT_IMMNUM
	    && rtype != RT_IMMED
	    )
	{
	    clearval(valptr);		/* discard cache value */
	    numcache--;			/* one less value in cache */
	}
    return;
}
/* vfind -- find an operand in the cache
**
** This routine looks for an operand in the cache and returns a
** pointer to the RAND entry for the corresponding register.
** The operand must be of the proper size.
**
** Choose D registers preferentially, since A registers have many
** restrictions.
*/

static RAND *
vfind(rand,size)
RAND * rand;				/* operand we're looking for */
unsigned int size;			/* size of desired operand */
{
    register VAL * valptr;		/* pointer to cache element */
    register int count;			/* bookkeeper */
    RAND * found = RNULL;		/* operand to return */

    /* certain operand addressing modes are never in the cache: */

	TRACE(vfind);
    switch (rand->rtype)
    {
    case RT_REG:
    case RT_INCR:
    case RT_DECR:
	return(RNULL);			/* say not found */
    
    default:
	break;				/* look for the rest */
    }

    for ALLVAL(valptr, count)
	if (valptr->rand == rand && valptr->size == size)
	{
	    found = valptr->regrand;	/* found one */
	    if ((found->ruses & REG_DREG) != 0)
		return(found);		/* return a value in a d register */
	}
    
    return(found);			/* return whatever we found */
}
/* vset -- set value in cache
**
** This routine sets a cache value.  We assume any killing of related
** registers has been done explicitly elsewhere.
*/

static void
vset(regrand, rand, size)
RAND * regrand;				/* new register we're setting */
RAND * rand;				/* the value for that register */
unsigned int size;			/* size of operand being cached */
{
    register VAL * valptr;		/* pointer to cache entry */
    register int count;			/* for bookkeeping */

    /* Disallow setting operands where the operand in the cache uses the
    ** same register as the register in which it's said to reside.
    ** (This is a conservative assumption.  It eliminates things like
    **		mov.l	%a0,4(%a0)
    **
    ** but guards against things like
    **
    **		mov.l	4(%a0),%a0
    */

	TRACE(vset);
    if ((regrand->ruses & rand->ruses) != 0)
	return;				/* operand uses register */

    /* Prevent certain address modes from being set in cache */

    switch (rand->rtype)
    {
    case RT_INCR:
    case RT_DECR:
	return;				/* forget these */
    
    case RT_REG:			/* for register, copy all operands
					** related to rand for "regrand",
					** as well
					*/
	if (rand == regrand)
	    return;			/* no need to copy same reg. */
	
	for ALLVAL(valptr, count)
	    if (valptr->inreg == rand->ruses && valptr->size == size)
		vset(regrand, valptr->rand, valptr->size);
	return;

    default:
	break;				/* accept these */
    }
    /* (We must run this loop explicitly, rather than use the ALLVAL macro,
    ** because we want empty entries here
    */

    for (valptr = valarray, count=NUMVALS; count > 0; valptr++, count--)
	if (valptr->regrand == RNULL)	/* found empty entry */
	{
	    valptr->regrand = regrand;	/* set holding register */
	    if (regrand->rtype != RT_REG) /* make sure it IS a register */
		oerr("vset:  non-register being set");
	    
	    valptr->inreg = regrand->ruses; /* set register's bit mask */
	    valptr->rand = rand;
	    valptr->size = size;
	    numcache++;			/* another cached value */
	    return;
	}
    
    return;				/* couldn't cache value because out
					** of room
					*/
}
/* vkill -- kill cache items based on an operand
**
** This routine invalidates cache items that are affected by an operand.
** We need to know whether the argument operand is a source or destination
** operand, as this affects what things are invalidated.  The invalidation
** is driven by operand types.
*/

static void
vkill(rand,flag)
RAND * rand;				/* the operand invalidating things */
enum sdflag flag;			/* type of operand */
{
    register VAL * valptr;		/* pointer to cache entries */
    register int count;			/* bookkeeper */
    RTYPE rtype = rand->rtype;		/* operand type */
    register REG reg;			/* operand register bits */

	TRACE(vkill);
    switch( rtype )
    {
    case RT_REG:			/* register direct */
	if (flag == src)		/* source operands don't affect stuff */
	    return;
	break;				/* but destination ones do! */
    
    case RT_DECR:			/* pre-decrement */
	if (flag == dest && rand->ruses == REG_SP)
					/* if push onto stack... */
	    break;			/* needn't clear everything */
	/* fall through */
    case RT_INCR:			/* post-increment */
	if (flag == src)		/* for source operands, only things
					** using the register are invalid
					*/
	    break;
	
	vreset();			/* otherwise, memory is affected
					** and we don't know what specifically
					*/
	break;
    case RT_IREG:			/* register indirect */
	if (flag == src)
	    return;			/* no problem as source */
	if (rand->ruses != REG_SP)	/* if not stack destination, reset */
	{
	    vreset();
	    return;
	}
	/* fall through, clear out other reference to (%sp) */
    case RT_MEM:			/* memory direct (or stack item) */
    case RT_SMEM:
	if (flag == dest)		/* assume only the destination is
					** affected
					*/
	    for ALLVAL(valptr,count){
		if (valptr->rand == rand)
		{
		    clearval(valptr);	/* kill off this value */
		    numcache--;		/* one less item in cache */
		}
		if ( valptr->rand->rtype == rand->rtype 
			&& valptr->rand->ruses == rand->ruses
			&& ((long)valptr->rand->dsize > 0 && (long)valptr->rand->dsize < 5 
			&& (long)rand->dsize > 0 && (long)rand->dsize < 5)){
			register int nl;
			register long poff, vpoff;
			register long pdsize=(long)rand->dsize;
			register long vpdsize=(long)valptr->rand->dsize;
			register char *vp =  valptr->rand->rname;
			register char *p =  rand->rname;
			if( ! rand->ruses){ /* form name + dig */
				nl = strspn(vp,p);
				if(!nl)continue; /* no name match */
				vp += nl;
				p += nl;
				if ( *p == '\0' ) poff = 0;
				else if (isdigit(*p) && (*(p-1) == '+'
					|| *(p-1) == '-' ))
					poff=atol(p-1);
				else if( *p == '+' || *p == '-' )
					poff=atol(p);
				else {continue;}
				if ( *vp == '\0' ) vpoff = 0;
				else if (isdigit(*vp) && (*(vp-1) == '+'
					|| *(vp-1) == '-' ))
					vpoff=atol(vp-1);
				else if( *vp == '+' || *vp == '-' )
					vpoff=atol(vp);
				else { continue;}
				if(poff==vpoff && ((poff+pdsize) == (vpoff+vpdsize))){continue;}
				if( vpoff <= poff && poff+pdsize-1 
				<= vpoff+vpdsize-1)
				{
		    			clearval(valptr);	
					numcache--;	
				}
			}else{			/* form dig + reg */
				poff = atol(p);
				vpoff = atol(vp);
				if(!strcmp(p,vp) && poff==vpoff && ((poff+pdsize) == (vpoff+vpdsize))){continue;}
				p=strrchr(p,'+');
				vp=strrchr(vp,'+');
				if( !strcmp(p,vp) && (vpoff <= poff 
				&& poff+pdsize-1 
				<= vpoff+vpdsize-1))
				{
		    			clearval(valptr);	
					numcache--;	
				}
			} /* ruses */
		} /* rtype and ruses same */
	   }/* end for */
	return;
#ifdef old
    case RT_MEM:			/* memory direct (or stack item) */
    case RT_SMEM:
	if (flag == dest)		/* assume only the destination is
					** affected
					*/
	    for ALLVAL(valptr,count)
		if (valptr->rand == rand)
		{
		    clearval(valptr);	/* kill off this value */
		    numcache--;		/* one less item in cache */
		}
	return;
#endif

#if m68k
    case RT_REG_PAIR:			/* Ri:Rj for '020 mul and div */
	break;				/* These set both Ri and Rj   */

    case RT_020:
#endif
    case RT_OREG:			/* offset from register */
    case RT_DBLIND:			/* double index */
	if (flag == dest)		/* if these are destinations, all is
					** lost, so to speak
					*/
	    vreset();			/* reset cache */

    /* fall through */

    case RT_IMMNUM:			/* immediate number */
    case RT_IMMED:			/* general immediate */
    case RT_RTSUB:			/* special subroutine label */
    case RT_SUB01:			/* special subroutine label */
    case RT_LABEL:			/* normal label */
	return;				/* these have no ill effects */

    default:				/* something else */
	oerr("Missing operand type in vkill");
    }
/* Reaching here, we need to invalidate all items in the cache which
**
**	a) are associated with some register the operand uses, or
**	b) have as the "regrand" one of the registers the operand uses
**
** Consider these cache entries:
**
**	%d0	contains	10(%a0)
**	%d1	contains	&35
**
** An example of the first case is a source operand "-(%a0)".
** An example of the second is a destination operand of "%d1".
*/

    for ALLVAL(valptr,count)
	if (   (valptr->rand->ruses & (reg = rand->ruses)) != 0
					/* case a */
	    || (valptr->inreg & reg) != 0
					/* case b */
	    )
	{
	    clearval(valptr);		/* eliminate from the cache */
	    numcache--;			/* indicate one less entry */
	}

    return;
}
/* vprint -- print value cache
**
** Print contents of value cache for debugging purposes.
*/

static void
vprint()
{
    register VAL * valptr;		/* pointer to cache entry */
    register int count;			/* bookkeeping */

    PRINTF("%d items in cache\n", numcache);

    for ALLVAL(valptr, count)
	PRINTF("\t\t\t\t%s\tcontains\t%s\n", valptr->regrand->rname,
		valptr->rand->rname);
    
    return;
}
/* vtrace -- value trace routine
**
** This is the routine that actually does the value trace.  It scans
** all of the instruction nodes once, trying to substitute register
** uses for other operands.
*/

/* Define macro */

#define	changedto(flag)	if(flag) \
			PRINTF(">>>>> changed to\n"), \
			prinst(node)
void
vtrace(tflag, peepflag)
boolean tflag;				/* true to print debugging info */
boolean peepflag;			/* print progress of peephole */
{
    OP * op;				/* current node's OP structure */
    unsigned int nsrc;			/* number of source operands in
					** current node
					*/
    register RAND ** rands;		/* current node's operands */
    register NODE * node;		/* current node */
    RAND * newrand;			/* replacement operand */
    boolean changed;			/* whether instruction changed */
    unsigned int dnum;			/* destination operand number */
    unsigned int size;			/* operand size */
    boolean usedld = false;		/* true if used (and changed) live/dead
					** information
					*/
    extern boolean dflag;		/* print live/dead info */
    extern boolean hflag;		/* true to inhibit peephole */
    void prinst();
    void vrevbr();			/* special branch reverser */
    boolean vchgtst();			/* special tst changer */
    void killld();			/* kill current live/dead knowledge */
    void dold();
    boolean peep();

	TRACE(vtrace);
    vclear();				/* initialize value cache */
    dold();				/* get live/dead information for
					** peephole and vchgtst
					*/

    for (ALLN(node))
    {
	if (tflag)
	    vprint();			/* print current cache */
	if (dflag)			/* print live/dead info? */
	    PRINTF("%c(live: 0x%.6x)", CC, node->nlive);
	if (tflag || dflag || peepflag)
	    prinst(node);		/* print instruction */

	rands = node->userdata.ud_rand; /* point at operands */

	/* handle non-standard value-trace ops */

	if ((node->userdata.ud_flags & UD_VTRACE) != 0)
	    switch(node->op)
	    {
	    case BSR:			/* assume all is lost for these */
	    case BSR_B:
	    case JSR:
	    case MOVM_L:		/* (conservative for these three) */
	    case MOVM_W:
#ifdef M68881
	    case FMOVM:
#endif
	    case LABEL:			/* label kills all because flow merges
					** here
					*/
		vclear();
		break;
	    
	    case MOVP_L:		/* assume bad things for MOVP */
	    case MOVP_W:
		vkill(rands[0], src);
		vkill(rands[1], dest);
		break;
	    
	    case LEA_L:			/* kill destination */
		vkill(rands[1], dest);
		break;
	    
	    case PEA_L:			/* don't examine source at all */
		break;

	    case EXG:			/* both operands act as destinations */
		vkill(rands[0], dest);
		vkill(rands[1], dest);
		break;
	    
	    default:			/* not found */
		oerr("Special case not found in vtrace");
	    /* The cmp.[bwl] instructions require some special processing for
	    ** two reasons:  they are badly non-orthogonal, and the order in
	    ** which the operands appear in the assembly language is the
	    ** reverse of the execution order.
	    **
	    ** We need to be careful about changing the cmp.[bwl] operands
	    ** in one special case:
	    **
	    **		cmp.[bwl]	E,&n
	    **
	    ** This could get changed into
	    **
	    **		cmp.[bwl]	E,%d0	(say)
	    **
	    ** if the literal was in a register.  Unfortunately, this form
	    ** of the instruction doesn't exist, although
	    **
	    **		cmp.[bwl]	%d0,E
	    **
	    ** does.  Therefore, we change the order and reverse the
	    ** (presumed) following conditional branch for this case.
	    */

	    case CMP_B:
	    case CMP_W:
	    case CMP_L:
		size = node->userdata.ud_op->op_ssize[0];

		/* change both operands, if possible, but in reversed order */

		if ((newrand = vfind(rands[1], size)) != RNULL)
		{
		    setrand(node, 2, newrand);	/* have replacement */
		    changed = true;
		}
		vkill(rands[1], src);		/* kill affected registers */

		if ((newrand = vfind(rands[0], size)) != RNULL)
		{
		    setrand(node, 1, newrand);
		    changed = true;
		}
		vkill(rands[0], src);
		/* now check for the funny case mentioned above */

		if (changed && isdreg(rands[1]) && ! isreg(rands[0]))
		{
		    if (! isrev(node->forw))	/* better be reversible */
			oerr("vtrace in trouble with cmp");
		    
		    newrand = rands[0];		/* interchange operands */
		    setrand(node, 1, rands[1]);	/* (beware operand numbering */
		    setrand(node, 2, newrand);
		    vrevbr(node->forw);		/* reverse following branch */
		}

		changedto(tflag && changed);	/* comment on changes */
		break;				/* onward! */
#ifdef M68881
	    /* The fcmp.[xsd] instructions must have a floating point register
	    ** as the first operand; if the second operand is not a register
	    ** we attempt to find a value in an %fp register as a substition
	    */

	    case FCMP_X:
	    case FCMP_D:
	    case FCMP_S:
		size = node->userdata.ud_op->op_ssize[1];

	 	if( ( ( newrand = vfind( rands[1], size ) ) != NULL  ) &&
		      isfpreg( newrand ) ) 
	    	{	changed = true;
			setrand( node, 2, newrand );
	    	}
	
	    	vkill( rands[1], src );
		changedto(tflag && changed);	/* comment on changes */
	    	break;

#endif
	    /* For tst.[bwl] instructions, the replacement cannot live in an
	    ** address register because there is no
	    **		tst.[bwl] %an
	    ** instruction.  However, if there's an available D register,
	    ** we can move the A register to a D register, thus setting the
	    ** condition codes and getting a comparable result.
	    **
	    ** Under happier circumstances this improvement (move A to D)
	    ** would be purely a peephole improvement, but we need to know
	    ** the value cache state to pick a register that contains the
	    ** operand.
	    */

	    case TST_B:
	    case TST_W:
	    case TST_L:
		size = node->userdata.ud_op->op_ssize[0];

		newrand = vfind(rands[0], size);
					/* try to find replacement */
		if (newrand == RNULL)
		    break;		/* no replacement possible */

		if (isareg(newrand))	/* if replacement in A reg, call
					** special routine
					*/
		{
		    if (! vchgtst(node, newrand, size))
			break;		/* no change made */
		    
		    usedld = true;	/* we have used/changed live/dead */
		}
		else
		    setrand(node, 1, newrand);
					/* found suitable one; replace */

		changedto(tflag);	/* report change */
		break;
/* The mov.[bwl] instructions are the only ones that actually SET a
** cache value.  They are handled specially.
*/

	    case MOVS_B:
	    case MOVS_W:
	    case MOVS_L:
	    case MOV_B:
	    case MOV_W:
	    case MOV_L:
#ifdef M68881
	    case FMOV_B:
	    case FMOV_L:
	    case FMOV_W:
	    case FMOV_S:
	    case FMOV_D:
	    case FMOV_X:
#endif
		size = node->userdata.ud_op->op_ssize[0];

		if( redund_mov( node, rands, size, tflag ) )
		/* if move is redundant, delete it */
		{	usedld = true;
			break;
		}

		if ((newrand = vfind(rands[0], size)) != RNULL)
		{			/* we have a replacement */
		    setrand(node, 1, newrand);
		    changedto(tflag);	/* report change */
		}

		vkill(rands[0], src); /* kill things affected by source */

		/* kill destination, unless same as source */

		if (rands[0] != rands[1])
		{
		    vkill(rands[1], dest);

		    /* set new cache elements, as appropriate */

		    if (rands[1]->rtype == RT_REG)
			vset(rands[1], rands[0], size);

		    /* Add first operand to cache if it's a register, but
		    ** only if the destination isn't.  Otherwise we get
		    ** duplicate entries.
		    */

		    else if (rands[0]->rtype == RT_REG)
			vset(rands[0], rands[1], size);
		}
		break;			/* end mov.[bwl] case */
	/* hanna Fri Oct 18 14:20:42 PDT 1985
	 * These guys won't work if the source is an address register
	 */
	case AND_L:
	case AND_W:
	case AND_B:
	case EOR_L:
	case EOR_W:
	case EOR_B:
	case OR_L:
	case OR_W:
	case OR_B:
		size = node->userdata.ud_op->op_ssize[0];
		newrand = vfind(rands[0], size); /* try to find replacement */

		if (newrand == RNULL )
		    goto normal;	/* no replacement possible */
		if (!isareg(newrand)){
			setrand(node, 1, newrand);
			changedto(tflag);	/* report change */
		}
					/* we have a replacement */
		vkill(rands[0], src); /* destination is changed */
		vkill(rands[1], dest); /* destination is changed */
		break;
		


	    }	/* end special case switch */
	/* normal instruction tracing */

	else
	{
normal:
	    changed = false;		/* no changes yet */
	    op = node->userdata.ud_op;	/* set some temporaries */
	    nsrc = op->op_nsrc;
	    dnum = op->op_dnum;

	    /* handle source operands first */

	    if (nsrc != 0)
	    {
		if (   dnum != 0
		    && (newrand = vfind(rands[0], op->op_ssize[0])) != RNULL
		    )
		{
		    changed = true;	/* we'll change instruction */
		    setrand(node, 1, newrand); /* set new operand */
		}
		vkill(rands[0], src);	/* kill what needs to be killed */

		if (nsrc > 1)
		{
		    if (    dnum != 1
			&&	(newrand = vfind(rands[1], op->op_ssize[1])) != RNULL
			)
		    {
			changed = true;	/* changing an operand */
			setrand(node, 2, newrand);
		    }
		    vkill(rands[1], src);
		}
	    }
	    if (dnum != NODEST)		/* kill destination if there is one */
		vkill(rands[dnum], dest);

	    changedto(tflag && changed); /* report changes */
	}	/* end else */
	
	if (! hflag)			/* do peephole if not inhibited */
		usedld |= (peep(node, peepflag));
					/* L/D changes if inst. deleted */

    }	/* end of for ALLN */

#ifdef	LIVEDEAD
    if (usedld)				/* use live/dead information? */
	killld();
#endif

    return;				/* done trace */
}
/* vrevbr -- reverse branch for value tracing
**
** This routine reverses certain branches for value tracing.  The
** specific situation in which it is called is to reverse a branch
** that follows a cmp.[bwl] instruction when the order of the operands
** is reversed.  That means, equal/non-equal conditions are retained,
** but inequalities are reversed.  Since this is different from
** what "revbr" does, we need a separate function.
*/

static void
vrevbr(node)
NODE * node;				/* node to reverse */
{
    register OPCODE newop;		/* new opcode number to set */

	TRACE(vrevbr);
    switch (node->op)			/* determine what to change */
    {
    case BNE:
    case BNE_B:
    case BEQ:
    case BEQ_B:
	return;				/* these are unchanged */

    case BCC:	newop = BCS;	break;
    case BCC_B:	newop = BCS_B;	break;
    case BCS:	newop = BCC;	break;
    case BCS_B:	newop = BCC_B;	break;
    case BGE:	newop = BLE;	break;
    case BGE_B:	newop = BLE_B;	break;
    case BGT:	newop = BLT;	break;
    case BGT_B:	newop = BLT_B;	break;
    case BHI:	newop = BLO;	break;
    case BHI_B:	newop = BLO_B;	break;
    case BHS:	newop = BLS;	break;
    case BHS_B:	newop = BLS_B;	break;
    case BLE:	newop = BGE;	break;
    case BLE_B:	newop = BGE_B;	break;
    case BLO:	newop = BHI;	break;
    case BLO_B:	newop = BHI_B;	break;
    case BLS:	newop = BHS;	break;
    case BLS_B:	newop = BHS_B;	break;
    case BLT:	newop = BGT;	break;
    case BLT_B:	newop = BGT_B;	break;
    case BMI:	newop = BPL;	break;
    case BMI_B:	newop = BPL_B;	break;
    case BPL:	newop = BMI;	break;
    case BPL_B:	newop = BMI_B;	break;
    case BVC:	newop = BVS;	break;
    case BVC_B:	newop = BVS_B;	break;
    case BVS:	newop = BVC;	break;
    case BVS_B:	newop = BVC_B;	break;
    default:				/* found something else! */
	oerr("Unreversible op in vrevbr");
    }
    setop(node, getOP(newop));		/* set new op for node */
    return;
}
/* tables for vchgtst */

/* List of register bit masks for search below.  Ordered by most
** likely to be available.
*/

static REG dlist[] =
{
/*
    REG_D7,
    REG_D6,
    REG_D5,
    REG_D4,
    REG_D3,
    REG_D2,
*/
    REG_D1,
    REG_D0,
    REG_NONE			/* end of list sentinel */
};


/* This list contains the strings corresponding to the masks above. */

static char * nlist[] =
{
/*
    "%d7",
    "%d6",
    "%d5",
    "%d4",
    "%d3",
    "%d2",
*/
    "%d1",
    "%d0"
};
/* vchgtst -- change tst.[wl] to mov.[wl] if possible.
**
** This routine changes a tst.[wl] instruction to an equivalent
** mov.[wl] instruction.  We are given an address (A) register
** which contains a cached value equivalent to the first operand
** of the tst.[wl].  Since there is no
**
**	tst.[wl]	%an
**
** instruction, we try to replace it by a
**
**	mov.[wl]	%an,%dm
**
** where %dm is an available data (D) register.  We determine
** which D register is available by checking live/dead information.
**
** (Note that this improvement cannot be performed when the operand
** size is byte.)
*/

static boolean				/* return true if we make transform. */
vchgtst(node, aregrand, size)
NODE * node;				/* pointer to tst instruction */
RAND * aregrand;			/* pointer to replacement A reg. */
unsigned int size;			/* size of tst operand:  1, 2, 4 */
{
    REG * regptr;			/* pointer for register search */
    RAND * dregrand;			/* gets pointer to D register operand */
	TRACE(vchgtst);

#ifndef	LIVEDEAD
    return(false);			/* if no live/dead info */
#else
    if (size == 1)			/* doesn't work for tst.b */
	return(false);
    
    /* find D register which is "dead" */

    for (regptr = dlist; ; regptr++)	/* scan list */
	if (*regptr == REG_NONE)	/* find end of list? */
	    return(false);		/* yes.  No available register */
	else if ((*regptr & node->nlive) == 0)
					/* is register free? */
	    break;			/* yes.  Exit loop */
    /* Regptr points at the register mask for a free register.  Make
    ** an operand of the D register (3 is string length).
    */

    dregrand = saverand(nlist[regptr-dlist], 3, RT_REG, *regptr, true);

    /* build the mov.[wl] instruction with correct operands */

    setop(node, getOP((OPCODE) (size == 4 ? MOV_L : MOV_W)));
    setrand(node, 1, aregrand);		/* change source */
    setrand(node, 2, dregrand);		/* change destination */
    vkill(dregrand, dest);		/* kill destination cache contents */
    vset(dregrand, aregrand, size);	/* set new cache content */
    return(true);			/* we changed something */

#endif	/* def LIVEDEAD */
}

	/* If the sequence
	** 
	** mov.[bwl] Rn,Rm
	**  .
	**  .
	**  .
	** mov.[bwl] Rm,Rn
	** 
	** occurrs,  and niether Rn or Rm  has been killed, and the
	** condition codes are dead then the second move is redundant,
	** delete it
	*/

redund_mov( node, rands, size, pflag )
NODE *node;
RAND **rands;
unsigned int size;
boolean pflag;
{	
	register VAL *valptr;
	register int count;
	VAL *src = NULL,
	    *dest = NULL;

	TRACE(redund_mov);
	if( isreg(rands[0]) && isreg(rands[1]) && isdead(REG_CC, node) )
		for ALLVAL(valptr, count)
		{
			if( valptr->inreg == rands[0]->ruses )
				src = valptr;
			if( valptr->inreg == rands[1]->ruses )
				dest = valptr;
			if( src != (VAL *)NULL && src->rand == dest->rand )
		 	{	rmnode( node, pflag );
				return( 1 );
			}
		}

	return( 0 );
}


/*				NOTE
**
**	The following file handles the peephole improver. This file needs
** to have access to the value cache and the routines which manipulate the
** cache.  2/9/84
*/

#include "peep.c"
