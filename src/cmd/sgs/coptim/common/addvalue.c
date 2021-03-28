#ident	"@(#)addvalue.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*      addvalue.c  7.3             */
/*      Copyright 1985 Motorola Inc.                    */

#include "declar.h"

#define INVALID 0               /* operation can not be used in lcse */

/* The structure of the hash table which supports local common subexpression
   elimination consists of the following structures. 

   The hash table is an array of pointers to buckets of ENTRYs which have 
   the same hash value. The hash value is a function of a node's operator
   and the value number of its left and right siblings.

   Each ENTRY points to an LCSE, which contains specific information about
   the possible lcse. Part of the information of an LCSE is a pointer to
   a link list of LCSELINKs. Each LCSELINK points to an occurrence of
   the lcse in the code tree.

   If two or more entries occur in the LCSELINK list, then a pointer to the 
   LCSE is added to the alteration list. This list is processed at the end
   of a basic block.
*/

typedef struct Tlcselink        /* element in list of lcse */
      {
      NODE *lcsenode;           /* pointer to expression in code tree */
      struct Tlcselink *next;   /* pointer to next element in list */
      } LCSELINK;

typedef struct Tlcse            /* local common subexpression info. node */
      {
      int count;                /* the number of expressions in LCSELINK list */
      LCSELINK *lcselink;       /* link list of expressions */
      int var;                  /* index into symbol table for the variable 
                                ** which holds this value number */
      } LCSE;

typedef struct Talt             /* element in LCSE alteration list */
      {
      LCSE *expr;               /* pointer to local common subexpression
                                ** involved */
      struct Talt *next;        /* next element in list */
      } ALTITEM;

typedef struct Tlcsekey         /* local common subexpression hash table key */
      {
      int op;                   /* operator involved in expression */
      int type;                 /* type of operator involved in expression */
      ENTRY *left;              /* value number of left sibling */
      ENTRY *right;             /* value number of right sibling */
      } KEY;

/* Prototypes for static functions */
static int  binaryop _PARMS((union ndu *));
static int  unaryop _PARMS((union ndu *));
static int  leafvalue _PARMS((union ndu *));
static void  addconst_tosym _PARMS((union ndu *));
static void  replace _PARMS((union ndu *, int ));
static void  addalteration _PARMS((struct Tlcse *));
static struct entry * enterht _PARMS((struct Tlcsekey *, union ndu *));
static void  makealterations _PARMS((struct Tbasicblock *));
#ifndef NODBG
static void  dumpBB _PARMS((struct Tbasicblock *));
#endif

static ALTITEM *altlist;        /* points to the head of the alteration list */


/*F************************** FUNCTION HEADER ****************************

 addvalueno

        This function controls the assignment of a value number to node "p".
        The type of value number assigned to a node depends on the type of
        the node. BITYPE and UTYPE nodes may obtain their value number
        from the lcse hash table, if "p" is not an INVALID operator.  LTYPE 
        nodes may be unique or get their value number from the symbol table, or 
        the constant hash table. If "allowable" is FALSE then the expression
        can not be entered into the expression hash table.
        Finally, if a possible assignment to an aliased variable is found,
        all former variable value numbers are invalidated.

PARAMETERS:

        Receives:
                
                p: pointer to node which will receive value number
        allowable: boolean flag which indicates if expression rooted by
                   "p" should be added to lcse hash table

        Returns:	nothing

EXTERNAL DATA REFERENCES:

        NODE: see description associated with declaration

FUNCTIONS CALLED:

	addvalue: recursive call for ++ and -- operators
        optype: classifies the nodes operator as BITYPE, UTYPE, or LTYPE
        binaryop: can binary operator be a part of lcse
        flushstvn: clear all symbol table variable value numbers when
                   an aliased variable is modified
        getvalueno: returns unique value number
        enterht: enter expression into lcse hash table
        unaryop: can unary operator be a part of lcse
        leafvalue: gets value number for a leaf node

CALLED BY:

        recognizebb

*FE*************************************************************************/

void
addvalueno( p, allowable )

