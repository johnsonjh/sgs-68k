#ident	"@(#)regasg.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	regasg.c    7.1		*/
/*	Copyright 1985 Motorola Inc.	*/

#include <storclass.h>
#include "declar.h"

/* Prototypes for static functions */
static void  weighlocals _PARMS((void));
static void  travbb _PARMS((union ndu *));
static void  travexpr _PARMS((union ndu *));
static void  assign _PARMS((void));
static void  regannounce _PARMS((struct Tlevel *, int ));
static void  altertrees _PARMS((void));
static void  alterblocks _PARMS((struct Tlevel *));
static void  alterbb _PARMS((struct Tbasicblock *));
static union ndu * addparminit _PARMS((union ndu *));
static void  alterexpr _PARMS((union ndu *));
static void  chkCONV _PARMS((union ndu *, struct syment *));
static void  addCONV _PARMS((union ndu *, unsigned ));
static void  initregavail _PARMS((struct Tlevel *));


static int bbweight;    /* weight of current basic block */
static int prolog;      /* boolean flag indicating PROLOG node has been seen */
extern void adjframe();

/*F*********************** FUNCTION HEADER *******************************

 asgregvars

	This function controls the process that determines
	which local variables may be assigned to registers.

PARAMETERS:

        Receives: none

        Returns: none

EXTERNAL DATA REFERENCES:

	symflag
	aregstat
	dregstat
	fpregstat

FUNCTIONS CALLED:

	initregavail
	weighlocals
	assign
	scopedump
	altertrees

CALLED BY:

        optimize

*FE********************************************************************/

void
asgregvars()
{
FILE *fd;

   initregavail(scoperoot);     /* traverse the scope tree, bottomup, and 
                                ** initialize the regavail masks for each 
                                ** scope */
   weighlocals();	/* traverse basic blocks determining the weight
			** of the local variables */

   assign();		/* using the weights of the local variables
			** assign registers to the heaviest locals */
   adjframe();
#ifndef NODBG
   if (symflag)		/* show which variables were assigned to registers */
      scopedump();
#endif
   if (aregstat || dregstat || fpregstat) 
      altertrees();	/* if variables have been assigned to registers then 
			** walk the basic blocks and alter the code
			** trees to include the new register variables */
}


/*F*********************** FUNCTION HEADER *******************************

 weighlocals

	This function begins the process of determining the weight of simple 
	local variables for a function.   The code tree for each basic block
	is traversed and the weight of the basic block is added to the local 
	variable's weight on each occurrence of the variable.     

PARAMETERS:

        Receives: none

        Returns: none

EXTERNAL DATA REFERENCES:

        basic block flow graph
        bbweight

FUNCTIONS CALLED:

        travbb

CALLED BY:

        asgregvars

*FE********************************************************************/
   
static void
weighlocals()
{
   BBPTR bb;
   for (bb = fgroot->next; bb; bb = bb->next)
      {
      bbweight = bb->weight; 
      travbb( bb->codetree );
      }
}


/*F*********************** FUNCTION HEADER *******************************

 travbb

	Traverse code tree adding the weight of the current basic block
	to each simple local variable.

PARAMETERS:

        Receives: q: root of code tree

        Returns: nothing

EXTERNAL DATA REFERENCES:

        none

FUNCTIONS CALLED:

        travexpr

CALLED BY:

        weighlocals

*FE********************************************************************/

static void
travbb ( q )
      
NODE *q;        /* head of code tree */
{              
   NODE *p;
   for ( p = q; p; p = p->stmt.succ)
      {
      switch( p->tn.op )
         {
         case SBLOCK:
         case EBLOCK: 
#ifdef ASM_COMMENT
	 case ASMNO:
#endif
            break;    
         case EXPRNO:
            travexpr( p );
            break;
         }
      }
}


