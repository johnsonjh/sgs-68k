#ident	"@(#)symtab.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)symtab.c	7.2		*/
/*	Copyright 1985 Motorola Inc.	*/

#include <stdio.h>	
#include <storclass.h>
#include "declar.h"

#define BITSPERBYTE 8
#define BITS(type) (BITSPERBYTE * (int)sizeof(type))
#define MAXNEGI (1<<BITS(int)-1)

#define NULL 0
#define EXTERNAL 0
#define LOCAL 1

static char envirn = EXTERNAL;    /* describes the current scope environment */
static	TWORD	typeq = 0;			/** type qualifiers **/

#ifndef NODBG
/*F*********************** MODULE HEADER *******************************

FUNCTION NAMES:

	scopedump
	indent
	traversescope
	dumpsymtab

ACTION:

	DEBUG ONLY
	These routines dump the symbol table for the entire scope tree.


EXTERNAL DATA REFERENCES:

	scoperoot
	symtab

CALLED BY:

	main
	asregvars

*FE********************************************************************/

void
scopedump()
{
   printf("\n\nSCOPE INFORMATION FOR FUNCTION\n");
   traversescope(scoperoot, 0);
   printf("\n\n");
}

/* indent
 *	Outputs "level" number of tab characters for debug messages.
 */

void
indent(level)

int level;
{
   int i;
   for (i = 0; i < level; ++i)
      printf("\t");
}

/* traversescope
 *	Function traverses the scope tree dumping out the symbol table
 *	for each scope level.
 */

void
traversescope( q, level )

SCOPE *q;
int level;
{
   printf("\n\n");
   indent(level);
   printf("Scope Info  maxoffset: %d  regexport: %o  regavail: %o  qnode: %d\n",
           q->maxoffset, q->regexport, q->regavail, q->qnode);
   dumpsymtab(q->scopehead, level);
   if (q->lower)
      traversescope(q->lower, level+1);
   for (q = q->same; q ; q = q->same)
      traversescope(q, level);
}


/* dumpsymtab
 *	Write out the information associated with symbol table "p".
 */

void
dumpsymtab(p, level)

int p;
int level;
{
   int i;
   SYMENT *q;
   if (p)
      {
      indent(level);
      printf("Symbol Table\tlevel:\t%d\n\n", level);
      for ( i = p; i; i = (symtab + i)->scopendx)
          {
          q = (symtab + i);
          indent(level);
          printf("name:%8s  offset:%d  sclass:%d  type:%o  size:%d  flags:%o\n",
                 q->name, q->offset, q->sclass, q->type, q->size, q->flags);
          indent(level);
          printf("\t       weight:%d  val#:%d  reg:%d  frmwgt:%d",
                  q->weight, q->valueno, q->reg, q->frmwgt);
	  if ( q->sclass == ICON || q->sclass == FCON )
               printf("  value:%d\n", q->p->tn.lval);
          else printf("\n");
          }
      }
}
#endif

/*F*********************** FUNCTION HEADER *******************************

 chgtype

	Converts the type information found in the sdb lines to the
	type information found in the Ctrees.  The sdb type was
	derived from the Ctree type in C0. This is the inverse function.

PARAMETERS:

	Receives:	ntype: sdb type

	Returns:	ctree type

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	none

CALLED BY:

	rdsymtab

*FE********************************************************************/
                                                                        
#define O_TNULL         020
#define O_TVOID         040
#define O_UNDEF         00
#define O_FARG          01
#define O_CHAR          02
#define O_SHORT         03
#define O_INT           04
#define O_LONG          05
#define O_FLOAT         06
#define O_DOUBLE        07
#define O_STRTY         010
#define O_UNIONTY       011
#define O_ENUMTY        012
#define O_MOETY         013
#define O_UCHAR         014
#define O_USHORT        015
#define O_UNSIGNED      016
#define O_ULONG         017


TWORD
chgtype( ntype )

