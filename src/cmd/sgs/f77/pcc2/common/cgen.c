/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cgen.c: version 25.1 created on 12/2/91 at 17:42:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cgen.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sgs/comp2/common:cgen.c	1.1"

/*	OLD MOT: cgen.c	7.1		*/
/*static	char	SCCSID[] = "OLD MOT:  cgen.c:	2.1 83/07/08";*/

# include "mfile2.h"
# define istnode(p) (p->in.op==REG && istreg(p->tn.rval))

int ASSIFLAG = 0;
/*
** For God's workes are, like him, all infinite
** And curious search but craftie sin's delight.
*/

rewcom( p, goal )
NODE *p; 

{
	/* find all , ops, move as high as is legal */
	/* rewrite p in place; this takes some doing! */
	/* while we are at it, take care of setting the goal field */
	int o, ty, g1, g2;
	NODE *l, *r, *ql, *qr;

	o = p->tn.op;
	g1 = g2 = NRGS;
	p->tn.goal = goal;

	/* special cases for subtrees:
	** GENBR has left as Condition codes
	** COMOP has left as Effects
	** COLON, GENUBR, CM, GENLAB have descendents = node
	** CALL, STCALL, FORTCALL has right as Effects
	** All others use registers
	*/

	switch( o )
	{

	case FREE:
		cerror( "rewcom(%d) is FREE", p-node );

	case GENBR:
		g1 = CCC;
		break;

	case COMOP:
		g1 = CEFF;
		g2 = goal;
		break;

	case COLON:
	case GENUBR:
	case CM:
	case GENLAB:
		g1 = g2 = goal;
		break;

	case CALL:
#ifdef IN_LINE
	case INCALL:		/* handle asm calls like regular calls */
#endif
	case STCALL:
	case FORTCALL:
		g2 = CEFF;
		break;
	}


	switch( ty = optype(o) )
	{

	case BITYPE:
		rewcom( r = p->in.right, g2 );
	case UTYPE:
		rewcom( l = p->in.left, g1 );
		break;
	case LTYPE:
		return;
	}

	if( o==COMOP || o==COLON || o==GENLAB ) return;

	/* look for (A,B) op C and A op (B,C), and rewrite */
	/* A,B if A headed by GENBR can't be rewritten */
	/* the assumption is that B is executed immediately after A, */
	/* and this won't necessarily be true if op is commutative */

	if( l->tn.op == COMOP && l->in.left->tn.op != GENBR ) 
	{
		/* rewrite it... */
		/* (A,B) op C => A,(B op C) */
		/* also, for unary ops,  op (A,B) => A , (op B)  */
		ql = l->in.left;
		qr = l->in.right;
		*l = *p;  /* copies op, and other stuff if op is unary */
		l->in.left = qr;
		p->in.right = l;
		p->in.left = ql;
		p->tn.op = COMOP;
		rewcom( p, p->tn.goal );
		return;	/* must not go on since p is now a COMOP! */
	}
	if( ty == UTYPE ) return;
	if( r->tn.op == COMOP && r->in.right->tn.op != GENBR ) 
	{
		/* rewrite, again */
		/* A op (B,C) => B,(A op C) */
		/* op is not unary now */
		ql = r->in.left;
		qr = r->in.right;
		*r = *p;
		p->tn.op = COMOP;
		p->in.left = ql;
		r->in.right = qr;
		rewcom( p, p->tn.goal );
	}
}

rewlhs(p)
NODE *p; 

{
	/* rewrite x op= y as (x op= y),x */
	/* it would be really nice to optimize after doing this . . . */
	NODE *q, *t;
	q = talloc();
	*q = *p;
	t = tcopy( p->in.left );
	p->in.left = q;
	p->in.right = t;
	p->tn.op = COMOP;
	return;
}

