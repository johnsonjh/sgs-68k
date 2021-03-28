#ident	"@(#)vtrace.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* vtrace.c */
/* static char SCCSID[] = "vtrace.c:  2.1 83/07/08"; */

/*      MC68020 Optimizer       */

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
**      1) Try to replace a memory reference by a use of an equivalent
**              operand in the cache whose value is in a register.
**      2) Update the cache by eliminating values which may be destroyed
**              by the instruction, and by adding values which are
**              newly present.
**
** The f/mov family and lea are the only instructions which can
** add things to the cache.  The cache distinguishes between float, pointer
** immediate and binary data whenever possible.  An alteration to one type 
** usually will not invalidate another.  Immediates in registers are never
** invalidated unless their residing register gets clobbered.  Usually 
** flushes of one type will not invalidate another.  
**
** As an example, add.w &1,(%a3) is presumed
** to destroy any binary and pointer values in the cache, although %a3 may
** be saved if we can determine that it is immediately used again as a pointer.
*/

#include "optim.h"

/* Define the value cache and related items. */

/* This is the basic structure. */

struct val
{
    REG inreg;                  /* REGister bit mask for the register in which
                                ** the cached value lives
                                */
    RAND * regrand;             /* the RAND structure for that register */
    RAND * rand;                /* the RAND structure for the register's
                                ** contents
                                */
    short size; 		       /* size of the operand in bytes */
							   /* a minus indicates exclusive lea use */
};

typedef struct val VAL;         /* a shorthand */

/* The values are held in an array which is searched associatively.
** There can be more than one value associated with a register.
*/

/* Size of array.  Note that increasing the size of the array does
** not necessarily mean that a better value trace will occur.  The
** lifetime of items in the value cache is relatively short anyway,
** and clearing it each time is costly.
*/

#define NUMVALS 30              /* maximum number of values in array */

static VAL valarray[NUMVALS];   /* the cache array */
static int numcache = 0;        /* number of real values currently in cache */

enum sdflag {src, dest, floatdest};  /* vreset operand indicator flag */
/* These macros simplify the routines the are defined below. */

/* This first one scans the value array for non-empty entries, and is used as:
**      for ALLVAL(ptr,n) {...}
** where
**      ptr     is VAL *, and points to a non-empty value entry
**      n       is a bookkeeping counter
*/

#define ALLVAL(ptr,n) \
        (ptr=valarray, n=numcache; n > 0; ptr++) \
            if ((ptr->regrand != RNULL) ? n-- : 0)


/* This macro kills an entry in the table.  It is used as
**      clearval(ptr)
**
** where
**      ptr     is VAL *
*/

#define clearval(ptr)   ptr->regrand = RNULL

/* Prototypes for static functions */
static void  vclear _PARMS((void));
static void  vreset _PARMS((long ));
static struct rand * vfind _PARMS((struct rand *, unsigned , int ));
static void  vset _PARMS((struct rand *, struct rand *, unsigned ));
static void  vkill _PARMS((struct rand *, enum sdflag ));
static void  vprint _PARMS((void));
static void  vrevbr _PARMS((struct node *));

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
    register VAL * valptr;              /* pointer to cache array item */
    register int count;

    for ALLVAL(valptr,count)            /* clear all non-empty entries */
        clearval(valptr);
    
    numcache = 0;                       /* no cached values */
    return;
}
/* vreset() -- reset cache contents
**
** This routine discards all elements effected by memory writes and 
** belonging to the registers indicated.  In particular it retains
** immediate operands in registers and any value data associated with
** registers other than in the passed list.
**
*/

static void
vreset(reglist)
REG	reglist;							/* registers to invalidate */
{
    register VAL * valptr;              /* pointer to cache array item */
    register int count;
    register RTYPE rtype;               /* operand type of register contents */

    for ALLVAL(valptr,count)            /* clear anything not an immediate */
        if (   (rtype = valptr->rand->rtype) != RT_IMMNUM
            && rtype != RT_IMMED
			&& (valptr->inreg & reglist) != 0	/* or associated with reglist */
            )
        {
            clearval(valptr);           /* discard cache value */
            numcache--;                 /* one less value in cache */
        }
    return;
}
/* vfind -- find an operand in the cache
**
** This routine looks for an operand in the cache and returns a
** pointer to the RAND entry for the corresponding register.
** The operand size must match for non-floating point registers.
**
** If the floatflag on then first choice is a floating-point register, second
** choice a data register.  Otherwise first choice is a data register, second
** choice a address register.
*/