TWORD ntype;
{                          
   TWORD btype;         /* basic type */
   TWORD other;         /* remaining bits */
   TWORD newtype;       /* Ctrees type */

   btype = ntype & 017;                 /* last four bits */
   other = ntype & ~((TWORD) 017);
   
   switch( btype )      /* convert btype to Ctrees type */
      {
   case O_TNULL:        newtype = TNULL; break;
   case O_FARG:         newtype = FARG; break;
   case O_CHAR:         newtype = CHAR; break;
   case O_SHORT:        newtype = SHORT; break;
   case O_INT:          newtype = INT; break;
   case O_LONG:         newtype = LONG; break;
   case O_FLOAT:        newtype = FLOAT; break;
   case O_DOUBLE:       newtype = DOUBLE; break;
   case O_STRTY:        newtype = STRTY; break;
   case O_UNIONTY:      newtype = UNIONTY; break;
   case O_ENUMTY:       newtype = INT; break;		/* treat as INT */
   case O_MOETY:        newtype = MOETY; break;
   case O_UCHAR:        newtype = UCHAR; break;
   case O_USHORT:       newtype = USHORT; break;
   case O_UNSIGNED:     newtype = UNSIGNED; break;
   case O_ULONG:        newtype = ULONG; break;
   case O_TVOID:        newtype = TVOID; break;	/* bk: this will never be true*/
   case O_UNDEF:        newtype = TVOID; break;	/* bk: temporary kluge, see */
												/* comp2/mach/debug.c too */
   
   default:             cerror("Unknown basic type: in chgtype\n");
      }
   return( (other << 1) | newtype );
}

/*F*********************** FUNCTION HEADER *******************************

 acceptable

	Function determines if the variable is a simple variable or an array
	(e.g. NOT: a member of a structure/union, an enumeration type, ...)
	Types are acceptable iff they can describe variables which can be
	placed in registers.

PARAMETERS:

	Receives:	type: type to be classified

	Returns:	TRUE or FALSE

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	none

CALLED BY:

	rdsymtab

*FE********************************************************************/

int
acceptable( type )

TWORD type;
{
   switch ( type & TMASK ) 
      {
   case PTR: return(TRUE);      /* pointers are ok */
   case ARY: return(TRUE);	/* arrays are acceptable */
   case 0:   break;             /* just a basic type */
   default:  return(FALSE);     /* everything else is unacceptable */
      }                                                          

   /* just a basic type: is it acceptable? */
   switch (type)
      {
   case FARG:
   case CHAR:
   case SHORT:
   case INT:
   case LONG:
   case FLOAT:
   case DOUBLE:
   case UCHAR:
   case USHORT:
   case UNSIGNED:
   case ULONG:
   case STRTY:		/* \  */
   case UNIONTY:	/*  > made these acceptable for frame optimization */
   case ENUMTY:		/* /  */
        return(TRUE);
   case TNULL:
   case MOETY:
   case VOID:
   case UNDEF:
        return(FALSE);
   default:
        cerror("Unknown basic type: in acceptable");
      }
}

/*F*********************** FUNCTION HEADER *******************************

 getsyment

	This function controls the symbol table. It is responsible for
	creating the symbol table, allocating the next free entry in the
	symbol table, and extending the table if it is not big enough.

PARAMETERS:

	Receives:	nothing

	Returns:	index into symbol table

EXTERNAL DATA REFERENCES:

	symtab
	symtabndx
	symtabsz

FUNCTIONS CALLED:

	calloc
	realloc

CALLED BY:

	rdsymtab
	addtemp

*FE********************************************************************/

int
getsyment()
{
   if (symtab == NULL)
      {                     /* allocate the symbol table */
      symtabsz = SYMTABSZ;
      symtabndx = 1;
      symtab = (SYMENT *) calloc(symtabsz, sizeof(SYMENT));
      if (symtab == NULL)
         cerror("out of space: getsyment");
      symtab->offset = MAXNEGI;  /* the first entry in the symbol is a */
      }                          /* sentinel for all of the scope lists */
   
   if (symtabndx == symtabsz)
      {                     /* increase the size of the symbol table */
      symtabsz += SYMTABSZ;
      symtab = (SYMENT *) realloc(symtab, symtabsz * sizeof(SYMENT));
      if (symtab == NULL)
         cerror("out of space: getsyment(2)");
      }
   return( symtabndx++ );
}                            


/*F*********************** FUNCTION HEADER *******************************

 getvalueno

	Returns the next unique value number.
	This routine is called when the node in question does not qualify
	for a value number which could be used in an LCSE.

PARAMETERS:

	Receives:	none

	Returns:	a unique value number

EXTERNAL DATA REFERENCES:

	valueno

FUNCTIONS CALLED:

	none

CALLED BY:

	addvalueno (7 times)
	leafvalue (2 times)

*FE********************************************************************/