rewsto(p)
NODE *p; 
{
	/* a temp, t, is generated, and p is rewritten as ((t=p),t) */
	/* if p has the form x op= A, and x is of the right form, rewrite
	/* as ((x op= A), x) */

	int o, ao;
	NODE *t, *q;
	/* probably not perfect for structs: CHECK UP.. */

	while( (o=p->tn.op) == COMOP ) p = p->in.right;
	if( o == TEMP ) return(0);  /* nothing to do */
	if( o == STARG ) 
	{
		 /* store a structure argument */		/* like storing a usual argument, but we have addresses */

		t = talloc();
		*t = *p->in.left;  /* copy contents, mainly for type, etc. */
		q = talloc();
		*q = *t;
		t->tn.op = TEMP;
		t->tn.lval = freetemp(argsize(p)/SZINT );
		t->tn.lval = BITOOR(t->tn.lval);
#ifdef MC68K
		/* t->tn.rval == 0 indicates stack temporary (not register) */
		t->tn.rval = 0;
#endif
		t->tn.name = (char *) 0;
		t->tn.type = TSTRUCT;
		q->tn.op = UNARY AND;
		q->in.left = t;
		/* now, q has & TEMP */
		t = talloc();
		*t = *p;
		t->in.left = q;
		t->in.right = p->in.left;
		t->tn.op = STASG;
		/* now, t has (&TEMP) = struct */
		p->in.left = talloc();
		p->in.left->tn.op = COMOP;
		p->in.left->in.left = t;
		p->in.left->in.right = t = talloc(); /* copy q here */
		*t = *q;
		t->in.left = talloc();
		*t->in.left = *q->in.left;
		/* finally, have (&TEMP = struct),(&TEMP) */
		/* this should do it: whew! */
		if( odebug>1 ) e2print( p );
		return( 1 );
	}

	if( odebug>1 ) 
	{
		e2print( p );
		printf( "\nrewritten by rewsto as:\n" );
	}
	if( asgop(o) && o!=INCR && o!=DECR && lhsok( p->in.left ) ) {
		/* x op= y turns into (x op= y), x */
		rewlhs( p );
		return( 1 );
	}
	ao = ASG o;
	if( asgbinop(ao) ) 
	{
		if( p->in.left->tn.op == TEMP ) 
		{
			p->tn.op = ao;
			rewlhs( p );
			if( odebug>1 ) e2print( p );
			return( 1 );
		}
	}

	/* to rewrite in place, p becomes a COMOP; rhs is the temp, lsh
	** /* is t = p, where p has been converted to the intermediate type 
	*/
	/* after some debate, the type of the temp will be the type of p */

	t = talloc();
	*t = *p;  /* copy contents, mainly for type, etc. */
	q = talloc();
	*q = *p;
	t->tn.op = TEMP;
#ifdef MC68K
	/* see if we can allocate an unused register for the temp */
	/* if so rval == 1 && lval == register number */

	t->tn.rval = tempreg(&t->tn.lval, argsize(p)/SZINT, t->tn.type);
	if (t->tn.rval == 0)
	{
	/* if not, get one from the stack as usual (rval == 0) - jgh */
#endif
	t->tn.lval = freetemp(argsize(p)/SZINT );
	t->tn.lval = BITOOR(t->tn.lval);
#ifdef MC68K
	}
#endif
	t->tn.name = (char *) 0;
	q->tn.op = ASSIGN;
	q->in.left = t;
	q->in.right = talloc();
	*(q->in.right) = *p;
		/* now, q has (t=p) */
	p->in.right = talloc();
	*(p->in.right) = *t;
	p->tn.op = COMOP;
	p->in.left = q;
	/* this should do it: whew! */
	if( odebug>1 ) e2print( p );
	return( 1 );
}

iseff( p )
NODE *p; 

{
	/* return 1 if p has some side effects, 0 otherwise */
	int o;
	o = p->tn.op;
	if( callop(o) || asgop(o) ) return( 1 );
	switch( optype( o ) )
	{
	case BITYPE:
		if( iseff( p->in.right ) ) return( 1 );
	case UTYPE:
		return( iseff( p->in.left ) );
	}
	return( 0 );
}

NODE *
lhsto( p )
NODE *p; 

{
	/* find a piece of the LHS to be stored */
	/* if found, rewrite tree */
	NODE *q;
	int o;

	for( q = p->in.left; (o=q->tn.op)!=STAR; q=q->in.left )
	{
		if( optype(o) == LTYPE ) return( (NODE *)0);
	}
	/* q is now the * node, if there one */
	q = q->in.left;
	o = q->tn.op;
	if( optype(o) == LTYPE ) return( (NODE *)0 );
	else return( q );
}

static int
c2bigger( p ) NODE *p; {
	/* p is a conversion op; does it make things bigger */
	register TWORD t, tl;

	t = p->tn.type;
	tl = p->in.left->tn.type;

	if( (t|tl)&TPOINT ) return( 0 );  /* pointers are funny */
	if( t&TDOUBLE ) return( 1 );
	if( tl&TDOUBLE ) return( 0 );
	if( t&TFLOAT ) return( 1 );
	if( tl&TFLOAT ) return( 0 );
	if( t&(TLONG|TULONG) ) return( 1 );
	if( tl&(TLONG|TULONG) ) return( 0 );
	if( t&(TINT|TUNSIGNED) ) return( 1 );
	if( tl&(TINT|TUNSIGNED) ) return( 0 );
	if( t &(TSHORT|TUSHORT) ) return( 1 );
	return( 0 );
	}