/*F*********************** FUNCTION HEADER *******************************

 travexpr

	Traverse expression adding weight of current basic block to each
	simple local variable.

PARAMETERS:

        Receives: p: pointer to current node in expression tree

        Returns: nothing

EXTERNAL DATA REFERENCES:

        none

FUNCTIONS CALLED:

        travexpr

CALLED BY:

        travbb

*FE********************************************************************/

static void
travexpr( p )

NODE *p;
{
   SYMENT *sym;

   switch (optype(p->tn.op))
   {
   case BITYPE:
      travexpr( p->in.left );
      travexpr( p->in.right );
      break;
   case UTYPE:
      travexpr( p->in.left );
      break;
   case LTYPE:

	  /** dont bother if volatile;
	   ** but be warned: this only handles a volatile variable or
	   ** a volatile pointer... it does not YET handle a non-volatile
	   ** pointer to a volatile variable **/

	sym = symtab + p->opn.sourceno;
	if ( ! is_volatile(sym->stypeq) ) {

   		if ((p->opn.op == VAUTO || p->opn.op == VPARAM) && ISSIMPLE(p) 
	 		&& !(sym->flags & ADDRESSTAKEN))
	  	{	
			if (asgop(p->opn.parent->opn.op) 
				&& p->opn.parent->opn.op != ASSIGN
		   		&& p->opn.parent->opn.op != CAST)
		     /* actually used twice if ++, --, +=, -=, etc. */
        	sym->weight += bbweight*2;

       		else sym->weight += bbweight;
	 	}
/*
 *  increment frmwgt when the access was 0 offset of an automatic
 *  variable ( this includes Structures, Unions and Enumerations ).
 *  do not increment frmwgt if the offset is referencing a part of
 *  the symbol ( caused by casting ).
 */
		if ( p->opn.op == VAUTO && ( p->tn.lval < sym->offset + sym->size))
		{
			sym->frmwgt += bbweight;
		}
	}
	break;
	}
}
                           

/*F*********************** FUNCTION HEADER *******************************

 assign

	Computes which variables (locals and parameters) should be
	assigned to registers. This is done in a two step process:

	1) create an array consisting of the weight of each variable 
	being considered for a register, and an index to the variable's 
	symbol table entry. This array is then sorted according to weight. 

	2) The array is then processed according to weight.  For each
	variable, the scope in which it was declared is checked to see
	if a register is free. If a register is free, the register is
	assigned to the variable, and the usage of the register is
	propogated to the surrounding environments.

PARAMETERS:

	Receives: nothing

	Returns: nothing

EXTERNAL DATA REFERENCES:

	symflag

FUNCTIONS CALLED:

	createsv
	getreg
	regannounce

CALLED BY:

	asgregvars

*FE********************************************************************/

static void
assign()

{
   int i;
   SORTED *p;
   SYMENT *q;
   SCOPE *r;
   int regmask;

   sortedvars = createsv(); /* creates and sorts the array */

#ifndef NODBG
   if (symflag)
      {
      printf("\n\nSORTED VARIABLES ACCORDING TO WEIGHT\n\n");
      for (i = 0; i < symtabndx /* (sortedvars + i)->ndx*/ ; ++i)
	  if ( (sortedvars+i)->ndx )
	     {
         	printf("weight:%d\tvariable:%s", (sortedvars+i)->weight,
                	(symtab + (sortedvars+i)->ndx)->name);
		if (    (symtab + (sortedvars+i)->ndx)->sclass == ICON
		     || (symtab + (sortedvars+i)->ndx)->sclass == FCON )
         	   printf("\tvalue:%d\n", (symtab+(sortedvars+i)->ndx)->p->tn.lval);
		else printf("\n");
	    }
      }
#endif
   
   /* walk through sortedvars array and assign registers */

   for (i = 0; i < symtabndx /* (sortedvars + i)->ndx */; ++i)
      {
      p = sortedvars + i;
      if ( p->ndx == 0 ) continue;
#if 0
	  /* if not a leaf function and used only twice then unprofitable */
	  if (leaffunc == FALSE && p->weight < 3)
			break;
	  /* if used only once then unprofitable */
      if (p->weight < 2) 
		break;
#else
		/* bk: a better heuristic might be to check sibling scopes
			and use 2 references as the threshold if register promotion
			does not entail allocating a new register */
      if (p->weight < 3) 
		break;
#endif
      q = symtab + p->ndx;
      regmask = getreg(q->scope, q->type, &q->reg, q);
      if (regmask)
         {
         for (r = q->scope->higher; r ; r = r->higher)
            r->regavail |= regmask;
         if (q->scope->lower)
            regannounce(q->scope->lower, regmask);
         }
      }
}