NODE *p;
int allowable;
{
   int op;
   KEY *key;
   NODE *r, *l, *n;
   
   switch( optype(p->opn.op) )
      {
   case BITYPE:
      op = binaryop( p );       /* make the operator unique */
      l = p->in.left;           /* left sibling */
      r = p->in.right;          /* right sibling */
      if (op == INVALID)        /* operator can not be involved in a lcse */
         p->opn.valueno = (ENTRY *)getvalueno();
      else if (op == ASGFLG)    
         {      
	 if ( (p->opn.op==INCR) || (p->opn.op==DECR) )
	    {
		/* operator is ++ or --, result is the value number of the
		   left side but we must also simulate the effects of
		   a post assignment to the left side as well */

		/* hoss#39 */
		/*p->opn.valueno = l->opn.valueno;*/
		p->opn.valueno = (ENTRY *)getvalueno();
#if 0 /*jcl*/
		if (l->opn.flags & LCSEFLAG)
			p->opn.flags |= LCSEFLAG;       /* copy HASH flag */
		/* now we must simulate a l+=r or l-=r operation */
		n = talloc();           /* get a pseudo node */
		n->in.op = (p->opn.op==INCR) ? ASG PLUS : ASG MINUS;
		n->in.left = l;
		n->in.right = r;
		n->in.type = l->in.type;
		addvalueno(n, allowable);  /* simulate the assignment */
		n->in.op = FREE;
#endif
		}

                /* operator is an assignment node, use the value number of
                ** the right side.  If the left side is a simple variable
                ** then assign the value number to the simple variable. */

	 else if (l->opn.op == STAR) /* modification of an aliased variable */
            {
            p->opn.valueno = (ENTRY *)getvalueno();
            flushstvn();        /* Invalidate all variable value nos.  */
            }
         else if (p->opn.op == ASSIGN)
            {
            p->opn.valueno = r->opn.valueno;
            if (r->opn.flags & LCSEFLAG)  /* indicate if this value number is */
               p->opn.flags |= LCSEFLAG; /* a pointer into the hash structure */
            if (l->opn.flags & SIMPLE)
               { /* assignment to a simple variable - update symbol table */
               (symtab + l->opn.sourceno)->valueno = p->opn.valueno;

               /* save the simple variable's symbol table index 
               ** in the LCSE struct iff the value number is a pointer */
   
               if ( r->opn.flags & LCSEFLAG )
                  ((LCSE *) r->opn.valueno->data)->var = l->opn.sourceno;
               }
            }
         else
            {
            p->opn.valueno = (ENTRY *)getvalueno();
            if (l->opn.flags & SIMPLE)
               /* assignment to a simple variable - update symbol table */
               (symtab + l->opn.sourceno)->valueno = p->opn.valueno;
			   /* jcl */
	    else if ((l->opn.op == CONV) && (l->in.left->opn.flags & SIMPLE))
               (symtab + l->in.left->opn.sourceno)->valueno = p->opn.valueno;
            }
         }
      else 
         {
         if (!allowable)
            p->opn.valueno = (ENTRY *)getvalueno();   
         else
         /* make the hash key - the space will be freed later */
            {
            key = (KEY *)malloc(sizeof(KEY));
            key->op = op;
            key->type = p->opn.type;
            key->left = l->opn.valueno;
            key->right = r->opn.valueno;

            p->opn.valueno = enterht(key, p);
            }
         }
      break;

   case UTYPE:

      op = unaryop(p);
      l = p->in.left;
      if (op == INVALID)        /* operator can not be involved in a lcse */
         p->opn.valueno = (ENTRY *)getvalueno();
      else
         {
         if (!allowable || op == COMOP) 
            p->opn.valueno = (ENTRY *)getvalueno();
         else
            {
            /* make the hash key - space will be freed later */
            key = (KEY *)malloc(sizeof(KEY));
            key->op = op;
            key->type = p->opn.type;
	    if (op == UNARY AND) key->left = (ENTRY *)p->in.left->tn.lval;
	      else key->left = l->opn.valueno;
            key->right = 0;

            p->opn.valueno = enterht(key, p);
            }
         }
      break;
   
   case LTYPE:
      p->opn.valueno = (ENTRY *)leafvalue( p );
      break;
      }
}


/*F*********************** FUNCTION HEADER *******************************

 binaryop

        Function classifies a binary operator according to its ability to
        be the operator in local common subexpression. All the assignment
        operators are classified as ASGFLG. Operators which can not 
        be operators in a lcse return INVALID. For all the other operators
        the function returns a unique operator which identifies the
        operator (in most cases this is just the operator itself).

PARAMETERS:

        Receives:	p: pointer to the node being classified

        Returns:	A unique value for "p"'s operator

EXTERNAL DATA REFERENCES:

        Operator identifiers found in manifest.h

FUNCTIONS CALLED:

        none

CALLED BY:

        addvalueno

*FE********************************************************************/

static int
binaryop( p )

NODE *p;
   {
   if (asgop(p->opn.op))        /* is it an assignment operator? */
      return (ASGFLG);

   switch (p->opn.op)
      {
   case COMOP:			/* to stop such things as "i=7,j=4" */
   case CALL:
#ifdef IN_LINE
   case INCALL:
#endif
   case FORTCALL:
   case STCALL:
   case CMP:
   case CBRANCH:
      return(INVALID);
   default:
      return( p->opn.op );
      }
}