int
getvalueno()

{
   return(++valueno);  
}                                                            

/*F*********************** FUNCTION HEADER *******************************

 addsymbol

	This function adds the symbol indicated by "ndx" to the current
	list of symbols for the current scope. The symbol is added to the
	list according to its offset. Parameters have positive offsets and
	locals have negative offsets.  There is an sentinel element with a
	very large negative number.

PARAMETERS:

	Receives:	start: 
			ndx:

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	symtab
	cursymlist

FUNCTIONS CALLED:

	none

CALLED BY:

	rdsymtab
	addtemp

*FE********************************************************************/

void
addsymbol( start, ndx )

int start;
int ndx;
{
   SYMENT *p, *q, *r;

   /* find the location for insertion */
   p = symtab + ndx;             
/*
 *    ICONs and FCONs are always added at the end of the symbol table.
 *    ( Note that their offsets are set to zero)
 *    Parameters will of been entered before any constants are encountered.
 */
   r = NULL;
   if ( p->sclass == ICON || p->sclass == FCON )
	{	/* append constants to the end of the symbol table */
	if ( scoperoot && scoperoot->scopehead != 0 )
           {
		for( q= symtab + start; q->scopendx; q=symtab + q->scopendx );
			/* empty */
	        r = q;		/* r points to the last entry for this scope */
	   }
	}
   else for( q= symtab + start; p->offset < q->offset; q= symtab + q->scopendx)
        	r = q;

  /* assertion: r points at entry which p should follow */

  if (r)
      { /* insert p after r */
      p->scopendx = r->scopendx;
      r->scopendx = ndx;
      }
   else
      { /* p is the first element in the list */
      p->scopendx = start;
/*
 *	must reset cursymlist now for constants in case we go to another
 *	scope. This is a problem since constants aren't seen until symbols
 *	have already been read and enterscope() has already been called
 *	making the assignment "curscope->scopehead = cursymlist".
 */
      if ( p->sclass == ICON || p->sclass == FCON )
	   cursymlist = 0;
      else cursymlist = ndx;
      }
}

/*F*********************** FUNCTION HEADER *******************************

 rdsymtab

	This function is passed a string, buff, which contains some
	symbol table information in the "sdb" format.  This routine
	decides what should be done, if anything, with the information.
	Called while the function is being read.

PARAMETERS:

	Receives:	buff: line from the intermediate file

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	symtab
	cursymlist

FUNCTIONS CALLED:

	getvalueno
	cursymlist

CALLED BY:

	gatherfunc

*FE********************************************************************/

void
rdsymtab( buff )

