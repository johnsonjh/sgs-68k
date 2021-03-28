#ident	"@(#)adjframe.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*      adjframe.c  7.1          */
/*      Copyright 1987 Motorola Inc. */

#include <storclass.h>
#include "declar.h"

/* Prototypes for static functions */
static int  movable _PARMS((struct Tlevel *));
static void  marklower _PARMS((struct Tlevel *));
static int  compress _PARMS((struct Tlevel *, int ));
static void  set_movables _PARMS((struct Tlevel *, int ));
static void  alteroffsets _PARMS((union ndu *));
static int  get_align _PARMS((int ));


int absmaxoffset;

/*F************************* FUNCTION HEADER **********************************

 adjframe

	Performs two optimizations on the stack frame:

	1) Moves the most used local variable for a scope to the frame
	bottom. If the highest used variable is in an inner scope the
	highest used variable from each sibling scope is also moved to
	the frame bottom without conflict since the variables in sibling
	scopes are not active at the same time. 
	   This optimization allows the more often used local variables
        (which were not promoted to a register) to be accessed zero off
	of the frame pointer.

	2) Compresses the frame by not allocating space for variables
	which were promoted to registers.

PARAMETERS:

	none

EXTERNAL DATA REFERENCES:

	fgroot	  - flow graph root for the function
	scoperoot - pointer to scopes for the function

FUNCTIONS CALLED:

	movable	- decides if a local is eligible for being moved to zero off fp
	compress - 
	absmaxoffset -
	find_maxoffset -
	set_movables - 
	alteroffsets - change the C trees frame offsets to match symtab's

CALLED BY:

	asgregvars() - regasg.c

*FE****************************************************************************/

void
adjframe()
{
   BBPTR bb;
   NODE *p;
   int misalign;

   movable( scoperoot );
   compress( scoperoot, 0 ); 
   absmaxoffset = find_maxoffset( scoperoot, 0 );
/*
 *   align the stack if necessary
 */
   misalign = (-absmaxoffset * SZCHAR ) % ALSTACK;

   if ( misalign > 0 )
	absmaxoffset -= ( ALSTACK - misalign ) / SZCHAR;

   set_movables( scoperoot, absmaxoffset );
/*
 *	The absolute maximum offset ( from all scopes in the function )
 *	is assigned to scoperoot's maxoffset because it is convenient
 *	since the largest offset from all scopes will be the one used
 *	as the frame size.
 */
   scoperoot->maxoffset = absmaxoffset;

#ifndef NODBG
   if ( symflag ) {
	printf("\n\nFRAME VARIABLES\n\n");
	prmoves( scoperoot, 0 );
   }
#endif

   for ( bb = fgroot->next ; bb ; bb = bb->next )

         for ( p = bb->codetree ; p ; p = p->stmt.succ )

               if ( p->tn.op == EXPRNO && p->in.left )
                    alteroffsets( p->in.left );	/* change offsets in Ctrees */
}

/*F************************* FUNCTION HEADER **********************************

 movables

	Identifies the most used variable for each scope.

PARAMETERS:

	Recieves:	curscope - pointer to the current scope
	Returns:	the maximum frame usage weight of same and lower scopes

EXTERNAL DATA REFERENCES:

	symtab - symbol entry table

FUNCTIONS CALLED:

	marklower()
	movable()

CALLED BY:

	movable()

*FE****************************************************************************/