NODE *
ind2type( p )
register NODE *p; 

{
	/* make the type of p be the appropriate type for an argument */
	register TWORD t;
	NODE *q;

	t = p->tn.type;
/*
	if( t == TPOINT )fprintf(stderr,"ind2type changes TPOINT to TINT\n");
*/
	if( t == TPOINT || t == TCHAR || t == TSHORT ) t = TINT;
	else if( t == TUCHAR || t == TUSHORT ) t = TUNSIGNED;
	else if( t == TFLOAT ) t = TDOUBLE;
	else return( p );

	if( p->tn.op == CONV && c2bigger(p) ) 
	{
		p->tn.type = t;
		return( p );
	}
	q = talloc();
	q->tn.op = CONV;
	q->in.left = p;
	q->in.right = 0;
	q->tn.name = (char *) 0;
	q->tn.type = t;
	q->tn.goal = NRGS;
	return( q );
}

NODE *
reweop( p )
register NODE *p; 

{
	/* rewrite A op= B as A = A op B */
	/* also, rewrite (CONV A) op= B as A = (CONV ( (CONV A) op B ) ) */
	/* rewritten in place */
	/* on input, the type of the op= equals the type of A */
	/* the type of the op node on output is the type of B */
	/* the type of the = node on output is the type of A */

	register NODE *q, *r, *s, *t;
	register TWORD ty;

	if( odebug>1 ) 
	{
		e2print( p );
		printf( "\nrewritten by reweop as:\n" );
	}
	q = talloc();
	*q = *p;	/* Copy to get type and op info */
	r = p->in.right;
	s = tcopy( p->in.left );
	q->in.left = s; /* q->in.right = r, side effect of copy. */
	if( p->in.left->tn.op == CONV ) 
	{
		/* ( CONV (A) ) op= B becomes A = CONV ( CONV(A) op B ) */
		/* the op is done to the type of B */
		/* the assignment is done to the type of A */
		t = p->in.left;
		ty = t->in.left->tn.type;
		p->in.left = t->in.left;
		p->in.right = t;
		t->in.left = q;
		/* now, have the tree built; fix the types */
		t->tn.type = ty;
	}
	else 
	{
		p->in.right = q;
	}
	/* NOTE: no =ops for structures... */
	p->tn.op = ASSIGN;
	q->tn.op = NOASG q->tn.op;
	p->tn.type = p->in.left->tn.type;
	q->tn.type = q->in.right->tn.type;
	if( odebug>1 ) e2print( p );
}

rewass( p )
NODE *p; 

