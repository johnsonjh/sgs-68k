#ident	"@(#)local.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.1	89/06/08 00:38:45 root
	date and time created 89/06/08 00:38:45 by root

*/
/*	@(#)local.c	7.2		*/
static	char	SCCSID[] = "@(#) local.c:	@(#)local.c	7.2	";

# include "mfile1.h"
#include "storclass.h"

extern int compiler_mode;
/*	the following used to be on the old local.c file */
/* the register variables are %d2 through %d7 and %a2 through %a5 */
/* everything is saved on the stack as a 4 byte quantity */
/* the machine-independent part maintains a quantity regvar that is */
/* saved and restored across block boundaries */
/* if cisreg likes the type, it calls rallc to get a register number */
/* rallc puts the register number into nextrvar, and updates regvar */
/* rallc returns 0 if there are no free regs of that type, else 1 */

#define EXPSHIFT 7
#define	EXPMASK	(0xff << EXPSHIFT)	/* mask to access exponent on VAX */
#define EXPLORD ( 1L << EXPSHIFT )	/* low order exponent bit on VAX */
#define SIGNMASK ( 1L << 15 )		/* Vax sign bit */
fincode( d, sz )
double d;
{
	/* initialization of floats */
	union flt_dbl x;

	x.d = d;
#if vax
	dbltolong( &x, sz );
	if ( sz == SZDOUBLE )
	{
	   printf("	long	0x%lx\t# %e\n", x.lg.l, d );
	   printf("	long	0x%lx\n", x.lg.l2);
	}
	else
	{
	   printf( "	long	0x%lx\t# %e\n", x.lg.l, d );
	}
#else		/* m68k */
#ifndef ONEFLOAT
	if ( sz == SZDOUBLE )
	{
	   x.d = d;
	   printf("	long	0x%lx\t# %e\n", x.lg.l, d );
	   printf("	long	0x%lx\n", x.lg.l2);
	}
	else
#endif
	{
	   x.f = d;
	   printf("	long	0x%lx\t# %e\n", x.lg.l, d );
	}
#endif
	inoff += sz;
}

#ifdef vax
dbltolong ( x, sz )
union flt_dbl *x;
int sz;
{
	long z;
	register long exponent,fract1,fract2,sign;

	    				/* decr. exponent by two to get IEEE
					** format
					*/
	exponent = ( (x->lg.l&EXPMASK) >> EXPSHIFT ) -2;
	if ( x->d == 0e0 ) exponent=0;	/* zero exponent for 0.0 */
	sign     = ( x->lg.l&SIGNMASK ) << 16;	/* vax sign bit to high bit */
	fract1   = ( (x->lg.l&0x7F)<<16 ) | (  ( x->lg.l >> 16 )&0xFFFF );
	fract2   = ( (x->lg.l2&0xFFFF)<<16 ) | (  ( x->lg.l2 >> 16 )&0xFFFF );
#ifndef ONEFLOAT
	if ( sz == SZDOUBLE )
	{
	   exponent = exponent + 896;	/* convert to double bias */
	   if ( x->d == 0e0 ) exponent=0;	/* zero exponent for 0.0 */
	   if ( (exponent&0x7ff) != exponent )
	     {
	     cerror("Bad convert to double");
	     }

	   x->lg.l = ( sign ) |
		    (  ( exponent&0x7ff ) << 20 ) |
		    ( (fract1 >>3 ) & 0x1fffffff );
	   x->lg.l2= ( (fract1&0x7)<<29 ) |
		    ( (fract2 >> 3) & 0x1fffffff );

	}
	else
#endif
	{
	   x->lg.l =   sign  |
		    (exponent<<23) |
		    ( fract1 );
	}
}
#endif

rallc( t )
register TWORD t;
{
	register int i;
	if( ISPTR( t ) )
	{
		/* address register */
		for( i=2; i<=5; ++i )
		{
			if( regvar & (0400<<i) ) continue;
			nextrvar = i;
			regvar |= (0400<<i);
			return(1);
		}
		return(0);
	}
	else 
#ifdef M68881
		if( FP_TYPE( t ) )
		{
			if( is_m68881 )
			{
				/* coprocessor fp register */
				for( i=2; i<=7; ++i )
				{
					if( regvar & (0200000<<i) ) continue;
					nextrvar = i;
					regvar |= (0200000<<i);
					return(1);
				}
				return(0);
			}
		}
		else
#endif
		{
			/* data register */
			for( i=2; i<=7; ++i )
			{
				if( regvar & (1<<i) ) continue;
				nextrvar = i;
				regvar |= (1<<i);
				return( 1 );
			}
			return(0);
		}
}