/*F*********************** FUNCTION HEADER *******************************

 unaryop

        Function determines if the operator is permissable in a local
        common subexpression.  If it isn't the function returns an
        INVALID value, otherwise it returns a unique value for the
        operator which is usually just the value of the operator.

        To uniquely classify CONV nodes it is necessary to include their
        type in the computation.  Therefore, CONV nodes return an offset of 
        MANY, the maximum number of operators.  This computation may need 
        to change if other nodes are found which require additional information.

PARAMETERS:

        Receives:	p: pointer to the node being classified

        Returns:	A unique value for "p"'s operator

EXTERNAL DATA REFERENCES:

        Operator identifiers found in manifest.h

FUNCTIONS CALLED:

        none

CALLED BY:

        addvalueno

*FE********************************************************************/

static int
unaryop ( p )

NODE *p;
{
   switch (p->opn.op)
      {
   case STAR:
   case FUNARG:
   case STARG:
   case UNARY CALL:
#ifdef IN_LINE
   case UNARY INCALL:
#endif
   case UNARY STCALL:
   case UNARY FORTCALL:
   case GENBR:
   case GENUBR:
   case GENLAB:
      return( INVALID );
   case CONV:
      /* MANY is the maximum  number for an operator in manifest. If
         this method for achieving unique operators is used in other
         instances than it might be necessary to actually create the
         operators. */
      return( p->opn.op + p->opn.type + MANY);
   case UNARY AND:
      if ( !(p->in.left->opn.flags & (SIMPLE | ARRAY)) )
	 return ( INVALID );
   default:
      return( p->opn.op );
      }
}

/*F*********************** FUNCTION HEADER *******************************

 leafvalue

        Function returns a value number for a leaf node.
        Value numbers for leaves are:
                1) symbol table # for simple locals/parameters and arrays
                2) pointers to LCSE structures in the constant hash table
                   if "p" is an ICON or FCON
                3) all other leaves return a unique number

PARAMETERS:

        Receives:	p: pointer to the node being classified

        Returns:	A unique value for leaf "p"

EXTERNAL DATA REFERENCES:

        Operator identifiers found in manifest.h

FUNCTIONS CALLED:

        none

CALLED BY:

        addvalueno

*FE********************************************************************/


static int
leafvalue( p )

