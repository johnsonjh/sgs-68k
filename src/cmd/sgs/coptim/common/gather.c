#ident	"@(#)gather.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)gather.c	7.1		*/
/*	Copyright 1985 Motorola Inc.	*/

#include "declar.h"

/* Prototypes for static functions */
static void  dataread _PARMS((void));

#define DATA 0
#define TEXT 1

static int segment = TEXT;	/* designates if the label is in TEXT or DATA */
static void dataread();
static void getlabtab();
static char buff[257];


/*F*********************** FUNCTION HEADER *******************************

 gatherfunc

	This function reads in the ctrees for a function and calls
	the appropriate functions to handle the following items:
	symbol table information, beginning of blocks, end of blocks,
	and expression trees.

PARAMETERS:

	Receives:	nothing

	Returns:	last character read - which should normally be 0
			    for end of file

EXTERNAL DATA REFERENCES:

	statsfile
	nodesin
	segment

FUNCTIONS CALLED:

	rdsymtab
	rdin
	enterscope
	recognizebb
	talloc
	exitscope
	dataread
	linearize
	eread

CALLED BY:

	main

*FE********************************************************************/

char
gatherfunc()
{

   register c, count=0;
   register char *cp;
   register NODE *p;
   char dupbuff[257];
   
   p = NULL;		/* default no block in process */
   do
   {
      if (count++,(c = getchar()) <= 0)
	 c = 0;
      switch( c ) 
	 {
      case 0:		/* end of file */
         break;
      default:
         {
         char *ptr;
         
	 /* read in a line; check to see if it is related to the symbol
	 ** table or an in-line assembler statement; if not then write
	 ** it out, otherwise creat an ASM node or enter the information
	 ** into the symbol table */
         
         buff[0] = c;
         gets(&buff[1]);
         strcpy(dupbuff, buff);
         ptr = strtok(buff," \t\n"); /* get first token */
         if (strcmp(ptr,"def") == 0 )
            {
	    rdsymtab(dupbuff);	/* read symbol table */
            }
	 else if (strcmp(ptr,"data") == 0)	/* switch to data segment */
	    {
	    segment = DATA;
            printf("%s\n",dupbuff);
	    }
	 else if (strcmp(ptr,"text") == 0)	/* switch to text segment */
	    {
		/* bk: catch "text 2" and "text 3", ie readonly data */

	        segment = TEXT;

		/* get next token */
         	if ( ptr = strtok(NULL," \t\n") )
			if ( (strcmp( ptr, "3" ) == 0) || 
				(strcmp( ptr, "2" ) == 0) )
				segment = DATA;
		printf("%s\n",dupbuff);
	    }
         else if (strcmp(ptr,"ln") == 0)
            ; /* these lines are dropped */
#ifdef ASM_COMMENT
	 else if (segment == TEXT && strcmp(ptr,ASM_COMMENT)==0)
	 {   /* in-line asm statement; store as an ASM node */
#ifdef ASM_END
	 for(gets(buff);strncmp(buff,ASM_END,strlen(buff)-1) != 0;gets(buff))
#endif
	    {
		nodesin += 1;
		p = talloc();
		p->stmt.op = ASMNO;
		p->stmt.astring = malloc(strlen(buff)+1);
		strcpy(p->stmt.astring, buff);
		recognizebb(p);
		}
	    break;
	    }  /* end in-line handling */
#endif
         else
            {
            if ((statsflag) && (strcmp(ptr, "file") == 0))
                  /* name of file being read */
                  fprintf(statsfile, "%s\n", strtok(NULL," \t\n")); 
            printf("%s\n",dupbuff);
            }
         continue;
         }
      
      case BBEG:
      {
         register myftn, aoff, regvar;
      
         /* beginning of a block */
	 nodesin += 1;
         myftn = rdin(10);  /* ftnno */

         /* autooff for block gives max offset of autos in block */
         aoff = (unsigned int) rdin(10); 
	 SETOFF( aoff, ALSTACK );
         regvar = rdin(10);
         if( getchar() != '\n' ) cerror( "intermediate file format error");
      
         p = talloc();
         p->opn.op = SBLOCK;
         p->tn.lval = myftn;	/* current function number */
         p->tn.rval = regvar;	/* register mask */
         p->opn.csiz = aoff;	/* max offset */
         
         enterscope(p);		/* set up a new block environment */
	 recognizebb(p);
         break;
         } /* end the BBEG block */
      
      
      case BEND:  /* end of block */
         {
         int level, regvar;
         
	 nodesin += 1;
         level = rdin (10);
         regvar = rdin (10);
	 while( (c=getchar()) != '\n')
	    {
	    if ( c <= 0 ) cerror("intermediate file format eof");
	    }
         
         p = talloc();
         p->opn.op = EBLOCK;
         p->tn.lval = level;	/* level of block */
         p->tn.rval = regvar;	/* register mask */
         
	 recognizebb(p);
         exitscope(p);		/* return to scope of parent */
         break;
         }
      
      case EXPR:	
	 if (segment == DATA)
	    {
	    /* read DATA expression and write it out - do not buffer */
   	    buff[0] = '.';
   	    gets(&buff[1]);	/* read rest of line number */
            printf("%s\n",buff);
	    dataread();
	    }
	 else
	    {
	    /* read code for an expression */
	    nodesin += 1;
	 
	    p = talloc();
	    p->stmt.newflag = 0;
	    p->opn.op = EXPRNO;
	    p->tn.rval = rdin( 10 );
	    for( cp=ftitle; (*cp=getchar()) != '\n'; ++cp ) ; /* VOID, reads filename */
	    *cp = '\0';
	    
	    p->in.left = eread();
	    
	    linearize( p );
	    break;
	    }
      
      }  /* end of switch */

   
   }  /* end of do-while */
   while ( (c > 0) && ! (p != NULL && p->opn.op == EBLOCK && p->tn.lval == 0));
   return(c);
}