NODE *
clocal(p)
register NODE *p;
{
#ifdef NEVER			/* bk's fault */
	register NODE *l;
	register TWORD t, lt;

	if ( p->in.op == CONV ) {
		l = p->in.left;
		lt = l->tn.type;
		t = p->tn.type;

				/* bk: Get rid of CONV done involving INT<->LONG. */
				/* CONV nodes left by makety for reasons of syntax checking */
				/* (i.e. cast expressions used as lvalues) */
		if( (BTYPE(lt) == INT && BTYPE(t) == LONG) ||
			(BTYPE(lt) == LONG && BTYPE(t) == INT) ) {

			l->tn.type = p->tn.type;
			l->fn.csiz = p->fn.csiz;
			l->fn.cdim = p->fn.cdim;
			p->tn.op = FREE;
			return( l );
		}
	}
#endif
	return(p);
}

myinit( p, sz )
register NODE *p;
{
	register TWORD t;
	register NODE *q;
	if( p->in.left->tn.op != ICON ) yyerror( "illegal initializer" );
	t = p->tn.type;
	q = p->in.left;
	inoff += sz;
	if( q->tn.rval != NONAME )	/* do real pointers in pass 2 */
	{
	    ecode( p );
	    return;
	}

/* NOTE:  assembler will complain about constants that are too big
** for the type of item that is being initialized (like "byte 0x1000").
** The compiler should produce a message about this some day.
*/

	switch( t )
	{
	case CHAR:
	case UCHAR:
		printf( "	byte	%ld\n", q->tn.lval );
		return;

#ifdef	INT16BIT
	case INT:
	case UNSIGNED:
#else
	case SHORT:
	case USHORT:
#endif
		printf( "	short	%ld\n", q->tn.lval );
		return;

	default:
		if( !ISPTR(t) )
			werror( W_DFLT, "initialization error probable");
		/* FALLTHRU */

#ifndef	INT16BIT
	case INT:
	case UNSIGNED:
#endif
	case LONG:
	case ULONG:
		printf( "	long	%ld\n", q->tn.lval );
		return;
	}
}

/* bk: changed to return -1 if user's requesting register for */
/* incompatible type (we return 0 if plain out of allocatable registers) */
cisreg( t )
register TWORD t;
{
	/* is an automatic variable of type t OK for a register variable */

/*	if ((IS_NOT_SCALAR(type)) ||
		(ISARY(type)))
		werror( W_DFLT, "illegal register declaration: ignored" ); */

	switch( t )
	{
	default:
		if( !ISPTR(t) ) return( -1 );
		/* FALLTHRU */
			/* we should allow enum type to reside in reg */
			/* if it has been declared so */
	case ENUMTY:	/* hoss#7 */
	case CHAR:
	case UCHAR:
	case SHORT:
	case USHORT:
	case INT:
	case UNSIGNED:
	case LONG:
	case ULONG:
	case FLOAT:
		return(rallc(t));
		break;
#if defined( ONEFLOAT )| defined( M68881 )
	case DOUBLE:
#ifdef M68881
		if( is_m68881 )
			return(rallc(t));
		else
			return( 0 );
#else
		return(rallc(t));
#endif
#endif
	}
}

static int	inwd;	/* current bit offset in word */
static long	word;	/* word being built from fields */