static int
movable( curscope )
    SCOPE *curscope;
{
   int i, highwgt, lowermax, samemax;
   SYMENT *sym;
/*
 *  Structures, Unions, Enumerations and any variables that were not promoted
 *  to registers are eligible for being placed zero off of the frame pointer.
 */
   curscope->highsym = NULL;
   for (i = curscope->scopehead ; i ; i = sym->scopendx)
   {
      sym = symtab + i;
      if ( sym->reg == 0 && ( sym->flags & USED ) && sym->offset < 0 )
      {
         if ( curscope->highsym != NULL )
         {
            if ( sym->frmwgt > curscope->highsym->frmwgt )
               {
                 curscope->highsym = sym;
               }
         }
         else curscope->highsym = sym;
      }
   }
   if ( curscope->highsym )
        highwgt = curscope->highsym->frmwgt;
   else highwgt = 0;

   if ( curscope->lower )
        lowermax = movable( curscope->lower );
   else lowermax = 0;
       
   if ( highwgt > lowermax ) {
        if ( curscope->lower )
             marklower( curscope->lower );
      }
   else curscope->highsym = NULL;

   if ( curscope->same )
        samemax = movable( curscope->same );
   else samemax = 0;

   if ( samemax > lowermax  &&  samemax >= highwgt ) {
        curscope->highsym = NULL;
        return ( samemax );
      }
   else if ( lowermax >= highwgt )
      return ( lowermax );
   else return ( highwgt );
}

/*F************************** FUNCTION HEADER *********************************

 marklower

	Sets all sibling and child scope's highsym to NULL for the scopes
	whose previously found highsym were not as highly used as all its
	higher scope. 

PARAMETERS:

	Recieves:	curscope:	pointer to current scope
	Returns:	all same & lower scope's highsym marked as unmovable

EXTERNAL DATA REFERENCES:


FUNCTIONS CALLED:

	marklower()

CALLED BY:

	marklower()
	movable()

*FE****************************************************************************/

static void
marklower ( curscope )
   SCOPE *curscope;
{
   int i;

/* mark the highest used variable in the current scope as unmovable */
   curscope->highsym = NULL;

   if ( curscope->lower )
      marklower( curscope->lower ); /* mark all children scopes as unmovable */

   if ( curscope->same )
      marklower( curscope->same );  /* mark sibling scopes as unmovable */
}

/*F************************* FUNCTION HEADER **********************************

 compress

	Compute the maximum frame offset ( a negative value ) from all
	scopes without including the highsym.		

PARAMETERS:

	Recieves:	curscope: pointer to the current scope
			sco:	  current scope offset
	Returns:	The maximum offset of all same and lower scopes

EXTERNAL DATA REFERENCES:

	symtab

FUNCTIONS CALLED:

	compress()
	get_align()

CALLED BY:

	adjframe()
	compress()

*FE****************************************************************************/

static int
compress( curscope, sco )
    SCOPE *curscope;
    int   sco;
{
   int co, i, lowermaxoff, samemaxoff, align_size, misalign, nontempmaxoff;
   SYMENT *sym;

   co = sco;
   nontempmaxoff = sco;
/*
 *   do every variable in the current scope
 */
   for ( i = curscope->scopehead ; i ; i = sym->scopendx )
   {
      sym = symtab + i;
/*
 *  IF the symbol wasn't promoted AND is used 
 *     AND isn't a parameter AND isn't the most used for the scope
 *  THEN increment current offset by symbol size
 */
      if (   !sym->reg
	   && sym->flags & (USED|ADDRESSTAKEN)
           && sym->offset < 0		/* parameters have a positive offset */
	   && sym != curscope->highsym
	 )
         {
/*
 *   subtract the local variable's size from the current offset,
 *	 align it to appropriate boundary
 *   and assign that offset to the local variable's newoffset.
 */
            co -= sym->size;

	    align_size = get_align( sym->type );

	    misalign = ( -co * SZCHAR ) % align_size;

	    if ( misalign > 0 )
		 co -= ( align_size - misalign ) / SZCHAR;
            sym->newoffset = co;
/*
 *   as long as the variable's name isn't %TEMP update nontempmaxoff
 *   %TEMP's are for lcses and are reused across scopes
 */
            if ( strcmp(sym->name, "%TEMP") != 0 )
                 nontempmaxoff = co;
         }
   }
   curscope->maxoffset = co;
/*
 *   compress lower scopes and set lowermaxoff (maximum offset of lower scopes)
 */
   if ( curscope->lower )
        lowermaxoff = compress( curscope->lower, nontempmaxoff );
   else lowermaxoff = 0;
/*
 *   compress sibling scopes and set samemaxoff (maximum offset of same scopes)
 */
   if ( curscope->same )
        samemaxoff = compress( curscope->same, sco );
   else samemaxoff = 0;
/*
 *    set the current scope's maximum offset to the most negative value
 *    chosing from : the current scope, sibling and children scopes.
 *
 *    even though the value assigned to maxoffset could be larger than the
 *    space required for the current scope, its purpose is for determining
 *    the frame space needed for the function.
 */
   if ( samemaxoff < lowermaxoff  &&  samemaxoff < curscope->maxoffset ) 
        curscope->maxoffset = samemaxoff;
   else if ( lowermaxoff < curscope->maxoffset )
        curscope->maxoffset = lowermaxoff;

   return( curscope->maxoffset );
}

