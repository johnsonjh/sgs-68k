#ident	"@(#)bb.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)bb.c	7.3		*/
/*	Copyright 1985 Motorola Inc.	*/

#include	"declar.h"

/* Prototypes for static functions */
static void  freescope _PARMS((struct Tlevel *));
static void  announcelabel _PARMS((struct Tbasicblock *, int ));
static void  resolvebr _PARMS((struct Tbasicblock *, int ));


static	NODE *codetail;	/* pointer to the tail of the list of expressions
			** which will make up the next basic block
			*/
static	int exprno;	/* the line number currently being processed */
static	int lastinstr;	/* retains the last instruction's opcode */ 
static  int fallthrough; /* flag indicating if current basic block falls
                         ** through to next basic block (e.g. no GENUBR) */
static	int leader;	/* flag indicating that the next expression 
			** will be a leader of a basic block */


/*F*********************** FUNCTION HEADER *******************************

 recognizebb

	This function recognizes the beginning or end of a basic block,
	and controls the building of the basic block flow graph.  
	The beginning of a basic block triggers the handling of the local 
	common subexpressions for the previous block. 
	
	Branches, labels, beginning of scopes (SBLOCKs), end of scopes 
	(EBLOCKS), and switch statements indicate a change in the
	flow graph.

	All nodes pass through this function and are attached to a basic block.

PARAMETERS:

	Receives:	p: the current node

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	leader
	fallthrough
	lastbb
	currentbb
	lastinstr
	lcsectrl

FUNCTIONS CALLED:

	lcse
	linkbbs
	announcelabel
	resolvebr
	addnodetobb

CALLED BY:

	linearize:	normal entry
	gather:		for SBLOCK and EBLOCK

*FE********************************************************************/

void
recognizebb( p )

NODE *p;

{
   /* Is "p" the leader of a basic block? Leaders followed by EBLOCKS, and
      SBLOCKS followed by GENLABs are joined together to make fewer basic
      blocks, and guarantee that EBLOCKS and SBLOCKS will have predecessors
      in the flow graph.
   */

   if ((leader && p->tn.op != EBLOCK) 
       || (p->tn.op == GENLAB && lastinstr != SBLOCK ) 
       || p->tn.op == SBLOCK)	
      {
      BBPTR lastbb;

      if (! leader)

	 if (lcsectrl)
			  /* perform local commom subexpression elimination for
			  ** current basic block and clean up supporting
			  ** data structures */
	    lcse(currentbb); 

      lastbb = currentbb;
      currentbb = createbb();
      lastbb->next = currentbb;
      if (fallthrough)
         linkbbs(lastbb, currentbb);
      fallthrough = TRUE;	/* assume this bb won't end in a GENUBR */
      leader = FALSE;		/* assume this expr doesn't end a bb */
      }

   lastinstr = p->tn.op;

   switch (p->tn.op) 
      {
      case GENLAB:	/* start a new basic block */
	 if (! clseonly)
	    announcelabel(currentbb, p->tn.rval);
	 break;

      case SBLOCK:	/* start a new basic block */
	 break;

      case EBLOCK:	/* end of a basic block */
         leader = TRUE;
	 break;

      case GENUBR:	/* end of a basic block */
	 resolvebr(currentbb, p->tn.rval);
         fallthrough = FALSE;  /* the next bb is not a successor */
         leader = TRUE;
	 break;

      case CBRANCH:	/* end of a basic block */
/*
 *   note that the label number for a CBRANCH is contained in an
 *   ICON node on the right of CBRANCH.
 */
	 resolvebr(currentbb, p->in.right->tn.lval);
         leader = TRUE;
	 break;

      case SWBEG:	/* end of a basic block */
	 {
	 NODE *q;

	 fallthrough = FALSE;

	 /* process the SWDEF and SWCASE labels */
	 for (q = p->in.left; q->in.op != SWEND ; q = q->in.left )
	    resolvebr( currentbb, q->tn.rval );
	 leader = TRUE;
	 break;
	 }

      default:
	 break;
      }

   addnodetobb(p);

   if ( leader && lcsectrl )
      lcse( currentbb );
}