NODE *p;
{
   ENTRY item;
   FLTDBL *key;
   ENTRY *q;
   NODE *parent;
   int ndx;         /* index of symbol in symbol table */

/*
 *   Flag leaf nodes that are involved in operations that are illegal
 *   for address registers as COMPLEX. If a leaf node isn't COMPLEX
 *   and is promoted to an address register, a flag is set in its
 *   lval for c1 to catch these special cases. Ultimately this allows
 *   INTS to be promoted to address registers.
 */
   if ( ISCOMPLEX( p ) )
        (symtab + p->opn.sourceno)->flags |= COMPLEX;
   else if (  p->opn.parent
	   && p->opn.sourceno
	   && ( p->opn.parent->in.op == CONV || asgop( p->opn.parent->in.op ))
	   )
	{
/*
 *    Must check ASGOPs and CONVersions parents for COMPLEX operations.
 *    If it has a parent(s) which are ASGOPs, their type must be one of
 *    INT, UNSIGNED, LONG, ULONG. If the parents don't meet this criteria
 *    it is marked as COMPLEX. This is needed because ASGOPs can have their
 *    lvals propogated up the tree. Consider:  float = (int += 2);
 */
	parent = p->opn.parent;
	while ( parent && ( parent->in.op == CONV || asgop( parent->in.op )))
	      {
	      if ( (  parent->opn.type != INT
		   && parent->opn.type != UNSIGNED
	 	   && parent->opn.type != LONG
		   && parent->opn.type != ULONG
		   )
		   || parent->opn.flags & LCSEFLAG
		 )
        	   (symtab + p->opn.sourceno)->flags |= COMPLEX;
	      parent = parent->opn.parent;
	      }
	}

	if (p->opn.flags & UNIQUE)
		/* node must be unique since is casted or not first address of item */
		return(getvalueno()); /* all other leaves return a unique number

/*
 *	simple locals/parameters return symbol table value #
 */
     if (p->opn.flags & SIMPLE)
      return((int)(symtab + p->opn.sourceno)->valueno);

/*
 *	arrays return symbol table value #
 */
   if (p->opn.flags & ARRAY)
      return((int)(symtab + p->opn.sourceno)->valueno);

/*
 *	constants return value # from constant hash table
 */
   if ((p->opn.op == ICON || p->opn.op == FCON)
       && p->tn.name == NULL )
      {
      int ok_constant; /* flag for acceptable symbol table entry */

      /* make key */
      key = (FLTDBL *) malloc(sizeof(FLTDBL));
      if ( p->opn.op == ICON )
         {
         key->lg.l = p->tn.lval;
         key->lg.l2 = 0;
         }
      else
         {
         key->lg.l2 = ((FLTDBL *) &p->fp.dval)->lg.l2;
         key->lg.l = ((FLTDBL *) &p->fp.dval)->lg.l;
         }
      item.key = (char *) key;
      item.data = NULL;
      q = hsearch(&constht, consthash, constcmp, item, ENTER);
/*
 *      Decide if the constant is an acceptable symbol table entry
 *	( for possible promotion ). The criteria is as follows:
 *
 *      	1. They are not a child of one of the following :
 *			CBRANCH  Conditional BRANCH
 *			GENLAB   GENerate LABel
 *			GENUBR   GENerete Unconditional BRanch
 *		    (Labels are put in the constant table for ?OP, &&, and || )
 *      	2. Or its not a structure offset of a local ( due to less
 *		    efficient addressing mode if a 16 bit displacement is
 *		    promoted to a register,(%an,d16) is more efficient
 *		    than (0,%an,%Xn).
 *		3. Or quick addressing mode can be used instead (ie values 0-7)
 *			with exceptions as noted below.
 *		4. Or any floating point constant (FCON) not involved in 
 *			an ASSIGN.
 *
 *	Note: compare, multiply and divide do not have quick addressing
 *            modes, so these are exempt from #3 above with the exception
 *	      of a divide by a power of 2 or ANY multiply. The promotion
 *	      of an integer constant used in a multiply would defeat the
 *	      iconmul optimization in c1. Divides by a power of two can 
 *	      be done with a shift right. Assignments using a FCON are
 *	      best done with a mov.l &FCON,dest.
 */
	 if ( !( p->opn.parent			        /* cant have a parent */
	         && (( p->opn.parent->opn.op == CBRANCH /* that is a CBRANCH */
		     || p->opn.parent->opn.op == GENLAB   /* or a GENLAB */
		     || p->opn.parent->opn.op == GENUBR   /* or a GENUBR */
		     )
		    || ( p->opn.parent->opn.op == PLUS
			&& p->opn.parent->opn.parent
			&& p->opn.parent->opn.parent->opn.op == STAR
	                && ((unsigned) p->tn.lval < 0x0000FFFF)
		       )
		    )
	       )
/* omit divide by a power of two */
	       && (((   p->opn.parent
		     && (  p->opn.parent->opn.op == DIV
		        || p->opn.parent->opn.op == ASG DIV
		        || p->opn.parent->opn.op == MOD
		        || p->opn.parent->opn.op == ASG MOD
			)
		    )
		    && (p->tn.lval & ((p->tn.lval) - 1))/* not POW2 */
		   )
/* use quick addr mode if const <= 8 */
	          || ( p->opn.op == ICON && p->tn.lval > 8
/* omit multiply */
		     && !( p->opn.parent
		         && (  p->opn.parent->opn.op == MUL
			    || p->opn.parent->opn.op == ASG MUL
			    )
			 )
		     )
	          || ( p->opn.op == FCON && p->opn.parent->opn.op != ASSIGN )
		  )
	    )
		ok_constant = TRUE;	/* acceptable constant for promotion */
	 else ok_constant = FALSE;	/* unacceptable constant */

      if ( q->data == NULL )	/* a new constant */
	 {
         q->data = (char *) getvalueno();
	 if ( ok_constant )
	    { 
		addconst_tosym( p );
	    }
         else p->opn.sourceno = 0; /* so travexpr won't access out of symtab */
	 }
      else                     /* old constant */
         {
	 if ( ok_constant )
	    {
/*
 *	Search the symbol table to see if an entry already exists.
 *	If not add it, else set the node's sourceno to the symtab index
 *	for access by alterexpr() if promoted. When the lvals are equal and
 *	the names are the same we have a match.
 */
	    for ( ndx = 1; ndx < symtabndx ; ndx++ )
		if ( (   (  (symtab+ndx)->sclass == ICON
		  	 && ( p->opn.op == ICON || p->opn.op == NAME )
		         && (symtab+ndx)->p->tn.lval == p->tn.lval
			 )
		      || ( (symtab+ndx)->sclass == FCON
			 && p->opn.op == FCON
		         && (symtab+ndx)->p->tn.lval == p->tn.lval
			 )
		     )
		     && (( p->tn.name == 0 && (symtab+ndx)->p->tn.name == 0)
		        || (  p->tn.name
		           && (symtab+ndx)->p->tn.name
			   && (strcmp((symtab+ndx)->p->tn.name,p->tn.name)== 0)
			   )
			)
		   )
			break;
/*
 *    if the symbol table exists and the symbol isn't already in symbol table 
 */
	    if ( !symtab || ndx == symtabndx )
		 addconst_tosym( p );	/*    put it in  */
	    else p->opn.sourceno = ndx;
	    }
        else p->opn.sourceno = 0; /* so travexpr won't access out of symtab */
        free(key);
        }

      if ( ISCOMPLEX(p) )
   	   (symtab + p->opn.sourceno)->flags |= COMPLEX;

      return((int)q->data);
      }
/*
 *	Create a symbol table entry for address constant to allow for promotion
 */
   if ((  (p->opn.op == NAME && p->opn.name)
       || (  p->opn.op == ICON
	  && p->tn.name != NULL
	  && !callop(p->opn.parent->opn.op)
	  )
       )		   /* exclude branch labels */
       && !( p->opn.parent			      /* cant have a parent */
	     && (   p->opn.parent->opn.op == CBRANCH  /* that is a CBRANCH */
		 || p->opn.parent->opn.op == GENLAB   /* or a GENLAB */
		 || p->opn.parent->opn.op == GENUBR   /* or a GENUBR */
		)
	   )
      )
      {
/*
 *   Search the symbol table to see if an entry already exists.
 *   If not add it, else set the node's sourceno to the symtab index
 */
      for ( ndx = 1; ndx < symtabndx ; ndx++ )
	    if (   (symtab+ndx)->sclass == ICON
		&& (symtab+ndx)->p->tn.name
	        && strcmp( (symtab+ndx)->p->tn.name, p->tn.name ) == 0
	       )
		 break;
/*
 *    if the symbol table exists and the symbol isn't already in symbol table 
 */
      if ( !symtab || ndx == symtabndx )
	   addconst_tosym( p ); /*    put it in  */
      else p->opn.sourceno = ndx;
      }

   if ( ISCOMPLEX(p) )
       	(symtab + p->opn.sourceno)->flags |= COMPLEX;

   return(getvalueno()); /* all other leaves return a unique number */
}