incode( p, sz )
register NODE *p;
{
	register v;

	/* generate initialization code for assigning a constant c
	** to a field of width sz 
	*/
	/* we assume that the proper alignment has been obtained */
	/* inoff is updated to have the proper final value */
	/* go to some pain to handle the left-to-right fields, and defend
	/* against possible long bitfield initialization */
	/* ordinary (int sized) fields are
	/* left to right for the 68000 */

	v = p->tn.lval & ((1<<sz)-1);
	/* hoss#14 */
	if ( inwd + sz > SZLONG ) 
	{/* crossing boundaries */
		word |= v >> (sz - (SZLONG - inwd));
		if( inwd == SZSHORT )
			printf( "	short	0x%.4x\n",
				(unsigned short)(word>>SZSHORT));
		else
			printf( "	long	0x%.8lx\n", word);
		inwd = (sz - (SZLONG - inwd));
		word = v << (SZLONG - inwd);
		inoff += sz;
		return;
	}
	if( SZLONG-inwd-sz ) v <<= (SZLONG-inwd-sz);
	word |= v;
	inwd += sz;
	inoff += sz;
	if( inoff%ALSHORT == 0 )
	{
		if( inwd == SZSHORT )
			printf( "	short	0x%.4x\n",
				(unsigned short)(word>>SZSHORT));
		else
			printf( "	long	0x%.8lx\n", word);
		word = inwd = 0;
	}
}

vfdzero( n )
register n;
{
	/* define n bits of zeros in a vfd */

	if( n <= 0 ) return;
	inwd += n;
	inoff += n;
	if( inoff%ALSHORT == 0 )
	{
		if( inwd == SZSHORT )
			printf( "	short	0x%.4x\n",
				(unsigned short)(word>>SZSHORT));
		else
			printf( "	long	0x%.8lx\n", word);
		word = inwd = 0;
	}
}

char *
exname( p )
char *p;
{
	return( p );
}

commdec( id )
{
	/* make a common declaration for id, if reasonable */
	register struct symtab *q;
	register OFFSZ sz;

	q = &stab[id];
	sz = tsize( q->stype, q->dimoff, q->sizoff )/SZCHAR;

# ifndef NOSYMB
	if ( q->slevel == 0)
		    prdef(q, 0);	/* output debug info */
# endif
	if( q->sclass==STATIC ) 
	{
		/* always local */
		if( q->slevel ) printf( "\tlcomm\tL%%%d,%ld\n", q->offset, sz );
		else
		        printf( "\tlcomm\t%s,%ld\n", exname( q->sname ), sz );
	}
	else
		printf( "	comm	%s,%ld\n", exname( q->sname ), sz );
}

/* the following is the stuff on the old code.c file */

branch( n )
{
#ifdef ITREES
	register NODE	*p,*q;
#endif
	/* output a branch to label n */
	if( !reached ) return;
#ifdef ITREES
	q = talloc();
	q->tn.op = ICON;
	q->tn.type = INT;
	q->tn.lval = 1;
	q->tn.rval = NONAME;
	p = talloc();
	p->tn.type = INT;
	p->tn.rval = n;
	p->in.left = q;
	p->tn.op = GENUBR;
#ifdef TWOPASS
	locctr(PROG);
	printf("%c%d\t%s\n", EXPR, lineno, ftitle);
	prtree(p);
	tfree(p);
#else
	p2compile( p );
#endif
#else
	printf( "	br	L%%%d\n", n );
#endif
}

deflab( n )
{
	/* output something to define the current position as label n */
#ifdef ITREES
	register NODE *p,*q;
	/*	pass tree GENLAB(ICON 1) to backend. */
	q = talloc();
	q->tn.op = ICON;
	q->tn.type = INT;
	q->tn.lval = 1;
	q->tn.rval = NONAME;
	p = talloc();
	p->tn.type = INT;
	p->tn.rval = n;
	p->in.left = q;
	p->tn.op = GENLAB;
#ifdef TWOPASS
	printf(".%d\t%s\n", lineno, ftitle);
	prtree(p);
	tfree(p);
#else
	p2compile( p );
#endif
#else
	/*	write code string from front end */
	printf( "L%%%d:\n", n );
#endif
}

defalign(n)
register n;
{
	/* cause the alignment to become a multiple of n */
	n /= SZCHAR;
	if (curloc != PROG)
	{
#ifdef m68k
	    if (n>2) printf( "\talign\t%d\n",n );
/*	    if (n>2) printf( "\tlongeven\n" ); 	ken#0:1 */
		else
#endif
	    if (n>1) printf( "\teven\n" );
	}
}

char *locnames[] = 
{
/* location counter names for PROG, ADATA, DATA, ISTRNG, STRNG, TEXT2, STRWRT */
	"	text\n",
	0,
	"	data	1\n",
	0,
	"	text	3\n",		/* bk: for readonly string literals */
	"	text	2\n",		/* bk: for global "const" data */
	"	data	2\n",		/* for writable string literals */
};