/*F*********************** FUNCTION HEADER *******************************

 createbb

	This function creates a basic block node and initializes it.

PARAMETERS:

	Receives:	nothing

	Returns:	a pointer to the new initialized basic block 

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	malloc

CALLED BY:

	initfunc
	recognizebb

*FE********************************************************************/

BBPTR
createbb()
{

   BBPTR p;

   p = (BBPTR) malloc(sizeof(BB));
   if (p == NULL)
      cerror("internal error - not enough memory: in createbb");
   bbcnt += 1;
   p->codetree = NULL;
   p->succs = NULL;
   p->preds = NULL;
   p->next = NULL;
   p->dom = NULL;
   p->vnaptr = NULL;
   p->header = NULL;
   p->dfn = 0;
   p->flags = 0;
   p->weight = 1;
   p->id = bbcnt;
   return(p);
}



/*F*********************** FUNCTION HEADER *******************************

 addnodetobb

	This function adds the node "p" to the list of nodes for the
	current basic block.  The linking node is an EXPRNO, SBLOCK,
	ASMNO, or EBLOCK node which points at its predecessor and
	successor as well as the expression itself (EXPRNO only).

PARAMETERS:

	Receives:	p: pointer to node being added

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	codetail
	exprno

FUNCTIONS CALLED:

	talloc

CALLED BY:

	recognizebb

*FE********************************************************************/

void
addnodetobb(p)

NODE *p;

{
   NODE *q;

   switch (p->tn.op)
      {
   case SBLOCK:
   case EBLOCK:
   case ASMNO:
   case	DEFTYP:
      if (currentbb->codetree == NULL)
	 {
	 currentbb->codetree = p;
	 p->stmt.pred = NULL;
	 }
      else
	 {
	 codetail->stmt.succ = p;
	 p->stmt.pred = codetail;
	 }
      p->stmt.succ = NULL;
      codetail = p;
      break;

   default:
      q = talloc();		/* create the EXPRNO */
      p->opn.parent = q;	/* link p with it's parent */
      if (currentbb->codetree == NULL)
         {
         currentbb->codetree = q;
         q->stmt.pred = NULL;
         }
      else
         {
         codetail->stmt.succ = q;
         q->stmt.pred = codetail;
         }
      q->stmt.op = EXPRNO;
      q->stmt.newflag = 0;
      q->stmt.bb = NULL;
      q->stmt.succ = NULL;
      q->stmt.left = p;
      q->stmt.rval = exprno;
      codetail = q;
      break;
      }
}


/*F*********************** FUNCTION HEADER *******************************

 lvalue

	This function determines for an assignment node whether the
	left side is appropriate for LCSE substutution. This is
	necessary because the front-end may leave us with something
	that appears to be a valid expression but which in reality
	will be done away with when rewritten in the back-end before
	code generation.

	In particular, the tree:   =    has the conversion node removed
	in the back-end, yet     /   \     this appears to be a valid
	conversion node        CONV  ...     at this point in time.
			       /
			     AUTO

PARAMETERS:

	Receives:	p:	left node of an assignment op

	Returns:	an integer set to TRUE or FALSE

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	none

CALLED BY:

	parentlink

*FE********************************************************************/

int
lvalue(p)

NODE *p;

{
     switch(p->opn.op)
	{
	case STAR:
	case VAUTO:
	case VPARAM:
	case REG:
	case TEMP:
	    return(TRUE);
	    break;
	default:
	    return(FALSE);
	    break;
	}
}



/*F*********************** FUNCTION HEADER *******************************

 parentlink

	This function has the following responsibilities:
	1) walks the expression tree 'p' and links 'p' with its parent.
	2) initiates the computation of the node's value number 
	3) All value numbers in the symbol table are flushed when
	   function calls are processed (call to flushstvn).  This protects
	   against aliased variables.

	The function is recursive.


PARAMETERS:

	Receives:

   	  p:		root of an expression tree
	  parent: 	parent of "p"
	  allowable:	= FALSE if ?:, &&, or || operators encountered. 
			When these operators are encountered expressions will
			not be entered into the hash table since each of these
			operators introduces more than one basic block. 
			Assignment statements in these expressions will be 
			handled normally.

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	odebug
	allowable

FUNCTIONS CALLED:

	addvalueno
	classify
	flushstvn
	lvalue
	optype
	parentlink

CALLED BY:

	linearize

*FE********************************************************************/

