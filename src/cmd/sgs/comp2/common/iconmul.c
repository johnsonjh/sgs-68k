     #ident	"@(#)iconmul.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
     /****    Copyright (C) 1987 by Motorola Inc.    ****/
     /****             All Rights Reserved           ****/

#include "mfile2.h"

#define	MAXINSTR	12   /* maximum instruction count decision value */

#define OUTPUT(x) {if(costing) icount++; else output(x);}

/*  X = operation size,  S = source register,  T = temporary register  */
static char negS[]	=	"	neg.X	S\n";
static char movST[]	=	"	mov.X	S,T\n";
static char addST[]	=	"	add.X	S,T\n";
static char addSS[]	=	"	add.X	S,S\n";
static char subTS[]	=	"	sub.X	T,S\n";
static char addTS[]	=	"	add.X	T,S\n";
static char lsl2S[]	=	"	lsl.X	&2,S\n";
static char lsl3S[]	=	"	lsl.X	&3,S\n";
static char lsl4S[]	=	"	lsl.X	&4,S\n";
static char lsl5S[]	=	"	lsl.X	&5,S\n";
static char lsl6S[]	=	"	lsl.X	&6,S\n";
static char lsl7S[]	=	"	lsl.X	&7,S\n";
static char lsl8S[]	=	"	lsl.X	&8,S\n";
static char swapS[]	=	"	swap	S\n";
static char clrS[]	=	"	clr.w	S\n";

static int 	icount;		/* counter for instructions generated  */
static int	costing;	/* flag indicating only costing to be done */
static unsigned long num; /* value of the constant multiplican */
static NODE	*srcnode;	/* source register node */
static NODE	*tempnode;	/* temporary register node */
static char	size;		/* size of multiply, 'l', 'w', 'b' */

/* Prototypes for static functions */
static int  icongen _PARMS((void));
static void  shiftl _PARMS((int ));
static int  exceptn _PARMS((ulong , ulong ));
static void  sub0s _PARMS((ulong ));
static void  output _PARMS((char *));

/*
 *  iconmult - test constant multiplier to decide if multiply should be replaced
 *
 *  input parameters:
 *      value     - integer constant
 *
 *  output result:
 *	0	- hardware multiply is better
 *	1	- replacement shift/add/... is better
 *
 *  Note: must assume longword multiply since compiler routine which calls this
 *		doesn't know the size of the operation.
 */

int
iconmult(value)
unsigned long value;			/* value to test with */
{
	num = value;			/* fill in value */
	costing = 1;			/* set costing flag */
	size = 'l';			/* must assume worse case of 'longword' */
	icount = 0;			/* clear instruction count value */
	icongen();			/* count instructions */

	/* return indication of replacement strategy */
	return ( icount <= MAXINSTR );

} /* iconmult() */


/*
 *	iconmulg() - generate replacement instructions for constant multiply
 *
 *  input parameters:
 *      value   - integer constant
 *		size	- 'l', 'w', or 's' representing size of the multiply
 *		snode	- source value register node
 *		tnode	- temporary work value register node
 */

iconmulg( value, sz, sn, tn)
    unsigned long value;	/* immediate constant value of multiplican */
    int		sz;		/* character representing size of operation */
    NODE	*sn;		/* source register node with variable multiplican */
    NODE	*tn;		/* temporary register node for intermediate work */
{
    num = value;		/* setup value */
    costing = 0;		/* not costing */
    size = sz;			/* size character */
    srcnode = sn;		/* source node */
    tempnode = tn;		/* temporary node */
    icongen();			/* generate the instructions */

} /* iconmulg() */