/*F*********************** FUNCTION HEADER *******************************

 addconst_tosym

	Creates a symbol table entry for a constant ( ICON, FCON or 
	address constant- an ICON with a name and no CALLFLG )

PARAMETERS:

	Receives:	p: pointer to the leaf node

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	curscope

FUNCTIONS CALLED:

	getsyment
	getvalueno
	addsymbol - add the symbol to syment

CALLED BY:

	leafvalue

*FE********************************************************************/

static void
addconst_tosym(p)

NODE *p;
{
	int ndx;         /* index of symbol in symbol table */
	SYMENT *sym;     /* pointer to symbol table entry */

/*
 *   Don't add to symbol table if conflag or name is "L%" (label)
 */
	if (   conflag
            || ((p->tn.name != NULL) && strncmp("L%", p->tn.name, 2)==0)
/*
 *   Don't add constants with an INTCALL as a parent
 */
	    || ( p->opn.parent != NULL
	        && (  p->opn.parent->in.op == INTCALL1
	    	   || p->opn.parent->in.op == INTCALL2
	    	   || p->opn.parent->in.op == INTCALL3
	   	   )
	       )
	   )
		return;
/* 
 *  Enter the constant into the symbol table ( symtab ) so it will be
 *  considered for promotion to a register.  Initialize the symbol table entry
 */
	ndx = getsyment();
/*
 *	must set the sourceno for access by travexpr if promoted
 */
	p->opn.sourceno = ndx;
	sym = (symtab + ndx);
	sym->scope = scoperoot; /* to prevent reg reuse of constants curscope;*/
	sym->p = p;
	sym->scopendx = 0;
	sym->offset = 0;
	sym->newoffset = 0;
	sym->flags = 0;
	if (    p->opn.op == NAME
	   || ( p->opn.op == ICON && p->tn.name != NULL ) /* Address constant */
	   )
	{
	   sym->sclass = ICON;
	   sym->type = PTR | p->tn.type; /* type of reg needed if promoted */
	   sym->size = SZPOINT/SZCHAR;	 /* size in bytes */
	   sym->name = p->tn.name;
	}
	else if ( p->opn.op == ICON )
	{
	   sym->sclass = ICON;
	   sym->type = p->tn.type;	/* type of reg needed if promoted */
	   if ( sym->type == CHAR || sym->type == UCHAR )
	   	sym->size = SZCHAR/SZCHAR;	/* size in bytes */
	   else if ( sym->type == SHORT || sym->type == USHORT )
	   	sym->size = SZSHORT/SZCHAR;	/* size in bytes */
	   else sym->size = SZINT/SZCHAR;	/* size in bytes */
	   sym->name = "%ICON"; 
	}
	else /* FCON */
	{
	   sym->sclass = FCON;
	   sym->type = p->fp.type;  /* type of reg needed if promoted */
	   if ( sym->type == FLOAT )
	   	sym->size = SZFLOAT/SZCHAR; /* size in bytes */
	   else
	   	sym->size = SZDOUBLE/SZCHAR; /* size in bytes */
	   sym->name = "%FCON";
	}
	sym->weight = 0;
	sym->frmwgt = 0;
	sym->reg = 0;
	sym->valueno = (ENTRY *)getvalueno();
      	addsymbol( scoperoot->scopehead, ndx ); /* add to symbol list */
	if ( ! scoperoot->scopehead )
	     scoperoot->scopehead = ndx;
}
/*F*********************** FUNCTION HEADER *******************************

 replace

        If expression "p" has already been evaluated and is stored in
        "var", replace "p" with a reference to "var".

PARAMETERS:

        Receives:

                p: pointer to the root of the expression
              var: symbol table index for the variable which contains
                   the value of expression "p"

        Returns:	nothing

EXTERNAL DATA REFERENCES:

        NODE: see description associated with declaration in manifest.h

FUNCTIONS CALLED:

        none

CALLED BY:

        enterht

*FE********************************************************************/