void
parentlink(p, parent, allowable)

NODE	*p, *parent;
int	allowable;

{
	register	ty;

#ifndef NODBG
	if (odebug > 2) 
	   printf("parentlink( (node %d): op = %d (%s) , allowable %s)\n",
		(int)(p-node), p->tn.op, opst[p->tn.op], 
		allowable ? "TRUE" : "FALSE" );
#endif
	p->opn.parent = parent;	/* make the link to the node's parent */
	ty = optype(p->tn.op);
	if (ty == BITYPE)
	   {
	   switch (p->tn.op)
	      {
	      /* subexpressions/substatements of these operators will 
	         not be entered into the expression hash table. Note
		 that once the operator has been handled "allowable" returns
		 to its parent's value */
	      case QUEST:
	      case OROR:
	      case ANDAND:
		 allowable = FALSE;
		 break;
	      default:
		 break;
	      } 
	   if ( asgop(p->opn.op) && (lvalue(p->in.left) == FALSE) )
		parentlink(p->in.left, p, FALSE);
	   else parentlink(p->in.left, p, allowable);
	   parentlink(p->in.right, p, allowable);
	   }
	else if (ty == UTYPE)
	   {
		/* Special stuff.  In the case where the tree looks like
		 * 
		 *                  \
		 *                 conv  <-
		 *                 /
		 *                =
		 *               / \
		 *            name icon
		 *
		 * We need to disallow lcs replacement
		 * Switch allowable for the call below to addvalueno
		*/
		if ( ( p->tn.op == CONV ) && ( asgop( p->in.left->opn.op ) &&
			(lvalue(p->in.left->in.left) == FALSE) ) )
		    parentlink(p->in.left, p, allowable = FALSE );
		else
		    parentlink(p->in.left,p,allowable);
	   } 
	

	/* process current node */

	switch (p->tn.op)
	   {
           case VAUTO:
           case VPARAM:
              classify(p);  /* determine if the variable is simple */
	      
	      /* If the variable is simple and the address of the variable
	      ** is taken, then the variable is not eligible for a register
	      ** due to the possibility of aliasing. */
	      if ((p->opn.flags & SIMPLE) && parent && 
			parent->tn.op == (UNARY AND))
		 (symtab + p->opn.sourceno)->flags |= ADDRESSTAKEN;
	      break;
	   case CALL:
#ifdef IN_LINE
	   case INCALL:
	   case UNARY INCALL:
#endif
	   case STCALL:
	   case UNARY CALL:
	   case UNARY STCALL:
		leaffunc = FALSE;		/* indicate this is not a leaf function */
		/* On function calls, invalidate variable value numbers 
		** in the symbol table.  Easy to guard against 
		** modification of aliased variables.
		*/
		flushstvn();
		break;
	   }
          if (lcsectrl) 
	     addvalueno(p, allowable);
}


/*F*********************** FUNCTION HEADER *******************************

 linearize

	Expression trees are traversed linking a node with its parent. Trees
	containing ?:, &&, or || operations cause the lcse recognized before
	this expression to be processed. The lcse hash structure is then
	flushed. The expression tree is then added to a basic block.

	Only EXPRNO trees are passed to this function.

PARAMETERS:

	Receives:	p: pointer to an expression node

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	exprno: set by this function

FUNCTIONS CALLED:

	parentlink
	recognizebb

CALLED BY:

	gather

*FE********************************************************************/

void
linearize (p)

NODE *p;

{

	exprno = p->tn.rval;
	p->opn.op = FREE;     /* free up the current EXPRNO node */
	parentlink(p->in.left, NULL, TRUE);
	recognizebb(p->in.left);
}