{
	NODE *q;
	int o;
	/* look for =ops to be rewritten */

	if( odebug ) 
	{
		printf( "rewass called with:\n" );
		e2print( p );
	}
	o = p->tn.op;
	if( o == UNARY AND ) 
	{
		if( p->in.left->tn.op == RNODE ) 
		{
			/* this should happen only with structure returns */
			q = p->in.left;
			q->tn.op = ICON;
			*p = *q;
			q->tn.op = FREE;
			return(0);  /* keep going in costs */
		}
		/* this case should happen only with short structures */
		rewsto( p->in.left );
		/* & f() has turned into & ( t=f(),t) */
		if( odebug ) 
		{
			printf( "\nrewritten by rewass as:\n" );
			e2print( p );
		}
		return(1);
	}
	if( !asgop(o) || o==ASSIGN ) 
	{
		if( o==ASSIGN ) 
		{
			/* look for funny nodes on lhs */
			o = p->in.left->tn.op;
			if( o==RNODE || o==QNODE || o==SNODE ) 
			{
				/* force into r0 */
				p->in.left->tn.op = REG;
				p->in.left->tn.rval = callreg( p->in.right );
				if( odebug ) 
				{
					printf( "funny node redone\n" );
					e2print(p);
				}
				return(0);
			}
		}
		else 
ASSI:
		{
			TWORD t = p->in.left->tn.type;
  			TWORD t2= p->in.right->tn.type;
			/* this case is, for example, 
				unsigned char a, b;
				...   a*b
			/* we convert both to a reasonable type */
			/* the result is assumed to be automatically
			/* converted downwards if it should be... */

			p->in.left = ind2type( p->in.left );
			p->in.right = ind2type( p->in.right );
			if( odebug ) {
				printf( "conversions inserted" );
				e2print(p);
			}
			/* if this didn't work, we are in trouble */
  			if( ( t  != p->in.left->tn.type ) ||
  			    ( t2 != p->in.right->tn.type) )
  			    {
				/* we have changed something */
				return( 0 );
			}
		}
/* Tue Dec 31 09:30:20 PST 1985 hanna
 * Give assigns one more chance.
 * On very complicated, nested QUEST operations that return (void), but
 * have side effects like incrementing a val, these sometimes got confused
 * but if they went through and tried to reorder the sides one more time
 * the solution was found.
 * This is a klugey solution, but seems to work until the real soln is found.
 */
		if (p->tn.op == ASSIGN && !ASSIFLAG){
/*fprintf(stderr,"ENTERING ASSIFLAG\n");*/
			ASSIFLAG++;
if(odebug){
	printf("ENTERING ASSIFLAG\n");e2print(p);
	}
			goto ASSI;
		}

		printf("ASSIFLAG FAILED\n");
		e2print(p);
		cerror( "can't deal with op %s", opst[o] );
	}
	if( o == INCR || o == DECR ) 
	{
		/* very crude: a++ becomes (a+=1)-1 */
		if( odebug>1 ) 
		{
			e2print( p );
			printf( "\nrewritten by rewass as:\n" );
		}
		q = tcopy(p);
		regrcl( p->in.left );
		tfree( p->in.left );
		p->in.left = q;
		q->tn.op = ((o==INCR)?ASG PLUS:ASG MINUS);
		p->tn.op = ((o==INCR)?MINUS:PLUS);
		if( odebug ) 
		{
			printf( "\nrewritten by rewass as:\n" );
			e2print( p );
		}
		return(1);
	}
	/* find out if some subtree has to be stored into a temp... */
	if( q = lhsto(p) ) 
	{
		if( !rewsto( q ) ) cerror( "rewass0" );  /* q => t=q,t */
		rewcom( p, p->tn.goal );  /* move COMOP to the top */
		if( p->tn.op != COMOP ) cerror( "rewass1" );
		if( !asgop( p->in.right->tn.op ) ) cerror( "rewass2" );
		reweop( p->in.right );
	}
	else reweop( p );  /* rewrite p as an =OP */
	return(1);
}

# ifdef NONEST
subcall( p )
register NODE *p; 

{
	/* return 1 if p contains a callop */
	register o, t;

	o = p->tn.op;
	if( callop(o)
# ifndef UCALLBAD
	    && o!=UNARY CALL
# endif
# ifdef IN_LINE
	    && o!=UNARY INCALL	/* pretend asm calls are regular calls */
# endif
# ifndef STCALLBAD
	    && o!=UNARY STCALL
# endif
# ifndef UFCALLBAD
	    && o!=UNARY FORTCALL
# endif
	    ) return( 1 );
	t = optype( o );
	if( t==BITYPE && subcall(p->in.right) ) return( 1 );
	if( t!=LTYPE ) return( subcall( p->in.left ) );
	return( 0 );
}

nonest( p )
register NODE *p; 

{
	register o, t;
	/* right now, this is very crude */
	/* find arguments below a call; store them */
	/* nonest is called up to a call; stocm is called within args */

	o = p->tn.op;
	t = optype( o );
	if( o==CALL || o==STCALL || o==FORTCALL) 
	{
		stocm( p->in.right );
		nonest( p->in.left );
		return;
	}
	if( t == BITYPE ) nonest( p->in.right );
	if( t != LTYPE ) nonest( p->in.left );
}

stocm( p )
register NODE *p; 

{
	/* all call arguments below p must be stored */
	register NODE *q;
	register o;

	while( (o=p->tn.op) == CM )
	{
		stocm( p->in.right );
		p = p->in.left;
	}
	if( o != STARG && o != FUNARG ) cerror( "stocm" );

	q = p->in.left;
	if( subcall( q ) ) 
	{
		if( o == FUNARG ) rewsto( q );
		else 
		{
			/* structure argument with call beneath */
			rewsto( p );
		}
		/* now q will be done outside of a call, so use nonest */
		nonest( q );
	}
}
# endif

outshp( pp )
SHAPE **pp; 