static void
replace (p, var)
        

NODE *p;
int var;
{
   tfree(p);
   /* using p preserves parent link */
   p->tn.op = VAUTO;
   p->tn.lval = (symtab + var)->offset;
   p->tn.rval = 0;
   p->tn.type = (symtab + var)->type;
}

/*F*********************** FUNCTION HEADER *******************************

 addalteration

        This function adds a lcse to the list of alterations. New
        lcses are added to the beginning of list. This guarantees that
        parents will be replaced before their children.

PARAMETERS:

        Receives:	p: pointer to LCSE

        Returns:	nothing

EXTERNAL DATA REFERENCES:

        altlist: alteration list

FUNCTIONS CALLED:

        malloc

CALLED BY:

        enterht

*FE********************************************************************/

static void
addalteration( p )

LCSE *p;
{
   ALTITEM *q;

   q = (ALTITEM *) malloc(sizeof(ALTITEM));
   if (q == NULL)
      cerror("out of space - addalteration");
   q->expr = p;
   q->next = altlist;
   altlist = q;
   lcsestat++;  /* statistics counter */
# ifndef NODBG
   if (lcsedbflg)
      {
      printf("\n-----\nLocal Common Subexpression added to alteration list\n");
      e22print(p->lcselink->lcsenode, "T");
      printf("\n-----\n");
      }
# endif
}

/*F*********************** FUNCTION HEADER *******************************

 enterht

        This function enters an expression into the LCSE hash table. If
        the expression is a new entry into the table then the LCSE
        data structure is created and initialized. If the expression
        already exists in the table, then add another reference to it, and
        add to the alteration list for later replacement.

        Certain variables may already contain the values for expressions.
        If this circumstance is encountered, replace the expression with
        the variable.

PARAMETERS:

        Receives:	key: the key which uniquely identifies the expression
			p: pointer to the expression

        Returns:	pointer to the entry in the hash table.

EXTERNAL DATA REFERENCES:

        lcseht
        NODE

FUNCTIONS CALLED:

        hsearch: enters expresion into the hash table 
        replace: replace an expression with a variable
        classify: identify the variable 
        addalteration: add to alteration list for later replacement


CALLED BY:

        addvalueno

*FE********************************************************************/

static ENTRY *
enterht( key, p )

KEY *key;
NODE *p;
{
   LCSE *lcse;
   LCSELINK *link;
   ENTRY *q;
   ENTRY item;

   item.key = (char *) key;
   item.data = NULL;

   /* ENTER the expression in the lcseht */
   q = hsearch(&lcseht, lcsehash, lcsecmp, item, ENTER);

   if (q->data == NULL) /* if this is a new entry initialize its information */
      {
      lcse = (LCSE *) malloc(sizeof(LCSE));
      if (lcse == NULL)
         cerror("out of space - addvalueno 1");
      q->data = (char *) lcse;
      lcse->count = 0;
      lcse->var = 0;
      lcse->lcselink = NULL;
      }
   else
      { /* expression already existed in hash table */
      free(key);
      lcse = (LCSE *)q->data;
      }

   if (symtab!=0 && q == (symtab + lcse->var)->valueno)  
      {
      /* if a variable already contains this expression */
      replace(p, lcse->var);
      classify(p);
      return(q);
      }
   else
      {
      /* add expression to LCSE list */
  
      link = (LCSELINK *) malloc(sizeof(LCSELINK));
      if (link == NULL)
          cerror("out of space - addvalueno 2");
      link->next = lcse->lcselink;      /* place at head of list */
      link->lcsenode = p;
      lcse->lcselink = link;
  
      if (++lcse->count == 2) /* add to alteration list for later replacement */
         addalteration(lcse);
      }
   p->opn.flags |= LCSEFLAG;
   return( q );
}


/*F*********************** FUNCTION HEADER *******************************

 freelcse

        This function returns the space required for the local common
        subexpression structure. This function is called when the hash
        table supporting lcses is being cleaned up at the beginning of
        each basic block.

PARAMETERS:

        Receives:	p: pointer to the LCSE in the hash table

        Returns:	nothing

EXTERNAL DATA REFERENCES:

        LCSE hash table

FUNCTIONS CALLED:

        free

CALLED BY:

        lcse

*FE********************************************************************/

