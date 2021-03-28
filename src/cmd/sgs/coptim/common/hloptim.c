#ident	"@(#)hloptim.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)hloptim.c   7.1		*/
/*	Copyright 1985 Motorola Inc.	*/

#include "declar.h"

static  int dfnndx;     /* index into depth first number array as it is
			** being built */

/*F*********************** FUNCTION HEADER *******************************

 optimize

	Controlling function for the optimizing routines. Function
	has been read in, basic blocks have been formed, symbol
	table has been created, local common subexpressions have
	been handled, basic block flow graph is built, and now
	it's time to get to work.

PARAMETERS:

	Receives:	none

	Returns:	none

EXTERNAL DATA REFERENCES:

	symtab
	regctrl

FUNCTIONS CALLED:

	dfncompute
	detectloops
	asgregvars

CALLED BY:

	main

*FE********************************************************************/

void
optimize()
{
   if (symtab)
      {
      dfncompute();     /* compute depth first numbers for the flow graph */
      detectloops();    /* detect loops in the flow graph and assign a
                        ** a weight to each basic block */
      if (regctrl)
         asgregvars();  /* perform static analysis on simple variables and
                        ** place the most deserving variables in registers */
      }
}


/*F*********************** FUNCTION HEADER *******************************

 dfncompute

	Creates and initializes the depth first array.  The depth
	first numbers are important for the efficiency of several
	data flow algorithms.

PARAMETERS:

	Receives:	nothing

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	dfn
	bbflag

FUNCTIONS CALLED:

	dfns

CALLED BY:

	optimize

*FE********************************************************************/

void
dfncompute()

{
   dfnndx = bbcnt - 1; /* used in dfns */
   dfn = (BBPTR*) calloc(bbcnt, sizeof(BBPTR));
   dfns(fgroot);
#ifndef NODBG
   if (bbflag)
      {
      int i;

      printf("\n\nDEPTH FIRST ORDERING\n\n");
      for(i = 0; i < bbcnt; ++i)
         if (dfn[i]) printf("%d:  %d\n", i, dfn[i]->id);
      }
#endif
}


/*F*********************** FUNCTION HEADER *******************************

 dfns

	This function determines the depth first number for a basic block.
	Algorithm is taken from Aho and Ullman's Dragon book.

PARAMETERS:

	Receives:	bb: basic block of concern

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	dfnndx
	dfn

FUNCTIONS CALLED:

	dfns

CALLED BY:

	dfncompute

*FE********************************************************************/

void
dfns( bb )

BBPTR bb;

{
   BBLINK *p;

   bb->dfn = -1;        /* mark bb visited */
   for (p = bb->succs; p ; p = p->next)
      if (p->bbptr->dfn == 0)   /* if successor hasn't been visited */
         dfns(p->bbptr);
   dfn[dfnndx] = bb;
   bb->dfn = dfnndx;
   dfnndx -= 1;
}


/*F*********************** FUNCTION HEADER *******************************

 detectloops

	This function analyzes the depth first numbers of the basic
	blocks in order to recognize a back edge of the flow graph
	which indicates the presence of a loop.

        Traverse flow graph looking for back edges which indicate
        loops (in most cases).

PARAMETERS:

	Receives:	nothing

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	loopstat
	basic block flow graph
	bbflag

FUNCTIONS CALLED:

	findnaturalloop

CALLED BY:

	optimize

*FE********************************************************************/

void
detectloops()

{
   BBPTR   bb;
   BBLINK *p;
   SET    *loop;
   int	   i;


   for (bb = fgroot->next; bb; bb = bb->next)   
      for (p = bb->succs; p; p = p->next)       /* linked list of possible
                                                ** successors for this bb. */
         if (bb->dfn >= p->bbptr->dfn)
            {   /* back edge of a loop found */
            loopstat++;         /* statistics counter */
            loop = findnaturalloop(bb, p->bbptr); 
            FREESET(loop);  
            /* NOTE: This call to FREESET is temporary.  "loop" will be 
            ** needed later for dominators.
            */
            }
/*
 *	Change weights greater than one to 2^(weight). This is to increase
 *	the probability of inner loop variables being promoted.
 */
    for (i=0; i<bbcnt; ++i)
        if ( dfn[i] && dfn[i]->weight > 1 )
	     dfn[i]->weight = 1 << dfn[i]->weight;
#ifndef NODBG
    if (bbflag)
       {        /* Dump basic blocks */
       printf("\n\nDEPTH FIRST ORDERING    WEIGHT\n\n");
       for (i=0; i<bbcnt; ++i)
          if (dfn[i]) printf("%d:\t%d\t\t%d\n",i,dfn[i]->id,dfn[i]->weight);
       }
#endif
}


/*F*********************** FUNCTION HEADER *******************************

 findnaturalloop

	This function controls the creation of the loop identified
	by the backedge head->tail.

	The natural loop of the edge (head->tail) is found by identifying
	those nodes that reach "head" without going through "tail". Those
	nodes plus node "tail" form the loop. "tail" is the header of
	the loop.

			head ----------> tail

PARAMETERS:

	Receives:	head: head of the back edge
			tail: tail of the back edge (header of the loop)

	Returns:	a pointer to the set indicating which nodes are
			   part of the loop

EXTERNAL DATA REFERENCES:

        none

FUNCTIONS CALLED:

	CREATESET
	insertloop

CALLED BY:

	detectloops

*FE********************************************************************/

SET *
findnaturalloop(head, tail)
                    
BBPTR head; /* the head of the back edge */
BBPTR tail; /* the tail of the back edge, the header of the loop */
{  
   SET *loop;

   loop = CREATESET( bbcnt );
   SET_BIT(tail->dfn, loop);    /* include the header of the loop */
   tail->weight += 1;           /* increment the depth of the node */
   insertloop( head, loop );    /* look at head and predecessors to it */
   return(loop);
}                                 


/*F*********************** FUNCTION HEADER *******************************

 insertloop

	This function adds "bb" to "loop", and then adds "bb"s
 	predecessors to "loop" if they are not already added.

PARAMETERS:

	Receives:	bb: current basic block in flow graph
			loop: loop being indentified

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	basic block flow graph

FUNCTIONS CALLED:

	insertloop

CALLED BY:

	findnaturalloop

*FE********************************************************************/

void
insertloop( bb , loop)

BBPTR bb;                
SET *loop;
{
   BBLINK *p;
                                      
   SET_BIT(bb->dfn, loop);
   bb->weight += 1;  /* increment the depth of the node */
   for (p = bb->preds; p; p = p->next)
      if ( ! IS_IN(p->bbptr->dfn, loop) )            
         insertloop( p->bbptr, loop );
}