/*F************************* FUNCTION HEADER **********************************

 find_maxoffset

	Finds the maximum frame offset required for the function
 
PARAMETERS:

	Recieves:	curscope - the current scope
			absmaxoffset - the frame bottom
	Returns:	absmaxoffset - the frame bottom.

EXTERNAL DATA REFERENCES:


FUNCTIONS CALLED:

	find_maxoffset()

CALLED BY:

	adjframe()
	find_maxoffset()

*FE****************************************************************************/

int
find_maxoffset( curscope, absmaxoffset )
   SCOPE *curscope;
   int absmaxoffset;
{
   int tmaxoffset;
/*
 *   compare to see if the current scope's maxoffset is more negative
 *   ( and take into account the size of the highsym if one exists )
 */
   if ( curscope->highsym )
      {
        if (( curscope->maxoffset - curscope->highsym->size ) < absmaxoffset )
              absmaxoffset = curscope->maxoffset - curscope->highsym->size;
      }
   else if ( curscope->maxoffset < absmaxoffset )
        absmaxoffset = curscope->maxoffset;
/*
 *   check current scope's children for a more negative offset
 */
   if ( curscope->lower ) {
        tmaxoffset = find_maxoffset( curscope->lower, absmaxoffset );
        if ( tmaxoffset < absmaxoffset )
             absmaxoffset = tmaxoffset;
   }
/*
 *   check current scope's siblings for a more negative offset
 */
   if ( curscope->same ) {
        tmaxoffset = find_maxoffset( curscope->same, absmaxoffset );
        if ( tmaxoffset < absmaxoffset )
             absmaxoffset = tmaxoffset;
   }
  
   return( absmaxoffset );
}

/*F************************* FUNCTION HEADER **********************************

 set_movables

	Changes the newoffset of the highest used variables to the frame
	bottom for every scope which has a highsym (high usage frame
	variable).

PARAMETERS:

	Recieves:	curscope - the current scope
			absmaxsz - absolute maximum size of the frame
	Returns:

EXTERNAL DATA REFERENCES:


FUNCTIONS CALLED:

	set_movables()

CALLED BY:
	adjframe()
	set_movables()

*FE****************************************************************************/

static void
set_movables( curscope, absmaxsz )
   SCOPE *curscope;
   int absmaxsz;
{
   if ( curscope->highsym )
        curscope->highsym->newoffset = absmaxsz;

   if ( curscope->lower )
        set_movables( curscope->lower, absmaxsz );	/* do the children */

   if ( curscope->same )
        set_movables( curscope->same, absmaxsz );	/* do the siblings */
}

/*F************************* FUNCTION HEADER **********************************

 alteroffsets 

	Changes the frame offsets of local variables in the Ctrees to the
	new offset computed in compress() and set_movables().

	Uses recursion to set every occurrence of the offset for that 
	variable to the new offset.

PARAMETERS:

	Recieves:	p - pointer into the Ctrees.
	Returns:	altered frame offsets in the C trees

EXTERNAL DATA REFERENCES:


FUNCTIONS CALLED:

	alteroffsets()

CALLED BY:

	adjframe()
	alteroffsets()

*FE****************************************************************************/