/*F*********************** FUNCTION HEADER *******************************

 talloc

	This function allocates Ctree nodes from an array. If no more
	FREE nodes exist then the current basic blocks are written out,
	optimization is terminated and the remainder of the function
	is passed through. 

	If a single expression exceeds TREESZ then the optimizer exits 
	with an error.

PARAMETERS:

	Receives:	nothing

	Returns:	pointer to node

EXTERNAL DATA REFERENCES:

	node array
	atendofblock - flag indicating that an entire basic block has been
		       read in.

FUNCTIONS CALLED:

	writebb
	freebb
	cleanupht
	createbb

CALLED BY:

	extensively from all over

NOTES:

	This function needs to be replaced to allow functions of any size
	to be handled.

*FE********************************************************************/

# define TNEXT(p) (p== &node[TREESZ-1]?node:p+1)

NODE *
talloc()
{
	register NODE *p, *q;
	BBPTR bb;


	q = lastfree;
	for( p = TNEXT(q); p!=q; p= TNEXT(p))
	  {	if (p > highnode)
		  {
		  highnode = p;
		  memset( p , 0,  sizeof( NODE ) );			/* bk */
		  p->in.op = FREE;
		  p->opn.flags = '\0';
		  p->opn.sourceno = 0; /* zero index into symtab */
		  return(lastfree=p);
		  }
		if( p->in.op == FREE ) 
		  {
		  memset( p , 0,  sizeof( NODE ) );			/* bk */
		  p->in.op = FREE;			/* bk*/
		  p->opn.flags = '\0';
		  p->opn.sourceno = 0; /* zero index into symtab */
		  return(lastfree=p);
		  }
	  }

	/* !!! As of Version 4.1d we MUST optimize at all costs since c0 is
	** now spacing local frame items with large padding spaces and it
	** is up to the later frame compaction code to compress it.  This
	** means non-virtual host systems with insufficient memory will
	** simply have to skip optimizing super large programs.
	*/

	p = (NODE *)malloc(sizeof(NODE));
  memset( p , 0,  sizeof( NODE ) );			/* bk */
    p->in.op = FREE;				/* bk */
	p->opn.flags = '\0';
	p->opn.sourceno = 0; /* zero index into symtab */
	return( p );

#if 0
/*
 *    In the case where all the nodes for a basic block have been read in and
 *    we are trying to process it, you can't just write out the current basic
 *    block to try and free up nodes ( as the code below would do ). The malloc
 *    below is a fix for this rare situation.
 */
	  if ( atendofblock )
	     {
	  	p = (NODE *)malloc(sizeof(NODE));
		p->opn.flags = '\0';
		p->opn.sourceno = 0; /* zero index into symtab */
		return( p );
	     }

	fprintf(stderr,"\n WARNING -- function %s exceeded the available tree space.\n", funcname);
	fprintf(stderr,"            Only local common subexpression elimination can be\n            performed on this function.\n");

	if (statsflag)
	   {
	   fprintf(statsfile,"\n WARNING -- function %s exceeded the available tree space.\n", funcname);
	   fprintf(statsfile,"            Only local common subexpression elimination can be\n            performed on this function.\n");
	   }

	/* indicate overflow and currently terminate all optimizations */

	clseonly = TRUE;
	regctrl = FALSE;
	lcsectrl = FALSE;

	/* write out the flow graph as it currently exists */

	for (bb = fgroot; bb != currentbb; bb = bb->next )
	   {
	   writebb( bb );
	   freebb( bb );
	   }

	/* cleanup the labels' hashtable */

	cleanupht( &labelsht, freebblink );

	/* rebuild the flow graph */

	fgroot = createbb();
	fgroot->next = currentbb;	/* put dummy BB at head of fgroot */

	/* find a FREE node and return it */

	q = lastfree;
	for( p = TNEXT(q); p!=q; p= TNEXT(p))
		if( p->in.op ==FREE ) 
		  {
		  p->opn.flags ='\0';
		  p->opn.sourceno = 0; /* zero index into symtab */
		  return(lastfree=p);
		  }

	cerror( "a basic block exceeded tree space; simplify function %s", funcname);
	/* NOTREACHED */
#endif
}


/*F*********************** FUNCTION HEADER *******************************

 dataread

	Read an expression from the data segment and don't buffer it. Just
	pass it through to the output. These expressions consist of ctrees
	announcing labels.

PARAMETERS:

	Receives:	nothing

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	dataread

CALLED BY:

	gatherfunc

*FE********************************************************************/

static void
dataread()
{
   int op;
   int i;

   gets(&buff[0]);
   printf("%s\n", buff);
   sscanf(buff,"%d", &op);
   i = optype(op);
   if (i != LTYPE) dataread();
   if (i == BITYPE) dataread();
}