static RAND *
vfind(rand,size,floatflag)
RAND * rand;                            /* operand we're looking for */
unsigned int size;                      /* size of desired operand */
int	floatflag;			/* floating-point operation flag */
{
    register VAL * valptr;              /* pointer to cache element */
    register int count;                 /* bookkeeper */
    RAND * found = RNULL;               /* operand to return */
    REG regset;				/* set of registers allowed */
    REG regpick;			/* favorite registers wanted */

    /* certain operand addressing modes are never in the cache: */

    switch (rand->rtype)
    {
    case RT_REG:
    case RT_INCR:
    case RT_DECR:
        return(RNULL);                  /* say not found */
    
    default:
        break;                          /* look for the rest */
    }

    /* setup register type universe and favorite type wanted */
#ifdef M68881
    if (floatflag)
      {	regset = REG_DREG | REG_FPREG;	/* float and data registers */
	regpick = REG_FPREG;		/* floats are preferred */
      }
    else
#endif
      {	regset = REG_DREG | REG_AREG;	/* data and address registers wanted */
	regpick = REG_DREG;		/* data registers more universal */
      }
    for ALLVAL(valptr, count)
        if (   valptr->rand == rand 
			&& (   valptr->size == size)			/* jcl */
			&& (	valptr->regrand->ruses & regset)
	   )
	  {	found = valptr->regrand;
            	if ((found->ruses & regpick) != 0)
                return(found);          /* return favorite register hit */
	  }
    
    return(found);                      /* return whatever we found */
}
/* vset -- set value in cache
**
** This routine sets a cache value.  We assume any killing of related
** registers has been done explicitly elsewhere.
*/

static void
vset(regrand, rand, size)
RAND * regrand;                         /* new register we're setting */
RAND * rand;                            /* the value for that register */
unsigned int size;                      /* size of operand being cached */
{
    register VAL * valptr;              /* pointer to cache entry */
    register int count;                 /* for bookkeeping */

    /* Disallow setting operands where the operand in the cache uses the
    ** same register as the register in which it's said to reside.
    ** (This is a conservative assumption.  It eliminates things like
    **          mov.l   %a0,4(%a0)
    **
    ** but guards against things like
    **
    **          mov.l   4(%a0),%a0
    */

    if ((regrand->ruses & rand->ruses) != 0)
        return;                         /* operand uses register */

    /* Prevent certain address modes from being set in cache */

    switch (rand->rtype)
    {
    case RT_INCR:
    case RT_DECR:
        return;                         /* forget these */
    
    case RT_REG:                        /* for register, copy all operands
                                        ** related to rand for "regrand",
                                        ** as well
                                        */
        if (rand == regrand)
            return;                     /* no need to copy same reg. */
        
        for ALLVAL(valptr, count)
            if (valptr->inreg == rand->ruses && valptr->size == size)
                vset(regrand, valptr->rand, valptr->size);
        return;

    default:
        break;                          /* accept these */
    }

/* jcl: do not allow two different sizes in same fp reg */
    for ALLVAL(valptr, count)
	if ((valptr->inreg & REG_FPREG) != 0
		&& valptr->inreg == regrand->ruses
		&& valptr->size != size)
	    return;

    /* (We must run this loop explicitly, rather than use the ALLVAL macro,
    ** because we want empty entries here
    */

    for (valptr = valarray, count=NUMVALS; count > 0; valptr++, count--)
        if (valptr->regrand == RNULL)   /* found empty entry */
        {
            valptr->regrand = regrand;  /* set holding register */
            if (regrand->rtype != RT_REG) /* make sure it IS a register */
                oerr("vset:  non-register being set");
            
            valptr->inreg = regrand->ruses; /* set register's bit mask */
            valptr->rand = rand;
            valptr->size = size;
            numcache++;                 /* another cached value */
            return;
        }
    
    return;                             /* couldn't cache value because out
                                        ** of room
                                        */
}
/* vkill -- kill cache items based on an operand
**
** This routine invalidates cache items that are affected by an operand.
** We need to know whether the argument operand is a source or destination
** operand, as this affects what things are invalidated.  The invalidation
** is driven by operand types and whether the destination is float or non-float.
** Float types are only of concern when a destination is being considered.
*/

static void
vkill(rand,flag)
RAND * rand;                            /* the operand invalidating things */
enum sdflag flag;                       /* type of operand */
{
    register VAL * valptr;              /* pointer to cache entries */
    register int count;                 /* bookkeeper */
    RTYPE rtype = rand->rtype;          /* operand type */
    register REG reg;                   /* operand register bits */