/*
 *	icongen() - function to generate/count instructions for multiply
 *
 *	Uses globally static variables num, costing, size, srcnode, tempnode.
*/
static
icongen()
{
    int cnt0;			/* zero count in multiplier */
    int cnt1;			/* ones count in multiplier */
    int initcnt1;		/* initial (most significant) string of 1's */
    int firstime;		/* first time in main loop (in case mask is 0) */
    int shift;			/* count of trailing 0's */
    unsigned long mask;		/* used to obtain one bit out of the multiplier */
    unsigned long temp;		/* multiplier shifted right so lsb is a 1 */
    unsigned tmask;		/* temporary copy of mask */

    if ( num >= 0x80000000 ) { 
/*
 *  handle negative numbers
 */
         num = -num;
         OUTPUT(negS);
    }
    if ( num == 0 || num == 1 ) 
	cerror("Ze illegal value"); /* compiler error */
    switch( size )
	{ case 'l':
		break;

	  case 'w':
		num &= 0xffff;			/* ignore useless high bits */
		break;

	  case 'b':
		num &= 0xff;			/* ignore useless high bits */
		break;

	  default:
		cerror("Ze illegal size");
		break;
	}
    temp = num;
    cnt0 = cnt1 = 0;
    shift = 0;
    mask = 1;
    while ( mask ) {
        if ( temp & mask )              /* if bit is set */
            cnt1++;                     /* increment the ones counter */
        else if ( !cnt1 )
            shift++;
        mask <<= 1;                     /* shift mask to next bit */
    }
    temp >>= shift;
/*
 *  find the most significant one
 */
    mask = 0x80000000;
    while ( !( temp & mask )) 
      	    mask = ( mask >> 1 ) & 0x7FFFFFFF; /* shift mask to next bit */
    tmask = mask;
/*
 *  count the number of 0's in the number ignoring leading and trailing 0's
 */
    while ( tmask ) {
            if (!( temp & tmask ))
                 cnt0++;
      	    tmask = ( tmask >> 1 ) & 0x7FFFFFFF;
    }
    if (( cnt0 > 1 ) && (( cnt0 +2 ) < cnt1 ) && !exceptn( temp, mask ))
        sub0s(temp);
    else if ( cnt1 != 1 ) {
/*
 *  the number is not a power of two
 */
        OUTPUT(movST);
        initcnt1 = 0;
/*
 *  count the number of ones adjacent to the most significant one
 */
        while ( mask  && (temp & mask)) {
                initcnt1++;
	        mask = ( mask >> 1 ) & 0x7FFFFFFF;
        }
        firstime = 1;
        while ( mask || firstime ) {
            firstime = 0;
            cnt0 = 0;  
/*
 *  count the number of zeroes till the next '1'
 */
            while ( mask && !(temp & mask)) {
                    cnt0++;
                    mask = ( mask >> 1 ) & 0x7FFFFFFF;
            }
/*
 *  count the number of ones till the next '0'
 */
            cnt1 = 0;
            while ( mask && (temp & mask)) {
                cnt1++;
	        mask = ( mask >> 1 ) & 0x7FFFFFFF;
            }
/*
 *  handle sequences with two groups of 1's seperated by a single 0
 */
            if ( !mask && cnt1 && cnt0 == 1 && initcnt1 >= 2 ) {
                 shiftl( cnt1 );
                 OUTPUT(addST);
                 if ( cnt0 == 1 )
                      shiftl( initcnt1 +1 ); 
                 else {
                        OUTPUT(addSS);
                        OUTPUT(addST);
                        shiftl( initcnt1 +1 ); 
                 }
                 OUTPUT(subTS);
               }
            else {
                   if ( initcnt1 > 2 ) {
                        shiftl( initcnt1 );
                        OUTPUT(subTS);
                        initcnt1 = 0;
                      }
                   else if ( initcnt1 == 2 ) {
                             OUTPUT(addSS);
                             OUTPUT(addTS);
                             initcnt1 = 0;
                   }

                   if ( cnt1 > 2 ) {
                        shiftl( cnt0 ); 
                        OUTPUT(addTS);
                        shiftl( cnt1 ); 
                        OUTPUT(subTS);
                      }
                   else if ( cnt1 == 2 ) {
                             shiftl( cnt0+1 );
                             OUTPUT(addTS);
                             OUTPUT(addSS);
                             OUTPUT(addTS);
                      }
                   else if ( cnt1 == 1 ) {
                             shiftl( cnt0+1 ); 
                             OUTPUT(addTS);
                      }
                   else shiftl( cnt0 ); 
            }
        } /* end while */
      } /* end if cnt1 >= 2 */
      if ( shift )
           shiftl( shift );
    
}