eccode(blevel)
int blevel;
{
				/* called at end of every block */
#ifdef TWOPASS
	/* print out the register mask of surrounding block */
	if (blevel != 0)
		printf("]%d\t%d\t\n", blevel, regvar);
#endif
}

efcode()
{
	/* code for the end of a function */

	deflab( retlab );  /* define the return location */

	efdebug();		/* end of function debug info */

#ifdef ITREES
	{
	register NODE *p;

	p = talloc();
	p->tn.op = EPILOG;
	p->tn.type = INT;
	p->tn.lval = strftn;
	p->tn.rval = ftntype;
# ifdef TWOPASS
	locctr(PROG);
	printf("%c%d\t%s\n", EXPR, lineno, ftitle);
	prtree(p);
	tfree(p);
# else
	p2compile( p );
# endif
	}
#else
	if( strftn )
	{
		  /* copy output (in r0) to caller */
		printf( "	mov.l	&__StRet,%%a0\n" );
	}

	if ( ISPTR(ftntype) || strftn )
	{
		/* copy A0 to D0 for more reliability in returns */
		/* from functions that return pointers.		 */
		printf("	mov.l	%%a0,%%d0\n");
	}
	printf( "	movm.l	S%%%d(%%fp),&M%%%d\n", ftnno, ftnno );
	printf( "	unlk	%%fp\n" );
	printf( "	rts\n" );
#endif
	eobl2dbg();			/* very end debug code */
}

bfcode( a, n )
int a[];
{

	/* code for the beginning of a function; a is an array of
	** indices in stab for the arguments; n is the number 
	*/

#ifdef	STKCK	
	/* bk: STKCK define not used anymore; fix this if one-pass desired */
	register	spgrow_lbl;
	extern int stkckflg;
#endif

#ifdef ITREES
	register NODE *p;

	bfdebug( a, n );		/* do debug info at fcn begin */
	p = talloc();
	p->tn.op = PROLOG;
	p->tn.type = INT;
	p->tn.rval = ftnno;
	p->tn.lval = 0;
# ifdef PROFILE
	{
	extern int proflg;
	p->tn.lval |= (proflg != 0);
	}
# endif
# ifdef STKCK
	p->tn.lval |= ((stkckflg != 0) << 1);
# endif
# ifdef TWOPASS
	locctr(PROG);
	printf("%c%d\t%s\n", EXPR, lineno, ftitle);
	prtree(p);
	tfree(p);
# else /* TWOPASS */
	p2compile( p );
# endif /* TWOPASS */

#else /* ITREES */

	bfdebug( a, n );		/* do debug info at fcn begin */
#ifdef	PROFILE
	extern int proflg;

	/*
	** code to provide profiling via prof(1)
	**
	*/
	if( proflg )
	{
		register int i;

		i = getlab();
		printf("	mov.l	&L%%%c%d,-(%%sp)\n",'P',i); /* ken#8 */
		if ( compiler_mode = MODE_STRICT_ANSI )
		printf("	jsr	_mcount%%\n");
		else
		printf("	jsr	mcount%%\n");
		printf("	add.w	&4,%%sp\n"); 		    /* ken#8 */
		printf("	data\n");
		printf("	even\n");
		printf("L%%%c%d:\n", 'P', i);
		printf("	long	0\n");
		printf("	text\n");
	}
#endif /* PROFILE */

#ifdef	STKCK

	/*
	** code needed for M68000 to check stack growth
	** against current splimit%.  if we are about to
	** exceed current limit, ask system for more stack.
	**
	*/

	if( stkckflg ) 			/* if stack checking requested */
	{
		spgrow_lbl = getlab();
		printf( "	lea.l	F%%%d-256(%%sp),%%a0\n", ftnno );
		printf( "	cmp.l	%%a0,splimit%%\n" );
		printf( "	bhi.b	L%%%d\n", spgrow_lbl );
		printf( "	jsr	spgrow%%\n" );
		deflab( spgrow_lbl );
	}
#endif /* STKCK */

	/* routine prolog */

	printf( "	link	%%fp,&F%%%d\n", ftnno );
	printf( "	movm.l	&M%%%d,S%%%d(%%fp)\n", ftnno, ftnno );
#endif /* ITREES */

	retlab = getlab();

}