char *buff;
{
   int offset;          /* the offset of the variable */
   char *p;             /* pointer to current token */
   int scl;             /* storage class of variable */
   int size;		/* size of the variable */
   TWORD type;          /* type of the variable */
   SYMENT *sym;         /* pointer to symbol table entry */
   char *name;          /* name of variable */
   int ndx;             /* index of symbol in symbol table */
   register	NODE	*pp;
   
   offset = 0; /* initialize these variables in case sscanf fails */
   scl = C_NULL;
   type = TNULL;

	nodesin += 1;
	pp = talloc();
	pp->stmt.op = DEFTYP;
	pp->stmt.astring = malloc(strlen(buff)+1);
	strcpy(pp->stmt.astring, buff);
	addnodetobb(pp);

   p = strtok(buff, " \t\n;");   /* read "def" */
   do
      {
      if (strcmp(p, "val") == 0)
         {
         /* read the offset of the symbol */
         p = strtok(NULL, " \t\n;");
         sscanf(p, "%d", &offset);
         }
      else if (strcmp(p, "scl") == 0)
         {                     
         /* read the storage class of the variable */
         p = strtok(NULL, " \t\n;");
         sscanf(p, "%d", &scl);                       
         if ( scl != C_AUTO && scl != C_ARG && 
	      scl != C_EXT && scl != C_STAT) return;
         }
      else if (strcmp(p, "size") == 0)
         {
         /* read size */
         p = strtok(NULL, " \t\n;");
/*
 *   read the size of non-basic types (e.g. structures, enumerations, unions )
 *   except for a pointer. the sdb size information for a pointer is the size
 *   of what the pointer points to.
 */
	 if (( type & TMASK ) != PTR )
	      sscanf(p, "%d", &size);                       

         }
      else if (strcmp(p, "type") == 0)
         {                                                     
         /* read type */
         p = strtok(NULL, " \t\n;");
         sscanf(p, "%o", &type);
         type = chgtype(type);     /* convert to Ctrees type */
/*
 *   sdb info for size is only put out for Structures, Unions or Enumerations
 *   so we initialize all others here. ( type comes before size in sdb info )
 */
	 if ( type == CHAR || type == UCHAR )
	      size = SZCHAR / SZCHAR;
	 else if ( type == SHORT || type == USHORT )
	      size = SZSHORT / SZCHAR;
	 else if ( type == DOUBLE )
	      size = SZDOUBLE / SZCHAR;
	 else size = SZINT / SZCHAR;
	 /* Save the name of the function for statistics file */
	 if (ISFTN(type) && (scl == C_EXT || scl == C_STAT)) 
	    {
	    strcpy(funcname,name);
	    return;
	    }

         if ( ! acceptable(type) ) return;
         }
      else if (strcmp(p, "typeq") == 0)
         {                                                     
         	/* read type qualifier */
         	p = strtok(NULL, " \t\n;");
         	sscanf(p, "%d", &typeq);
         }
      else if (strcmp(p, "def") == 0)
         {
         p = strtok(NULL, " \t\n;");          
         if (  strcmp(p, "~bf") == 0  || strcmp(p, "~ef") == 0 )
            {
            return;
            }
         name = malloc(strlen(p)+1); /* save name of variable/function */
         strcpy(name,p);
         }
      /* the remaining fields are ignored */
      else if (   strcmp(p, "line") == 0
              ||  strcmp(p, "tag") == 0
              ||  strcmp(p, "dim") == 0
	      )
         {
         p = strtok(NULL, " \t\n;");
         }
      else if (strcmp(p, "endef") == 0)
         ;
      }
    while( p = strtok(NULL, " \t\n;"));                 

    /* ignore EXTERNAL and STATIC declarations - envirn could probably 
       be replaced by scl == C_EXT */
    if ( scl == C_EXT || scl == C_STAT )  return; 

    ndx = getsyment();
    sym = symtab + ndx;
    /* Initialize the symbol table entry */
    sym->offset = offset;
    sym->newoffset = 0;
    sym->type = type;
    sym->size = size;
    sym->flags = 0;
    sym->sclass = scl;
    sym->weight = 0;
    sym->frmwgt = 0;
    sym->reg = 0;
    sym->valueno = (ENTRY *)getvalueno();
    sym->p = NULL;
	sym->stypeq = typeq;
    sym->name = name;           
    addsymbol( cursymlist, ndx);	/* to the current symbol list */
}

/*F*********************** FUNCTION HEADER *******************************

 addscope

        Add "child" scope to "parent" scope's list of siblings.

PARAMETERS:

	Receives:	child: new scope
			parent: old scope

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	scope tree

FUNCTIONS CALLED:

	none

CALLED BY:

	enterscope

*FE********************************************************************/

void
addscope( parent, child )

SCOPE *parent;
SCOPE *child;
{
   SCOPE *q;
   if (parent->lower == NULL)
      parent->lower = child;
   else
      {
      for(q = parent->lower; q->same ; q = q->same);
      q->same = child;
      }
}

/*F*********************** FUNCTION HEADER *******************************

 enterscope

	This function creates a new scope level.

PARAMETERS:

	Receives:	p: pointer to the node (SBLOCK) which created the
				 new scope.

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	scoperoot
	cursymlist
	curscope

FUNCTIONS CALLED:

	malloc
	addscope
	scopestat

CALLED BY:

	gatherfunc

*FE********************************************************************/