/*F*********************** FUNCTION HEADER *******************************

 initfunc

	This function is called before the processing of a function.
	It is responsible for initializing the data structures associated
	with the data flow analysis of the function.

PARAMETERS:

	Receives:	nothing

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	absmaxoffset
	cursylist
	tempstat
	aregstat
	fpregstat
	dregstat
	nodesout
	nodesin
	bbcnt
	loopstat
	lcsestat
	scopestat
	fgroot
	lastinstr
	currentbb
	clseonly
	leader
	fallthrough
	valueno
	maxautooffset
	lcsectrl
	regctrl

FUNCTIONS CALLED:

	cleanupht
	freescope
	scoperoot
	curscope
	free
	freebb
	createbb

CALLED BY:

	main

*FE********************************************************************/

void
initfunc()
{
   BBPTR bb;

   /* CLEANUP FROM PREVIOUS FUNCTIONS */

   tinit();			/* Free all the expression nodes */
   cleanupht( &labelsht, freebblink );
   cleanupht( &constht, NULL );
   freescope(scoperoot);	/* return the scope tree */
   scoperoot = NULL;
   curscope = NULL;
   if (dfn != NULL)		/* free the depth first number array */
      {
      free(dfn);
      dfn = NULL;
      }
   if (symtab != NULL)		/* free up the symbol table */
      {
      free(symtab);
      symtab = NULL;
      symtabndx = 1;		/* next symbol table entry # (the first) */
      }
   for (bb = fgroot; bb; bb = bb->next)	/* return the flow graph */
      freebb( bb );

   /* initialize various counters */

	  {	extern absmaxoffset;

		absmaxoffset = 0;
		}

   cursymlist = 0;
   tempstat = 0;
   aregstat = 0;
   dregstat = 0;
   fpregstat = 0;
   nodesout = 0;
   nodesin = 0;
   bbcnt = 0;
   loopstat = 0;
   lcsestat = 0;
   scopestat = 0;

   fgroot = createbb();   /* sentinel for flow graph */
   lastinstr = FREE;
   currentbb = fgroot;
   clseonly = FALSE;
   leader = TRUE;
	leaffunc = TRUE;		/* default to leaf function */
   fallthrough = TRUE;
   valueno = 0;
   maxautooffset = 0;

   lcsectrl = lcseflag;   /* initialize control flags with command line flags */
   regctrl = regflag;
}


/*F*********************** FUNCTION HEADER *******************************

 freescope

	This function returns the memory allocated for this basic block.

PARAMETERS:

	Receives:	bb: pointer to basic block

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	freescope
	free

CALLED BY:

	initfunc

*FE********************************************************************/

static void
freescope( p )

SCOPE *p;
{
   if (p)
      {
      freescope(p->lower);
      freescope(p->same);
      free( p );
      }
}

/*F*********************** FUNCTION HEADER *******************************

 freebb

	Frees links to successors and predecessors in basic block flow 
	graph.

PARAMETERS:

	Receives:	bb: pointer to current basic block

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	basic block flow graph

FUNCTIONS CALLED:

	freeblink
	free

CALLED BY:

	initfunc
	talloc

*FE********************************************************************/

void
freebb( bb )

BBPTR bb;
{

   /* free links to successors and predecessors */

   freebblink( bb->succs );
   freebblink( bb->preds );
   free( bb );

}


/*F*********************** FUNCTION HEADER *******************************

 freebblink

	This function frees up the nodes in a BBLINK list. These nodes
	connect basic block nodes with each other.

PARAMETERS:

	Receives:	p: pointer to current BBLINK

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	free

CALLED BY:

	freebb

*FE********************************************************************/

void
freebblink( p )

BBLINK *p;

{
   while (p)
      {
      free(p);
      p = p->next;
      }
}


/*F*********************** FUNCTION HEADER *******************************

 announcelabel

	Used in building the basic block flow graph.
	This function announces that 'label' has been seen, and
	satisfies all the outstanding references to it by linking
	basic block nodes. 'label' is added to the known labels.

PARAMETERS:

	Receives:	bb: current basic block
			label: the label encountered

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	basic block flow graph

FUNCTIONS CALLED:

	malloc
	hsearch
	free
	linkbbs

CALLED BY:

	recognizebb


*FE********************************************************************/

