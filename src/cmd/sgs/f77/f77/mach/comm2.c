/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) comm2.c: version 25.1 created on 12/2/91 at 17:36:01	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)comm2.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	comm2.c	6.1		*/

#ident	"@(#)f77/f77/mach:comm2.c	25.1"

# include "mfile2.h"


/*	common	7.2		*/

# ifndef EXIT
# define EXIT exit
# endif

int nerrors = 0;  /* number of errors */

NODE *lastfree;  /* pointer to last free node; (for allocator) */

# ifndef MYWHERE
where(c)
{
	 /* print location of error. c is either 'u', 'c', or 'w' */
	fprintf( stderr, "%s, line %d: ", ftitle, lineno );
}
# endif

/* VARARGS1 */
uerror( s, a )
char *s; 
{
	/* nonfatal error message 
	** the routine where is different for pass 1 and pass 2;
	** it tells where the error took place 
	*/
	++nerrors;
	where('u');
	fprintf( stderr, s, a );
	fprintf( stderr, "\n" );
	if( nerrors > 30 ) cerror( "too many errors");
	fflush(stderr);
}

/* VARARGS1 */
cerror( s, a, b, c )
char *s; 
{
	 /* compiler error: die */
	where('c');
	if( nerrors && nerrors <= 30 )
	{
		/* give the compiler the benefit of the doubt */
		fprintf( stderr,
		"cannot recover from earlier errors: goodbye!\n" );
	}
	else 
	{
		fprintf( stderr, "compiler error: " );
		fprintf( stderr, s, a, b, c );
		fprintf( stderr, "\n" );
		fflush( stdout );
	}
	EXIT(1);
}

/* VARARGS1 */
werror( s, a, b )
char *s; 
{
	/* warning */
	where('w');
	fprintf( stderr, "warning: " );
	fprintf( stderr, s, a, b );
	fprintf( stderr, "\n" );
}

tinit()
{
	 /* initialize expression tree search */
	register NODE *p;

	for( p=node; p<= &node[TREESZ-1]; ++p ) p->in.op = FREE;
	lastfree = node;
}

# define TNEXT(p) (p== &node[TREESZ-1]?node:p+1)

NODE *
talloc()
{
	register NODE *p, *q;

	q = lastfree;
	for( p = TNEXT(q); p!=q; p= TNEXT(p))
		if( p->in.op ==FREE ) return(lastfree=p);

	cerror( "out of tree space; simplify expression");
	/* NOTREACHED */
}

tcheck()
{
	 /* ensure that all nodes have been freed */
	register NODE *p;

	if( !nerrors )
		for( p=node; p<= &node[TREESZ-1]; ++p ) 
		{
			if( p->in.op != FREE )
			{
				cerror( "wasted space: %d", p-node );
			}
		}
	else
		tinit();
	freestr();
}

tfree(p)
register NODE *p; 
{
	/* allow tree fragments to be freed, also */
	if( !p ) return;
	switch( optype( p->tn.op ) )
	{
	case BITYPE:
		tfree( p->in.right );
	case UTYPE:
		tfree( p->in.left );
	}
	p->in.op = FREE;
}


int dope[ DSIZE ];
char *opst[DSIZE];
NODE node[TREESZ];

char	ftitle[100] = "\"\"";	/* title of the file */
extern int	lineno;		/* line number of the input file */

