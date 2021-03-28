#ident	"@(#)localopt.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	localopt.c	7.1		*/
/*	Copyright 1985 Motorola Inc.			*/

#ifndef NODBG
/*F*********************** MODULE HEADER *******************************

FUNCTION NAME:

	miscellaneous assortment of functions

ACTION:

	These routines exist to support the printing of the ctrees.
	This occurs if the option "-o" is included on the command
	line.


*FE********************************************************************/

#include "hash.h"
#include "mfile2.h"

int sideff;
char *rnames[] =
{
	/* keyed to register number tokens */
	"0", "1", "2", "3", "4", "5", "6", "7", "?8", "?9", "?10"
};


conput( p )
register NODE *p;
{
	switch( p->in.op )
	{
	case ICON:
		acon( p );
		return;
	case REG:
		if( TPOINT&p->tn.type ) printf( "%%a" );
		else printf( "%%d" );
		printf( "%s", rnames[p->tn.rval] );
		return;
	default:
#ifndef NODBG
		e2print(p);
#endif
		cerror( "illegal conput" );
	}
}

insput( p )
NODE *p;
{
#ifndef NODBG
	e2print(p);
#endif
	cerror( "insput" );
}

upput( pp )
register NODE *pp;
{
	/* output the address of the second word in the
	** pair pointed to by p (for LONGs)
	*/
	register long v;
	register NODE *r, *l, *p;

	v=0;
	p = pp;
	if( p->tn.op == PLUS )
	{
		v=0;
		r = p->in.right;
		l = p->in.left;
		if( r->tn.op == ICON &&  l->tn.op == PLUS )
		{
			v = r->tn.lval;
			p = l;
			r = p->in.right;
			l = p->in.left;
		}
		if( r->tn.op == REG )
		{
			l = p->in.left;
			if( l->tn.op == REG )
			{
				printf( "%ld(%%a%d,%%d%d.l)", v, r->tn.rval,
					l->tn.rval );
				return;
			}
			if( l->tn.op != CONV ) goto ill;
			l = l->in.left;
			if( l->tn.op != REG ) goto ill;
			printf("%ld(%%a%d,%%d%d.w)",v,r->tn.rval,l->tn.rval);
			return;
		}
		if( r->tn.op != ICON ) goto ill;
		v = r->tn.lval;
		p = p->in.left;
	}
	else if( p->tn.op == MINUS )
	{
		r = p->in.right;
		if( r->tn.op != ICON || r->in.name != (char *) 0 ) goto ill;
		v = -r->tn.lval;
		p = p->in.left;
	}
	else if( p->tn.op == ASG MINUS )
	{
		r = p->in.right;
		if( r->tn.op != ICON ) goto ill;
		r = p->in.left;
		if( r->tn.op != REG ) goto ill;
		/* always do the side effect */
		else printf( "-(%%a%s)", rnames[r->tn.rval] );
		sideff = 1;  /* cream it */
		return;
	}
	else if( p->tn.op == INCR )
	{
		r = p->in.right;
		if( r->tn.op != ICON ) goto ill;
		r = p->in.left;
		if( r->tn.op != REG ) goto ill;
		if( sideff ) printf( "(%%a%s)+", rnames[r->tn.rval] );
		else printf( "(%%a%s)", rnames[r->tn.rval] );
		return;
	}
	if( p->tn.op != REG ) goto ill;
	if( v )
		printf( "%ld(%%a%s)", v, rnames[p->tn.rval] );
	else
		printf( "(%%a%s)", rnames[p->tn.rval] );
	return;
ill:
#ifndef NODBG
	e2print(pp);
#endif
	cerror( "illegal address: upput" );
}

adrput( p )
register NODE *p;
{
	/* output an address, with offsets, from p */

	while( p->in.op == FLD || p->in.op == CONV )
	{
		p = p->in.left;
	}
	switch( p->in.op )
	{

	case NAME:
		acon( p );
		sideff = 0;
		return;

	case ICON:
		/* addressable value of the constant */
		printf( "&" );
		acon( p );
		sideff = 0;
		return;

	case REG:
		conput( p );
		sideff = 0;
		return;

	case STAR:
		upput( p->in.left );
		return;

	case VAUTO:
		printf( "%ld+S%%(%%fp)", p->tn.lval );
		sideff = 0;
		return;

	case VPARAM:
		printf( "%ld(%%fp)", p->tn.lval );
		sideff = 0;
		return;

	case TEMP:
		if (p->tn.rval)
		   {
		   register int i = (int)p->tn.lval;

		   printf( "%%%c%d", ((i>7) ? 'a' : 'd'), i&7);
		   }
		else
		   printf( "%ld+T%%(%%fp)", p->tn.lval );
		sideff = 0;
		return;

	default:
#ifndef NODBG
		e2print(p);
#endif
		cerror( "illegal address" );
		return;
	}
}

acon( p )
register NODE *p;
{
	/* print out a constant */

	if( p->in.name == (char *) 0)
	{
		/* constant only */
		printf( "%ld", p->tn.lval);
	}
	else if( p->tn.lval == 0 )
	{
		/* name only */
		printf( "%s", p->in.name );
	}
	else
	{
		/* name + offset */
		printf( "%s+%ld", p->in.name , p->tn.lval );
	}
}


int regtmp = 0;


tprint( t )
register TWORD t; 
{
	 /* output a nice description of the type of t */
	static char * tnames[] = 
	{
		"undef",
		"farg",
		"char",
		"short",
		"int",
		"long",
		"float",
		"double",
		"strty",
		"unionty",
		"enumty",
		"moety",
		"uchar",
		"ushort",
		"unsigned",
		"ulong",
		"?", "?"
	};

	for(;; t = DECREF(t) )
	{
		if( ISPTR(t) ) printf( "PTR " );
		else if( ISFTN(t) ) printf( "FTN " );
		else if( ISARY(t) ) printf( "ARY " );
		else 
		{
			printf( "%s", tnames[t] );
			return;
		}
	}
}
/*
 * 	Copied form common/optim to make twopass version.
 */

ispow2( c ) 
register CONSZ c; 
{
	register i;
	if( c <= 0 || (c&(c-1)) ) return(-1);
	for( i=0; c>1; ++i) c >>= 1;
	return(i);
}

deflab(lab)
	int lab;
	{
	if (lab < 0)
		printf("L%%_%d:\n", -lab);
	else
		printf("L%%%d:\n", lab);
	}

#endif