{
	SHAPE *p;

	if (pp == 0)
		return;

	for( ; p = *pp; ++pp )
	{
		printf("\t\t");
		shpr(p);
		printf( " (%d)\n", p->sc );
	}
}

tabpr()
{
	register	OPTAB	*p;
	for (p =table; ;p++)
	{
		printf("Dump of table[%d] (stinline %d)\n", p-table, p->stinline );
		printf("\top = %s\n", opst[p->op]);
		printf("\tnextop = %d\n", p->nextop?p->nextop-table:-1 );
		printf("\tlshape = %d\n", p->lshape-pshape);
		printf("\tltype = 0%o\n", p->ltype);
		printf("\trshape = %d\n", p->rshape-pshape);
		printf("\trtype = 0%o\n", p->rtype);
		printf("\tneeds = %d\n", p->needs);
		printf("\trewrite = %d\n", p->rewrite);
		printf("\tcstring = %s", p->cstring);
		printf("\tcost = %d\n", p->cost);
		printf("\tLeft:\n");
		outshp(p->lshape);
		printf("\tRight:\n");
		outshp(p->rshape);
		printf("\n");
	}
}

codgen( p )
NODE *p; 

{

	/* generate the code for p; */
	int i, flag;

	if (odebug > 5)
	{
		tabpr();
		/* NOTREACHED */
	}

# ifndef IN_LINE	/* nonest interferes with asm expansion */
# ifdef NONEST
	nonest(p);
# endif
# endif

	/* if we make drastic changes to the tree (e.g., introduce temps)
	** /* we will go back and do the whole thing again 
	*/
	/* statistics indicate that this happens about 10% of the time */
	/* if the percentage rises, there are many things that can be done to
	** /* improve matters 
	*/
	/* for example, RNODE, etc., could be removed by reader, and some of
	** /* the op rewriting could be discovered by reader as well 
	*/

again:

	/* move the comma ops as high as practical */

	rewcom( p, CEFF );

	if( odebug ) 
	{
		printf( "After goals are computed:" );
		e2print( p );
	}

	/* compute the costs */

	if( costs( p ) ) goto again;  /* if rewritten, do again */

	if( odebug ) 
	{
		printf( "After costs are computed:" );
		e2print( p );
	}

	/* do a trial code generation */
	nins = 0;
	insout( p, CEFF );

	/* rewrite stored subtrees as assignments to temps, with COMOP's */
	flag = 0;
	for( i=0; i<nins; ++i ) 
	{
		if( inst[i].goal == CTEMP ) 
		{
			if( odebug ) 
			{
				printf( "subtree is stored in temp:\n" );
				e2print( inst[i].p );
			}
			if( rewsto( inst[i].p ) ) {
				if( !fast ) goto again;
				/* otherwise, rewrite all temps now */
				flag = 1;
			}
		}
	}
	if( flag ) goto again;

	if( odebug ) e2print(p);
	/* output the actual instructions */
	insprt();
}

INST inst[NINS];
int nins;
/********************************DEBUG**************************/