/*F*********************** FUNCTION HEADER *******************************

 regannounce

	This function announces the usage of a register to lower scope levels.

PARAMETERS:

        Receives:	p:	current scope
                 	reg:	register being added

        Returns:	nothing

EXTERNAL DATA REFERENCES:

        none

FUNCTIONS CALLED:

        regannounce

CALLED BY:

        assign
        regannounce

*FE********************************************************************/

static void
regannounce(p, reg)

SCOPE *p;
int reg;
{
   p->regavail |= reg;
   p->regexport |= reg;
   if (p->lower)
      regannounce(p->lower, reg);
   if (p->same)
      regannounce(p->same, reg);
} 


/*F*********************** FUNCTION HEADER *******************************

 altertrees

	Walk the Ctrees and replace the simple local variables and simple
	parameters with their designated registers.

PARAMETERS:

        Receives: nothing

        Returns: nothing

EXTERNAL DATA REFERENCES:

	scoperoot
	prolog
	basic block flow graph

FUNCTIONS CALLED:

	alterblocks
	alterbb

CALLED BY:

	asgregvars      

*FE********************************************************************/

static void
altertrees()

{
   BBPTR bb;

   alterblocks( scoperoot );    /* alter SBLOCK and EBLOCK nodes */   
   prolog = FALSE;
   for (bb = fgroot->next; bb; bb = bb->next)
      alterbb(bb);
}


/*F*********************** FUNCTION HEADER *******************************

 alterblocks

	Alter the SBLOCK and EBLOCK nodes to reflect the new register 
	assignments.

PARAMETERS:

	Receives: p: pointer to a scope

	Returns: nothing

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	alterblocks

CALLED BY:

	alterblocks
	altertrees

*FE********************************************************************/

static void 
alterblocks( p )

SCOPE *p;
{
   p->sblock->tn.rval = p->regavail;
   if (p->eblock)
      p->eblock->tn.rval = p->regexport;
   if (p->lower)
      alterblocks(p->lower);
   if (p->same)
      alterblocks(p->same);
}


/*F*********************** FUNCTION HEADER *******************************

 alterbb

	Traverse the code tree for this basic block, recognizing 
	the expression trees so they can be traversed.

	Identify the beginning of a function for parameter initialization.

PARAMETERS:

	Receives: bb: pointer to the current basic block

	Returns: nothing

EXTERNAL DATA REFERENCES:

	prolog

FUNCTIONS CALLED:

	alterexpr
	addparminit

CALLED BY:

	altertrees      

*FE********************************************************************/

static void
alterbb( bb )

BBPTR bb;
{
   NODE *p;

   for (p = bb->codetree; p; p = p->stmt.succ)
      {
      switch( p->tn.op )
         {
         case SBLOCK:
         case EBLOCK:
#ifdef ASM_COMMENT
	 case ASMNO:
#endif
            break;
         case EXPRNO:
            alterexpr(p);
            if (prolog)
               {
               p = addparminit(p); /* add the initialization of parameters */
               prolog = FALSE;
               }
            break;
         }
      }
}