void
enterscope(p)
NODE *p;	/* pointer to SCOPE node */
{
   SCOPE *q;
   int i;

   q = (SCOPE *)malloc(sizeof(SCOPE));
   if (q == NULL)
      cerror("not enough memory -- enterscope");
   if (scoperoot == NULL)
      {
      scoperoot = q;
      scoperoot->higher = NULL;
      }
   else
      {
      addscope(curscope, q);
      q->higher = curscope;
      }
   curscope = q;
   curscope->sblock = p;
   curscope->eblock = NULL;
   curscope->maxoffset = p->opn.csiz;
   curscope->regexport = p->tn.rval;
   curscope->regavail = p->tn.rval;
   curscope->lower = NULL;
   curscope->same = NULL;
   curscope->highsym = NULL;
   curscope->qnode = 0;
   curscope->scopehead = cursymlist;
   for ( i = curscope->scopehead; i ; i = (symtab + i)->scopendx)
      (symtab + i)->scope = curscope;
   cursymlist = 0;
   ++scopestat;
}

/*F*********************** FUNCTION HEADER *******************************

 exitscope

	This function is called when exiting a block (EBLOCK encountered). 

PARAMETERS:

	Receives:	p: pointer to EBLOCK node

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	curscope

FUNCTIONS CALLED:

	none

CALLED BY:

	gatherfunc

*FE********************************************************************/

void
exitscope(p)
NODE *p;
{
   curscope->eblock = p;        /* save pointer to eblock node */

   /* update "["'s maximum automatic offset */
   /* TEMPORARY - the following line has been commented out due to the
   front end not producing a ] node for each [ node. The statement's
   function has been moved to addtemp.  The front end should be modified
   to solve this problem.
   curscope->sblock->opn.csiz = curscope->maxoffset; */

   curscope = curscope->higher; /* return to next parent's scope level */
}

/*F*********************** FUNCTION HEADER *******************************

 gettemp

	Get a temporary for a lcse.  Use previously created temporaries
	before allocating another one. Return the offset of the temporary.

	CURRENTLY only allocates another temporary and does not try to
	reuse an existing one.

PARAMETERS:

	Receives:	type: type of variable required

	Returns:	index into symbol table for temporary

EXTERNAL DATA REFERENCES:

	curscope

FUNCTIONS CALLED:

	addtemp

CALLED BY:

	makealterations

*FE********************************************************************/

int
gettemp(type)
int type;
{
   return( addtemp(curscope, type) );
}

/*F*********************** FUNCTION HEADER *******************************

 classify

	This function is called to determine if the node "p" is a simple
	local or a simple parameter (VAUTO, VPARAM) or the name of an
	array.  The offset of the variable is searched for in the symbol
	table, and if found the index of the symbol table entry is returned.

PARAMETERS:

	Receives:	p: pointer to a VAUTO or VPARAM node

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	symtab

FUNCTIONS CALLED:

	lookup

CALLED BY:

	enterht
	makealterations
	parentlink

*FE********************************************************************/

void
classify( p )
NODE *p;
{
   int ndx;

   if (ndx = lookup(p))
      {  /* the variable is present in the symbol table */
      p->opn.sourceno = ndx;
      if ( ISARY((symtab + ndx)->type) )
	 p->opn.flags |= ARRAY;
      else if ( ! (    (symtab + ndx)->type == STRTY
		    || (symtab + ndx)->type == UNIONTY
		    || (symtab + ndx)->type == ENUMTY
		  )
	      )
		  p->opn.flags |= SIMPLE;
      }
}

/*F*********************** FUNCTION HEADER *******************************

 lookup

	The "offset" of the variable is sought in the symbol table.  If
	it is found in one of the currently active scopes, the index to
	the variable is returned, otherwise a 0 is returned.

PARAMETERS:

	Receives:	offset: offset of a variable

	Returns:	returns the ndx of the variable or FALSE if it
			    doesn't exist

EXTERNAL DATA REFERENCES:

	symtab

FUNCTIONS CALLED:

	typesize

CALLED BY:

	classify

*FE********************************************************************/