void
freelcse( p )

LCSE *p;
{
   LCSELINK *q;

   for (q = p->lcselink; q ; q = q->next)
      free(q);
   free( p );
}

/*F*********************** FUNCTION HEADER *******************************

 delaltlist

        Returns the space required for the alteration list.

PARAMETERS:

        Receives:	nothing

        Returns:	nothing

EXTERNAL DATA REFERENCES:

        alteration list

FUNCTIONS CALLED:

        none

CALLED BY:

        lcse

*FE********************************************************************/

void
delaltlist()

{
   ALTITEM *p;

   for (p = altlist; p ; p = p->next)
      free(p);
   altlist = NULL;
}


/*F*********************** FUNCTION HEADER *******************************

 lcse

        This function handles the local common subexpressions recognized
        in a single basic block. The alterations are made and
        the space required for the supporting data structures is freed.
        Also, the value numbers for symbol table variables are set to new
        values (i.e., cleared).

PARAMETERS:

        Receives:	bb: a pointer to a basic block

        Returns:	nothing

EXTERNAL DATA REFERENCES:

        lcseht

FUNCTIONS CALLED:

        makealterations
        delaltlist
        cleanupht
        flushstvn

CALLED BY:

        recognizebb

*FE********************************************************************/

void
lcse(bb)

BBPTR bb;
{
   if (bb)
      {
      makealterations(bb);      /* process alteration list */   
      delaltlist();     /* return space required for alteration list */
      cleanupht( &lcseht, freelcse); /* return space for lcse hash table */
      flushstvn();      /* alter value numbers of all symbol table entries */
      }
}

/*F*********************** FUNCTION HEADER *******************************

 makealterations

        Process the alteration list, if any, and replace the lcse with
        temporaries.  

PARAMETERS:

        Receives:	bb: pointer to the current basic block

        Returns:	nothing

EXTERNAL DATA REFERENCES:

        altlist: alteration list

FUNCTIONS CALLED:

        gettemp: get a temporary for this basic block
        classify: classify the temporary node
        talloc: allocate a node
        tfree: free a node

CALLED BY:

        lcse

*FE********************************************************************/

static void
makealterations(bb)
BBPTR bb;
{
   ALTITEM *p;
   LCSE *lcse;
   LCSELINK *link;
   int offset;
   int type;
   NODE *node;
   NODE *l, *eq, *r;
   int count;

   for (p = altlist; p ; p = p->next)
      {
      lcse = p->expr;

#ifndef NODBG
      if (lcsedbflg)
         {
         printf("\n--------\nReplacing Local Common Subexpression \n");
	 e22print(lcse->lcselink->lcsenode, "T");
         printf("\n--------\n");
         }
#endif

      /* check to see if the lcse still exists - was it consumed by another
      ** one? */

      count = 0;
      for( link = lcse->lcselink; link; link = link->next)
         if (link->lcsenode->opn.op != FREE)
            {
            type = link->lcsenode->opn.type;
            ++count;
            }

      /* if it exists in sufficient quantity then replace it with a temporary */

      if (count >= 2)
         {
         offset = gettemp(type);

         /* replace all references to the lcse with the temporary and free
         ** up the space required by the old references - the last instance
         ** in the list is handled differently as an assignment must be added
         ** prior to the expression */

         link = lcse->lcselink;
         for ( ; count > 1; --count )
            {
            for( ; link->lcsenode->opn.op == FREE; link = link->next);
            node = link->lcsenode;
            tfree(node);

            /* using node preserves parent link and type */
            node->tn.op = VAUTO;
            node->tn.lval = offset;
            node->tn.rval = 0;
            classify(node);
            link = link->next;
            }

         /* replace the first reference to the expression with the temporary and
         ** create the assignment statement */

         for( ; link->lcsenode->opn.op == FREE; link = link->next);
         node = link->lcsenode; 
         r = talloc();  /* right sibling for the assignment statement */
         *r = *node;    /* preserve parent's pointer to this node, and 
                        ** preserve opn.parent and type */

         /* reset parent pointers for siblings as they now point to node */
         r->in.left->opn.parent = r;
         if (optype(r->opn.op) == BITYPE)
            r->in.right->opn.parent = r;

         node->tn.op = VAUTO;
         node->tn.lval = offset;
         node->tn.rval = 0;
         classify(node);

         l = talloc();  /* create left sibling for the assignment statement */
         *l = *node;

         eq = talloc(); /* create assignment node */
         eq->in.op = ASSIGN;
         eq->in.left = l;
         eq->in.right = r;
         eq->in.type = r->tn.type;

         r->opn.parent = eq;
         l->opn.parent = eq;

         /* add assignment statement to codetree - r and l are temps now */

         /* walk up the tree to the EXPRNO node */
         for (r = node->opn.parent; r->opn.op != EXPRNO; r = r->opn.parent);

	 /* make sure we insert before any other insertions */
	 /* as superset LCSE may require subset LCSE results */
	 while( ((l=r->stmt.pred)!=NULL) && l->stmt.newflag) r = l;

         /* duplicate EXPRNO node and link in the assignment node */
         l = talloc();
         *l = *r;
	 l->stmt.newflag = 1;
         eq->opn.parent = l;
         l->stmt.left = eq;
         l->stmt.succ = r;
         l->stmt.pred = r->stmt.pred;
         if (r->stmt.pred == NULL)
            bb->codetree = l;
         else
            r->stmt.pred->stmt.succ = l;
         r->stmt.pred = l;
         }
         
      }
}

