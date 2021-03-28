#ident	"@(#)grey.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* Diagnose grey expressions */
/* See ANSI Standard, Section 3.3, Paragraph 2 */
/* for the definition of non-grey expressions. */
#include <mfile1.h>

#define MAX_SIDEFF	50			/* reflects worst-case assumptions regarding */
								/* the number of side effects in an expression*/
#define MAX_LVAL	MAX_SIDEFF * 5

static int is_noop;					/* if true, expression has no effects */
static int stk_idx;
static NODE *sideff_stack[MAX_SIDEFF];
static int lval_idx;
static NODE *lval_stack[MAX_LVAL];
static int warned;
static int subtree_save();
static int subtree_restore();

/* Prototypes for static functions */
static void  traverse _PARMS((union ndu *, int ));
static void  push_lval _PARMS((union ndu *));
static int  lvalsrch _PARMS((union ndu *, int ));
static void  push_sideff _PARMS((union ndu *));
static int  lvalcmp _PARMS((union ndu *, union ndu *));
static int  side_eff _PARMS((int ));
static int  sequence_point _PARMS((int ));
static int  subtree_save _PARMS((union ndu **, union ndu **, int , int ));
static int  subtree_restore _PARMS((union ndu **, union ndu **, int , int , int ));

/* Entry point and setup */
void
grey( p )
NODE *p;
{
								/* Initialize global control variables */
	is_noop = 1;				/* assume the worst */
	stk_idx = 0;
	lval_idx = 0;
	warned = 0;

	if ( optype(p->in.op) != LTYPE )
		traverse( p, 0 );

	if ( is_noop ) {
		switch ( p->in.op ) {
			case CBRANCH:
			case INIT:
				break;
			case ICON:			/* Don't warn for NDEBUG assert() */
				/* We should really be look down all legs of the tree. */
				/* For now, we'll incorrectly complain about e.g. */
				/* "assert(1), assert(2);" */
				if ( p->tn.type == VOID && p->tn.lval == 0 )
					break;
				/* Fallthrough */
			default:
				werror( W_DFLT, "expression has no effects" );
				break;
		}
	}
}

/* Main control routine for grey expression diagnosing. */
/* Conduct (modified) post-order traversal of expression tree, */
/* dividing the expression along sequence points, and identifying */
/* side-effected lvalues within each "boxed" expression.  For a */
/* discussion of the issues involved, see Expressions & Grouping */
/* in Thom Plum, Notes on the Draft C Standard. */

#define	RTOL(op)	(op==CALL)
#define LEG1(p)		(RTOL(p->in.op) ? p->in.right : p->in.left )
#define LEG2(p)		(RTOL(p->in.op) ? p->in.left : p->in.right )

static void
traverse( p, lhs )
NODE *p;
int lhs;
{
	int ty;
	NODE *lval_save[MAX_LVAL];
	NODE *sideff_save[MAX_SIDEFF];
	int bitype = 0;

	ty = optype( p->in.op );
	if ( ty == LTYPE ) {
		if ( lhs )
			push_lval( p );
	} else {
		int subtree[2];
		subtree[0] = stk_idx;				/* root of current subtree */
		subtree[1] = lval_idx;

		traverse( LEG1(p), side_eff(p->in.op) );
		if ( ty == BITYPE ) {
			int restore[2];
			int restore_flag = 0;
			int i,j,k;

			restore[0] = restore[1] = 0;
			/* The COLON acts as a sequence point when considering
			 * data flow.  Thus
			 *
			 *  i ? i++ : i--
			 *
			 * is NOT grey.  Only one side of the expression will
			 * be evaluated
			*/
			if ( p->in.op == COLON || sequence_point(p->in.op) ) {
				restore[0] = stk_idx = subtree_save( sideff_stack, sideff_save, 
												subtree[0], stk_idx );
				restore[1] = lval_idx = subtree_save( lval_stack, lval_save, 
												subtree[1], lval_idx );
				restore_flag = 1;
			}
			traverse( LEG2(p), 0 );
			if ( restore_flag ) {
				stk_idx = subtree_restore( sideff_stack, sideff_save, 
										restore[0], stk_idx, MAX_SIDEFF );
				lval_idx = subtree_restore( lval_stack, lval_save, 
										restore[1], lval_idx, MAX_LVAL );
			}
		}
		if ( side_eff(p->in.op) ) {
			push_sideff( p->in.left );
			is_noop = 0;
		} else {
			if ( callop(p->in.op) )
				is_noop = 0;
		}
	}
}