/*
 *   shiftl - function to produce assembly code for any left shift < 31 bits 
 *
 *   input parameters:
 *          siz     - the shift size.
 */
static void
shiftl( siz )
int siz;
{

    while ( siz )
      switch ( siz )
      {	case 1:
            OUTPUT(addSS);
	    siz = 0;
	    break;

	case 2:
	    OUTPUT(lsl2S);
	    siz = 0;
	    break;

	case 3:
	    OUTPUT(lsl3S);
	    siz = 0;
	    break;

	case 4:
	    OUTPUT(lsl4S);
	    siz = 0;
	    break;

	case 5:
	    OUTPUT(lsl5S);
	    siz = 0;
	    break;

	case 6:
	    OUTPUT(lsl6S);
	    siz = 0;
	    break;

	case 7:
	    OUTPUT(lsl7S);
	    siz = 0;
	    break;

	default:
	    if (siz >= 16)
	  	{    OUTPUT(swapS);
		     OUTPUT(clrS);
		     siz -= 16;
	        }
	    else
	  	{ OUTPUT(lsl8S);
		  siz -= 8;
	  	}
	    break;

	} /* end case */

} /* shiftl() */


/*
 * exceptn - returns a 1 for exceptions to the rule:
 *
 *       if ( cnt0 + 2 < cnt1 ) && (cnt0 > 1) 
 *       {
 *           subtract out the powers of two not present in the
 *           constant from one less than the power of two greater
 *           than the largest power of two in the constant. 
 *       }
 *
 *       exceptions have a sequence of '10' (at least 2)
 *            followed by a sequence of 1's (at least 3).
 *
 *       examples of exceptions :     01010111          010101111      ...
 *                                  0101010111         0101011111      ...
 *                                010101010111       010101011111      ...
 *                                      .                  .
 *                                      .                  .
 *
 *   input parameters:
 *       temp2 - input multiplier constant shifted right so that its 
 *               least significant one is the least significant bit.
 *
 *       mask  - contains a '1' in the same bit position as the input
 *               multiplier's most significant bit.
 *
 *   returns a one for exceptions else returns zero.
 */               
static
exceptn( temp2, mask )
   unsigned long temp2, mask;
{
    int maskb, cnt1, leadone;

    maskb = 1;
    cnt1 = 0;
    if ( temp2 & (mask >> 1))
         leadone = 0;
    else leadone = 1;
   
    while ( temp2 & maskb ) {
            maskb <<= 1;
            cnt1++;
          }
/*
 * find the second ones group from the right
 */
    while ( !( temp2 & maskb ) && maskb )
            maskb <<= 1;
    if (( cnt1 > 2) && ( temp2 & maskb ) && !( temp2 & (maskb << 1)) && 
          leadone || ( mask <= maskb ))
          return(1);
    else return(0);
}

/*
 *  sub0s - function to subtract out the powers of two not present in the 
 *          input multiplier.
 *
 *          example: '010111' subtract 2 to the 3rd from 2 to the 5th minus 1
 */
static void
sub0s( num )
  unsigned long num;
{
    short dist;

    dist = 0;
    OUTPUT(movST);
    while ( num ) {
        for ( ; (( num & 1 ) && num > 0 ) ; dist++ )
            num >>= 1;
        if ( dist == 1 ) {
             OUTPUT(addSS);
           }
        else {   
               shiftl( dist );
        }
        num >>= 1;
        if ( num ) {
             OUTPUT(addST);  /* add number to mask */
        }
        dist = 1;
    }
    OUTPUT(subTS);
}


/* output() - print out the given instruction string after proper substitutions
**
**	The following substitutions are recognized:
**
**		S	- source register
**
**		T	- temporary work register
**
**		Z	- size modifier for multiply width
*/

static void
output(str)
char	*str;			/* instruction image */
{
    register	c;			/* character being examined */

    while (c=*str++)			/* while not end of string */
	switch (c)
	{  case 'S':
		adrput(srcnode);	/* print source register */
		break;

	  case 'T':
		adrput(tempnode);	/* print temporary register */
		break;

	  case 'X':
		PUTCHAR(size);		/* print size out */
		break;

	  default:
		PUTCHAR(c);		/* simply issue character */
		break;
	}

} /* output */