/*F*********************** FUNCTION HEADER *******************************

 addparminit

	If a parameter has been allocated to a register then add the
	expression which initializes the parameter. These expressions
	are added after the PROLOG node.

PARAMETERS:

	Receives: q: pointer to the PROLOG node

	Returns: returns pointer to the new tree

EXTERNAL DATA REFERENCES:

	symtab
	scoperoot

FUNCTIONS CALLED:

	talloc

CALLED BY:

	alterbb

*FE********************************************************************/

static NODE*
addparminit( q )

NODE *q;        /* pointer to the PROLOG node */
{
   int i;
   NODE *p, *t, *eq, *l, *r, *a, *bbeg;
   SYMENT *sym;
   FILE *fd;

   for ( i = 0 ; i < symtabndx ; i++ )
      {
      sym = symtab + i;
      if ( ! ( sym->reg			  /* continue if not assigned */ 
	     && (  sym->sclass == C_ARG   /*    or not a parameter    */
		|| sym->sclass == ICON    /*    or not an ICON        */
		|| sym->sclass == FCON    /*    or not an FCON        */
		)
	     )
	 ) continue;

      /* parameter has been assigned to a register; create an assignment
         node and add it to code tree just after the prolog node "q" */

      /* create EXPRNO */

      t = talloc();
      t->stmt.op = EXPRNO;
      t->stmt.bb = NULL;

      /* link the node into the code tree */

      if (t->stmt.succ = q->stmt.succ) /* this should be an assignment */
          t->stmt.succ->stmt.pred = t;
      t->stmt.pred = q;
      q->stmt.succ = t;

      t->stmt.rval = q->stmt.rval;  /* keep the same line number */
      q = t;

      /* create "l" the left side of the assignment, the register node */

      l = talloc();
      l->tn.op = REG;
/*
 *   If the variable is to reside in an address register, indicate in the lval
 */
      l->tn.lval = sym->flags & USE_AREG;
      l->tn.rval = sym->reg;     /* save register in node */
      l->tn.type = sym->type;

      /* create "r" the right side of the assignment, the parameter's node */

      r = talloc();
      r->tn.rval = 0;		/* FCONs change this with p->fp.dval assign */
      r->tn.type = sym->type;

      if ( sym->sclass == C_ARG )
	 {
      	   r->tn.op = VPARAM;
      	   r->tn.lval = sym->offset;
	 }
      else if ( sym->sclass == ICON )
         {
	   r->tn.op = ICON;
           if ( sym->p->tn.name != NULL )    /* copy name of address constant */
	      {
/*
 *  load the base of a structure only, the offset (lval) therefore must be zero
 */
      	      r->tn.lval = 0;
      	      r->tn.name = sym->p->tn.name;
	      }
      	   else
	      {
	      r->tn.lval = sym->p->tn.lval; /* copy the ICONS value */
      	      r->tn.name = NULL;
	      }
         }
      else /* FCON */
         {
      	 r->tn.op = FCON;
         ((FLTDBL *)&r->fp.dval)->lg.l2 = ((FLTDBL *)&sym->p->fp.dval)->lg.l2;
	 ((FLTDBL *)&r->fp.dval)->lg.l  = ((FLTDBL *)&sym->p->fp.dval)->lg.l;
         }

      /* create "=" node */

      eq = talloc();
      eq->in.op = ASSIGN;
      eq->in.left = l;
      eq->in.right = r;
      eq->in.type = sym->type;
         
      t->stmt.left = eq;
      }
   return(q);
}


/*F*********************** FUNCTION HEADER *******************************

 alterexpr

	Traverse the expression tree and for every VAUTO and VPARAM
	encountered see if it should be replaced by a register.

PARAMETERS:

	Receives: p: current node

	Returns: nothing

EXTERNAL DATA REFERENCES:

	symtab
	prolog

FUNCTIONS CALLED:

	alterexpr
	chkCONV

CALLED BY:

	alterbb

*FE********************************************************************/

static void
alterexpr(p)