#ifndef NODBG
static void dumpBB(bb)  /* For debug only */
BBPTR   bb;
{       NODE *p;

	for (p = bb->codetree; p; p = p->stmt.succ)
	    if (p->tn.op == EXPRNO)
	    {   printf("**Next EXPRNO\n");
		e22print(p->opn.left,"T");
	    }
}
#endif

/*F*********************** FUNCTION HEADER *******************************

 constcmp

        Compares two constants and returns -1, 0 , or 1 depending if
        key1 < key2, key1 == key2, or key1 > key2.
        The double variables are not used for comparison due to byte
        swapping, also the double comparison takes longer.
        The actual value of the number doesn't really matter, as this
        routine is not trying to order them in a binary tree. Just
        make it fast to determine if the number exists in the tree.

PARAMETERS:

        Receives:	key1, key2: pointers to the two items being compared

        Returns:	-1, 0, or 1 as explained above

EXTERNAL DATA REFERENCES:

        none

FUNCTIONS CALLED:

        none

CALLED BY:

        leafvalue via hsearch

*FE********************************************************************/

int
constcmp( key1, key2 )

FLTDBL *key1;
FLTDBL *key2;
{
   if (key1->lg.l == key2->lg.l)
      {
      if (key1->lg.l2 == key2->lg.l2)
         return(0);
      else if (key1->lg.l2 < key2->lg.l2)
         return(-1);
      else 
         return(1);
      }
   else if (key1->lg.l < key2->lg.l)
      return(-1);
   else 
      return(1);
}

/*F*********************** FUNCTION HEADER *******************************

 consthash

        Hashing function for constant hash table.

PARAMETERS:

        Receives:	ht: pointer to the hash table
			key: pointer to constant to be entered/found

        Returns:	index into the hash table

EXTERNAL DATA REFERENCES:

        hash table

FUNCTIONS CALLED:

        none

CALLED BY:

        hsearch via leafvalue

METHOD:

        Add the two parts of the constant and find the modulo number of
        the bucket

*FE********************************************************************/

unsigned int
consthash( ht, key )

HASHTAB *ht;
FLTDBL *key;
{
   int result;

   result = (key->lg.l < 0 ? -key->lg.l : key->lg.l) + 
            (key->lg.l2 < 0 ? -key->lg.l2 : key->lg.l2);
   return( ((result<0)?-result:result) % ht->length);
}


/*F*********************** FUNCTION HEADER *******************************

 lcsehash

        Hashing function for local common subexpression hash table.

PARAMETERS:

        Receives:	ht: pointer to hash table

        Returns:	index into the hash table

EXTERNAL DATA REFERENCES:

        none

FUNCTIONS CALLED:

        none

CALLED BY:

        hsearch via enterht

METHOD:

        Add the operators for current entry and its left and right child,
        and divide by the size of the hash table.

*FE********************************************************************/

unsigned int
lcsehash( ht, key )

HASHTAB *ht;
KEY *key;
{
   int result;

   result = (key->op + key->type + (int)key->left + (int)key->right);
   return( ((result<0)?-result:result) % ht->length);
}


/*F*********************** FUNCTION HEADER *******************************

 lcsecmp

        Compares two value numbers and returns -1, 0 , or 1 (less than,
        equal, or greater than) if key1 < key2, key1 == key2, or key1 > key2.

PARAMETERS:

        Receives:	key1, key2: keys to be compared

        Returns:	-1, 0, or 1 as described above

EXTERNAL DATA REFERENCES:

        none

FUNCTIONS CALLED:

        none

CALLED BY:

        enterht

*FE********************************************************************/

int
lcsecmp( key1, key2 )

KEY *key1;
KEY *key2;
{
   if (key1->op == key2->op 
       && key1->type == key2->type 
       && key1->left == key2->left 
       && key1->right == key2->right)
       return(0);
   else if (key1->op < key2->op)
       return(-1);
   else
       return(1);
}
