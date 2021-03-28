#ident	"@(#)write.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	write.c	7.1		*/
/*	Copyright 1985 Motorola Inc.	*/

# include "declar.h"

/*F*********************** FUNCTION HEADER *******************************

 writefunc

	This function writes out the ctrees for all of the basic blocks.

PARAMETERS:

	Receives: nothing

	Returns: nothing

EXTERNAL DATA REFERENCES:

	basic block flow graph

FUNCTIONS CALLED:

	writebb

CALLED BY:

	talloc
	writefunc

*FE********************************************************************/

void
writefunc()
{

   BBPTR bb;

   for( bb = fgroot; bb; bb = bb->next )
      {
#ifndef NODBG
      if (bbflag) printf("\nBASIC BLOCK #%d\n\n", bb->id);
#endif
      writebb( bb );
      }
}


/* writebb
	This function writes out the ctree for one basic block.
*/

void 
writebb( bb )

BBPTR bb;
{

   NODE *p;

   for( p = bb->codetree; p ; p = p->stmt.succ)
      {
      prtree(p);/* write the nodes in this expression */
      tfree(p);	/* free the nodes in this expression */
      }
}


/*F*********************** FUNCTION HEADER *******************************

 prtree

	Print out the Ctrees for one expression.

PARAMETERS:

	Receives: p: pointer to root of tree

	Returns: nothing

EXTERNAL DATA REFERENCES:

	nodesout

FUNCTIONS CALLED:

	prtree

CALLED BY:

	prtree
	writebb

*FE********************************************************************/

void
prtree(p) 

register NODE *p; 
{
	register ty;
	int autooffset;

   if (p)
	{
	nodesout += 1;
	switch (p->opn.op) {
	case EXPRNO: 
		printf("\.%d\t\"\"\n",p->tn.rval);
		prtree(p->in.left);
		break;
	case SBLOCK:
		if (p->opn.csiz > maxautooffset)
		   maxautooffset = p->opn.csiz;
		/* only use absmaxoffset if frame compression code executed */
		if (!clseonly)
			autooffset = -absmaxoffset*SZCHAR;	/* use max compressed size */
		else
			autooffset = maxautooffset;	/* use max size directly from c0 */
		printf("[%d\t%d\t%d\t\n",p->tn.lval, autooffset, p->tn.rval);
		break;
	case EBLOCK:
		printf("]%d\t%d\t\n",p->tn.lval, p->tn.rval);
		break;
	case DEFTYP:
/*		printf("%s\n",p->stmt.astring); 	** bk: breaks M4.1 optim **/
		free(p->stmt.astring);
		break;
#ifdef ASM_COMMENT
	case ASMNO:
		printf("%s\n",ASM_COMMENT);
		printf("%s\n",p->stmt.astring);
		free(p->stmt.astring);
#ifdef ASM_END
		printf("%s\n",ASM_END);
#endif
		break;
#endif
	default:
		ty = optype(p->opn.op);
		printf( "%d\t", p->opn.op );
	
		if( ty == LTYPE ) {
			if (p->opn.op == FCON)
			   printf("0\t");
			else
			   printf( "%ld\t", p->tn.lval );
			}
		if( ty != BITYPE ) {
			if( p->opn.op == NAME || p->opn.op == ICON 
			    || p->opn.op == FCON) printf( "0\t" );
			else printf( "%ld\t", p->tn.rval );
			}
	
		printf( "%o\t", p->opn.type );
	
		/* handle special cases */
	
		switch( p->opn.op ){
	
		case VAUTO:				/* bk: print type qualifier info */
		case VPARAM:
       		printf( "%o\t\n", p->in.typeq );
			break;

		case NAME:
		case ICON:
			/* print external name if it exists */
			if (p->opn.name)
			   printf("%s\n", p->opn.name);
			else
			   printf("\n");
			break;
	
		case STARG:
		case STASG:
		case STCALL:
		case UNARY STCALL:
			/* print out size */
			printf( CONFMT, p->stn.stsize);
			printf( "\t%d\t\n", p->stn.stalign);
			break;
#ifdef M68881
		case FCON:
			if(p->fp.type == DOUBLE )
			{
				printf( "%ld\t%ld\t\n", 
					((FLTDBL *) &p->fp.dval)->lg.l, 
					((FLTDBL *) &p->fp.dval)->lg.l2); 
			}
			else
			{
				printf( "%ld\t\n", 
					((FLTDBL *) &p->fp.dval)->lg.l); 
			}
			break;
#endif
	
		default:
			printf(  "\n" );
			}
	
		if( ty != LTYPE ) prtree( p->in.left );
		if( ty == BITYPE ) prtree( p->in.right );

	}
	}
}


/*F*********************** FUNCTION HEADER *******************************

 cerror

	Routine for handling fatal optimizer errors.

PARAMETERS:

	Receives: s: format of message
		  a,b,c: values of variables to be displayed

	Returns: nothing

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	none

CALLED BY:

	extensively

*FE********************************************************************/

/* VARARGS1 */
void
cerror( s, a, b, c)

char *s;
{
   fprintf(stderr, "High Level Optimizer Error: \n");
   fprintf(stderr, s, a, b, c );
   fprintf(stderr, "\n");
   fflush( stdout );
   if (statsflag)
      {
      fprintf(statsfile," Error occurred while processing %s,\n nodes sucessfully read = %d\n", funcname, nodesin);
      fflush(statsfile);
      }
   exit(1);
}


/*F*********************** FUNCTION HEADER *******************************

 writestats

	This function writes the statistics for the current function
	out to the statistics file.

PARAMETERS:

	Receives: nothing

	Returns: nothing

EXTERNAL DATA REFERENCES:

	funcname
	nodesin
	nodesout
	bbcnt
	aregstat
	dregstat
	fpregstat
	symtabndx
	lcsestat
	valueno
	scopestat
	tempstat
	loopstat
	statsflag

FUNCTIONS CALLED:

	none

CALLED BY:

	main

*FE********************************************************************/

void
writestats()
{

   if (statsflag)
      {
      fprintf(statsfile,"\t%-12s%-6d%-6d%-6d%-6d%-6d%-6d%-6d%-6d%-6d%-6d%-6d%-6d\n", 
	      funcname, nodesin, nodesout, bbcnt, aregstat, dregstat, fpregstat,
	      symtabndx, lcsestat, valueno, scopestat, tempstat, loopstat);
      }
}

#ifndef NODBG
/*F*********************** FUNCTION HEADER *******************************

 prflowgraph

	This function's sole purpose is for debugging. It prints out each basic 
	block's number followed by its predecessors and successors.

PARAMETERS:

	Receives: nothing

	Returns: nothing

EXTERNAL DATA REFERENCES:

	basic block flow graph

FUNCTIONS CALLED:

	none

CALLED BY:

	main

*FE********************************************************************/

void
prflowgraph()

{
   BBPTR bb;
   BBLINK *p;

   printf("\nFLOW GRAPH FOR FUNCTION\n\n");
   for( bb = fgroot; bb; bb = bb->next )
      {
      printf("%d:\tPredecessors\t", bb->id);
      for ( p = bb->preds; p ; p = p->next)
	 printf("%d\t", p->bbptr->id);
      printf("\n\tSuccessors  \t");
      for ( p = bb->succs; p ; p = p->next)
	 printf("%d\t", p->bbptr->id);
      printf("\n");
      }
}
#endif