static void
alteroffsets( p )
   NODE *p;
{
   SYMENT *sym;	/* pointer to the variables' symbol table entry */

   switch ( optype( p->tn.op ))
     {
     case BITYPE:
	     alteroffsets( p->in.left );
	     alteroffsets( p->in.right );
	     break;
     case UTYPE:
	     alteroffsets( p->in.left );
	     break;
     case LTYPE: 
/*
 *	Assign sym to be a pointer to the variable's entry in the symbol table
 */
	     sym = symtab + p->opn.sourceno;
	     if (     p->opn.op == VAUTO	/* is a local variable  */
		  && !sym->reg			/* wasn't promoted      */
		  &&  sym->flags & (USED|ADDRESSTAKEN)	/* the variable is used */
		)
 		{
/*
 *   set the Ctrees offset to the newoffset and take into account any casting
 */
		   p->tn.lval = sym->newoffset - (sym->offset - p->tn.lval);
		}
	     break;
      }
}

/*F************************* FUNCTION HEADER **********************************

 get_align

	Finds the most restrictive alignment requirement for a type

PARAMETERS:

	Recieves:	type : type of the local variable to be aligned.
	Returns:	align_size : the alignment based on the constants
				     defined in macdefs.h

EXTERNAL DATA REFERENCES:


FUNCTIONS CALLED:

	none

CALLED BY:

	adjframe()
	compress().

*FE****************************************************************************/

static int
get_align( type )
   int type;
{                          
   int align_size;
   
/*
 *   Check each derived type slot in the type information to see if
 *   the object is some kind of pointer.
 */

   if (   (type & TMASK) == PTR
       || (type & TMASK<<2) == PTR<<2
       || (type & TMASK<<4) == PTR<<4
       || (type & TMASK<<6) == PTR<<6
       || (type & TMASK<<8) == PTR<<8
       || (type & TMASK<<10) == PTR<<10
      )
        align_size = ALPOINT; 
   else switch ( BTYPE( type ) )
      {
	case CHAR:
	case UCHAR:	align_size = ALCHAR;
			break;
	case SHORT:
	case USHORT:	align_size = ALSHORT;
			break;
	case INT:
	case UNSIGNED:	align_size = ALINT;
			break;
	case LONG:
	case ULONG:	align_size = ALINT;
		 	break;
	case FLOAT:	align_size = ALFLOAT;
			break;
	case DOUBLE:	align_size = ALDOUBLE;
			break;
	case STRTY:	align_size = ALSTRUCT;
			break;
	case UNIONTY:	align_size = ALSTRUCT;
			break;
	case ENUMTY:	align_size = ALSTRUCT;
			break;
   
	default:	printf("Unexpected basic type BTYPE = %d\n", type);
			cerror("Unexpected basic type: in get_align()\n");
      }
/*
 *   for arrays, choose the more restictive alignment
 */
   if ( ISARY( type ) && align_size < ALARRAY )
	align_size = ALARRAY;

   return( align_size );
}

/*F************************* FUNCTION HEADER **********************************

 prmoves

	Prints debug information consisting of the high usage
	variable(s) for the function and all local variables
	with their offset ( from c0 ) and newoffset.

PARAMETERS:

	Recieves:	curscope: pointer to the current scope
			indent:   the indentation amount
	Returns:

EXTERNAL DATA REFERENCES:

	symtab

FUNCTIONS CALLED:

	prmoves()

CALLED BY:

	adjframe()

*FE****************************************************************************/

#ifndef NODBG
prmoves( curscope, indent )
   SCOPE *curscope;
   int indent;
{
   int i;
   SYMENT *sym;

   if ( curscope->highsym ) {
        printf("most used frame variable: %s\n", curscope->highsym->name);
   }
   for (i = curscope->scopehead ; i ; i = sym->scopendx)
   {
      sym = symtab + i;
      if ( sym->offset < 0 )	/* print frame variables only */
      	   printf("%*s\toffset: %d\tnewoffset: %d\n", indent+strlen(sym->name)+1
		   ,sym->name, sym->offset, sym->newoffset);
   }
   if ( curscope->lower )
        prmoves( curscope->lower, indent+4 );

   if ( curscope->same )
        prmoves( curscope->same, indent );
}
#endif