    switch( rtype )
    {
    case RT_REG:                        /* register direct */
        if (flag == src)                /* source operands don't affect stuff */
            return;
        break;                          /* but destination ones do! */
    
    case RT_DECR:                       /* pre-decrement */
        if (flag != src && rand->ruses == REG_SP)
                                        /* if push onto stack... */
            break;                      /* needn't clear everything */
        /* fall through */
    case RT_INCR:                       /* post-increment */
        if (flag == src)                /* for source operands, only things
                                        ** using the register are invalid
                                        */
            break;
        
#ifdef M68881
		if (flag == floatdest)
			vreset(REG_FPREG);			/* clear memory for float types */
		else
#endif
        	vreset(REG_AREG | REG_DREG);    /* clear memory for binary types */
        break;
    case RT_IREG:                       /* register indirect */
        if (flag == src)
            return;                     /* no problem as source */
        if (rand->ruses != REG_SP)      /* if not stack destination, reset */
        {
#ifdef M68881
		if (flag == floatdest)
			vreset(REG_FPREG);			/* clear memory for float types */
		else
#endif
        vreset(REG_AREG | REG_DREG);    /* clear memory for binary types */
            return;
        }
        /* fall through, clear out other reference to (%sp) */
    case RT_MEM:                        /* memory direct (or stack item) */
asmem:
        if (flag != src)                /* assume only the destination is
                                        ** affected
                                        */
            for ALLVAL(valptr,count)
                if (valptr->rand == rand)
                {
                    clearval(valptr);   /* kill off this value */
                    numcache--;         /* one less item in cache */
                }
        return;

#ifdef m68k
    case RT_REG_PAIR:                   /* Ri:Rj for '020 mul and div */
        break;                          /* These set both Ri and Rj   */
#endif
    case RT_OREG:                       /* offset from register */
    case RT_INDX:                     /* double index */
		if (rand->ruses & (REG_SP | REG_FP))
			goto asmem;					/* frame variables like memory */
#ifdef m68k
    case RT_EXTINDX:
#endif
        if (flag != src)                /* if these are destinations ... */
#ifdef M68881
		if (flag == floatdest)
			vreset(REG_FPREG);			/* clear memory for float types */
		else
#endif
        	vreset(REG_AREG | REG_DREG);    /* clear memory for binary types */

    /* fall through */

    case RT_IMMNUM:                     /* immediate number */
    case RT_IMMED:                      /* general immediate */
    case RT_RTSUB:                      /* special subroutine label */
    case RT_SUB01:                      /* special subroutine label */
	case RT_SUB02:						/* special subroutine label */
    case RT_LABEL:                      /* normal label */
        return;                         /* these have no ill effects */

    default:                            /* something else */
        oerr("Missing operand type in vkill");
    }
/* Reaching here, we need to invalidate all items in the cache which
**
**      a) are associated with some register the operand uses, or
**      b) have as the "regrand" one of the registers the operand uses
**
** Consider these cache entries:
**
**      %d0     contains        10(%a0)
**      %d1     contains        &35
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
            clearval(valptr);           /* eliminate from the cache */
            numcache--;                 /* indicate one less entry */
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
    register VAL * valptr;              /* pointer to cache entry */
    register int count;                 /* bookkeeping */

#if 0
    PRINTF("%d items in cache\n", numcache);
#endif

    for ALLVAL(valptr, count)
        PRINTF("\t\t\t%s\tcontains\t%s size=%d\n", valptr->regrand->rname,
                valptr->rand->rname, valptr->size);
    
    return;
}
/* vtrace -- value trace routine
**
** This is the routine that actually does the value trace.  It scans
** all of the instruction nodes once, trying to substitute register
** uses for other operands.  It returns an indication of whether it
** was able to delete any statements.
*/

/* Define macro */

#define changedto(flag) if(flag) \
                        PRINTF(">>>>> changed to\n"), \
                        prinst(node)