NODE *p;
{
   SYMENT *sym;

   switch( optype(p->tn.op) )
      {
   case BITYPE:
      alterexpr( p->in.left );
      alterexpr( p->in.right );
      break;
   case UTYPE:
      alterexpr( p->in.left );
      break;
   case LTYPE:
      sym = (symtab + p->opn.sourceno);
      if (    (p->opn.flags & SIMPLE && sym->reg)
	   || ( ( sym->sclass == ICON || sym->sclass == FCON )
		 && sym->reg
		 && sym->p->opn.parent->in.op != INTCALL1
		 && sym->p->opn.parent->in.op != INTCALL2
		 && sym->p->opn.parent->in.op != INTCALL3
		 )
	 )
	 {
	 NODE *l, *r, *plus;
/*
 *   If the op has a name ( not just op NAME ) then we have an external
 *   variable or structure name that has been promoted to a register.
 *
 *   The reference is to be changed to the register after dereferencing
 *   it as follows:
 *
 *		      p-> STAR
 *			 /    \
 *		 plus-> PLUS   0
 *		       /    \
 *		l-> REG  r-> ICON
 *		   /  \      /   \
 *	    sym->reg   0  p->lval  0	
 *
 *    NOTE: p->lval contains the offset for the structure element. If
 *	    the offset is zero then the PLUS and ICON nodes will not
 *	    be inserted.
 */
	 if ( sym->sclass == ICON && p->opn.op == NAME )
	    {
	    l = talloc();
	    l->tn.op = REG;		/* the REG node: a terminal node */
/*
 *   If the variable is to reside in an address register, mark it now (for c1).
 */
	    l->tn.lval = sym->flags & USE_AREG;
	    l->tn.rval = sym->reg;	/* register number */
	    l->tn.type = sym->type;	/* same as PTR | BTYPE(p->opn.type) */
	    l->tn.name = NULL;
	    l->opn.sourceno = p->opn.sourceno; /* copy pointer into symtab */
	    sym->p = l;		/* change the node pointer in symtab */

	    if ( p->tn.lval != 0 )	/* if the offset isn't zero */
	       {
	       r = talloc();		/* the ICON node: a terminal node */
	       r->tn.op = ICON;
	       r->tn.lval = p->tn.lval;		/* offset into the structure */	
	       r->tn.rval = 0;
	       r->tn.type = INT;		/* offset is always INT type */
	       r->tn.name = NULL;
	       r->opn.sourceno = 0;

	       plus = talloc();		/* the PLUS node: an interior node */
	       plus->in.op = PLUS;
	       plus->in.left = l;		/* left points to REG node */
	       plus->in.right = r;		/* right points to ICON node */
	       plus->tn.type = sym->type;	/* same as PTR | BTYPE(p->opn.type) */
	       plus->in.name = NULL;
	       plus->opn.parent = p;	/* PLUS's parent is STAR */

	       p->in.left = plus;
	       r->opn.parent = plus;	/* ICON's parent is PLUS */
	       l->opn.parent = plus;	/* REG's parent is PLUS */
	       }
	    else {
		 p->in.left = l;
		 l->opn.parent = p;	/* REG's parent is STAR if 0 offset */
		 }

	    p->opn.op = STAR;		/* the STAR node: an interior node */
	    p->in.right = 0;
	    p->in.name = NULL;
	    p->opn.sourceno = 0;
	    }
/*
 *   an ICON which has a name and its offset is less than 16 bits
 *   are handled next.  This code is to handle the following type
 *   of case:
 *
 *	int array[10][10];
 *	func(i)
 *	  int i;
 *	{ ... array[1][i] = 1; ... }
 * 
 *   The compiler previously would generate:
 *		mov.l	&1,(%d1*4,array+40)
 *
 *   ( even though the address of array had promoted )
 *   but with the following code will catch these cases and
 *   produce :
 *		mov.l	&1,(%a2,%d1*4,40.w)
 *
 *    The tree will already have a STAR dereference, so all that needs
 *    to be done is to insert a PLUS node as follows:
 *
 *		 plus-> PLUS
 *		       /    \
 *		p-> REG  r-> ICON
 *		   /  \      /   \
 *	    sym->reg   0  p->lval  0	
 */
	 else if (  sym->sclass == ICON
		 && p->opn.op == ICON 
		 && p->tn.name != NULL
/*
 *	check to see if the offset is a 16 bit offset
 */
		 && (short) p->tn.lval == (long) p->tn.lval
		 )
		{
		if ( p->tn.lval != 0 )	/* has a nonzero offset */
		   {
	           r = talloc();		/* ICON node: a terminal node */
	           r->tn.op = ICON;
	           r->tn.lval = p->tn.lval;	/* offset into the structure */	
	           r->tn.rval = 0;
	           r->tn.type = INT;	/* ICON offset always INT type */
	           r->tn.type = sym->type;
	           r->tn.name = NULL;
	           r->opn.sourceno = 0;

	           plus = talloc();		/* PLUS node: a interior node */
	           plus->in.op = PLUS;
	           plus->in.left = p;		/* left points to REG node */
	           plus->in.right = r;		/* right points to ICON node */
	           plus->tn.type = sym->type; /* type if PTR | BTYPE */
	           plus->in.name = NULL;
	           plus->opn.parent = p->opn.parent; /* PLUS's parent */

/*
 *   decide if the leaf node we are processing is on the left or
 *   the right of its parent. Reassign the appropriate pointer to plus
 */
	           if ( p->opn.parent->in.left == p )
	           	    p->opn.parent->in.left = plus;
	           else p->opn.parent->in.right = plus;
	           r->opn.parent = plus;	/* ICON's parent is PLUS */
	           p->opn.parent = plus;	/* REG's parent is PLUS */
	           }
		   p->tn.op = REG;
		   p->tn.lval = sym->flags & USE_AREG;
	   	   p->tn.rval = sym->reg;   /* register number */
	    	   p->tn.type = sym->type;  /* same as PTR|BTYPE(p->opn.type) */
	    	   p->tn.name = NULL;
	    	   p->opn.sourceno = p->opn.sourceno; /* copy ptr into symtab */
	    	   sym->p = p;		/* change the node pointer in symtab */
	       }
/*
 *    Leave address constants with offsets greater than 16 bits alone
 */
	 else if (  sym->sclass == ICON
		 && p->opn.op == ICON 
		 && p->tn.name != NULL
		 && (short) p->tn.lval != (long) p->tn.lval
		 )
		{} /* do nothing */
	 else { /* If it is a variable which should be in a register
                 ** change the reference to the register */
              p->opn.op = REG;
/*
 *   If the variable is to reside in an address register, mark it now.
 */
	      p->tn.lval = sym->flags & USE_AREG;
              p->tn.rval = sym->reg;
	      chkCONV(p, sym);		/* add conversion node if necessary */
	      }
         }
      if ( p->tn.op == PROLOG )
         prolog = TRUE; /* Flag the beginning of the function, so any
                        ** registers which have been allocated variables
                        ** can be initialized */
      }
}


