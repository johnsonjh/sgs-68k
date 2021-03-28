#ident	"@(#)storage.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	"@(#)  storage.c  1.2"	*/

/*	storage.c	7.1		*/
/*	Copyright 1985 Motorola Inc.	*/

#include <storclass.h>
#include "declar.h"

/*F*********************** FUNCTION HEADER *******************************

 addtemp

	This function returns the byte offset for the requested temporary 
	storage location, and updates the block's maximum offset. 
	The alignment of the variable is taken into account.

PARAMETERS:

	Receives:	level: pointer to the current scope
			type: type of variable to be allocated

	Returns:	offset for the variable

EXTERNAL DATA REFERENCES:

	symtab
	tempstat
	curscope

FUNCTIONS CALLED:

	addsymbol

CALLED BY:

	gettemp

*FE********************************************************************/

int
addtemp(level, type)
               
SCOPE *level;
int type;
{
   int size;
   int offset;
   int ndx;
   SYMENT *sym;

   size = typesize(type);
   offset = (-level->maxoffset - size)/ 8; /* convert bits to bytes */
	offset -= COPTPAD;			/* allocate on padding boundary (like c0) */
   level->maxoffset += size < ALINT ? ALINT : size;
	level->maxoffset += COPTPAD * SZCHAR;  /* max offset in bits by padding */

   /* Following line can be deleted, iff front end fixed to output matching
      [] for ALL scopes. The corresponding line in exitscope can then be used */
   level->sblock->opn.csiz = level->maxoffset;

   ndx = getsyment();

   /* Add to the symbol table entry and initialize it */
   sym = (symtab + ndx);
   sym->offset = offset;
   sym->newoffset = 0;
   sym->type = type;
   sym->size = size / 8;
   sym->flags = COMPLEX;   /* so getreg won't try to use an A reg for LCSEs */
   sym->sclass = C_AUTO;
   sym->weight = 0;
   sym->frmwgt = 0;
   sym->reg = 0;
   sym->valueno = (ENTRY *)getvalueno();
   sym->name = "%TEMP";           
   sym->scope = level;
   sym->scopendx = 0;
   if (level->scopehead)  /* if symbol list exists */
      addsymbol( level->scopehead, ndx);  /* add to the current symbol list */
   else  /* this is the first symbol for this scope */
      level->scopehead = ndx;
   (symtab + ndx)->scope = curscope;
   ++tempstat;
   return(offset);
}



/*F*********************** FUNCTION HEADER *******************************

 typesize

	This function returns the size (in bits) of the requested type.

PARAMETERS:

	Receives:	type: type of concern

	Returns:	size (in bits) of the requested type

EXTERNAL DATA REFERENCES:

	none

FUNCTIONS CALLED:

	none

CALLED BY:

	addtemp
	lookup

*FE********************************************************************/

int
typesize( type )

int type;
{
   int size;
   if (type != BTYPE(type))  /* if its not simple substitute a long */
      type = LONG;
   switch (type)
      {
      case UCHAR:
      case CHAR: 
         size = SZCHAR;
         break;
      case USHORT:
      case SHORT: 
         size = SZSHORT;
         break;
      case UNSIGNED:
      case INT: 
         size = SZINT;
         break;
      case ENUMTY:
      case ULONG:
      case LONG:
         size = SZLONG;
         break;
      case FLOAT:
         size = SZFLOAT;
         break;
      case DOUBLE:
         size = SZDOUBLE;
         break;
      default:
         size = SZPOINT;
         break;
      }
   return(size);
}



/*F*********************** FUNCTION HEADER *******************************

 getreg

	This function returns the next free register 
	for the specified type in the specified scope.

PARAMETERS:

	Receives:	scope: scope of interes
			t: type of register required

	Returns:	reg: register found

EXTERNAL DATA REFERENCES:

	scope tree
	aregstat
	fpregstat
	dregstat

FUNCTIONS CALLED:

	none

CALLED BY:

	assign

*FE********************************************************************/

int
getreg( scope, t, reg, sym)

SCOPE *scope;
register TWORD t;
int *reg;	/* If function finds a register, the symbol table entry
		** for the variable is updated with this information */
SYMENT *sym;
{
        register int i;

#ifdef M68881
	if ( t == DOUBLE || t == FLOAT ) 
	{
		/* floating point register */
		for ( i=2; i<=7; ++i )
		{
                        if( scope->regavail & (0x10000<<i) ) continue;
			/* register found - now update environment */
			fpregstat++;	/* statistics counter */
                        *reg = i;	
			i = 0x10000<<i;
                        scope->regavail |= i;
                        return(i);
		 }
		 return(0);
	 }
        else 
#endif
             if( !ISPTR( t ) )
        {
                /* data register */
		int mask;
		int lastreg;

		if (t == DOUBLE) /* require two registers */
		   {
		   mask = 3;
		   lastreg = 6;
		   }
		else
		   {
		   mask = 1;
		   lastreg = 7;
		   }

                for( i=2; i<=lastreg; ++i )
                {
                        if( scope->regavail & (mask<<i) ) continue;
			/* register found - now update environment */
			dregstat++;	/* statistics counter */
			if(t == DOUBLE) dregstat++;
                        *reg = i;
			mask = mask<<i;
                        scope->regavail |= mask;
                        return( mask );
                }
        }

/*
 *   Always check for the possiblity of using an address register for
 *   cases that aren't COMPLEX. This allows ints to reside in address
 *   registers.
 */
		if( ISPTR( t ) || !(sym->flags & COMPLEX) )
        {
                /* address register */
                for( i=2; i<=5; ++i )
                {
                        if( scope->regavail & (0x100<<i) ) continue;
			/* register found - now update environment */
			aregstat++;	/* statistics counter */
                        *reg = i;	
			i = 0x100<<i;
                        scope->regavail |= i;
/*
 *    If the promoted variables type is int, unsigned int, long,
 *    or unsigned long set the USE_AREG flag for the code generator (c1)
 */
			if (  sym->type == INT
			   || sym->type == UNSIGNED
			   || sym->type == LONG
			   || sym->type == ULONG
			   )
			   sym->flags |= USE_AREG;
                        return(i);
                }
        }
        return(0);
}