boolean
vtrace()
{
    OP * op;                            /* current node's OP structure */
    unsigned int nsrc;                  /* number of source operands in
                                        ** current node
                                        */
    register RAND ** rands;             /* current node's operands */
    register NODE * node;               /* current node */
    RAND * newrand;                     /* replacement operand */
    boolean changed;                    /* whether instruction changed */
    unsigned int dnum;                  /* destination operand number */
    unsigned int size;                  /* operand size */
    boolean usedld;	                /* true if used (and changed) live/dead
                                        ** information
                                        */
    extern boolean dflag;               /* print live/dead info */
    extern boolean hflag;               /* true to inhibit peephole */
    void prinst();
    void vrevbr();                      /* special branch reverser */
    boolean vchgtst();                  /* special tst changer */
    boolean peep();
    boolean didrmstmt = false;		/* flag to return indicating success */
	void nullstore();					/* zero to memory processing */


    /* loop until nothing further has been deleted */
	/* live/dead and flow graph good coming into function */
again:

    usedld = false;			/* start out clean */
    vclear();                           /* initialize value cache */
	adrframe = false;		/* default no & operator on frame variable */
	for (ALLN(node))		/* scan for any pea or lea of frame variable */
	{	if (   (node->op == PEA_L || node->op == LEA_L)
		    && (node->userdata.ud_rand[0]->ruses & REG_FP) != 0
		   )
		{	adrframe = true;
			break;
		}
	}
    for (ALLN(node))
    {
        if (vtprintflag)
            vprint();                   /* print current cache */
        if (dflag)                      /* print live/dead info? */
		  {	PRINTF("%c(live: %#.8x)", CC, node->nlive);
			if (node->userdata.ud_nattr)
				PRINTF(" (nattr: %#.4x)", node->userdata.ud_nattr);
		  }
        if (vtprintflag || dflag || peepflag)
            prinst(node);               /* print instruction */

		/* if this is a trigger node for in-line optimizations ... */
		if (node->userdata.ud_nattr & UDN_fmask)
			if (inline(node))			/* attempt in-line improvements */
			  {	didrmstmt = true;		/* signal we optimized something */
				/* start over since the current node may no longer exist */
				goto again;
		  	  }

        rands = node->userdata.ud_rand; /* point at operands */

        /* handle non-standard value-trace ops */

        if ((node->userdata.ud_flags & UD_VTRACE) != 0)
            switch(node->op)
            {
		    case GENERIC:
            case BSR:                   /* assume all is lost for these */
            case BSR_B:
            case JSR:
            case MOVM_L:                /* (conservative for these three) */
            case MOVM_W:
#ifdef M68881
            case FMOVM:
#endif
            case LABEL:                 /* label kills all because flow merges
                                        ** here
                                        */
                vclear();
                break;
            
            case MOVP_L:                /* assume bad things for MOVP */
            case MOVP_W:
                vkill(rands[0], src);
                vkill(rands[1], dest);
                break;
            
        /* these guys can't use REG_AREG's */
            case AND_B:
            case AND_W:
            case AND_L:
            case OR_B:
            case OR_W:
            case OR_L:
            case EOR_B:
            case EOR_W:
            case EOR_L:
                size = node->userdata.ud_op->op_ssize[0];
                if ((newrand = vfind(rands[0], size, false)) != RNULL)
                        if (newrand->ruses & REG_DREG)
                                setrand(node, 1, newrand);
                vkill(rands[0], src);
                vkill(rands[1], dest);
                break;

            
            case PEA_L:                 /* don't examine source at all */
                break;

            case EXG:                   /* both operands act as destinations */
                vkill(rands[0], dest);
                vkill(rands[1], dest);
                break;
            
            default:                    /* not found */
                oerr("Special case not found in vtrace");
            /* The lea instruction uses entries in the value cache exclusively
			** for itself by setting using a size of minus one.  When lea hits
			** in the value cache we convert it into a mov.l to copy the
			** cached value over to it's destination.
			*/

            case LEA_L:
				if ( (newrand=vfind(rands[0], -1, false)) != RNULL )
				{	setop(node, getOP((OPCODE) MOV_L));
					setrand(node, 1, newrand);
					changedto(vtprintflag);
					usedld = true;
					}
                vkill(rands[1], dest);
				/* (%an) is same as register copy so don't add it to cache */
				if (rands[0]->rtype != RT_IREG)
					vset(rands[1], rands[0], -1);
                break;
            /* The cmp.[bwl] instructions require some special processing for
            ** two reasons:  they are badly non-orthogonal, and the order in
            ** which the operands appear in the assembly language is the
            ** reverse of the execution order.
            **
            ** We need to be careful about changing the cmp.[bwl] operands
            ** in one special case:
            **
            **          cmp.[bwl]       E,&n
            **
            ** This could get changed into
            **
            **          cmp.[bwl]       E,%d0   (say)
            **
            ** if the literal was in a register.  Unfortunately, this form
            ** of the instruction doesn't exist, although
            **
            **          cmp.[bwl]       %d0,E
            **
            ** does.  Therefore, we change the order and reverse the
            ** (presumed) following conditional branch for this case.
            */

            case CMP_B:
            case CMP_W:
            case CMP_L:
                size = node->userdata.ud_op->op_ssize[0];

                /* change both operands, if possible, but in reversed order */

                if (   (newrand = vfind(rands[1], size, false)) != RNULL
#ifdef M68881
					&& !isfpreg(newrand)
#endif
				   )
                {
                    setrand(node, 2, newrand);  /* have replacement */
                    changed = true;
                }
                vkill(rands[1], src);           /* kill affected registers */

                if (   (newrand = vfind(rands[0], size, false)) != RNULL
#ifdef M68881
					&& !isfpreg(newrand)
#endif
				   )
                {
                    setrand(node, 1, newrand);
                    changed = true;
                }
                vkill(rands[0], src);
                /* now check for the funny case mentioned above */

                if (changed && isdreg(rands[1]) && ! isreg(rands[0]))
                {
                    if (! isrev(node->forw))    /* better be reversible */
                        oerr("vtrace in trouble with cmp");
                    
                    newrand = rands[0];         /* interchange operands */
                    setrand(node, 1, rands[1]); /* (beware operand numbering */
                    setrand(node, 2, newrand);
                    vrevbr(node->forw);         /* reverse following branch */
                }

                changedto(vtprintflag && changed);    /* comment on changes */
                break;                          /* onward! */
#ifdef M68881
            /* The fcmp.[xsd] instructions must have a floating point register
            ** as the first operand; if the second operand is not a register
            ** we attempt to find a value in an %fp register as a substition
            */

            case FCMP_X:
            case FCMP_D:
            case FCMP_S:
                size = node->userdata.ud_op->op_ssize[1];

                if( ( ( newrand = vfind( rands[1], size, true ) ) != NULL  ) &&
                      isfpreg( newrand ) ) 
                {       changed = true;
                        setrand( node, 2, newrand );
                }
        
                vkill( rands[1], src );
                changedto(vtprintflag && changed);    /* comment on changes */
                break;

#endif
            /* For tst.[bwl] instructions, the replacement cannot live in an
            ** address register because there is no
            **          tst.[bwl] %an
            ** instruction.  However, if there's an available D register,
            ** we can move the A register to a D register, thus setting the
            ** condition codes and getting a comparable result.
            **
            ** Under happier circumstances this improvement (move A to D)
            ** would be purely a peephole improvement, but we need to know
            ** the value cache state to pick a register that contains the
            ** operand.
	  NOTE: All of this is totally needless for the MC68020 (which does
		allow tst %an) but is kept here so the instruction tables
		do not have to be different for the various processors.
		(The UD_VTRACE flag could be removed and this all deleted
		 but the way optab.base is made you can't #else to set
		 different lines for pre and post MC68020 type processors.)
            */

            case TST_B:
            case TST_W:
            case TST_L:
                size = node->userdata.ud_op->op_ssize[0];

                newrand = vfind(rands[0], size, false);
                                        /* try to find replacement */
		if (newrand != RNULL)
		{                       /* replacement possible */
#ifndef m68k
		    if (isareg(newrand))    /* if replacement in A reg, call
					    ** special routine
					    */
		    {
			if (vchgtst(node, newrand, size))
			    usedld = true;  /* we have used/changed live/dead */
		    }
		    else
#endif
			setrand(node, 1, newrand);
					    /* found suitable one; replace */

		    changedto(vtprintflag);       /* report change */
		    }
		vkill(rands[0], src);   /* kill what needs to be killed */
		break;
/* The mov.[bwl] and fmov.[bwlsdx] instructions are the only ones that 
** actually SET a cache value.  They are handled specially.  If we find 
** a 'mov.l &0,' to memory we convert it to a 'clr.l' (m68k only) and 
** do the same processing as for clr.
**
** We refuse to substitute a floating-point register source for a memory to
** memory move since the native move instruction is much faster.
*/

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

                if( redund_mov( node, rands, size, vtprintflag ) )
                /* if move is redundant, delete it */
                {       usedld = true;
                        break;
                }

                if (   (newrand = vfind(rands[0], size, isfloat(node))) 
						!= RNULL
					&& (!isfpreg(newrand) || isfpreg(rands[1]))
				   )
                {                       /* we have a replacement */
                    setrand(node, 1, newrand);
                    changedto(vtprintflag);   /* report change */
                }

                vkill(rands[0], src); /* kill things affected by source */

                /* kill destination, unless same as source */

                if (rands[0] != rands[1])
                {
#ifdef M68881
					if ( isfpreg(rands[0]) || isfpreg(rands[1]) )
					  vkill(rands[1], floatdest);  /* float destination */
					else
#endif
					{	/* If we have an immediate source storing to memory
						** don't be so hasty about clearing the entire value
						** cache
						*/
						if (   rands[1]->rtype != RT_REG
											/* destination isn't register */
						    && rands[0]->rtype == RT_IMMNUM
											/* source is a single constant */
							&& rands[0]->rdisp == 0
											/* and it's a zero */
							&& node->op == MOV_L
											/* and it's mov.l */
						   )
#ifdef m68k
						  /* convert it to a clear and */
						  /* process a store of zero */
						  {	setop(node, getOP((OPCODE) CLR_L));
							setrand(node, 1, rands[1]);
							setrand(node, 2, RNULL);
							changedto(vtprintflag);
						  	nullstore(node, 0);
							break;				/* exit move processing */
						  }
#else
						  /* process a store of zero */
						  nullstore(node, 1);
#endif
#ifdef M68881
						else if (node->userdata.ud_nattr & UDN_OPTF)
							vreset( REG_FPREG );	/* invalidate float regs */
#endif
						else if (   rands[0]->rtype == RT_IMMNUM
											/* source is a simple immediate */
								 && (   rands[0]->rdisp != 0
											/* and it's not zero */
									 || !node->op == MOV_L
											/* or not a mov.l */
									)
								 && rands[1]->rtype != RT_REG
											/* and it's to memory */
								 && rands[1]->rtype != RT_INCR
											/* and doesn't blow it's base */
								 && rands[1]->rtype != RT_DECR
								)
						  /* a non-zero immediate.   don't invalidate */
						  /* the pointer based on the move's destination */
						  vreset( ~(rands[1]->ruses & REG_AREG)
#ifdef M68881
								   & ~REG_FPREG		/* leave fp regs alone */
#endif
							    );
						else
						  /* not a simple immediate so trash entire cache */
                      	  vkill(rands[1], dest);	/* binary destination */
						}

                    /* set new cache elements, as appropriate */

                    if (   rands[1]->rtype == RT_REG
									/* destination is register */
						&& rands[0]->rtype != RT_EXTINDX
									/* source is not memory indirect */
					   )
                        vset(rands[1], rands[0], size);

                    /* Add first operand to cache if it's a register, but
                    ** only if the destination isn't.  Otherwise we get
                    ** duplicate entries.
					** jcl: But don't add fpreg in this case because of
					** implicit conversion when storing to memory.
                    */

                    else if (rands[0]->rtype == RT_REG && !isfpreg(rands[0]))
                        vset(rands[0], rands[1], size);
                }
                break;                  /* end mov.[bwl] case */
        			/* clr.l processing */
/*	Since zero is not a valid value for a dereferenced pointer to be using we
**	try to determine for a clr.l instruction whether the current pointer used
**	to base the clear is itself safe to continue using.  
**
**	If so then the value cache is cleared with the exception of our pointer
**	register.
*/

	case CLR_L:
		nullstore(node, 0);		/* process zero store into memory */
		break;
	} /* end special case switch */
        /* normal instruction tracing */

        else
        {
            changed = false;            /* no changes yet */
            op = node->userdata.ud_op;  /* set some temporaries */
            nsrc = op->op_nsrc;
            dnum = op->op_dnum;

            /* handle source operands first */

            if (nsrc != 0)
            {
                if (   dnum != 0
                    && (newrand = 
							vfind(rands[0], op->op_ssize[0], isfloat(node)))
								!= RNULL
					&& (   !isareg(newrand)
						|| (	dnum == 1
							&& rands[1]->rtype == RT_REG
							&& (   node->op == ADD_W
								|| node->op == ADD_L
								|| node->op == SUB_W
								|| node->op == SUB_L
							   )
						   )
					   )
                    )
                {
                    changed = true;     /* we'll change instruction */
                    setrand(node, 1, newrand); /* set new operand */
                }
                vkill(rands[0], src);   /* kill what needs to be killed */

                if (nsrc > 1)
                {
                    if (    dnum != 1
                        &&   (newrand = 
				vfind(rands[1], op->op_ssize[1], isfloat(node)))
						!= RNULL
                        )
                    {
                        changed = true; /* changing an operand */
                        setrand(node, 2, newrand);
                    }
                    vkill(rands[1], src);
                }
            }
            if (dnum != NODEST)         /* kill destination if there is one */
                vkill(rands[dnum], dest);

            changedto(vtprintflag && changed); /* report changes */
        }       /* end else */
        
        if (! hflag)                    /* do peephole if not inhibited */
                usedld |= (peep(node, peepflag));
                                        /* L/D changes if inst. deleted */

    }   /* end of for ALLN */

#ifdef LIVEDEAD
    if (usedld) 
	{   didrmstmt = true;	/* tell caller we were successful */
    	ldanal();           /* get live/dead information for
                            ** peephole and vchgtst
							*/
 	    goto again;		/* greatness doesn't come easy! */
	}
#endif

    return ( didrmstmt );       /* return indication of success */
}
 			/* NULL (zero) destination value trace handling */