/*F*********************** FUNCTION HEADER *******************************

 chkCONV

	When placing a variable in a register, it may be necessary to
	add a conversion node, and repaint the type of the variable.

	The following table illustrates what needs to be done when the
	item on the left is placed in a register and the parent node is
	the type indicated in the table.

	key to following action table

	- change variable to a register and keep the type information in
	  the node

	1 change the variable to a register and paint the node's type,
	  also add a conversion node

	x the conversion is semantically illegal and should not occur

	  i   pi   ul  pul    f   pf    d   pd    s   ps   us  pus
          ________________________________________________________
any ptr   1    -    1    -    x    -    x    -    1    -    1    -
i,l,ul,u  -    1    -    1    -    1    -    1    1    1    1    1
    d, f  -    x    -    x    -    x    -    x    -    x    -    x
   s, us  -    -    -    -    -    -    -    -    -    -    -    -
   c, uc  -    -    -    -    -    -    -    -    -    -    -    -

	  c   pc   uc  puc  str pstr    l   pl  uni puni
          ________________________________________________________
any ptr   1    -    1    -    x    -    1    -    x    -
i,l,ul,u  1    1    1    1    x    1    -    1    x    1
    d, f  -    x    -    x    x    x    -    x    x    x
   s, us  1    -    1    -    x    -    -    -    x    -   
   c, uc  -    -    -    -    x    -    -    -    x    -   


PARAMETERS:

        Receives:	p: current variable node
			q: pointer to variable's entry in symbol table

        Returns:	nothing

EXTERNAL DATA REFERENCES:	none

FUNCTIONS CALLED:	addCONV

CALLED BY:	alterexpr       

*FE********************************************************************/