static void
announcelabel( bb,  label )

BBPTR bb;
int label;

{
   ENTRY new;
   ENTRY *res;  /* result of hsearch */
   BBLINK *p;
   char labelstr[12];

   /* build key for hash table */
   sprintf(labelstr, "%-d", label);
   new.key = malloc(strlen(labelstr) + 1);
   strcpy( new.key, labelstr );
   new.data = NULL;

   res = hsearch(&labelsht, hashd, strcmp, new, ENTER); 
   p = (BBLINK *) res->data;
   
   /* resolve previous branches to this label, if any */

   if ( p == NULL) 
      {	/* this is the first time this label has been seen */

      res->data = malloc(sizeof(BBLINK));
      p = (BBLINK *) res->data;

      }

   else	/* previous references to this label existed - resolve them */
      {
      BBLINK *q;

      free(new.key);
      for( q = p->next; q; q = q->next)
         {
         linkbbs(q->bbptr, bb);
         free(q);
         }
      }

   p->bbptr = bb;
   p->next = NULL;
}


/*F*********************** FUNCTION HEADER *******************************

 resolvebr

	Used in building the basic block flow graph.
	A branch has been encountered. See if it has already been
	announced. If so create the link between the two basic blocks.
	If the label is not a known label, then enter the label 
	as a unknown label. If it is a known label, then link the 
	two basic blocks.

PARAMETERS:

	Receives:	bb: current basic block
			label: label encountered

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	basic block flow graph

FUNCTIONS CALLED:

	hsearch
	linkbbs
	malloc

CALLED BY:

	recognizebb

*FE********************************************************************/

static void
resolvebr( bb, label)

BBPTR bb;
int label;
{
   ENTRY new;
   ENTRY *dest;
   char labelstr[12];
   BBLINK *p, *q;

   if (clseonly) return;

   sprintf(labelstr, "%-d", label);
   new.key = malloc(strlen(labelstr) + 1);
   strcpy( new.key, labelstr );
   new.data = NULL;

   dest = hsearch(&labelsht, hashd, strcmp, new, ENTER);
   p = (BBLINK *) dest->data;
   if (p)
      { /* label already exists */
      free(new.key);
      if (p->bbptr)
	 {	/* label has been announced */
         linkbbs(bb, p->bbptr);
	 return;
	 }
      }
   else
      { /* this is the first time the label has been seen */

      dest->data = malloc(sizeof(BBLINK));
      p = (BBLINK *) dest->data;
      p->bbptr = NULL;
      p->next = NULL;
      }

   /* add a new reference to the label - insert at front of list */
   q = (BBLINK *) malloc(sizeof(BBLINK));
   q->bbptr = bb;
   q->next = p->next;
   p->next = q;
}


/*F*********************** FUNCTION HEADER *******************************

 linkbbs

	Used in creating the basic block flow graph.
	This function links two basic blocks by adding a successor or
	a predecessor link to the appropriate basic block.

PARAMETERS:

	Receives: from: basic block "to" is a successor of this basic block
		  to: basic block "from" is a predecessor of this basic block

	Returns: nothing

EXTERNAL DATA REFERENCES:

	clseonly

FUNCTIONS CALLED:

	malloc

CALLED BY:

	recognizebb
	announcelabel
	resolvebr

*FE********************************************************************/

void
linkbbs(from, to)

BBPTR from;
BBPTR to;
{
   BBLINK *p;

   if (clseonly) return;

   /* add a successor to the beginning of the list of successors */
   p = (BBLINK *) malloc(sizeof(BBLINK));
   p->next = from->succs;
   from->succs = p;
   p->bbptr = to;

   /* add a predecessor to the beginning of the list of predecessors */
   p = (BBLINK *) malloc(sizeof(BBLINK));
   p->next = to->preds;
   to->preds = p;
   p->bbptr = from;
}