/*	This routine is called when a zero is written to storage.
**	Since zero is not a valid value for a dereferenced pointer to be using we
**	can try to determine whether the current pointer (if any) in an A register
**	used to base the clear is itself safe to continue using.  We do this by
**	detecting if all of the following situations hold:
**
**	1) The clear is to memory and uses an address (pointer) register
**
**	2) The next instruction is a move which reloads the pointer register
**
**  3) The instruction after the move itself uses the pointer register as
**	   a base
**
**	If so then the value cache is cleared with the exception of our pointer
**	register. Otherwise we are forced to clear all the cache (except for
** immediates in registers.)
*/

void
nullstore(node, destarg)
NODE 	*node;							/* node being processed */
int		destarg;						/* which argument is the destination */
{	RTYPE	ctype = node->userdata.ud_rand[destarg]->rtype;
									/* operand type */
	REG		cuses = node->userdata.ud_rand[destarg]->ruses & REG_AREG;
									/* An pointer used */
	NODE	*next = node->forw;					/* next instruction */
	unsigned char dnum;							/* destination operand no. */

#ifdef M68881
	if ( node->userdata.ud_nattr & UDN_OPTF )
		vreset( REG_FPREG );		/* this is float only so clobber %fpn */
	else
#endif
	  if (   ctype != RT_INCR
							/* validate the destination addressing mode */
		&& ctype != RT_DECR
		&& ctype != RT_REG
		&& next->op == MOV_L
							/* the next instruction is a mov.l */
		&& next->userdata.ud_rand[1]->rtype == RT_REG
							/* to a register */
		&& (next->userdata.ud_rand[1]->ruses & cuses) != 0
							/* which is our pointer register */
		&& (next=next->forw) != NULL
							/* there is a third instruction */
		&& (   (   (dnum=next->userdata.ud_op->op_dnum) != NODEST
							/* which has a destination */
			    && (next->userdata.ud_rand[dnum]->ruses & cuses) != 0
							/* based upon our pointer register */
			   )
			|| (   (dnum=next->userdata.ud_op->op_nsrc) > 0
							/* or has a source */
			    && (next->userdata.ud_rand[0]->ruses & cuses) != 0
							/* based upon our pointer register */
			   )
			|| (   (dnum=next->userdata.ud_op->op_nsrc) > 1
							/* or has a second source operand */
			    && (next->userdata.ud_rand[1]->ruses & cuses) != 0
							/* based upon our pointer register */
			   )
		   )
	   )
		vreset( ~cuses   				/* reset all but our pointer */
#ifdef M68881
			   & ~REG_FPREG				/* and floating point registers */
#endif
				);
	  else
	  	 vkill(node->userdata.ud_rand[destarg], dest);
										/* must do normal reset */

	return;
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
NODE * node;                            /* node to reverse */
{
    register OPCODE newop;              /* new opcode number to set */

    switch (node->op)                   /* determine what to change */
    {
    case BNE:
    case BNE_B:
    case BEQ:
    case BEQ_B:
        return;                         /* these are unchanged */

    case BCC:   newop = BCS;    break;
    case BCC_B: newop = BCS_B;  break;
    case BCS:   newop = BCC;    break;
    case BCS_B: newop = BCC_B;  break;
    case BGE:   newop = BLE;    break;
    case BGE_B: newop = BLE_B;  break;
    case BGT:   newop = BLT;    break;
    case BGT_B: newop = BLT_B;  break;
    case BHI:   newop = BLO;    break;
    case BHI_B: newop = BLO_B;  break;
    case BHS:   newop = BLS;    break;
    case BHS_B: newop = BLS_B;  break;
    case BLE:   newop = BGE;    break;
    case BLE_B: newop = BGE_B;  break;
    case BLO:   newop = BHI;    break;
    case BLO_B: newop = BHI_B;  break;
    case BLS:   newop = BHS;    break;
    case BLS_B: newop = BHS_B;  break;
    case BLT:   newop = BGT;    break;
    case BLT_B: newop = BGT_B;  break;
    case BMI:   newop = BPL;    break;
    case BMI_B: newop = BPL_B;  break;
    case BPL:   newop = BMI;    break;
    case BPL_B: newop = BMI_B;  break;
    case BVC:   newop = BVS;    break;
    case BVC_B: newop = BVS_B;  break;
    case BVS:   newop = BVC;    break;
    case BVS_B: newop = BVC_B;  break;
    default:                            /* found something else! */
        oerr("Unreversible op in vrevbr");
    }
    setop(node, getOP(newop));          /* set new op for node */
    return;
}
#ifndef m68k
/* tables for vchgtst */

/* List of register bit masks for search below.  Ordered by most
** likely to be available.
*/

static REG dlist[] =
{
    REG_D7,
    REG_D6,
    REG_D5,
    REG_D4,
    REG_D3,
    REG_D2,
    REG_D1,
    REG_D0,
    REG_NONE                    /* end of list sentinel */
};


/* This list contains the strings corresponding to the masks above. */

static char * nlist[] =
{
    "%d7",
    "%d6",
    "%d5",
    "%d4",
    "%d3",
    "%d2",
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
**      tst.[wl]        %an
**
** instruction, we try to replace it by a
**
**      mov.[wl]        %an,%dm
**
** where %dm is an available data (D) register.  We determine
** which D register is available by checking live/dead information.
**
** (Note that this improvement cannot be performed when the operand
** size is byte.)
*/

static boolean                          /* return true if we make transform. */
vchgtst(node, aregrand, size)
NODE * node;                            /* pointer to tst instruction */
RAND * aregrand;                        /* pointer to replacement A reg. */
unsigned int size;                      /* size of tst operand:  1, 2, 4 */
{
    REG * regptr;                       /* pointer for register search */
    RAND * dregrand;                    /* gets pointer to D register operand */

#ifndef LIVEDEAD
    return(false);                      /* if no live/dead info */
#else
    if (size == 1)                      /* doesn't work for tst.b */
        return(false);
    
    /* find D register which is "dead" */

    for (regptr = dlist; ; regptr++)    /* scan list */
        if (*regptr == REG_NONE)        /* find end of list? */
            return(false);              /* yes.  No available register */
        else if ((*regptr & SCRATCHREGS) && ((*regptr & node->nlive) == 0))
                                        /* is register free? */
            break;                      /* yes.  Exit loop */
    /* Regptr points at the register mask for a free register.  Make
    ** an operand of the D register (3 is string length).
    */

    dregrand = saverand(nlist[regptr-dlist], 3, RT_REG, *regptr, true);

    /* build the mov.[wl] instruction with correct operands */

    setop(node, getOP((OPCODE) (size == 4 ? MOV_L : MOV_W)));
    setrand(node, 1, aregrand);         /* change source */
    setrand(node, 2, dregrand);         /* change destination */
    vkill(dregrand, dest);              /* kill destination cache contents */
    vset(dregrand, aregrand, size);     /* set new cache content */
    return(true);                       /* we changed something */

#endif  /* def LIVEDEAD */
}
#endif /* MC68020 */
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

        if( isreg(rands[0]) && isreg(rands[1]) && isdead(REG_CC, node) )
                for ALLVAL(valptr, count)
                {
                        if( valptr->inreg == rands[0]->ruses )
                                src = valptr;
                        if( valptr->inreg == rands[1]->ruses )
                                dest = valptr;
                        /* NULL pointer problem corrected for *dest 6/16/86.tm */
                        if( src != (VAL *)NULL && dest != (VAL *)NULL && src->rand == dest->rand )
                        {       rmnode( node, pflag );
                                return( 1 );
                        }
                }

        return( 0 );
}


/*                              NOTE
**
**      The following file handles the peephole improver. This file needs
** to have access to the value cache and the routines which manipulate the
** cache.  2/9/84
*/

#include "peep.c"