int
lookup(p)
NODE	*p;
{
	int	offset;
   SCOPE *q;
   SYMENT *sym;
   int ndx;
   int range;

	offset = p->tn.lval;		/* offset of AUTO */

   if (symtab == NULL) return(0);

   for (q = curscope; q ; q = q->higher )
       {

		/* Traverse symbol table for current scope looking for a symbol
		** with an offset smaller or equal to "offset".
		*/
		range = offset;			/* default to start of item test */
		if (offset < 0)			/* if a local frame variable ... */
			/* catch references just below the item as well */
			range += COPTPAD/2 - 1;
		for (ndx = q->scopehead; ndx ; ndx = (symtab + ndx)->scopendx)
			if ((symtab + ndx)->offset <= range) break;

       sym = (symtab + ndx);
       /* if the offsets are equal then the variable has been found */
       if (sym->offset == offset) 
		{ sym->flags |= USED; /* the variable is referenced in the frame */
          return(ndx);
		};

		/* Now we see if the offset is referencing a symbol other than at
		** it's start.  This means the access is through a casting operation, 
		** to an element of a larger structure, or to a boundary check
		** outside the symbol proper.  An example of the last case is:
		**
		**		for(p=&a[0]; p<&a[SIZE]; p++)
		**
		** Note that the second array access is actualy past the item
		** being referenced.  In order to properly catch this (and not 
		** erroneously assign this reference to the next item on the frame) 
		** we use a heuristic where c0 spreads apart each local item on the 
		** stack by a wide amount.  Then by matching to the closest symbol
		** in the table we can properly tag the reference.
		*/
		range = sym->offset + sym->size; /*typesize(sym->type)/8;*/
		/* if frame variable also allow one-half padding area above target */
		if (offset < 0)
		  	range += COPTPAD/2 + 1;
       if (offset < range)
	  {
	     sym->flags |= ADDRESSTAKEN; /* symbol is not eligible for a reg */
		p->opn.flags |= UNIQUE;		/* force unique value for this node */
	     return( ndx );
	  }
       }
    return(0);
}

/*F*********************** MODULE HEADER *******************************

 createsv
 cmpweights

	These functions create an array of SORTED structures, initialize
	the array with information from the symbol table, and then sort
	the array according to the weight of the variables (quick sort).

PARAMETERS:

	Receives:	nothing
		
	Returns:	a pointer to the sorted array

EXTERNAL DATA REFERENCES:

	symtab

FUNCTIONS CALLED:

	calloc
	qsort

CALLED BY:

	assign

*FE********************************************************************/

SORTED *
createsv()

{
   SORTED *p, *r;
   SYMENT *q;
   int ndx;
   int cmpweights();

   /* create and initialize the sorted array */

   p = (SORTED *) calloc(symtabndx, sizeof(SORTED));

   for (ndx = 0; ndx < symtabndx; ++ndx)
      {
      q = (symtab + ndx);
      r = (p + ndx);
      r->weight = q->weight;
/*
 *    If a parameter or constant  deduct from its weight.  This is because
 *    parameters require an extra load sequence plus force save/restore
 *    overhead for its register residence.
 */
      if (( q->sclass==C_ARG || q->sclass == ICON || q->sclass == FCON )
          && r->weight>0)
	    r->weight -= 1;
      r->ndx = ndx;
      }

   /* now sort the array */

   qsort((char *) p, symtabndx, sizeof (SORTED), cmpweights);
   return(p);
}

/* cmpweights
 *	The comparison routine for the call to qsort which is sorting the
 *	weights of the variables which are candidates for registers.
 */

int
cmpweights(p, q)

SORTED *p, *q;
{
   if(p->weight < q->weight)
      return( 1 );
   else if (p->weight == q->weight)
      return( 0 );
   return( -1 );
}

/*F*********************** FUNCTION HEADER *******************************

 flushstvn

	Flush value numbers for all variables in the symbol table.
	This is done at the beginning of all basic blocks.
	(Actually it is done in routine "lcse" after the previous BB
	has been examined for LCSE's.)
	Note that the symbol table is cleared at the beginning of each
	function so that this routine need not be called at that time.

PARAMETERS:

	Receives:	nothing

	Returns:	nothing

EXTERNAL DATA REFERENCES:

	lcsedbflg
	symflag
	symtab
	symtabndx

FUNCTIONS CALLED:

	none

CALLED BY:

	lcse

*FE********************************************************************/

void
flushstvn()

{
   register int ndx;            /* index of symbol in symbol table */
   register SYMENT *sym;        /* pointer to symbol table entry */

   sym = symtab+1;		/* First symbol is in symtab[1] ! */
   for (ndx = 1; ndx++ < symtabndx; sym++) 

       sym->valueno = (ENTRY *) getvalueno();

#ifndef NODBG
       if (symflag || lcsedbflg)
	   printf("---Leaving flushstvn.  All symbol table entry value numbers have been updated.\n");
#endif
}   