static void
chkCONV(p, q)

NODE *p;       /* current variable node */
SYMENT *q;     /* pointer to variable's entry in symbol table */
{
         TWORD newtype;
         TWORD oldtype;

         newtype = p->tn.type;
         oldtype = q->type;
         if (oldtype != newtype) /* CONVersion node may be necessary */
            {
            if (ISPTR(oldtype))
               {
               if ( ! ISPTR(newtype))
                  {
                  p->tn.type = oldtype;   /* repaint node */
                  addCONV( p, newtype);
                  }
               }
            else if (oldtype == INT || oldtype == LONG 
                        || oldtype == ULONG || oldtype == UNSIGNED)
               {
               if (ISPTR(newtype))
                  {
                  p->tn.type = oldtype;   /* repaint node */
                  addCONV( p, newtype);
                  }
               else
                  {
                  switch (newtype)
                     {
                  case SHORT:
                  case USHORT:
                  case CHAR:
                  case UCHAR:
                     p->tn.type = oldtype;
                     addCONV(p, newtype);
                     break;
                  default:
                     break;
                     }
                  }
               }
            else if (oldtype == SHORT || oldtype == USHORT)
               {
               if (newtype == CHAR || newtype ==UCHAR)
                  {
                  p->tn.type = oldtype;
                  addCONV(p, newtype);
                  }
               }
         }
}


/*F*********************** FUNCTION HEADER *******************************

 addCONV

	Add a convert node of type "newtype" to node p.

PARAMETERS:

	Receives:	p: pointer to node receiving CONVersion
			newtype: type of CONVersion noed

	Returns:	nothing

EXTERNAL DATA REFERENCES:	none

FUNCTIONS CALLED:	 talloc

CALLED BY:	chkCONV

*FE********************************************************************/

static void
addCONV(p, newtype)

NODE *p;
TWORD newtype;
{
        NODE *t;

        t = talloc();   /* create the CONV node */
        t->opn.op = CONV;
        t->tn.rval = 0;
        t->opn.type = newtype;
        t->opn.left = p;
        
        if (p->opn.parent->opn.left == p)       /* alter tree pointers */
           p->opn.parent->opn.left = t;
        else
           p->opn.parent->opn.right = t;
        t->opn.parent = p->opn.parent;
        p->opn.parent = t;
}


/*F*********************** FUNCTION HEADER *******************************

 initregavail

	Traverse the scope tree, bottomup, and initialize the regavail mask
	for each scope.

PARAMETERS:

        Receives:	p: scope of concern

        Returns:	nothing


EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	initregavail

CALLED BY:

	initregavail
	asgregvars

*FE********************************************************************/

static void
initregavail(p)

SCOPE *p;
{
   SCOPE *q;

   if (p->lower)        /* go all the way to the bottom */
      initregavail(p->lower);
   else
      for (q = p->higher; q ; q = q->higher)    /* propagate info upwards */
         q->regavail |= p->regavail;
   if (p->same)         /* do the same for siblings */
      initregavail(p->same);
}