defnam( p )
register struct symtab *p;
{
	/* define the current location as the name p->sname
	 * first give the debugging info for external definitions
	 */

	if( p->slevel == 0 )	/* make sure it's external */ /* bk: was lvl == 0 */
		prdef(p,0);

	if( p->sclass == EXTDEF )
		printf( "	global	%s\n", exname( p->sname ) );

	printf( "%s:\n", exname( p->sname ) );
}

/* hoss#21 */
static char	*xtab[] = 
{
	"0x00", "0x01", "0x02", "0x03", "0x04", "0x05", "0x06", "'\\a",
	"'\\b","'\\t", "'\\n", "'\\v", "'\\f", "'\\r", "0x0e", "0x0f"
};
bycode( t, i )
register t,i;
{
	/* put byte i+1 in a string */

	i &= 07;
	if( t < 0 )
	{
		 /* end of the string */
		if( i != 0 ) printf( "\n" );
	}
	else 
	{
		 /* stash byte t into string */
		if( i == 0 )
			printf( "	byte	" );
		else
			printf( "," );
		if((t=='\\') || (t=='\''))
			printf("'\\%c",t);
		else if((t > ' ') && (t <= '~'))
			printf( "'%c",t);
		else if(t <= 0x0f)
			printf("%s",xtab[t]);
		else
			printf( "0x%x", t );
		if( i == 07 )
			printf( "\n" );
	}
}

zecode( n )
register n;
{
	/* n integer words of zeros */
	register OFFSZ temp;

	if( n <= 0 ) return;
	printf( "	space	%d\n", (SZINT/SZCHAR)*n );
	temp = n;
	inoff += temp*SZINT;
}

main( argc, argv )
char *argv[]; 
{
	register r;
#ifdef	BLIT
	char errbuf[BUFSIZ];

	setbuf(stderr, errbuf);
#endif

	r = mainp1( argc, argv );
	return( r );
}


/*
**	generate switch/case construct
*/
genswitch(p, n)
register struct sw	*p;
{
	/*	p points to an array of structures, each consisting
	** of a constant value and a label.
	** The first is >=0 if there is a default label;
	** its value is the label number
	** The entries p[1] to p[n] are the nontrivial cases
	*/
	register	i;
	register long	j, range;
	register	dlab, swlab; /* default label, bottom-of-switch label */

#ifdef ITREES
	register NODE *t, *q;

	/* build a SWBEG [ SWDEF ] SWCASE SWCASE ... SWCASE SWEND tree */

	t = talloc();
	t->tn.op = SWEND;
	t->tn.type = INT;
	for (i=n; i>0; i--)
	   {
	   q = talloc();
	   q->in.op = SWCASE;
	   q->tn.rval = p[i].slab;
	   q->in.type = p[i].sval;	/* sorry about this !! */
	   q->in.left = t;
	   t = q;
	   }
	if (p->slab >= 0)
	   {
	   q = talloc();
	   q->in.op = SWDEF;
	   q->in.type = INT;
	   q->tn.rval = p[i].slab;
	   q->in.left = t;
	   t = q;
	   }
	q = talloc();
	q->in.op = SWBEG;
	q->in.type = INT;
	q->in.left = t;
	t = q;
# ifdef TWOPASS
	locctr(PROG);
	printf("%c%d\t%s\n", EXPR, lineno, ftitle);
	prtree(t);
	tfree(t);
# else
	p2compile( t );
# endif
#else
	range = p[n].sval - p[1].sval;

	/* table is faster & shorter for density > 20% - JGH */
	/*    direct: overhead=2 (for br); increment: 5*n */
	/*    table:  overhead=15; increment: 1*(range+1) */

	if(range+14 <= 5*n)
	{
		 /* implement a direct switch */
		dlab = p->slab >= 0 ? p->slab : getlab();

		if( p[1].sval )
		{
#ifdef	INT16BIT
			printf( "	sub.w	&%ld,%%d0\n", p[1].sval );
#else
			printf( "	sub.l	&%ld,%%d0\n", p[1].sval );
#endif
		}

		/* note that this is a logical compare; it thus checks
		** for numbers below range as well as out of range.
		*/
#ifdef	INT16BIT
		printf( "	cmp.w	%%d0,&%ld\n", range );
#else
		printf( "	cmp.l	%%d0,&%ld\n", range );
#endif

		printf( "	bhi	L%%%d\n", dlab );

		printf( "	add.w	%%d0,%%d0\n" );
/* Assume "swbeg &n" takes 4 bytes:  2 for swbeg, 2 for # of cases */
		printf( "	mov.w	10(%%pc,%%d0.w),%%d0\n" );
		printf( "	jmp	6(%%pc,%%d0.w)\n" );
		printf(	"	swbeg	&%d\n", range+1 );
				/* range runs from zero, so add 1 for cases */
		deflab( swlab = getlab() );


		/* output table */

		for( i=1, j=p[1].sval;  i<=n;  ++j )
		{
			printf( "	short	L%%%d-L%%%d\n",
				(j == p[i].sval) ? p[i++].slab : dlab, swlab );
		}

		if( p->slab < 0 )
			deflab( dlab );
		return;
	}
	/* out for the moment
	** if( n >= 4 )
	**	werror( W_LINT, "inefficient switch: %d, %d", n, (int) (range/n) );
	*/

	/* simple switch code */

	for( i=1; i<=n; ++i )
	{
		/* already in 0 */
		if( p[i].sval )
#ifdef	INT16BIT
			printf( "	cmp.w	%%d0,&%ld\n", p[i].sval );
		else
			printf( "	tst.w	%%d0\n" );
#else
			printf( "	cmp.l	%%d0,&%ld\n", p[i].sval );
		else
			printf( "	tst.l	%%d0\n" );
#endif

		printf( "	beq	L%%%d\n", p[i].slab );
	}

	if( p->slab >= 0 )
		printf( "	br	L%%%d\n", p->slab );

#endif

}