struct dopest 
{
	int dopeop; 
	char opst[8]; 
	int dopeval; 
} indope[] = 
{
	NAME, "NAME", LTYPE,
	STRING, "STRING", LTYPE,
	REG, "REG", LTYPE,
	TEMP, "TEMP", LTYPE,
	VAUTO, "AUTO", LTYPE,
	VPARAM, "PARAM", LTYPE,
	ICON, "ICON", LTYPE,
	FCON, "FCON", LTYPE,
	CCODES, "CCODES", LTYPE,
	UNARY MINUS, "U-", UTYPE,
	UNARY MUL, "STAR", UTYPE,
	UNARY AND, "U&", UTYPE,
	UNARY CALL, "UCALL", UTYPE|CALLFLG,
	UNARY FORTCALL, "UFCALL", UTYPE|CALLFLG,
	NOT, "!", UTYPE|LOGFLG,
	COMPL, "~", UTYPE,
	INIT, "INIT", UTYPE,
	CONV, "CONV", UTYPE,
	PLUS, "+", BITYPE|FLOFLG|SIMPFLG|COMMFLG,
	ASG PLUS, "+=", BITYPE|ASGFLG|ASGOPFLG|FLOFLG|SIMPFLG|COMMFLG,
	MINUS, "-", BITYPE|FLOFLG|SIMPFLG,
	ASG MINUS, "-=", BITYPE|FLOFLG|SIMPFLG|ASGFLG|ASGOPFLG,
	MUL, "*", BITYPE|FLOFLG|MULFLG,
	ASG MUL, "*=", BITYPE|FLOFLG|MULFLG|ASGFLG|ASGOPFLG,
	AND, "&", BITYPE|SIMPFLG|COMMFLG,
	ASG AND, "&=", BITYPE|SIMPFLG|COMMFLG|ASGFLG|ASGOPFLG,
	QUEST, "?", BITYPE,
	COLON, ":", BITYPE,
	ANDAND, "&&", BITYPE|LOGFLG,
	OROR, "||", BITYPE|LOGFLG,
	CM, ",", BITYPE,
	COMOP, ",OP", BITYPE,
	FREE, "FREE!?!", LTYPE,
	ASSIGN, "=", BITYPE|ASGFLG,
	DIV, "/", BITYPE|FLOFLG|MULFLG|DIVFLG,
	ASG DIV, "/=", BITYPE|FLOFLG|MULFLG|DIVFLG|ASGFLG|ASGOPFLG,
	MOD, "%", BITYPE|DIVFLG,
	ASG MOD, "%=", BITYPE|DIVFLG|ASGFLG|ASGOPFLG,
	LS, "<<", BITYPE|SHFFLG,
	ASG LS, "<<=", BITYPE|SHFFLG|ASGFLG|ASGOPFLG,
	RS, ">>", BITYPE|SHFFLG,
	ASG RS, ">>=", BITYPE|SHFFLG|ASGFLG|ASGOPFLG,
	OR, "|", BITYPE|COMMFLG|SIMPFLG,
	ASG OR, "|=", BITYPE|COMMFLG|SIMPFLG|ASGFLG|ASGOPFLG,
	ER, "^", BITYPE|COMMFLG|SIMPFLG,
	ASG ER, "^=", BITYPE|COMMFLG|SIMPFLG|ASGFLG|ASGOPFLG,
	INCR, "++", BITYPE|ASGFLG,
	DECR, "--", BITYPE|ASGFLG,
	STREF, "->", BITYPE,
	CALL, "CALL", BITYPE|CALLFLG,
#if M68881
	/* kludge, with m68881 the fcall is used to indicate that */
	/* fortan some fortran floating point intrinsics can be  */
	/* generated in line */
	FORTCALL, "FCALL", BITYPE,
#else
	FORTCALL, "FCALL", BITYPE|CALLFLG,
#endif
	EQ, "==", BITYPE|LOGFLG,
	NE, "!=", BITYPE|LOGFLG,
	LE, "<=", BITYPE|LOGFLG,
	LT, "<", BITYPE|LOGFLG,
	GE, ">=", BITYPE|LOGFLG,
	GT, ">", BITYPE|LOGFLG,
	UGT, "UGT", BITYPE|LOGFLG,
	UGE, "UGE", BITYPE|LOGFLG,
	ULT, "ULT", BITYPE|LOGFLG,
	ULE, "ULE", BITYPE|LOGFLG,
	ARS, "A>>", BITYPE,
	TYPE, "TYPE", LTYPE,
	LB, "[", BITYPE,
	CBRANCH, "CBRANCH", BITYPE,
	GENLAB, "GENLAB", UTYPE,
	GENUBR, "GENUBR", UTYPE,
	GENBR, "GENBR", UTYPE,
	CMP, "CMP", BITYPE,
	FLD, "FLD", UTYPE,
	PMUL, "P*", BITYPE,
	PDIV, "P/", BITYPE,
	RETURN, "RETURN", BITYPE|ASGFLG,
	CAST, "CAST", BITYPE|ASGFLG,
	GOTO, "GOTO", UTYPE,
	STASG, "STASG", BITYPE,
	STARG, "STARG", UTYPE,
	STCALL, "STCALL", BITYPE|CALLFLG,
	UNARY STCALL, "USTCALL", UTYPE|CALLFLG,
	RNODE, "RNODE", LTYPE,
	SNODE, "SNODE", LTYPE,
	QNODE, "QNODE", LTYPE,
	MANY, "MANY", BITYPE,
	FUNARG, "ARG", UTYPE,
	UOP0, "UOP0", UTYPE|SPFLG,
	UOP1, "UOP1", UTYPE|SPFLG,
	UOP2, "UOP2", UTYPE|SPFLG,
	UOP3, "UOP3", UTYPE|SPFLG,
	UOP4, "UOP4", UTYPE|SPFLG,
	UOP5, "UOP5", UTYPE|SPFLG,
	UOP6, "UOP6", UTYPE|SPFLG,
	UOP7, "UOP7", UTYPE|SPFLG,
	UOP8, "UOP8", UTYPE|SPFLG,
	UOP9, "UOP9", UTYPE|SPFLG,
#ifdef ITREES
	PROLOG, "PROLOG", LTYPE,
	EPILOG, "EPILOG", LTYPE,
	SWBEG, "SWBEG", UTYPE,
	SWCASE, "SWCASE", UTYPE,
	SWDEF, "SWDEF", UTYPE,
	SWEND, "SWEND", LTYPE,
#endif
	-1,	0
};

mkdope()
{
	register struct dopest *q;

	for( q = indope; q->dopeop >= 0; ++q )
	{
		dope[q->dopeop] = q->dopeval;
		opst[q->dopeop] = q->opst;
	}
}



#define NTSTRBUF	100
#define TSTRSZ		2048
char	itstrbuf[TSTRSZ];
char	*tstrbuf[NTSTRBUF] = { itstrbuf };
char	**curtstr = tstrbuf;
int	tstrused;

char *
tstr( cp )			/* place copy of string into temp storage */
	register char *cp;	/* strings longer than TSTRSZ will break tstr */
{
	register int i = strlen( cp );
	register char *dp;
	extern char * malloc();
	extern char * strcpy();

	if ( tstrused + i >= TSTRSZ )
	{
		if ( ++curtstr >= &tstrbuf[NTSTRBUF] )
			cerror( "out of temporary string space" );
		tstrused = 0;
		if ( *curtstr == 0 )
		{
			if ( ( dp = (char *) malloc( TSTRSZ ) ) == 0 )
				cerror( "out of memory [tstr()]" );
			*curtstr = dp;
		}
	}
	strcpy( dp = *curtstr + tstrused, cp );
	tstrused += i + 1;
	return ( dp );
}

#ifdef m68k
NODE *
cptree( p )
register NODE *p; 
{
	/* make a copy of subtree rooted at p*/

	register NODE *q;
	register r;

	q=talloc();
	*q = *p;
	switch( optype(q->in.op) )
	{
	case BITYPE:
		q->in.right = cptree(p->in.right);
	case UTYPE:
		q->in.left = cptree(p->in.left);
	}
	return(q);
}
#endif
