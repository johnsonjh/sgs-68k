#ident	"@(#)misc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	misc.c	7.1		*/
/*	Copyright 1985 Motorola Inc.			*/

# include "hash.h"
# include "mfile2.h"
# include "declar.h"

/* this may not be the best place for this routine... */
argsize( p )
register NODE *p; 
{
	/* size of the arguments */
	register t;
	t = 0;
	if( p->tn.op == CM )
	{
		t = argsize( p->in.left );
		p = p->in.right;
	}
	if( p->tn.type & (TDOUBLE|TFLOAT) )
	{
		SETOFF( t, ALDOUBLE );
		t += SZDOUBLE;
	}
	else if( p->tn.type & (TLONG|TULONG) )
	{
		SETOFF( t, ALLONG );
		t += SZLONG;
	}
	else if( p->tn.type & TPOINT )
	{
		SETOFF( t, ALPOINT );
		t += SZPOINT;
	}
	else if( p->tn.type & TSTRUCT )
	{
		SETOFF( t, p->stn.stalign );  /* alignment */
		t += p->stn.stsize;  /* size */
	}
	else 
	{
		SETOFF( t, ALINT );
		t += SZINT;
	}
	return( t );
}


int nerrors = 0;  /* number of errors */

NODE *lastfree;  /* pointer to last free node; (for allocator) */
NODE *highnode;	 /* pointer to highest node allocated so far */

tinit()
{
	 /* initialize expression tree search */
	register NODE *p;

	highnode = node - 1;	/* high node allocated */
	lastfree = &node[TREESZ-1]; /* prime to start at first node */
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
	FORTCALL, "FCALL", BITYPE|CALLFLG,
	EQ, "==", BITYPE|LOGFLG,
	NE, "!=", BITYPE|LOGFLG,
	LE, "<=", BITYPE|LOGFLG,
	LT, "<", BITYPE|LOGFLG,
	GE, ">=", BITYPE|LOGFLG,
	GT, ">", BITYPE|LOGFLG,
	MI, "MI", BITYPE|LOGFLG,
	PL, "PL", BITYPE|LOGFLG,
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
#ifdef HLOPTIM
	EXPRNO, ".", UTYPE,
	SBLOCK, "SBLOCK", LTYPE,
	EBLOCK, "EBLOCK", LTYPE,
	DEFTYP,	"DEFTYP", LTYPE,
#ifdef ASM_COMMENT
	ASMNO,  "ASMNO",  LTYPE,
#endif
#endif
#ifdef IN_LINE
	INCALL, "INCALL", BITYPE|CALLFLG,
	UNARY INCALL, "UINCALL", UTYPE|CALLFLG,
#endif
#ifdef  M68881
	INTCALL1, "INTCAL1", BITYPE,
	INTCALL2, "INTCAL2", BITYPE,
	INTCALL3, "INTCAL3", BITYPE,
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