#ifdef TWOPASS
/*
 *	Code added from common/reader.c to make 2 pass version
 *
 */


#ifndef NODBG
e2print( p )
register NODE *p; 
{
	printf( "\n********* costs=(0,...,NRGS;EFF;TEMP;CC)\n" );
	e22print( p ,"T");
	printf("=========\n");
}

e22print( p ,s)
register NODE *p; 
char *s;
{
	static down=0;
	register ty;

	ty = optype( p->tn.op );
	if( ty == BITYPE )
	{
		++down;
		e22print( p->in.right ,"R");
		--down;
	}
	e222print( down, p, s );

	if( ty != LTYPE )
	{
		++down;
		e22print( p->in.left, "L" );
		--down;
	}
}

e222print( down, p, s )
NODE *p; 
char *s;
{
	/* print one node */
	int i, d;

	for( d=down; d>1; d -= 2 ) printf( "\t" );
	if( d ) printf( "    " );

	printf( "%s.%d) op= '%s'",s, (int)(p-node), opst[p->in.op] );
	switch( p->in.op ) 
	{
		 /* special cases */
	case REG:
		printf( " %s", p->tn.rval );  /* Changed form reader. */
		break;

	case ICON:
	case NAME:
	case VAUTO:
	case VPARAM:
	case TEMP:
		printf( " " );
		if (p->in.name != NULL) printf(" %s", p->in.name );
		break;

	case STCALL:
	case UNARY STCALL:
		printf( " args=%d", p->stn.argsize );
	case STARG:
	case STASG:
		printf( " size=%d", p->stn.stsize );
		printf( " align=%d", p->stn.stalign );
		break;

	case GENBR:
		printf( " %d (%s)", p->bn.label, opst[p->bn.lop] );
		break;

	case CALL:
	case UNARY CALL:
#ifdef IN_LINE
	case INCALL:
	case UNARY INCALL:
#endif
		printf( " args=%d", p->stn.argsize );
		break;

	case GENUBR:
	case GENLAB:
		printf( " %d", p->bn.label );
		break;

	case FUNARG:
		printf( " offset=%d", p->tn.rval );

	}

	printf( ", type %d\n", p->in.type );
}

# else
e2print( p )
NODE *p; 
{
	werror( W_DFT, "e2print not compiled" );
}
e222print( p, s )
NODE *p; 
char *s;
{
	werror( W_DFT, "e222print not compiled" );
}
# endif
/*
 *	End of code added from common/reader.c to make 2 pass version
 *
 */
#endif