insprt()
{

	int i, ninsav;
	register INST *pi;
	register NODE *p;
	register OPTAB *q;
	register c, goal;
#ifdef	CG
	int any_exact;			/* Flag:  on if any DOEXACTs in tree */
#endif

	/* don't use nins for loop control in case IN_LINE is defined,
	/* giving the possibilty that nins gets changed, and insprt()
	/* gets called recursively.
	*/
	ninsav = nins;

	for( pi=inst,i=0; i<ninsav; ++i,++pi )
	{
		p = pi->p;
		q = pi->q;
		c = pi->goal;
		if( c == CCC && (q->rewrite&RESCC) ) goal = FORCC;
		else if( c == CEFF ) goal = FOREFF;
		else goal = INREG;
#ifndef NODBG
		if(odebug > 4)
		{
			printf("INSOUT: %d c=",i);
			preff(c);
			printf(" goal=");
			prgoal(goal);
			printf("\n");
			e2print(p);
		}
#endif

#ifdef IN_LINE
	        if (p->in.op == INCALL || p->in.op == UNARY INCALL)
		{
		    /* handle asm inline calls here, don't use expand() */
		    int n;
		    INST savinst[NINS];
		    char *memcpy();

		    /* have to preserve inst[] since 
		    /* it may be overwritten when insout() is called
		    /* to generate code for argument evaluation or
		    /* a real function call
		    */
		    n = sizeof(INST) * (ninsav-i);
		    (void) memcpy( (char*)savinst, (char*)(pi), n);

		    if (genicall(p,c))
		    	cerror("Can't expand asm function call");


		    /* At this point the asm call has either been expanded
		    /* or converted to a regular CALL or UNARY CALL
		    */

		    if (p->in.op == UNARY CALL || p->in.op == CALL)
		    {
		    	/* have to produce code for regular function call */
#ifndef	STINCC
			costs(p);
		    	nins = 0;
		    	insout(p,CEFF); 
#else
		    	nins = 0;
			if (INSOUT(p,CEFF))	/* expect zero return */
				cerror("Can't generate in-line CALL");
#endif
		    	insprt();  
		    }

		    /* put everything back the way it was */
		    (void) memcpy( (char*)(pi), (char*)savinst, n);
		    nins = ninsav;  /* to be good and safe */

		    allo( p, q );		/* safe to allocate now */

		 }
		else	
#endif
		{ /* non-asm template expansion */

		allo( p, q );

#ifdef CG
			/*If there are any DOEXACTs, protect them
			  from optimizers*/
			/*Also, if there are any ";lp" that have a
			  null left child, remove them from the tree*/

		if ( (any_exact = pre_ex(p)) != 0)
			protect();
#endif

# ifdef TMPSRET
		/* not the best place in the world, but... */
		if (p->in.op == STCALL || p->in.op == UNARY STCALL)
			expand(p, goal, TMPSRET, q);
#endif
		/* Look for the case where we wanted to use an ASGOP template
		** for a non-ASGOP tree.  If the left side of the tree is a
		** register variable, get it into a scratch register.
		*/

#ifndef	REGSET
/* REGSET guarantees things are in suitable register */
		if (   asgop(q->op)
		    && !asgop(p->in.op)
		    && p->in.left->tn.op == REG
		    && !istreg(p->in.left->tn.rval)
		    )
		    cfix( p->in.left, NRGS );
#endif	/* REGSET */

		expand( p, goal, q->cstring, q );

#ifdef	CG
		if ( any_exact)
			unprot();
#endif
		} /* end non-asm template expansion */


		reclaim( p, q->rewrite, goal );

		/* now, if condition codes needed, test */
		if( c == CCC && p->tn.op != CCODES ) 
		{
			cfix( p, CCC );
			if( p->tn.op != CCODES ) cerror( "ctest fails" );
		}
		/* sw7 */
		else if( c>=0 && c<=NRGS && !istnode( p )) {
                /*  if (p->tn.op != REG ) */
			cfix( p, NRGS );
#ifdef	REGSET
		    /* make sure we're in a suitable register */
		    if ((pi->rs_want & RS_BIT(p->tn.rval)) == 0) {
			int regno = rs_rnum( RS_CHOOSE(pi->rs_want) );

			RS_MOVE(p, regno);
			reallo(p, regno);	/* adjust reg. allo. info. */
		    }
#endif	/*REGSET*/
		}
	}
}


/********************************DEBUG**************************/

SHTABLE sha;
int odebug = 0;

cfix( p, goal )
NODE *p; 
{
	/* p is to be fixed according to goal (CCC or NRGS) */
	OPTAB *q;
	NODE *pp;
	int r;

#ifndef NODBG
	if(odebug > 4)
	{
		printf("CFIX: goal=");
		prgoal(goal);
		printf("\n");
		e2print(p);
	}
#endif
	if( goal == CCC ) 
	{
		r = RESCC;
		p->tn.goal = CCC;
	}
	else 
	{
		r = (RESC1|RESC2|RESC3);
		pp = getl( p );
		if( istnode( pp ) ) r |= RLEFT;
		pp = getr( p );
		if( istnode( pp ) ) r |= RRIGHT;
	}

	if( goal == CCC ) goal = FORCC;
	else goal = INREG;

	for( q=0; q = match( p, q ); )
	{
		/* takes the first match (may not be cheapest) */
		/* template writers, take note! */
		if( q->rewrite & r ) 
		{
			/* generate the code on the spot */
			allo( p, q );
			expand( p, goal, q->cstring, q );
			reclaim( p, q->rewrite, goal );
			return;
		}
	}
	e2print(p);
	cerror( "cfix trouble" );
}


preff(c)
{
	char buf[20];
	register char *p;

	p = c==CCC ? "CCC" : c==CTEMP ? "CTEMP" : c==CEFF ? "CEFF" : 0;
	if(!p)
	{
		sprintf(buf,"0%o",c);
		p = buf;
	}
	printf("%s",p);
}