static void
push_lval( p )
NODE *p;
{
	switch ( p->in.op ) {
		case VAUTO:
		case VPARAM:
		case NAME:
		case REG:
			break;
		default:
			return;
			break;
	}

	lvalsrch( p, 0 );

	if ( lval_idx >= MAX_LVAL )
		cerror( "Out of room, push_lval" );

	lval_stack[ lval_idx++ ] = p;
}


/* Search the stack for the given lvalue, which if found, */
/* indicates a grey expression. */
/* Return non-zero if given lvalue found. */
static int 
lvalsrch( p, flag )
NODE *p;
int flag;
{
	int i;

	if ( flag ) {
		for ( i = lval_idx-1; i >= 0; --i ) {
			if ( p != lval_stack[i] && lvalcmp( lval_stack[i], p ) ) {
				if ( !warned ) {
					werror( W_DFLT, "grey expression" );
					warned = 1;
				}
				return 1;
			}
		}
	}

	for ( i = stk_idx-1; i >= 0; --i ) {
		if ( p != sideff_stack[i] && lvalcmp( sideff_stack[i], p ) ) {
			if ( !warned ) {
				werror( W_DFLT, "grey expression" );
				warned = 1;
			}
			return 1;
		}
	}
	return 0;
}


/* Save information about the node on the stack of side-effected lvalues. */
static void
push_sideff( p )
NODE *p;
{
	lvalsrch( p, 1 );

	if ( stk_idx >= MAX_SIDEFF )
		cerror( "Out of room, side effect push" );

	sideff_stack[ stk_idx++ ] = p;
	is_noop = 0;
}

#if 0
/* This routine is called after we have found a sequence point in the */
/* middle of the expression tree.  It removes from the side-effect stack */
/* all lvalues which come before the sequence point. */
static void
pop( subtree )
int subtree[2];
{
	while ( stk_idx > subtree[0] )
		--stk_idx;
	while ( lval_idx > subtree[1] )
		--lval_idx;
}
#endif

/* Compare two nodes and return non-zero if they refer to the same lvalue. */
static int
lvalcmp( one, two )
NODE *one, *two;
{
	int same = 0;

	if ( one->in.op != two->in.op )
		return same;

	switch ( one->in.op ) {
		case VAUTO:
		case VPARAM:
			same = ( one->tn.lval == two->tn.lval );
			break;
		case NAME:	/* chk lval as well: could different members */
					/* of one structure instance */
			if ( one->tn.rval != NONAME )
				same = ( one->tn.rval == two->tn.rval && 
						one->tn.lval == two->tn.lval );
			break;
		case REG:
			{
				register int test;

				test = (ISPTR(one->tn.type) << 3 	| 
						ISPTR(two->tn.type) << 2 	| 
						FP_TYPE(one->tn.type) << 1	| 
						FP_TYPE(two->tn.type) );

				switch ( test ) {
					case 000:			/* both data reg */
					case 014:			/* both address reg */
					case 003:			/* both floating point reg */
						same = ( one->tn.rval == two->tn.rval );
						break;
					default:			/* incompatible types */
						same = 0;
						break;
				}
			}
			break;
		default:
			break;
	}
	return( same );
}

/* from cgen.c */
static int
side_eff( op )
int op;
{
	return ( /* callop(op) ||*/ asgop(op) || op==STASG || op== HIDASG );
}


/* Return non-zero if op produces a sequence point */
/* From ANSI Appendix 2 */
static int
sequence_point( op )
int op;
{
	int seq = 0;

	switch( op ) {
		case CALL:
		case ANDAND:
		case OROR:
		case COMOP:
		case QUEST:
		case CBRANCH:
			seq = 1;
			break;
		default:
			seq = 0;
			break;
	}

	return( seq );
}

static int
subtree_save( array, save, low, high )
NODE *array[];
NODE *save[];
int low;
int high;
{
	int i,j;
	int ret = low;

	for ( j=0; low < high; j++, low++ )
		save[j] = array[low];	
	save[j] = 0;

	return ret;
}



static int
subtree_restore( array, save, low, high, max )
NODE *array[];
NODE *save[];
int low;
int high;
int max;
{
	int i,k;
	int savecnt;
	int range;
	int ret;

	for ( savecnt=0; save[savecnt]; savecnt++ ) ;

	range = high - low;

	if ( low+range+savecnt >= max )
		cerror( "Out of room, subtree_restore" );

	for ( i=low; range>0 ; --range,i++ )
		array[i+savecnt] = array[i];
	ret = i+savecnt;

	for ( k=0,i=low; k<savecnt; k++,i++ )
		array[i] = save[k];

	return ret;
}
